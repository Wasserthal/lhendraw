//This unit transforms user input into commands to the program
struct menuref_
{
	char type;//0: buttons 1: popup and the like 2: PSE
	structenum what;//The structenum
	int alignx,aligny;//The position
};
menuref_ menu_list[20];
int menu_list_count=0;
char * menu_matrixsubmenuvariable;
SDL_Event control_Event;
int control_firstmenux,control_firstmenuy;
int control_lastmenux,control_lastmenuy;
int control_mousestate=0;//0: inactive; 0x1: from tool, mouseclick; 0x2: from special tool, keyboard 0x4: on menu, dragging 0x8: on button_function dependent menu, popup 0x10 popup-menu or PSE, multiple levels 0x20 dragging menuitem
int control_toolaction=0;//1: move 2: move selection 3: tool specific
int control_tool=0;//1: Hand 2: 2coordinate Selection 3: Lasso, no matter which 4: Shift tool 5: Magnifying glass 6: Element draw 7: chemdraw draw 8: eraser 9: Arrows 10: attributes 11: graphic 12: bezier 13: image 14: spectrum 15: tlc plate/gel plate 16: text tool
int control_menumode=0;//1: shliderhorz, 2: slidervert 3: colorchooser
AUTOSTRUCT_PULLOUTLISTING_ * control_menuitem=NULL;
#define control_toolcount 17
clickabilitymatrix_ clickabilitymatrixes[control_toolcount];
int control_keycombotool=0;//as above, but only valid if (mousestate & 2)
SDLKey control_toolstartkeysym;
int control_lastinterpret=-1;
int control_posx=0;
int control_posy=0;
long long control_id=-1;
float control_coorsx=0;
float control_coorsy=0;
int control_mousex,control_mousey;
char control_reticle=1;
char control_dragged=0;
char control_lastmousebutton=0;
char control_usingmousebutton=0;
char control_clickforpriority=0;//0: frontmost 1: rearmost 2: nearest 3: nearest3d
char control_ambiguousity=0;
float control_startx=0;
float control_starty=0;
int control_menudragint=0;
clickabilitymatrix_ clickabilitymatrix_tooldependent[control_toolcount];

char control_filename[stringlength]="";
char control_filetype[stringlength]=".cdx";
char control_nextfilename[stringlength]="";
char control_nextfiletype[stringlength]=".cdx";
int control_force=0;
int control_interactive=1;
int control_saveuponexit=0;
int control_GUI=1;
typedef struct control_toolinfo_
{
	_u32 undoes;//for BOTH mouse buttons, unless using other tool
	char forcedrag;//two bits, one for lmb and one for rmb, higher two bits of precise_drag: lower lmb, higher rmb.
	int rmbfunction;//0: use popup; -1: tool-specific, use lmb -2: tool-specific, add 0x10000 -3: use rmb function of menu >0: tool-specific, use other tool
} control_toolinfo_;
control_toolinfo_ control_toolinfo[]=
{
	{0,0,0},//0: none
	{0,0,0},//1: Hand
	{0,1,0},//2: 2Coordinate selection
	{0,0,0},//3: Lasso
	{(_u32)~0,1,1},//4: Shift tool
	{0,0,-1},//5: Magnifying glass
	{1<<STRUCTURE_OBJECTTYPE_n,0,-3},//Element put
	{(_u32)~0,0,0},//Chemdraw draw
	{(_u32)~0,7,-1},//eraser
	{(_u32)~0,0,0},//Arrow
};
typedef struct MODIFIER_KEYS_
{
	char CTRL;
	char ALT;
	char SHIFT;
	char SUPER;
}MODIFIER_KEYS_;
MODIFIER_KEYS_ MODIFIER_KEYS={0,0,0,0};
int CONTROL_ZOOMIN(float ifactor,char i_direction)
{
	if (i_direction==1)
	{
		SDL_zoomx/=ifactor;
		SDL_zoomy/=ifactor;
		SDL_scrollx+=(control_Event.button.x-gfx_canvasminx)*((1/ifactor)-1)/SDL_zoomx;
		SDL_scrolly+=(control_Event.button.y-gfx_canvasminy)*((1/ifactor)-1)/SDL_zoomy;
	}
	else
	{
		SDL_zoomx*=ifactor;
		SDL_zoomy*=ifactor;
		SDL_scrollx+=(control_Event.button.x-gfx_canvasminx)*(ifactor-1)/SDL_zoomx;
		SDL_scrolly+=(control_Event.button.y-gfx_canvasminy)*(ifactor-1)/SDL_zoomy;
	}
}

void checkupinconsistencies()
{
/*	for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
	{
		for (int ilv2=ilv1+1;ilv2<(*glob_n_multilist).filllevel;ilv2++)
		{
			if ((*glob_n_multilist).bufferlist[ilv1].id==(*glob_n_multilist).bufferlist[ilv2].id)
			{
				printf("%i==%i;%i\n",ilv1,ilv2,(*glob_n_multilist).bufferlist[ilv1].id);
			}
		}
	}*/
	for (int ilv1=0;ilv1<(*glob_b_multilist).filllevel;ilv1++)//removes defective bonds
	{
		b_instance * tl_b_instance=&((*glob_b_multilist).bufferlist[ilv1]);
		if ((*tl_b_instance).exist)
		{
			if ((*tl_b_instance).B==(*tl_b_instance).E)//destroys bonds with beginning==end
			{
				(*tl_b_instance).exist=0;
				goto i_b_destroyed;
			}
			for (char ilv2=0;ilv2<2;ilv2++)//destroys bonds with one side missing
			{
				if (!edit_locatebyid(STRUCTURE_OBJECTTYPE_n,ilv2?((*tl_b_instance).B):((*tl_b_instance).E),NULL))
				{
					(*tl_b_instance).exist=0;
					goto i_b_destroyed;
				}
			}
		}
		i_b_destroyed:;
	}
	getatoms();
	for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)//deletes atoms that have neither a bond nor a label.
	{
		n_instance * tl_n_instance=&((*glob_n_multilist).bufferlist[ilv1]);
		if ((*tl_n_instance).exist)
		{
			if (atom_actual_node[ilv1].bondcount==0)
			{
				if ((*tl_n_instance).Element==constants_Element_implicitcarbon)
				{
					(*tl_n_instance).exist=0;
				}
			}
		}
	}
	for (int ilv2=0;ilv2<(*glob_b_multilist).filllevel;ilv2++)//Adds bonds to each other. TODO: merge pasted form bonds to single bonds.
	{
		if ((*glob_b_multilist).bufferlist[ilv2].exist)
		{
			int masamune=bond_actual_node[ilv2].end;
			for (int ilv3=0;ilv3<atom_actual_node[bond_actual_node[ilv2].start].bondcount;ilv3++)
			{
				int thisbond=atom_actual_node[bond_actual_node[ilv2].start].bonds[ilv3];
				if (thisbond!=ilv2)
				{
					if ((*glob_b_multilist).bufferlist[thisbond].exist)
					{
						int murasame=getother(bond_actual_node[ilv2].start,thisbond);
						if (masamune==murasame)
						{
							(*glob_b_multilist).bufferlist[thisbond].exist=0;
							(*glob_b_multilist).bufferlist[ilv2].Order&=0xF0;
							(*glob_b_multilist).bufferlist[ilv2].Order+=16;
							if ((*glob_b_multilist).bufferlist[ilv2].Order>64)
							{
								(*glob_b_multilist).bufferlist[ilv2].Order=16;
							}
						}
					}
				}
			}
		}
	}
	selection_recheck(selection_currentselection,&selection_currentselection_found);
}
int issueshiftstart()
{
	control_posx=control_mousex-gfx_canvasminx;
	control_posy=control_mousey-gfx_canvasminy;
	control_coorsx=control_posx/SDL_zoomx+SDL_scrollx;
	control_coorsy=control_posy/SDL_zoomy+SDL_scrolly;
	control_dragged=0;
	control_toolstartkeysym=control_Event.key.keysym.sym;
}
#ifdef GFXOUT_SDL
int interpretkey(int listnr=-1)
{
	_u32 modifierpattern;
	int ilv1,ilv2;
	_u32 tltype;
	char keystring[4]={0,0,0,0};
	char erledigt=0;
	char ihot=0;
	char careaboutshift=1;
	_u32 hotties=0;
	getatoms();
	if (listnr!=-1) {ilv1=listnr;goto interpreted;}
	storeundo(~0);
	switch (control_Event.key.keysym.sym)
	{
		case SDLK_F1: keystring[0]='F';keystring[1]='1';break;
		case SDLK_F2: keystring[0]='F';keystring[1]='2';break;
		case SDLK_F3: keystring[0]='F';keystring[1]='3';break;
		case SDLK_F4: keystring[0]='F';keystring[1]='4';break;
		case SDLK_F5: keystring[0]='F';keystring[1]='5';break;
		case SDLK_F6: keystring[0]='F';keystring[1]='6';break;
		case SDLK_F7: keystring[0]='F';keystring[1]='7';break;
		case SDLK_F8: keystring[0]='F';keystring[1]='8';break;
		case SDLK_F9: keystring[0]='F';keystring[1]='9';break;
		case SDLK_F10: keystring[0]='F';keystring[1]='1';keystring[2]='0';break;
		case SDLK_F11: keystring[0]='F';keystring[1]='1';keystring[2]='1';break;
		case SDLK_F12: keystring[0]='F';keystring[1]='1';keystring[2]='2';break;
		case SDLK_F13: keystring[0]='F';keystring[1]='1';keystring[2]='3';break;
		case SDLK_F14: keystring[0]='F';keystring[1]='1';keystring[2]='4';break;
		case SDLK_F15: keystring[0]='F';keystring[1]='1';keystring[2]='5';break;
		case SDLK_MENU: strncpy(keystring,"MENU",4);break;
		case SDLK_BACKSPACE: strncpy(keystring,"BACKSPACE",4);break;
		case SDLK_TAB: strncpy(keystring,"TAB",4);break;
		case SDLK_RETURN: strncpy(keystring,"RET",4);break;
		case SDLK_DELETE: strncpy(keystring,"DEL",4);break;
		case SDLK_0: keystring[0]='0';break;
		case SDLK_1: keystring[0]='1';break;
		case SDLK_2: keystring[0]='2';break;
		case SDLK_3: keystring[0]='3';break;
		case SDLK_4: keystring[0]='4';break;
		case SDLK_5: keystring[0]='5';break;
		case SDLK_6: keystring[0]='6';break;
		case SDLK_7: keystring[0]='7';break;
		case SDLK_8: keystring[0]='8';break;
		case SDLK_9: keystring[0]='9';break;
		default:;
		careaboutshift=0;
		_u16 ihv1=(control_Event.key.keysym.unicode);
		if ((ihv1>=1) && (ihv1<=26))
		{
			ihv1+=0x60;
			if (MODIFIER_KEYS.SHIFT)
			{
				ihv1-=0x20;
			}
		}
		if (ihv1<0x7F)
		{
			keystring[0]=ihv1;
		}
		else
		{
			if (ihv1<0x7FF)
			{
				keystring[1]=(ihv1 & 0x3F) | 0x80;
				keystring[0]=(ihv1>>6) + 0xC0;
			}
			else
			{
				keystring[2]=(ihv1 & 0x3F) | 0x80;
				keystring[1]=((ihv1>>6) & 0x3F) | 0x80;
				keystring[0]=(ihv1>>12) + 0xC0;
			}
		}
	}
	ihot=0;
	selection_recheck(selection_currentselection,&selection_currentselection_found);
	hotties=selection_currentselection_found;
	if (hotties==0)
	{
		for (ilv2=1;ilv2<STRUCTURE_OBJECTTYPE_ListSize;ilv2++)
		{
			if (control_hot[ilv2]!=-1)
			{
				basicmultilist * tl_multilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv2].name);
				int isize=STRUCTURE_OBJECTTYPE_List[ilv2].size;
				if (control_hot[ilv2]>=(*tl_multilist).filllevel) {control_hot[ilv2]=-1;goto idontusethishot;}
				if ((*(basic_instance*)(((char*)((*tl_multilist).pointer))+control_hot[ilv2]*isize)).exist==0) {control_hot[ilv2]=-1;goto idontusethishot;}
				hotties|=1<<ilv2;
				ihot=1;
				idontusethishot:;
			}
		}
	}
	modifierpattern=MODIFIER_KEYS.SHIFT*careaboutshift+MODIFIER_KEYS.CTRL*2+MODIFIER_KEYS.ALT*4+MODIFIER_KEYS.SUPER*8;
	for (ilv1=0;ilv1<hotkeylist_count;ilv1++)
	{
		tltype=hotkeylist[ilv1].type;
		if (((1<<(tltype & 0xFFFF)) & hotties) || ((tltype & 0xFFFF)==0))
		{
			if (((hotties==0) || ((tltype & 0x10000)==0)) && ((hotties) || ((tltype & 0x20000)==0)))
			{
				if (strcmp(keystring,hotkeylist[ilv1].key)==0)
				{
					if ((hotkeylist[ilv1].modifiers & ((~((_u32)(1-careaboutshift)))))==modifierpattern)
					{
						if (tltype & 0x40000)
						{
							issueshiftstart();
							control_keycombotool=0x10000;
							control_lastinterpret=ilv1;
						}
						interpreted:
						erledigt=0;
						if (hotkeylist[ilv1].functype==0)
						{
							if (hotkeylist[ilv1].command!=NULL)
							{
								if (hotkeylist[ilv1].command(hotkeylist[ilv1].variable,hotkeylist[ilv1].value)){erledigt=1;};
							}
						}
						else
						{
							if (hotkeylist[ilv1].command!=NULL)
							{
								basicmultilist * tl_multilist=findmultilist(STRUCTURE_OBJECTTYPE_List[tltype & 0xFFFF].name);
								int icompare=(1<<(tltype & 0xFFFF));
								int tl_size=(*tl_multilist).itemsize;
								int ifilllevel=(*tl_multilist).filllevel;//separately, so it doesn't grow while executing the loop
								if (ihot==0)
								{
									for (ilv2=0;ilv2<ifilllevel;ilv2++)
									{
										if (selection_currentselection[ilv2] & icompare)
										{
											hotshunt:
											if ((*tl_multilist)[ilv2].exist)
											{
												if (((catalogized_command_iterated_functype)(hotkeylist[ilv1].command))(hotkeylist[ilv1].variable,hotkeylist[ilv1].value,tl_multilist,&((*tl_multilist)[ilv2]),ilv2)){erledigt=1;};
											}
											if ((ihot) && (erledigt==0))
											{
												goto hotbackshunt;
											}
										}
									}
								}
								else
								{
									ilv2=control_hot[tltype & 0xFFFF];
									goto hotshunt;
									hotbackshunt:;
								}
							}
						}
						if (tltype & 0x40000)
						{
							control_mousestate=2;
						}
						if (erledigt) goto commanderledigt;
					}
				}
			}
		}
	}
	commanderledigt:;
	checkupinconsistencies();
}
#endif
void clickforthem()
{
	float iclickradius=constants_clickradius;
	if (control_tool==8) iclickradius=2000;
	selection_clearselection(selection_clickselection);
	selection_clickselection_found=0;
	if (selection_clickabilitymatrix.mode==1)
	{
		if (selection_clickabilitymatrix.types1 & 1)
		{
			selection_clickselection_found|=clickfor(control_coorsx,control_coorsy,STRUCTURE_OBJECTTYPE_n,iclickradius)<<STRUCTURE_OBJECTTYPE_n;
			selection_clickselection_found|=clickfor(control_coorsx,control_coorsy,STRUCTURE_OBJECTTYPE_b,iclickradius)<<STRUCTURE_OBJECTTYPE_b;
			for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
			{
				
			}
//			selectwholestructure
		}
	}
	if (selection_clickabilitymatrix.mode==2)
	{
		for (int ilv1=1;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
		{
			if (selection_clickabilitymatrix.types2[1] & (1<<ilv1))
			{
				selection_clickselection_found|=(clickfor(control_coorsx,control_coorsy,ilv1,iclickradius)>0)<<ilv1;
			}
		}
	}
}
catalogized_command_funcdef(ISSUEDELETE)
{
	_u32 icompare;
	int isize;
	int ioffset;
	char * ibufferpos;
	char isuccessful=0;
	if (control_mousestate==0)
	{
		if (!storeundo(~0))
		{
			return 0;
		}
	}
	if (selection_currentselection_found)
	{
		for (int ilv1=1;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
		{
			icompare=1<<ilv1;
			isize= STRUCTURE_OBJECTTYPE_List[ilv1].size;
			basicmultilist * tlmultilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
			if (tlmultilist==NULL) goto i_delete_fertig;
			CDXMLREAD_functype tldummy;
			ibufferpos=(char*)((*tlmultilist).pointer);
			cdx_Point2D * tlpoint2d;
			for (int ilv2=0;ilv2<(*tlmultilist).filllevel;ilv2++)
			{
				if ((selection_currentselection[ilv2]) & icompare)
				{
					if ((*((basic_instance*)(ibufferpos+isize*ilv2))).exist)
					{
						TELESCOPE_aggressobject(tlmultilist,ilv2);
						TELESCOPE_clear();
						(*((basic_instance*)(ibufferpos+isize*ilv2))).exist=0;
					}
				}
			}
			i_delete_fertig:;
		}
		isuccessful=1;
	}
	else
	{
		if (control_hot[STRUCTURE_OBJECTTYPE_n]!=-1)
		{
			if (control_hot[STRUCTURE_OBJECTTYPE_n]<(*glob_n_multilist).filllevel)
			{
				ibufferpos=(char*)(*glob_n_multilist).pointer;
				isize=STRUCTURE_OBJECTTYPE_List[STRUCTURE_OBJECTTYPE_n].size;
				if ((*glob_n_multilist).bufferlist[control_hot[STRUCTURE_OBJECTTYPE_n]].exist)
				{
					TELESCOPE_aggressobject(glob_n_multilist,control_hot[STRUCTURE_OBJECTTYPE_n]);
					TELESCOPE_clear();
					(*((basic_instance*)(ibufferpos+isize*control_hot[STRUCTURE_OBJECTTYPE_n]))).exist=0;
					isuccessful=1;
				}
			}
			control_hot[STRUCTURE_OBJECTTYPE_n]=-1;
		}
	}
	checkupinconsistencies();
	return isuccessful;
}
int issueclick(int iposx,int iposy)
{
	int ibackval;
	n_instance * tlatom;
	b_instance * tlbond;
	control_posx=iposx;
	control_posy=iposy;
	control_coorsx=control_posx/SDL_zoomx+SDL_scrollx;
	control_coorsy=control_posy/SDL_zoomy+SDL_scrolly;
	storeundo(~0);
	control_dragged=0;
	control_id=-1;
	clickforthem();
	switch (control_tool)
	{
		case 2:
		{
			selection_frame.startx=control_coorsx;
			selection_frame.starty=control_coorsy;
			selection_frame.endx=control_coorsx;
			selection_frame.endy=control_coorsy;
			break;
		}
		case 5:
		{
			selection_frame.startx=control_coorsx;
			selection_frame.starty=control_coorsy;
			selection_frame.endx=control_coorsx;
			selection_frame.endy=control_coorsy;
			break;
		}
		case 6:
		{
			if (control_lastmousebutton==SDL_BUTTON_LEFT)
			{
				tlatom=summonatom(NULL);
				if (tlatom)
				{
					(*tlatom).xyz.x=control_coorsx;
					(*tlatom).xyz.y=control_coorsy;
					(*tlatom).xyz.z=0;
					(*tlatom).Z=edit_getnewZ();
					(*tlatom).Element=control_drawproperties.Element;
				}
			}
			else
			{
				OPEN_PSE("","");
				return 0;
			}
			break;
		}
		case 7:
		{
			control_startx=control_coorsx;
			control_starty=control_coorsy;
			control_id=-1;
			tlatom=NULL;
			if (MODIFIER_KEYS.ALT==0)
			{
				if (selection_clickselection_found & (1<<STRUCTURE_OBJECTTYPE_b))
				{
					tlbond=(b_instance*)getclicked(STRUCTURE_OBJECTTYPE_b);
					if ((control_drawproperties.bond_multiplicity==1) && (control_drawproperties.bond_Display1==0) && ((*tlbond).Display==0))
					{
						(*tlbond).Order&=0xF0;
						(*tlbond).Order+=16;
						if ((*tlbond).Order>64)
						{
							(*tlbond).Order=16;
						}
					}
					else
					{
						(*tlbond).Order=control_drawproperties.bond_multiplicity;
						(*tlbond).Display=control_drawproperties.bond_Display1;
					}
					control_mousestate=0;
					return 0;
				}
				else
				{
					if (selection_clickselection_found & (1<<STRUCTURE_OBJECTTYPE_n))
					{
						tlatom=snapatom(control_coorsx,control_coorsy);
						if (tlatom)
						{
							control_id=(*tlatom).id;
							control_startx=(*tlatom).xyz.x;
							control_starty=(*tlatom).xyz.y;
						}
					}
				}
			}
			break;
		}
		case 9:
		{
			control_startx=control_coorsx;
			control_starty=control_coorsy;
			control_id=-1;
			break;
		}
	}
	control_mousestate=1;
	control_usingmousebutton=control_lastmousebutton;
	//warning: there ARE in-function returns.
	return 0;
}
void issuedrag(int iposx,int iposy)
{
	int atomnr2=-1;
	int tlwhichtool=control_tool;
	int ideltax=iposx-control_posx;
	int ideltay=iposy-control_posy;
	control_coorsx=iposx/SDL_zoomx+SDL_scrollx;
	control_coorsy=iposy/SDL_zoomy+SDL_scrolly;
	if (control_mousestate==2)
	{
		tlwhichtool=control_keycombotool;
		goto shiftshunt;
	}
	if ((ideltax==0) && (ideltay==0))
	{
		if ((control_toolinfo[control_tool].forcedrag & control_usingmousebutton)==0)
		{
			return;
		}
	}
	else
	{
		control_dragged=1;
	}
	_u32 icompare;
	int isize;
	int ioffset;
	char * ibufferpos;
	shiftshunt:
	switch (tlwhichtool)
	{
		case 1:
		{
			SDL_scrollx-=ideltax/SDL_zoomx;
			SDL_scrolly-=ideltay/SDL_zoomy;
			break;
		}
		case 2:
		{
			selection_frame.endx=control_coorsx;
			selection_frame.endy=control_coorsy;
			break;
		}
		case 5:
		{
			selection_frame.endx=control_coorsx;
			selection_frame.endy=control_coorsy;
			break;
		}
		case 7:
		{
			restoreundo(~0,1);
			int atomnr=-1;
			int atomnr2=-1;
			n_instance * tlatom,*tlatom2;
			b_instance * tlbond;
			tlatom=NULL;
			tlatom2=NULL;
			tlbond=NULL;
			if (control_id!=-1)
			{
				tlatom=(n_instance*)edit_locatebyid(STRUCTURE_OBJECTTYPE_n,control_id,&atomnr);
			}
			if (!tlatom)
			{
				tlatom=summonatom(&atomnr);
				if (tlatom)
				{
					(*tlatom).xyz.x=control_startx;
					(*tlatom).xyz.y=control_starty;
					(*tlatom).xyz.z=0;
					(*tlatom).Z=edit_getnewZ();
				}
			}
			if (MODIFIER_KEYS.ALT==0)
			{
				clickforthem();
				if (selection_clickselection_found & (1<<STRUCTURE_OBJECTTYPE_n))
				{
					tlatom2=snapatom(control_coorsx,control_coorsy,&atomnr2);
				}
				else
				{
					if (tlatom)
					{
						control_startx=(*tlatom).xyz.x;
						control_starty=(*tlatom).xyz.y;
					}
					float distance=sqrt(sqr(control_coorsx-(*tlatom).xyz.x)+sqr(control_coorsy-(*tlatom).xyz.y));
					float angle=getangle(control_coorsx-(*tlatom).xyz.x,control_coorsy-(*tlatom).xyz.y);
					distance=constants_bondlength;
					float tlfmod=fmod(angle,constants_angular_distance/8);
					if (tlfmod<constants_angular_distance/16)
					{
						angle-=tlfmod;
					}
					else
					{
						angle-=tlfmod;
						angle+=constants_angular_distance/8;
					}
					control_coorsx=control_startx+cos(angle)*distance;
					control_coorsy=control_starty+sin(angle)*distance;
				}
			}
			if (tlatom2==NULL)
			{
				tlatom2=snapatom_short(control_coorsx,control_coorsy,&atomnr2,10);
			}
			if (!tlatom2)
			{
				tlatom2=summonatom(&atomnr2);
				if (tlatom2)
				{
					(*tlatom2).xyz.x=control_coorsx;
					(*tlatom2).xyz.y=control_coorsy;
					(*tlatom2).xyz.z=(*tlatom).xyz.z;
				}
			}
			if ((tlatom) && (tlatom2))
			{
				if (tlatom!=tlatom2)
				{
					tlbond=summonbond((*tlatom).id,(*tlatom2).id,atomnr,atomnr2);
					if (tlbond)
					{
						(*tlbond).Z=0;
						(*tlbond).Order=control_drawproperties.bond_multiplicity;//TODO: *4
					}
				}
			}
			break;
		}
		case 4:
		{
			for (int ilv1=0;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
			{
				icompare=1<<ilv1;
				int isize= STRUCTURE_OBJECTTYPE_List[ilv1].size;
				basicmultilist * tlmultilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
				if (tlmultilist==NULL) goto i_control4_fertig;
				CDXMLREAD_functype tldummy;
				ioffset=(*tlmultilist).getproperties("xyz",&tldummy);
				if (ioffset<0) goto i_control4_fertig;
				ibufferpos=(char*)((*tlmultilist).pointer);
				cdx_Point2D * tlpoint2d;
				for (int ilv2=0;ilv2<(*tlmultilist).filllevel;ilv2++)
				{
					if ((selection_currentselection[ilv2]) & icompare)
					{
						if ((*((basic_instance*)(ibufferpos+isize*ilv2))).exist)
						{
							tlpoint2d = ((cdx_Point2D*)(ibufferpos+isize*ilv2+ioffset));
							(*tlpoint2d).x+=ideltax/SDL_zoomx;
							(*tlpoint2d).y+=ideltay/SDL_zoomy;
						}
					}
				}
				i_control4_fertig:;
			}
			break;
		}
		case 8:
		{
			clickforthem();
			for (int ilv1=1;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
			{
				basicmultilist * tlmultilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
				int isize=STRUCTURE_OBJECTTYPE_List[ilv1].size;
				for (int ilv2=0;ilv2<(*tlmultilist).filllevel;ilv2++)
				{
					basic_instance * tlinstance=(basic_instance*)((((char*)(*tlmultilist).pointer))+ilv2*isize);
					if ((*tlinstance).exist)
					{
						icompare=1<<ilv1;
						if (selection_clickselection[ilv2] & (1<<ilv1))
						{
							TELESCOPE_aggressobject(tlmultilist,ilv2);
							TELESCOPE_clear();
							(*tlinstance).exist=0;
						}
					}
				}
			}
			checkupinconsistencies();
			break;
		}
		case 9:
		{
			restoreundo(~0,1);
			arrow_instance * tl_arrow=summonarrow();
			if (tl_arrow)
			{
				if (MODIFIER_KEYS.ALT==0)
				{
					float tl_length=sqrt(sqr(control_coorsx-control_startx)+sqr(control_coorsy-control_starty));
					float tlangle;
					float tl_snapdistance;
					tlangle=getangle(control_coorsx-control_startx,control_coorsy-control_starty);
					tl_snapdistance=fmod((tlangle+2*Pi+Pi/16),(Pi/2))-Pi/16;
					if (fabs(tl_snapdistance)<Pi/16)
					{
						tlangle-=tl_snapdistance;
						control_coorsx=control_startx+tl_length*cos(tlangle);
						control_coorsy=control_starty+tl_length*sin(tlangle);
					}
				}
				(*tl_arrow).Head3D.x=control_coorsx;
				(*tl_arrow).Head3D.y=control_coorsy;
				(*tl_arrow).Head3D.z=0;
				(*tl_arrow).Tail3D.x=control_startx;
				(*tl_arrow).Tail3D.y=control_starty;
				(*tl_arrow).Tail3D.z=0;
				(*tl_arrow).Center3D.x=(control_startx+control_coorsx)/2;
				(*tl_arrow).Center3D.y=(control_starty+control_coorsy)/2;
				(*tl_arrow).Center3D.z=0;
				(*tl_arrow).LineType=0;
				(*tl_arrow).AngularSize=0;
				(*tl_arrow).MajorAxisEnd3D.x=control_coorsx;
				(*tl_arrow).MajorAxisEnd3D.y=control_coorsy;
				(*tl_arrow).MajorAxisEnd3D.z=0;
				(*tl_arrow).MinorAxisEnd3D.x=(*tl_arrow).Center3D.x-(control_coorsy-(*tl_arrow).Center3D.y);
				(*tl_arrow).MinorAxisEnd3D.y=(*tl_arrow).Center3D.y+(control_coorsx-(*tl_arrow).Center3D.x);
				(*tl_arrow).MinorAxisEnd3D.z=0;
				(*tl_arrow).ArrowheadType=0;
				(*tl_arrow).ArrowheadHead=1;
				(*tl_arrow).ArrowheadTail=0;
				(*tl_arrow).ArrowShaftSpacing=0;
				(*tl_arrow).Z=0;//TODO: pretty urgent
			}
			break;
		}
		case 0x10000:
		{
			interpretkey(control_lastinterpret);
		}
	}
	control_posx=iposx;
	control_posy=iposy;
	//warning: there ARE in-function returns.
	return;
}
int rectifyselectionframe()
{
	float tlswap;
	if ((selection_frame.startx==selection_frame.endx) && (selection_frame.starty==selection_frame.endy)) return 0;
	if (selection_frame.startx>selection_frame.endx)
	{
		tlswap=selection_frame.startx;
		selection_frame.startx=selection_frame.endx;
		selection_frame.endx=tlswap;
	}
	if (selection_frame.starty>selection_frame.endy)
	{
		tlswap=selection_frame.starty;
		selection_frame.starty=selection_frame.endy;
		selection_frame.endy=tlswap;
	}
	return 1;
}
void issuerelease()
{
	_u32 icompare;
	int isize;
	int ioffset;
	char * ibufferpos;
	switch (control_tool)
	{
		case 2:
		{

			selection_clearselection(selection_currentselection);
			selection_currentselection_found=0;
			if (!rectifyselectionframe())
			{
				selection_clearselection(selection_clickselection);
				selection_clickselection_found=0;
				clickforthem();
				if (selection_currentselection_found)
				{
					selection_copyselection(selection_currentselection,selection_clickselection);
					selection_currentselection_found=selection_clickselection_found;
					break;
				}
			}
			for (int ilv1=0;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
			{
				icompare=1<<ilv1;
				int isize= STRUCTURE_OBJECTTYPE_List[ilv1].size;
				basicmultilist * tlmultilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
				if (tlmultilist==NULL) goto i_control2_fertig;
				CDXMLREAD_functype tldummy;
				ibufferpos=(char*)((*tlmultilist).pointer);
				float tlpx,tlpy;
				if (tlmultilist!=NULL)
				{
					for (int ilv2=0;ilv2<(*tlmultilist).filllevel;ilv2++)
					{
						if ((*((basic_instance*)(ibufferpos+isize*ilv2))).exist)
						{
							int ilv3=0;
							i_control2_back:
							if (retrievepoints_basic((basic_instance*)(ibufferpos+isize*ilv2),&tlpx,&tlpy,ilv3,ilv1)>0)
							{
								if ((tlpx>=selection_frame.startx) && (tlpx<=selection_frame.endx))
								{
									if ((tlpy>=selection_frame.starty) && (tlpy<=selection_frame.endy))
									{
										selection_currentselection[ilv2]|=icompare;
										selection_currentselection_found|=icompare;
									}
								}
								ilv3++;
								goto i_control2_back;
							}
						}
					}
				}
				i_control2_fertig:;
			}
			break;
		}
		case 5:
		{
			if (rectifyselectionframe())
			{
				if (control_lastmousebutton==SDL_BUTTON_LEFT)
				{
					SDL_scrollx=selection_frame.startx;
					SDL_scrolly=selection_frame.starty;
					SDL_zoomx=gfx_canvassizex/(selection_frame.endx-selection_frame.startx);
					SDL_zoomy=gfx_canvassizey/(selection_frame.endy-selection_frame.starty);
					if (SDL_zoomx>SDL_zoomy){SDL_zoomx=SDL_zoomy;} else {SDL_zoomy=SDL_zoomx;}
				}
				else
				{
					float tl_actualposx=(selection_frame.startx-SDL_scrollx)*SDL_zoomx;
					float tl_actualposy=(selection_frame.starty-SDL_scrolly)*SDL_zoomy;
					SDL_zoomx=(selection_frame.endx-selection_frame.startx)/(gfx_canvassizex)*SDL_zoomx*SDL_zoomx;
					SDL_zoomy=(selection_frame.endy-selection_frame.starty)/(gfx_canvassizey)*SDL_zoomy*SDL_zoomy;
					if (SDL_zoomx>SDL_zoomy){SDL_zoomx=SDL_zoomy;} else {SDL_zoomy=SDL_zoomx;}
					SDL_scrollx-=tl_actualposx/SDL_zoomx;
					SDL_scrolly-=tl_actualposy/SDL_zoomy;
				}
			}
			else
			{
				CONTROL_ZOOMIN(1.414213562,(control_lastmousebutton==SDL_BUTTON_RIGHT)*2-1);
			}
			break;
		}
		case 7:
		{
			if (control_dragged==0)
			{
				int tlatom=0;
				if (control_id!=-1)
				{
					madeitmyself1:
					if (edit_locatebyid(STRUCTURE_OBJECTTYPE_n,control_id,&tlatom))
					{
						edit_errichten(tlatom);
					}
				}
				else
				{
					n_instance * tl_atom=summonatom();
					if (tl_atom)
					{
						(*tl_atom).xyz.x=control_startx;
						(*tl_atom).xyz.y=control_starty;
						(*tl_atom).xyz.z=0;
						(*tl_atom).Z=edit_getnewZ();
						control_id=(*tl_atom).id;
						goto madeitmyself1;
					}
				}
			}
			checkupinconsistencies();
		}
	}
	control_mousestate=0;
	return;
}
int issuemenuclick(AUTOSTRUCT_PULLOUTLISTING_ * ilisting,int icount,int posx,int posy,int button,int pixeloriginposx,int pixeloriginposy,int starthitnr=-1)
{
	int ihitnr=0;
	AUTOSTRUCT_PULLOUTLISTING_ * ipulloutlisting=NULL;
	if (starthitnr!=-1)
	{
		ihitnr=starthitnr;
		goto ifound;
	}
	for (int ilv1=0;ilv1<icount;ilv1++)
	{
		if ((ilisting[ilv1].x==posx) && (ilisting[ilv1].y==posy))
		{
			ipulloutlisting=&(ilisting[ilv1]);
			ihitnr=ilv1;
			goto ifound;
		}
	}
	for (int ilv1=0;ilv1<icount;ilv1++)
	{
		if (ilisting[ilv1].lmbmode==0x103)
		{
			if (ilisting[ilv1].y==posy)
			{
				if ((ilisting[ilv1].x<=posx) && (ilisting[ilv1].x+(gfx_canvassizex/32)>posx))
				{
					ipulloutlisting=&(ilisting[ilv1]);
					ihitnr=ilv1;
					goto ifound;
				}
			}
		}
	}
	ifound:;
	if (ipulloutlisting)
	{
		switch(button)
		{
			case SDL_BUTTON_LEFT:
			{
				switch ((*ipulloutlisting).lmbmode)
				{
					case 1: clickabilitymatrix_tooldependent[control_tool]=selection_clickabilitymatrix;control_tool=(*ipulloutlisting).toolnr;selection_clickabilitymatrix=clickabilitymatrix_tooldependent[control_tool];break;
					case 2: *((char*)(*ipulloutlisting).variable)^=1;break;
					case 3: (*ipulloutlisting).LMB_function("","");break;
					case 4:
					{
						_u32 * tltl=(_u32 *)((*ipulloutlisting).variable);
						if ((*tltl) & 1) *tltl=0; else *tltl=~0;
						break;
					}
					case 5: 
					{
						_u32 * tltl=(_u32 *)((*ipulloutlisting).variable);
						(*tltl)^=(1<<ihitnr);
						break;
					}
					case 6:
					{
						if (*((_i32*)((*ipulloutlisting).variable))!=(*ipulloutlisting).toolnr)
						{
							*((_i32*)((*ipulloutlisting).variable))=(*ipulloutlisting).toolnr;
						}
						else
						{
							*((_i32*)((*ipulloutlisting).variable))=0;
						}
						break;
					}
					case 7: 
					{
						if ((*ipulloutlisting).LMB_function(control_nextfilename,control_nextfiletype))
						{
							strcpy(control_filename,control_nextfilename);
							strcpy(control_filetype,control_nextfiletype);
						}
						break;
					}
					default:
					{
						if ((((*ipulloutlisting).lmbmode) & (~0xFF))==0x100)
						{
							control_mousestate|=0x20;
							control_firstmenux=pixeloriginposx;
							control_firstmenuy=pixeloriginposy;
							control_lastmenux=pixeloriginposx;
							control_lastmenuy=pixeloriginposy;
							control_usingmousebutton=button;
							control_menuitem=ipulloutlisting;
							control_menudragint=0;
						}
					}
				}
				break;
			}
			case SDL_BUTTON_RIGHT:
			{
				switch ((*ipulloutlisting).rmbmode)
				{
					case 1: control_tool=(*ipulloutlisting).toolnr;break;
					case 2: *((char*)(*ipulloutlisting).variable)&=~1;break;
					case 3: (*ipulloutlisting).RMB_function("","");break;
					case 4:
					{
						control_mousestate=8;
						menu_matrixsubmenuvariable=(*ipulloutlisting).name;
						break;
					}
					case 6: *((_i32*)((*ipulloutlisting).variable))=0;break;
					default:
					{
						if ((((*ipulloutlisting).rmbmode) & (~0xFF))==0x100)
						{
							control_mousestate|=0x20;
							control_firstmenux=pixeloriginposx;
							control_firstmenuy=pixeloriginposy;
							control_lastmenux=pixeloriginposx;
							control_lastmenuy=pixeloriginposy;
							control_usingmousebutton=button;
							control_menuitem=ipulloutlisting;
							control_menudragint=0;
						}
					}
				}
				break;
			}
		}
		return 1;
	}
	return 0;
}
int sdl_commonmenucommon();
int issuepseclick(int x,int y,int ibutton)
{
	for (int ilv1=0;ilv1<sizeof(element)/sizeof(element_);ilv1++)
	{
		if ((x==element[ilv1].PSEX) && (y==element[ilv1].PSEY))
		{
			control_drawproperties.Element=ilv1;
			control_mousestate&=(~0x10);
			int icompare=(1<<STRUCTURE_OBJECTTYPE_n);
			if (ibutton==SDL_BUTTON_LEFT)
			{
				selection_recheck(selection_currentselection,&selection_currentselection_found);
				if (selection_currentselection_found & icompare)
				{
					storeundo(icompare);
					for (int ilv2=0;ilv2<(*glob_n_multilist).filllevel;ilv2++)
					{
						if (selection_currentselection[ilv2] & icompare)
						{
							if (((*glob_n_multilist).bufferlist+ilv2)->exist)
							{
								LABELTEXT("",element[ilv1].name,glob_n_multilist,(*glob_n_multilist).bufferlist+ilv2,ilv2);
								//It is by reason that uninterpreted text labels are not overwritten!
							}
						}
					}
					return 1;
				}
				if (control_tool!=7)
				{
					control_tool=6;
				}
			}
			return 1;
		}
	}
	control_mousestate&=(~0x10);
	if (ibutton==SDL_BUTTON_LEFT)
	{
		control_drawproperties.Element=constants_Element_implicitcarbon;
	}
	return 0;
}
int issuerectclick(AUTOSTRUCT_PULLOUTLISTING_ * ilisting,int icount,int iposx,int iposy,int ibutton)
{
	int ihitnr=0;
	AUTOSTRUCT_PULLOUTLISTING_ * ipulloutlisting=NULL;
	for (int ilv1=0;ilv1<icount;ilv1++)
	{
		if ((ilisting[ilv1].x>=iposx) && (ilisting[ilv1].y>=iposy) &&
		(ilisting[ilv1].maxx>=iposx) && (ilisting[ilv1].maxy>=iposy))
		{
			ipulloutlisting=&(ilisting[ilv1]);
			ihitnr=ilv1;
			goto ifound;
		}
	}
	return 0;
	ifound:;
	issuemenuclick(ilisting,icount,iposx,iposy,iposx,iposy,ibutton,ihitnr);
	return 1;
}
void issuemenuclicks(int iposx,int iposy,int ibutton)
{
	int tlsuccess=0;
	for (int ilv1=menu_list_count;ilv1>=0;ilv1--)
	{
		if (((iposx-menu_list[ilv1].alignx)>=0) && ((iposy-menu_list[ilv1].aligny)>=0))
		{
			switch (menu_list[ilv1].type)
			{
				case 0:tlsuccess|=issuemenuclick((AUTOSTRUCT_PULLOUTLISTING_*)menu_list[ilv1].what.pointer,menu_list[ilv1].what.count,(iposx-menu_list[ilv1].alignx)/32,(iposy-menu_list[ilv1].aligny)/32,ibutton,iposx,iposy);break;
				case 1:tlsuccess|=issuemenuclick((AUTOSTRUCT_PULLOUTLISTING_*)menu_list[ilv1].what.pointer,menu_list[ilv1].what.count,(iposx-menu_list[ilv1].alignx)/192,(iposy-menu_list[ilv1].aligny)/16,ibutton,iposx,iposy);break;
				case 2: tlsuccess|=issuepseclick((iposx-menu_list[ilv1].alignx)/32,(iposy-menu_list[ilv1].aligny)/48,ibutton);break;
				case 3: tlsuccess|=issuerectclick((AUTOSTRUCT_PULLOUTLISTING_*)menu_list[ilv1].what.pointer,menu_list[ilv1].what.count,iposx-menu_list[ilv1].alignx,iposy-menu_list[ilv1].aligny,ibutton);break;
				case 4: tlsuccess|=issuerectclick((AUTOSTRUCT_PULLOUTLISTING_*)menu_list[ilv1].what.pointer,menu_list[ilv1].what.count,iposx-menu_list[ilv1].alignx,iposy-menu_list[ilv1].aligny,ibutton);break;
			}
		}
		if (tlsuccess)
		{
			goto ifertig;
		}
	}
	ifertig:
	if (tlsuccess==0)
	{
		//TODO what about control_mousestate=16 (ordinary popup)
		if (control_mousestate & 8)
		{
			control_mousestate=control_mousestate & (~8);
		}
	}
}
long long counter_SDL;
void issuemenudrag(int posx,int posy,char ifinal=0)
{
	int diffx,diffy;
	switch ((*control_menuitem).lmbmode)
	{
		case 0x111:
		{
			if ((ifinal) && (posx>=0) && (posy>=0) && (posx<control_firstmenux+32) && (posy<control_firstmenuy+32))
			{
				(*(_u32*)(*control_menuitem).variable)=0;
				break;
			}
			diffx=posx-((control_firstmenux/32)*32)-32;
			diffy=posy-((control_firstmenuy/32)*32)-64;
			if (diffy<0) diffy=0;if (diffy>255) diffy=255;
			if (diffx<0) diffx=0;if (diffx>255) diffx=255;
			if (control_usingmousebutton==SDL_BUTTON_LEFT)
			{
				(*(_u8*)(*control_menuitem).variable)=diffx;
				(*(((_u8*)(*control_menuitem).variable)+1))=diffy;
			}
			else
			{
				(*(((_u8*)(*control_menuitem).variable)+2))=diffx;
				(*(((_u8*)(*control_menuitem).variable)+3))=diffy;
			}
			break;
		}
		case 0x103:
		{
			diffx=posx-control_lastmenux;
			diffy=posy-control_lastmenuy;
			control_menudragint+=diffx;
			break;
		}
	}
	control_lastmenux=posx;
	control_lastmenuy=posy;
}
void control_normal()
{
	char idirection=1;
	control_lastmousebutton=0;
	SDL_Event control_Event2;
	while ( SDL_PollEvent(&control_Event) ) 
	{
		irepeatlabel:;
		char irepeat=0;
		char idontrepeat=0;
		switch (control_Event.type) 
		{
			case SDL_MOUSEMOTION:
			{
				control_mousex=control_Event.motion.x;
				control_mousey=control_Event.motion.y;
				if (control_mousestate & 0x20)
				{
					sdl_commonmenucommon();
					issuemenudrag(control_Event.motion.x,control_Event.motion.y);
				}

				if (SDL_PollEvent(&control_Event2))
				{
					if (control_Event2.type==SDL_MOUSEMOTION)
					{
						goto irepeatlabel;
					}
					else
					{
						irepeat=1;//TODO: due the now changed event,, this issues MOUSEMOTION's with illegal coordinates.
					}
				}
				if ((control_mousestate & (~2))==0)
				{
					selection_clearselection(selection_clickselection);
					for (int ilv0=0;ilv0<STRUCTURE_OBJECTTYPE_ListSize;ilv0++)
					{
						_u32 tlfound=clickfor((control_Event.motion.x-gfx_canvasminx)/SDL_zoomx+SDL_scrollx,(control_Event.motion.y-gfx_canvasminy)/SDL_zoomy+SDL_scrolly,ilv0,constants_clickradius)>0;
						basicmultilist * tl_multilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv0].name);
						if (tlfound)
						{
							for (int ilv2=0;ilv2<(*tl_multilist).filllevel;ilv2++)
							{
								if (selection_clickselection[ilv2] & (1<<ilv0))
								{
									if ((*tl_multilist)[ilv2].exist)
									{
										control_hot[ilv0]=ilv2;
									}
								}
							}
						}
					}
					idontrepeat=1;
				}
				if (control_mousestate & 3)
				{
					float tlx=control_Event.motion.x-gfx_canvasminx;
					float tly=control_Event.motion.y-gfx_canvasminy;
					idontrepeat=1;
					issuedrag(tlx,tly);
				}
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
			{
				if ((control_mousestate & (~0x18))==0)
				{
					if ((control_mousestate & (24)) || (control_Event.button.x<gfx_canvasminx) || (control_Event.button.y<gfx_canvasminy) || (control_Event.button.x>=gfx_canvasmaxx) || (control_Event.button.y>=gfx_canvasmaxy))
					{
						sdl_commonmenucommon();
						issuemenuclicks(control_Event.button.x,control_Event.button.y,control_Event.button.button);
						break;
					}
				}
				switch (control_Event.button.button)
				{
					case SDL_BUTTON_RIGHT:
					{
						control_lastmousebutton=SDL_BUTTON_RIGHT;
						goto clickshunt;
					}
					case SDL_BUTTON_LEFT:
					{
						control_lastmousebutton=SDL_BUTTON_LEFT;
						clickshunt:
						if (control_mousestate==0)
						{
							issueclick(control_Event.button.x-gfx_canvasminx, control_Event.button.y-gfx_canvasminy);
						}
						break;
					}
					case SDL_BUTTON_WHEELUP:
					{
						idirection=-1;
					}
					case SDL_BUTTON_WHEELDOWN://FALLTHROUGH
					{
						if (MODIFIER_KEYS.CTRL)
						{
							float tl_factor=1.414213562;
							if (MODIFIER_KEYS.ALT)
							{
								tl_factor=1.090507733;
							}
							CONTROL_ZOOMIN(tl_factor,idirection);
							break;
						}
						if (MODIFIER_KEYS.SHIFT)
						{
							SDL_scrollx+=idirection*70.0/SDL_zoomx;
						}
						else
						{
							SDL_scrolly+=idirection*70.0/SDL_zoomy;
						}
						break;
					}
				}
				break;
			}
			case SDL_MOUSEBUTTONUP:
			{
				switch (control_Event.button.button)
				{
					case SDL_BUTTON_RIGHT:
					{
						control_lastmousebutton=SDL_BUTTON_RIGHT;
						goto dragshunt;
					}
					case SDL_BUTTON_LEFT:
					{
						control_lastmousebutton=SDL_BUTTON_LEFT;
						dragshunt:
						if (control_mousestate==1)
						{
							if (control_usingmousebutton==control_lastmousebutton)//Only when same button up as down
							{
								issuedrag(control_Event.motion.x-gfx_canvasminx, control_Event.motion.y-gfx_canvasminy);
								issuerelease();
							}
						}
						if (control_mousestate & 0x20)
						{
							if (control_usingmousebutton==control_lastmousebutton)//Only when same button up as down
							{
								issuemenudrag(control_Event.motion.x,control_Event.motion.y,1);
								control_mousestate&=~0x20;
							}
						}
						break;
					}
				}
				break;
			}
			case SDL_KEYUP:
			{
				if (control_Event.key.keysym.sym==control_toolstartkeysym)
				{
					if (control_mousestate == 2)
					{
						issuedrag(control_mousex-gfx_canvasminx, control_mousey-gfx_canvasminy);
						control_mousestate=0;
						break;
					}
				}
				goto sdl_keyup_fallthrough;
				break;
				sdl_keyup_fallthrough:;
				idirection=0;
			}
			case SDL_KEYDOWN://FALLTHROUGH
			{
				switch (control_Event.key.keysym.sym)
				{
					case SDLK_RCTRL:
					case SDLK_LCTRL://FALLTHROUGH
					{
						MODIFIER_KEYS.CTRL=idirection;
						break;
					}
					case SDLK_RALT:
					case SDLK_LALT://FALLTHROUGH
					{
						fallthrough:
						MODIFIER_KEYS.ALT=idirection;
						break;
					}
					case SDLK_RSHIFT:
					case SDLK_LSHIFT://FALLTHROUGH
					{
						MODIFIER_KEYS.SHIFT=idirection;
						break;
					}
					case SDLK_RSUPER:
					case SDLK_LSUPER://FALLTHROUGH
					{
						MODIFIER_KEYS.SUPER=idirection;
						break;
					}
					case SDLK_F11:
					{
						if (idirection==1)
						{
							SDL_WM_ToggleFullScreen(video);
						}
						break;
					}
					case SDLK_ESCAPE:
					{
						if (idirection==1)
						{
							LHENDRAW_leave=1;
						}
						break;
					}
					case SDLK_SPACE:
					{
						if (control_mousestate==0)
						{
							issueshiftstart();
							control_keycombotool=1;
							control_mousestate=2;
						}
						break;
					}
					default:
					if (idirection==1)
					{
						if (control_mousestate==0)
						{
							interpretkey();
						}
					}
				}
				break;
			}
			case SDL_QUIT:
			{
				LHENDRAW_leave=1;
				break;
			}
		}
		if (irepeat)
		{
			control_Event=control_Event2;
			goto irepeatlabel;
		}
		if (idontrepeat)
		{
			goto iloopendlabel;
		}
	}
	iloopendlabel:;
	if (LHENDRAW_leave==1)
	{
		if (video->flags & SDL_FULLSCREEN)
		{
			SDL_WM_ToggleFullScreen(video);
		}
	}
}
