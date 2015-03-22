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
int control_mousestate=0;//0: inactive; 0x1: from tool, mouseclick; 0x2: from special tool, keyboard 0x4: on menu, dragging 0x8: on button_function dependent menu, popup 0x10 popup-menu or PSE, multiple levels 0x20 dragging menuitem 0x40: text editing
int control_toolaction=0;//1: move 2: move selection 3: tool specific
int control_tool=2;//1: Hand 2: 2coordinate Selection 3: Lasso, no matter which 4: Shift tool 5: Magnifying glass 6: Element draw 7: chemdraw draw 8: eraser 9: Arrows 10: attributes 11: text tool 12: bezier 13: image 14: spectrum 15: tlc plate/gel plate 16: graphic 17: aromatic ring tool 18: Arrow_skip 19: Arrow_situp
int control_menumode=0;//1: shliderhorz, 2: slidervert 3: colorchooser
AUTOSTRUCT_PULLOUTLISTING_ * control_menuitem=NULL;
#define control_toolcount 20
int control_keycombotool=0;//as above, but only valid if (mousestate & 2)
_i32 control_toolstartkeysym;
int control_lastinterpret=-1;
int control_posx=0;
int control_posy=0;
long long control_id=-1;
float control_coorsx=0;
float control_coorsy=0;
int control_mousex,control_mousey;
char control_reticle=1;
char control_dragged=0;
int control_textedit_type=0;
int control_textedit_index=0;
int control_textedit_telescope=0;
_uXX control_textedit_cursor=0;
int control_textedit_selectmode=0;
char control_lastmousebutton=0;
char control_usingmousebutton=0;
char control_clickforpriority=0;//0: frontmost 1: rearmost 2: nearest 3: nearest3d
char control_ambiguousity=0;
float control_startx=0;
float control_starty=0;
int control_menudragint=0;
clickabilitymatrix_ control_clickabilitymatrixes[control_toolcount];

char control_filename[stringlength]="";
char control_filetype[stringlength]=".cdx";
char control_nextfilename[stringlength]="";
char control_nextfiletype[stringlength]=".cdx";
int control_force=0;
int control_interactive=1;
int control_saveuponexit=0;
int control_GUI=1;
int control_doubleclickenergy=0;
basic_instance * control_manipulatedinstance;
basic_instance * control_manipulatedinstance2;
#define KEYDEPENDENTSELECTION \
{\
	__label__ tl_found;\
	if (MODIFIER_KEYS.SHIFT)\
	{\
		if (MODIFIER_KEYS.CTRL)\
		{\
			selection_ANDselection(selection_currentselection,selection_clickselection);\
		}\
		else\
		{\
			selection_XORselection(selection_currentselection,selection_clickselection);\
		}\
		selection_recheck(selection_currentselection,&selection_currentselection_found);\
		goto tl_found;\
	}\
	if (MODIFIER_KEYS.CTRL)\
	{\
		selection_ORselection(selection_currentselection,selection_clickselection);\
		goto tl_found;\
	}\
	selection_copyselection(selection_currentselection,selection_clickselection);\
	selection_currentselection_found=selection_clickselection_found;\
	tl_found:;\
}
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
void CONTROL_ZOOMIN(float ifactor,char i_direction)
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
	for (int ilv1=0;ilv1<(*glob_t_multilist).filllevel;ilv1++)
	{
		if ((*glob_t_multilist)[ilv1].exist)
		{
			edit_textlength(glob_t_multilist,ilv1);
		}
	}
/*	for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
	{
		for (int ilv2=ilv1+1;ilv2<(*glob_n_multilist).filllevel;ilv2++)
		{
			if ((*glob_n_multilist)[ilv1].id==(*glob_n_multilist)[ilv2].id)
			{
				printf("%i==%i;%i\n",ilv1,ilv2,(*glob_n_multilist)[ilv1].id);
			}
		}
	}*/
	for (int ilv1=0;ilv1<(*glob_b_multilist).filllevel;ilv1++)//removes defective bonds
	{
		b_instance * tl_b_instance=&((*glob_b_multilist)[ilv1]);
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
		n_instance * tl_n_instance=&((*glob_n_multilist)[ilv1]);
		if ((*tl_n_instance).exist)
		{
			if (atom_actual_node[ilv1].bondcount==0)
			{
				if ((*tl_n_instance).Element==constants_Element_implicitcarbon)
				{
					TELESCOPE_aggressobject(glob_n_multilist,ilv1);
					TELESCOPE_clear();
					(*tl_n_instance).exist=0;
				}
			}
			else
			{
				if ((*tl_n_instance).Element==constants_Element_implicitcarbon)
				{
					(*tl_n_instance).protons=4-abs((*tl_n_instance).charge);
				}
			}
		}
	}
	for (int ilv2=0;ilv2<(*glob_b_multilist).filllevel;ilv2++)
	{
		if ((*glob_b_multilist)[ilv2].exist)
		{
			int masamune=bond_actual_node[ilv2].end;
			int murasame=bond_actual_node[ilv2].start;
			if ((*glob_n_multilist)[masamune].exist==0)
			{
				(*glob_b_multilist)[ilv2].exist=0;
				continue;
			}
			if ((*glob_n_multilist)[murasame].exist==0)
			{
				(*glob_b_multilist)[ilv2].exist=0;
				continue;
			}
			for (int ilv3=0;ilv3<atom_actual_node[murasame].bondcount;ilv3++)
			{
				int thisbond=atom_actual_node[murasame].bonds[ilv3];
				if (thisbond!=ilv2)
				{
					if ((*glob_b_multilist)[thisbond].exist)
					{
						if (masamune==getother(murasame,thisbond))
						{
							//would be nice to add the properties of both bonds, but may cause invalid property combinations
							TELESCOPE_aggressobject(glob_b_multilist,thisbond);
							TELESCOPE_clear();
							if ((*glob_b_multilist)[ilv2].B!=(*glob_b_multilist)[thisbond].B)
							{
								(*glob_b_multilist)[ilv2].B=(*glob_b_multilist)[thisbond].B;
								(*glob_b_multilist)[ilv2].E=(*glob_b_multilist)[thisbond].E;
							}
							(*glob_b_multilist)[ilv2].Order=max((*glob_b_multilist)[ilv2].Order,(*glob_b_multilist)[thisbond].Order);
							(*glob_b_multilist)[ilv2].Display=(*glob_b_multilist)[thisbond].Display;
							(*glob_b_multilist)[ilv2].color=(*glob_b_multilist)[thisbond].color;
							(*glob_b_multilist)[ilv2].DoublePosition=(*glob_b_multilist)[thisbond].DoublePosition;
							if (((*glob_b_multilist)[ilv2].Order & 0xF)>8)
							{
								if ((*glob_b_multilist)[ilv2].Display2==0) {(*glob_b_multilist)[ilv2].Display2=1;}
							}
							(*glob_b_multilist)[thisbond].exist=0;
						}
					}
				}
			}
		}
	}
	selection_recheck(selection_currentselection,&selection_currentselection_found);
}
void issueshiftstart()
{
	control_posx=control_mousex-gfx_canvasminx;
	control_posy=control_mousey-gfx_canvasminy;
	control_coorsx=control_posx/SDL_zoomx+SDL_scrollx;
	control_coorsy=control_posy/SDL_zoomy+SDL_scrolly;
	control_dragged=0;
	control_toolstartkeysym=control_Event.key.keysym.sym;
}
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
			selection_clickselection_found|=clickfor(control_coorsx,control_coorsy,STRUCTURE_OBJECTTYPE_n,iclickradius,1)<<STRUCTURE_OBJECTTYPE_n;
			selection_clickselection_found|=clickfor(control_coorsx,control_coorsy,STRUCTURE_OBJECTTYPE_b,iclickradius,1)<<STRUCTURE_OBJECTTYPE_b;
			for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
			{
				
			}
//			selectwholestructure
		}
	}
	if (selection_clickabilitymatrix.mode==2)
	{
		edit_clickpixels(control_coorsx,control_coorsy);
		for (int ilv1=1;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
		{
			if (selection_clickabilitymatrix.types2[1] & (1<<ilv1))
			{
				selection_clickselection_found|=clickfor(control_coorsx,control_coorsy,ilv1,iclickradius,1);
			}
			if (selection_clickabilitymatrix.types2[2] & (1<<ilv1))
			{
				selection_clickselection_found|=clickfor(control_coorsx,control_coorsy,ilv1,iclickradius,2);
			}
		}
	}
}
#ifdef GFXOUT_SDL
int interpretkey(int listnr=-1)
{
	_u32 modifierpattern;
	char undostored=0;
	int ilv1,ilv2;
	_u32 tltype;
	char keystring[4]={0,0,0,0};
	char erledigt=0;
	char ihot=0;
	char careaboutshift=1;
	int ihot_list=0;
	int ihot_instance=0;
	_u32 i_selectedtypes=0;
	getatoms();
	edit_singlepointselected=0;
	if (listnr!=-1) {ilv1=listnr;goto interpreted;}
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
		_u16 ihv1=getunicode(&control_Event);
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
	i_selectedtypes=selection_currentselection_found;
	ihot_list=0;
	ihot_instance=0;
	if (i_selectedtypes==0)
	{
		int backtype,backindex;
		control_posx=control_mousex-gfx_canvasminx;
		control_posy=control_mousey-gfx_canvasminy;
		control_coorsx=control_posx/SDL_zoomx+SDL_scrollx;
		control_coorsy=control_posy/SDL_zoomy+SDL_scrolly;
		clickforthem();
		if (selection_clickselection_found)
		{
			if (getclicked((1<<STRUCTURE_OBJECTTYPE_ListSize)-1,control_coorsx,control_coorsy,&ihot_list,&ihot_instance))
			{
				i_selectedtypes=1<<ihot_list;
				ihot=1;
			}
		}
		else
		{
			if (gethot()!=-1)
			{
				i_selectedtypes=1<<STRUCTURE_OBJECTTYPE_n;
				ihot=1;
				ihot_list=STRUCTURE_OBJECTTYPE_n;
				ihot_instance=control_hotatom;
			}
		}
	}
	modifierpattern=MODIFIER_KEYS.SHIFT*careaboutshift+MODIFIER_KEYS.CTRL*2+MODIFIER_KEYS.ALT*4+MODIFIER_KEYS.SUPER*8;
	for (ilv1=0;ilv1<hotkeylist_count;ilv1++)
	{
		tltype=hotkeylist[ilv1].type;
		if (((1<<(tltype & 0xFFFF)) & i_selectedtypes) || ((tltype & 0xFFFF)==0))
		{
			if (((i_selectedtypes==0) || ((tltype & 0x10000)==0)) && ((i_selectedtypes) || ((tltype & 0x20000)==0)))
			{
				if (strcmp(keystring,hotkeylist[ilv1].key)==0)
				{
					if ((hotkeylist[ilv1].modifiers & ((~((_u32)(1-careaboutshift)))))==modifierpattern)
					{

						if ((undostored==0) && (hotkeylist[ilv1].UNDO & 1)) {storeundo(~0); undostored=1;}
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
								_u32 icompare=(1<<(tltype & 0xFFFF));
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
									if (tltype==ihot_list)
									{
										ilv2=ihot_instance;
										goto hotshunt;
										hotbackshunt:;
									}
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
	return 0;
	commanderledigt:;
	checkupinconsistencies();
	return 1;
}
#endif
catalogized_command_funcdef(HELP)
{
	control_help();
	return 1;
}
catalogized_command_funcdef(UNDOTREE)
{
	return 1;
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
						if (ilv1==1) if (((*((n_instance*)(ibufferpos+isize*ilv2))).Element!=constants_Element_implicitcarbon) || ((*((n_instance*)(ibufferpos+isize*ilv2))).charge!=0))
						{
							(*((n_instance*)(ibufferpos+isize*ilv2))).Element=constants_Element_implicitcarbon;
							(*((n_instance*)(ibufferpos+isize*ilv2))).charge=0;
							goto i_delete_skip;
						}
						(*((basic_instance*)(ibufferpos+isize*ilv2))).exist=0;
					}
					i_delete_skip:;
				}
			}
			i_delete_fertig:;
		}
		isuccessful=1;
	}
	else
	{
		if (control_hotatom!=-1)
		{
			if (control_hotatom<(*glob_n_multilist).filllevel)
			{
				ibufferpos=(char*)(*glob_n_multilist).pointer;
				isize=STRUCTURE_OBJECTTYPE_List[STRUCTURE_OBJECTTYPE_n].size;
				if ((*glob_n_multilist)[control_hotatom].exist)
				{
					TELESCOPE_aggressobject(glob_n_multilist,control_hotatom);
					TELESCOPE_clear();
					if (((*((n_instance*)(ibufferpos+isize*control_hotatom))).Element!=constants_Element_implicitcarbon) || ((*((n_instance*)(ibufferpos+isize*control_hotatom))).charge!=0))
					{
						(*((n_instance*)(ibufferpos+isize*control_hotatom))).Element=constants_Element_implicitcarbon;
						(*((n_instance*)(ibufferpos+isize*control_hotatom))).charge=0;
						goto i_delete_hot_skip;
					}
					(*((basic_instance*)(ibufferpos+isize*control_hotatom))).exist=0;
					i_delete_hot_skip:;
					isuccessful=1;
				}
			}
			control_hotatom=-1;
		}
	}
	checkupinconsistencies();
	return isuccessful;
}
_u32 upfrom(int input)
{
	_u32 wert=0;
	_u32 tracker=1;
	for (int ilv1=0;ilv1<32;ilv1++)
	{
		if (ilv1>=input)
		{
			wert|=tracker;
		}
		tracker=tracker<<1;
	}
	return wert;
}
char arbitrarycursorstring[4]="\uE000";
int control_aggresstextcursor(const char * cursorname="\uE000")//searches the text cursor and places telescope_tempvar on it. must return 1 if present, and 0 if no cursor present
{
	if (cursorname==NULL)
	{
		cursorname=arbitrarycursorstring;
	}
	basicmultilist * tl_multilist=findmultilist(STRUCTURE_OBJECTTYPE_List[control_textedit_type].name);
	if ((*tl_multilist)[control_textedit_index].exist)
	{
		TELESCOPE_aggressobject(tl_multilist,control_textedit_index);
		control_textedit_telescope=0;
		int tl_backval=TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_s);
		iback:
		if (tl_backval)
		{
			char * tl_buffer=(char*)TELESCOPE_getproperty_contents();
			control_textedit_cursor=(_uXX)strstr(tl_buffer,cursorname);
			if (cursorname==arbitrarycursorstring)
			{
				arbitrarycursorstring[2]^=1;//try the other one, too...
				_uXX control_textedit_cursor2=(_uXX)strstr(tl_buffer,cursorname);
				if ((control_textedit_cursor2!=(_uXX)NULL))
				{
					if (control_textedit_cursor!=(_uXX)NULL)
					{
						int reversed=(control_textedit_cursor>control_textedit_cursor2);
						if (reversed)
						{
							((char*)control_textedit_cursor)[2]=0x81;
							((char*)control_textedit_cursor2)[2]=0x80;
							control_textedit_cursor=control_textedit_cursor2;
						}
					}
					else
					{
						control_textedit_cursor=control_textedit_cursor2;
					}
				}
			}
			if (control_textedit_cursor==0)
			{
				control_textedit_telescope++;
				tl_backval=TELESCOPE_searchthroughobject_next(TELESCOPE_ELEMENTTYPE_s);
				goto iback;
			}
			int returnvalue=(strncmp((char*)control_textedit_cursor,"\uE001",3)==0)?2:1;
			control_textedit_cursor-=(_uXX)tl_buffer;
			return returnvalue;
		}
		return 0;
	}
	return 0;
}
int control_swaptextcursor()
{
	if (control_aggresstextcursor(arbitrarycursorstring)==2)
	{
		((char*)TELESCOPE_getproperty_contents())[control_textedit_cursor+2]=0x80;//Third byte
		while (TELESCOPE_searchthroughobject_next(TELESCOPE_ELEMENTTYPE_s)>0)
		{
			char * tl_backstring=strstr(((char*)TELESCOPE_getproperty_contents()),"\uE000");
			if (tl_backstring)
			{
				tl_backstring[2]=0x81;//Third byte
				return 1;
			}
		}
	}
	return 0;
}
int control_splitattextcursor()
{
	char count=0;
	if (control_aggresstextcursor("\uE000"))
	{
		if (control_textedit_cursor!=0)
		{
			TELESCOPE_split(control_textedit_cursor,"",1);
			count++;
		}
	}
	if (control_aggresstextcursor("\uE001"))
	{
		if (control_textedit_cursor!=0)
		{
			TELESCOPE_split(control_textedit_cursor,"",1);
			count+=2;
		}
		control_swaptextcursor();
	}
	return count;
}
int control_squashselection()
{
	char * icursor;
	control_splitattextcursor();
	control_aggresstextcursor("\uE000");
	control_textedit_selectmode=0;
	icursorback:
	icursor=(char*)TELESCOPE_getproperty_contents();
	if (strstr(icursor,"\uE001")==NULL)
	{
		TELESCOPE_clear_item();
		goto icursorback;
	}
	if ((control_aggresstextcursor("\uE001"))<=0)
	{
		return 0;
	}
	icursor=(char *)TELESCOPE_getproperty_contents();
	icursor[0]=0xEE;icursor[1]=0x80;icursor[2]=0x80;
	return 1;
}
void issuetextclick(int iposx,int iposy,const char * whichcursor="\uE000")
{
	control_posx=iposx;
	control_posy=iposy;
	control_coorsx=iposx/SDL_zoomx+SDL_scrollx;
	control_coorsy=iposy/SDL_zoomy+SDL_scrolly;
	int tl_t_nr;
	int tl_backval;
	int tl_size;
	basicmultilist * tl_multilist=findmultilist(STRUCTURE_OBJECTTYPE_List[control_textedit_type].name);
	if ((*tl_multilist)[control_textedit_index].exist)
	{
		if (control_aggresstextcursor(whichcursor))
		{
			TELESCOPE_shrink(control_textedit_cursor,3);
		}
		control_textedit_selectmode=(strcmp(whichcursor,"\uE001")==0);
		int tl_distance=0;
		control_mousestate=0x42;
		if (edit_textlength(tl_multilist,control_textedit_index,control_coorsx,control_coorsy,&tl_distance)<=0)
		{
			TELESCOPE_aggressobject(tl_multilist,control_textedit_index);
			tl_backval=TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_s);
			if (tl_backval==0) {control_mousestate=0;return;}
/*			control_mousestate=0x40;
			control_textedit_selectmode=0;*/
			tl_distance=-1;
		}
		TELESCOPE_insertintoproperties_offset((char*)whichcursor,3,tl_distance);
		return;
	}
}
int issueclick(int iposx,int iposy)
{
	int ibackval;
	int icompare;
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
	if (control_tool>=control_toolcount)
	{
		fprintf(stderr,"Error! tool number out of range!\n");
	}
	edit_singlepointselected=0;
	switch (control_tool)
	{
		case 2:
		{
			if (control_lastmousebutton==SDL_BUTTON_LEFT)
			{
				if ((selection_clickselection_found) && (MODIFIER_KEYS.ALT==0))
				{
					control_mousestate=2;
					control_toolstartkeysym=SDLK_UNKNOWN;
					control_keycombotool=4;
					control_startx=control_coorsx;
					control_starty=control_coorsy;
					control_usingmousebutton=control_lastmousebutton;
					int backtype,backindex;
					float backdistance=2000000000;
					control_manipulatedinstance=getclicked((1<<STRUCTURE_OBJECTTYPE_ListSize)-1,control_coorsx,control_coorsy,&backtype,&backindex,NULL,&backdistance);
					if (control_manipulatedinstance!=NULL)
					{
						if (backdistance*SDL_zoomx*SDL_zoomy>400) goto try_subpoint;
						goto twopointselection_wantthem;
					}
					else
					{
						try_subpoint:;
						control_manipulatedinstance=getclicked(~0,control_coorsx,control_coorsy,&backtype,&backindex);
						if (control_manipulatedinstance!=NULL)
						{
							goto twopointselection_wantthem;
						}
					}
					return 0;
					twopointselection_wantthem:;
					if ((MODIFIER_KEYS.CTRL==0) && (MODIFIER_KEYS.SHIFT==0))
					{
						for (int ilv1=STRUCTURE_OBJECTTYPE_n;ilv1<=STRUCTURE_OBJECTTYPE_b;ilv1++)
						{
							int icompare=1<<ilv1;
							for (int ilv2=0;ilv2<findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name)->filllevel;ilv2++)
							{
								if ((selection_currentselection[ilv2] & icompare) && (selection_clickselection[ilv2] & icompare)) return 0;
							}
						}
						selection_clearselection(selection_currentselection);
						selection_currentselection_found=1<<backtype;
						selection_currentselection[backindex]|=1<<backtype;
						edit_singlepointselected=1;
						return 0;
					}
					KEYDEPENDENTSELECTION;
					edit_judgeselection(backindex);
					control_mousestate=0;
					return 0;
				}
				selection_frame.startx=control_coorsx;
				selection_frame.starty=control_coorsy;
				selection_frame.endx=control_coorsx;
				selection_frame.endy=control_coorsy;
			}
			if (control_lastmousebutton==SDL_BUTTON_RIGHT)
			{
				control_mousestate=2;
				control_toolstartkeysym=SDLK_UNKNOWN;
				control_keycombotool=4;
				control_startx=control_coorsx;
				control_starty=control_coorsy;
				control_usingmousebutton=control_lastmousebutton;
				if ((selection_currentselection_found==0) && (selection_clickselection_found!=0))
				{
					control_manipulatedinstance=getclicked((1<<STRUCTURE_OBJECTTYPE_n),control_coorsx,control_coorsy);
					selection_copyselection(selection_currentselection,selection_clickselection);
				}
				return 0;
			}
			break;
		}
		case 3:
		{
			if (control_lastmousebutton==SDL_BUTTON_LEFT)
			{
				if ((selection_clickselection_found) && (MODIFIER_KEYS.ALT==0))
				{
					control_mousestate=2;
					control_toolstartkeysym=SDLK_UNKNOWN;
					control_keycombotool=4;
					control_startx=control_coorsx;
					control_starty=control_coorsy;
					control_usingmousebutton=control_lastmousebutton;
					int backtype,backindex;
					control_manipulatedinstance=getclicked((1<<STRUCTURE_OBJECTTYPE_n),control_coorsx,control_coorsy);
					control_manipulatedinstance=getclicked((1<<STRUCTURE_OBJECTTYPE_ListSize)-1,control_coorsx,control_coorsy,&backtype,&backindex);
					if (control_manipulatedinstance!=NULL)
					{
						goto lasso_wantthem;
					}
					else
					{
						control_manipulatedinstance=getclicked(~0,control_coorsx,control_coorsy,&backtype,&backindex);
						if (control_manipulatedinstance!=NULL)
						{
							goto lasso_wantthem;
						}
					}
					return 0;
					lasso_wantthem:;
					if ((MODIFIER_KEYS.CTRL==0) && (MODIFIER_KEYS.SHIFT==0))
					{
						for (int ilv1=STRUCTURE_OBJECTTYPE_n;ilv1<=STRUCTURE_OBJECTTYPE_b;ilv1++)
						{
							int icompare=1<<ilv1;
							for (int ilv2=0;ilv2<findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name)->filllevel;ilv2++)
							{
								if ((selection_currentselection[ilv2] & icompare)==(selection_clickselection[ilv2] & icompare)) return 0;
							}
						}
						selection_clearselection(selection_currentselection);
						selection_currentselection_found=1<<backtype;
						selection_currentselection[backindex]|=1<<backtype;
						edit_singlepointselected=1;
						return 0;
					}
					KEYDEPENDENTSELECTION;
					edit_judgeselection(backindex);
					control_mousestate=0;
					return 0;
				}
			}
			if (control_lastmousebutton==SDL_BUTTON_RIGHT)
			{
				control_mousestate=2;
				control_toolstartkeysym=SDLK_UNKNOWN;
				control_keycombotool=4;
				control_startx=control_coorsx;
				control_starty=control_coorsy;
				control_usingmousebutton=control_lastmousebutton;
				return 0;
			}
			selection_lassostartx=iposx;
			selection_lassostarty=iposy;
			selection_lassoclear();
			break;
		}
		case 4:
		{
			control_manipulatedinstance=getclicked((1<<STRUCTURE_OBJECTTYPE_n),control_coorsx,control_coorsy);
			if (selection_currentselection_found==0)
			{
				selection_copyselection(selection_currentselection,selection_clickselection);
				selection_currentselection_found=selection_clickselection_found;
			}
			control_startx=control_coorsx;
			control_starty=control_coorsy;
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
				if ((MODIFIER_KEYS.ALT==0) && (selection_clickselection_found & (1<<STRUCTURE_OBJECTTYPE_n)))
				{
					for (int ilv2=0;ilv2<(*glob_n_multilist).filllevel;ilv2++)
					{
						if (selection_clickselection[ilv2] & (1<<STRUCTURE_OBJECTTYPE_n))
						{
							if ((*glob_n_multilist)[ilv2].exist)
							{
								tlatom=(*glob_n_multilist).bufferlist()+ilv2;
								edit_setelement(control_drawproperties.Element,tlatom,ilv2);
							}
						}
					}
				}
				else
				{
					int atomnr;
					tlatom=edit_summonatom(&atomnr);
					if (tlatom)
					{
						(*tlatom).xyz.x=control_coorsx;
						(*tlatom).xyz.y=control_coorsy;
						(*tlatom).xyz.z=0;
						edit_setelement(control_drawproperties.Element,tlatom,atomnr);
					}
				}
			}
			else
			{
				selection_clearselection(selection_currentselection);selection_currentselection_found=0;
				OPEN_PSE("","");
				control_mousestate=0;
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
					if (selection_currentselection_found!=0)
					{
						selection_clearselection(selection_currentselection);
						selection_currentselection_found=0;
					}
					tlbond=(b_instance*)getclicked((1<<STRUCTURE_OBJECTTYPE_b),control_coorsx,control_coorsy);
					if (tlbond==NULL) {control_mousestate=0;return 0;}
					if (control_lastmousebutton==SDL_BUTTON_RIGHT)
					{
						control_drawproperties.bond_multiplicity=(*tlbond).Order>>4;
						control_drawproperties.bond_Display1=(*tlbond).Display;
						control_drawproperties.color=(*tlbond).color;
						control_mousestate=0;
						return 0;
					}
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
						if ((control_drawproperties.bond_multiplicity==2) && (((*tlbond).Order==32)||((*tlbond).Order==24)))
						{
							SWAPDOUBLEPOSITION("","",glob_b_multilist,tlbond,0/*wrong, not important*/);
						}
						else
						{
							if (((*tlbond).Display==control_drawproperties.bond_Display1) && ((*tlbond).Order==control_drawproperties.bond_multiplicity<<4))
							{
								int tl_swap=(*tlbond).B;
								(*tlbond).B=(*tlbond).E;
								(*tlbond).E=tl_swap;
							}
							else
							{
								(*tlbond).Order=control_drawproperties.bond_multiplicity<<4;
								(*tlbond).Display=control_drawproperties.bond_Display1;
							}
						}
					}
					control_mousestate=0;
					return 0;
				}
				else
				{
					if (selection_currentselection_found!=0)
					{
						selection_clearselection(selection_currentselection);
						selection_currentselection_found=0;
					}
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
		case 10:
		{
			if (control_lastmousebutton==SDL_BUTTON_RIGHT)
			{
				icompare=(1<<(STRUCTURE_OBJECTTYPE_n+STRUCTURE_OBJECTTYPE_ListSize));
				if (selection_clickselection_found & icompare)
				{
					control_manipulatedinstance2=getclicked(icompare,control_coorsx,control_coorsy,NULL,NULL,&control_manipulatedinstance);
					if (control_manipulatedinstance2!=NULL) goto ifertig;
				}
			}
			else
			{
				if (selection_clickselection_found & (1<<STRUCTURE_OBJECTTYPE_n))
				{
					if (MODIFIER_KEYS.ALT==0)
					{
						for (int ilv2=0;ilv2<(*glob_n_multilist).filllevel;ilv2++)
						{
							if (selection_clickselection[ilv2] & (1<<STRUCTURE_OBJECTTYPE_n))
							{
								if ((*glob_n_multilist)[ilv2].exist)
								{
									if (atom_addsymbol(ilv2,control_drawproperties.attribute_tool)>=0)
									{
										//TELESCOPE_measure(TELESCOPE_ELEMENTTYPE_Symbol,glob_contentbuffer+STRUCTURE_OBJECTTYPE_n);
										control_manipulatedinstance=(basic_instance*)TELESCOPE_getproperty();
										control_manipulatedinstance2=(basic_instance*)(glob_n_multilist->bufferlist()+ilv2);
										goto ifertig;
									}
								}
							}
						}
					}
				}
			}
			control_mousestate=0;return 0;
		}
		case 11:
		{
			int tl_t_nr;
			int tl_backval;
			int ilv1;
			t_instance * i_t_instance;
			if (selection_clickselection_found & (1<<STRUCTURE_OBJECTTYPE_t))
			{
				for (ilv1=0;ilv1<glob_t_multilist->filllevel;ilv1++)
				{
					if (selection_clickselection[ilv1] & (1<<STRUCTURE_OBJECTTYPE_t))
					{
						if ((*glob_t_multilist)[ilv1].exist)
						{
							control_textedit_type=STRUCTURE_OBJECTTYPE_t;
							control_textedit_index=ilv1;
							control_textedit_selectmode=0;
							int tl_distance=-1;
							if (edit_textlength(glob_t_multilist,ilv1,control_coorsx,control_coorsy,&tl_distance)<=0)
							{
								TELESCOPE_aggressobject(glob_t_multilist,ilv1);
								tl_backval=TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_s);
								if (tl_backval==0) {control_mousestate=0;return 0;}
							}
							TELESCOPE_insertintoproperties_offset((char*)"\uE000",3,tl_distance);
							control_mousestate=0x40;return 0;
						}
					}
				}
			}
			else
			{
				if (selection_clickselection_found & (1<<STRUCTURE_OBJECTTYPE_n))
				{
					for (ilv1=0;ilv1<glob_n_multilist->filllevel;ilv1++)
					{
						if (selection_clickselection[ilv1] & (1<<STRUCTURE_OBJECTTYPE_n))
						{
							if ((*glob_n_multilist)[ilv1].exist)
							{
								n_instance * tl_n_instance=glob_n_multilist->bufferlist()+ilv1;
								control_textedit_type=STRUCTURE_OBJECTTYPE_n;
								control_textedit_index=ilv1;
								control_textedit_selectmode=0;
								TELESCOPE_aggressobject(glob_n_multilist,ilv1);
								if (tl_n_instance->Element!=-1)
								{
									float i_bond_sum=0;
									for (int ilv2=0;ilv2<atom_actual_node[ilv1].bondcount;ilv2++)
									{
										i_bond_sum+=(*glob_b_multilist)[atom_actual_node[ilv1].bonds[ilv2]].Order/16.0;
									}
									if (fmod(i_bond_sum,1.0)>0.4)
									{
										i_bond_sum=trunc(i_bond_sum)+1;
									}
									char istring[10];
									char tl_fill;
									char imatch;
									char tl_format=0;
									int tl_Element=tl_n_instance->Element;
									s_instance * tl_s_instance;
									s_f_instance * tl_s_f_instance;
									edit_formatstruct * tl_formatpointer;
									edit_formatstruct iformatstruct;
									edit_formatstruct * currentformatpointer=&iformatstruct;
									iformatstruct.color=(*tl_n_instance).color;
									iformatstruct.face=0x60;
									tl_backval=TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_s_f);
									if (tl_backval)
									{
										tl_format=(*(s_f_instance*)TELESCOPE_getproperty()).valids;
										tl_formatpointer=(edit_formatstruct*)TELESCOPE_getproperty_contents();
									}
									for (int ilv1=0;ilv1<5;ilv1++)
									{
										if (tl_s_f_instance)
										{
											tl_fill=0;
											for (int ilv2=0;ilv2<=ilv1;ilv2++)
											{
												imatch=1<<ilv2;
												if (tl_format & imatch)
												{
													currentformatpointer=tl_formatpointer+tl_fill;
													tl_fill++;
												}
											}
										}
										switch (ilv1)
										{
											case 0 : if (element[tl_Element].name[0]==0) {ilv1=2;sprintf(istring,"C");break;}sprintf(istring,"%c",element[tl_Element].name[0]);break;
											case 1 : if (element[tl_Element].name[1]==0) {ilv1=2;goto i_t_fertig;}sprintf(istring,"%c",element[tl_Element].name[1]);break;
											case 2 : if (element[tl_Element].name[2]==0) {ilv1=2;goto i_t_fertig;}sprintf(istring,"%c",element[tl_Element].name[2]);break;
											case 3 : sprintf(istring,"%s",((*tl_n_instance).protons-(int)i_bond_sum<=0)?"":"H");break;
											case 4 : if ((*tl_n_instance).protons-(int)i_bond_sum>1) sprintf(istring,"%i",(*tl_n_instance).protons-(int)i_bond_sum); else istring[0]=0;break;
										}
										TELESCOPE_add(TELESCOPE_ELEMENTTYPE_s,istring,strlen(istring)+1);
										tl_s_instance=(s_instance*)TELESCOPE_getproperty();
										tl_s_instance->color=currentformatpointer->color;
										tl_s_instance->font=1;
										tl_s_instance->face=currentformatpointer->face;
										tl_s_instance->size=12;
										i_t_fertig:;
									}
									TELESCOPE_aggressobject(glob_n_multilist,ilv1);
									tl_backval=TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_s_f);
									while (tl_backval)
									{
										TELESCOPE_clear_item();
										tl_backval=TELESCOPE_searchthroughobject_next(TELESCOPE_ELEMENTTYPE_s_f);

									}
									TELESCOPE_aggressobject(glob_n_multilist,ilv1);
									tl_n_instance->Element=-1;
								}
								tl_backval=TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_s);
								if (tl_backval==0) {tl_backval=TELESCOPE_add(TELESCOPE_ELEMENTTYPE_s,"",1);}
								if (tl_backval==0) {control_mousestate=0;return 0;}
								TELESCOPE_insertintoproperties_offset((char*)"\uE000",3,-1);
								(*glob_n_multilist)[ilv1].Element=-1;
								control_mousestate=0x40;return 0;
							}
						}
					}
				}
				else
				{
					i_t_instance=edit_summontext(&tl_t_nr);
					if (i_t_instance)
					{
						(*i_t_instance).xyz.x=control_coorsx;
						(*i_t_instance).xyz.y=control_coorsy;
						(*i_t_instance).xyz.z=0;
						TELESCOPE_aggressobject(glob_t_multilist,tl_t_nr);
						TELESCOPE_add(TELESCOPE_ELEMENTTYPE_s,"",1);
						s_instance * tl_s_instance=(s_instance*)TELESCOPE_getproperty();
						(*tl_s_instance).color=control_drawproperties.color;
						(*tl_s_instance).font=1;
						(*tl_s_instance).face=0;
						(*tl_s_instance).size=12;
						control_textedit_type=STRUCTURE_OBJECTTYPE_t;
						control_textedit_index=tl_t_nr;
						control_textedit_selectmode=0;
						TELESCOPE_insertintoproperties_offset((char*)"\uE000",3,-1);
						control_mousestate=0x40;return 0;
					}
				}
			}
			control_mousestate=0;return 0;
		}
		case 17:
		{
			char bond_shift=(MODIFIER_KEYS.SHIFT>0);
			n_instance ** tl_atom=(n_instance**)alloca(sizeof(n_instance*)*control_drawproperties.ring_element_count);
			int * atomnr=(int*)alloca(sizeof(int)*control_drawproperties.ring_element_count);
			control_mousestate=1;
			control_usingmousebutton=control_lastmousebutton;
			int skipatoms=0;
			control_startx=control_coorsx;
			control_starty=control_coorsy;
			float control_startz=0;
			float tl_angle=0;
			float radius=constants_bondlength/(2*sin(Pi/control_drawproperties.ring_element_count));
			float tl_h=radius*cos(Pi/control_drawproperties.ring_element_count);
			if (selection_clickselection_found & (1<<STRUCTURE_OBJECTTYPE_b))
			{
				for (int ilv1=0;ilv1<(*glob_b_multilist).filllevel;ilv1++)
				{
					if (selection_clickselection[ilv1] & (1<<STRUCTURE_OBJECTTYPE_b))
					{
						atomnr[0]=bond_actual_node[ilv1].start;
						atomnr[1]=bond_actual_node[ilv1].end;
						tl_atom[0]=(*glob_n_multilist).bufferlist()+atomnr[0];
						tl_atom[1]=(*glob_n_multilist).bufferlist()+atomnr[1];
						skipatoms=2;
						retrievepoints((*glob_b_multilist).bufferlist()+ilv1,&control_startx,&control_starty,&control_startz,0);
						tl_angle=getangle(tl_atom[1]->xyz.x-tl_atom[0]->xyz.x,tl_atom[1]->xyz.y-tl_atom[0]->xyz.y);
						float tl_mouseangle=getangle(control_coorsx-tl_atom[0]->xyz.x,control_coorsy-tl_atom[0]->xyz.y);
						if (fmod((tl_mouseangle-tl_angle+4*Pi),(2*Pi))<Pi)
						{
							tl_angle+=Pi/2;
						}
						else
						{
							atomnr[2]=atomnr[0];
							atomnr[0]=atomnr[1];
							atomnr[1]=atomnr[2];
							tl_atom[2]=tl_atom[0];
							tl_atom[0]=tl_atom[1];
							tl_atom[1]=tl_atom[2];
							tl_angle-=Pi/2;
						}
						control_startx+=cos(tl_angle)*tl_h;
						control_starty+=sin(tl_angle)*tl_h;
						goto ifoundbond;
					}
				}
			}
			ifoundbond:;
			for (int ilv1=skipatoms;ilv1<control_drawproperties.ring_element_count;ilv1++)
			{
				float tl_positionx,tl_positiony;
				tl_positionx=control_startx+cos(((ilv1*Pi*2.0)/control_drawproperties.ring_element_count)+tl_angle+Pi-Pi/control_drawproperties.ring_element_count)*radius;
				tl_positiony=control_starty+sin(((ilv1*Pi*2.0)/control_drawproperties.ring_element_count)+tl_angle+Pi-Pi/control_drawproperties.ring_element_count)*radius;
				tl_atom[ilv1]=snapatom_short(tl_positionx,tl_positiony,atomnr+ilv1);
				if (tl_atom[ilv1])
				{
					control_id=(*tl_atom[ilv1]).id;
				}
				else
				{
					tl_atom[ilv1]=edit_summonatom(atomnr+ilv1);
					if (tl_atom[ilv1])
					{
						(*tl_atom[ilv1]).xyz.x=tl_positionx;
						(*tl_atom[ilv1]).xyz.y=tl_positiony;
						(*tl_atom[ilv1]).xyz.z=control_startz;
					}
					else
					{
						control_mousestate=0;
						return -1;
					}
				}
			}
			for (int ilv1=0;ilv1<control_drawproperties.ring_element_count;ilv1++)
			{
				b_instance * tl_b_instance;
				int tl_b_nr;
				int end=(ilv1+1)%control_drawproperties.ring_element_count;
				tl_b_nr=get_bond_between(atomnr[ilv1],atomnr[end]);
				if (tl_b_nr<0)
				{
					tl_b_instance=edit_summonbond(tl_atom[ilv1]->id,tl_atom[end]->id,atomnr[ilv1],atomnr[end]);
				}
			}
			for (int ilv1=0;ilv1<control_drawproperties.ring_element_count;ilv1++)
			{
				b_instance * tl_b_instance;
				int tl_b_nr;
				int end=(ilv1+1)%control_drawproperties.ring_element_count;
				tl_b_nr=get_bond_between(atomnr[ilv1],atomnr[end]);
				if (tl_b_nr>=0)
				{
					tl_b_instance=glob_b_multilist->bufferlist()+tl_b_nr;
					if ((((ilv1%2)==0) ^ (bond_shift)) && (control_drawproperties.ring_unsaturation))
					{
						if ((getbondsum(atomnr[ilv1])<4))
						{
							if ((ilv1==control_drawproperties.ring_element_count-1) && ((ilv1&1)==0))
							{
								atom_addsymbol(atomnr[ilv1],((control_drawproperties.ring_element_count%4)==1)?5:4);
								Symbol_instance * tl_Symbol_instance=(Symbol_instance*)TELESCOPE_getproperty();
								tl_Symbol_instance->dxyz.x=cos(((ilv1*Pi*2.0)/control_drawproperties.ring_element_count)+tl_angle+Pi-Pi/control_drawproperties.ring_element_count)*10;
								tl_Symbol_instance->dxyz.y=sin(((ilv1*Pi*2.0)/control_drawproperties.ring_element_count)+tl_angle+Pi-Pi/control_drawproperties.ring_element_count)*10;
							}
							else
							{
								if (getbondsum(atomnr[end])<4)
								{
									(*tl_b_instance).Order=0x20;
								}
							}
						}
					}
				}
			}
			return 0;
		}
		case 18:
		//fallthrough
		case 19:
		{
			//fallthrough
			icompare=(1<<STRUCTURE_OBJECTTYPE_arrow)+(1<<(STRUCTURE_OBJECTTYPE_arrow+STRUCTURE_OBJECTTYPE_ListSize));
			if (selection_clickselection_found & icompare)
			{
				control_manipulatedinstance=getclicked(icompare,control_coorsx,control_coorsy);
				printf("%f\n",(*(arrow_instance*)control_manipulatedinstance).AngularSize);
			}
			else
			{
				control_mousestate=0;
				return 0;
			}
			break;
		}
	}
	ifertig:;
	control_mousestate=1;
	control_usingmousebutton=control_lastmousebutton;
	//warning: there ARE in-function returns.
	return 0;
}
void issuedrag(int iposx,int iposy)
{
	float tl_x=0;
	float tl_y=0;
	float tl_z=0;
	int internalpointcount;
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
		case 3:
		{
			selection_lassotrail(control_posx,control_posy,iposx,iposy);
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
			restoreundo(~0,1);//during dynamic undo, one must be careful that actual_node info may contain ghost hints
			undo_undodirty=1;
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
				control_hotatom=atomnr;
			}
			if (!tlatom)
			{
				tlatom=edit_summonatom(&atomnr);
				if (tlatom)
				{
					(*tlatom).xyz.x=control_startx;
					(*tlatom).xyz.y=control_starty;
					(*tlatom).xyz.z=0;
				}
				else
				{
					//TODO: overflow
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
				tlatom2=edit_summonatom(&atomnr2);
				if (tlatom2)
				{
					(*tlatom2).xyz.x=control_coorsx;
					(*tlatom2).xyz.y=control_coorsy;
					(*tlatom2).xyz.z=(*tlatom).xyz.z;
				}
			}
			if ((tlatom) && (tlatom2))
			{
				control_hotatom=atomnr2;
				if (tlatom!=tlatom2)
				{
					b_instance * oldbond=NULL;
					for (int ilv1=0;ilv1<atom_actual_node[atomnr].bondcount;ilv1++)
					{
						int tl_bondnr2=atom_actual_node[atomnr].bonds[ilv1];
						if (((*glob_b_multilist)[tl_bondnr2].exist) && (tl_bondnr2<(*glob_b_multilist).filllevel))
						{
							for (int ilv2=0;ilv2<atom_actual_node[atomnr2].bondcount;ilv2++)
							{
								if (tl_bondnr2==atom_actual_node[atomnr2].bonds[ilv2])
								{
									oldbond=&((*glob_b_multilist).bufferlist()[tl_bondnr2]);
								}
							}
						}
					}
					tlbond=edit_summonbond((*tlatom).id,(*tlatom2).id,atomnr,atomnr2);
					if (tlbond)
					{
						if (oldbond)
						{
							if (((*oldbond).color==control_drawproperties.color) && ((*oldbond).Display==control_drawproperties.bond_Display1) && (((*tlatom).id==(*oldbond).B)||(strchr("\x10\x11\x12\x15\x18\x23\x24",(*oldbond).Display+0x10))))
							{
								if (control_drawproperties.bond_multiplicity==1)
								{
									(*tlbond).Order+=control_drawproperties.bond_multiplicity<<4;
									if ((*tlbond).Order>64) (*tlbond).Order=64;
								}
								else
								{
									if ((control_drawproperties.bond_multiplicity==2) && (((*oldbond).Order==32)||((*oldbond).Order==24)))
									{
										(*tlbond).DoublePosition=(*oldbond).DoublePosition;
										(*tlbond).Order=(*oldbond).Order;
										SWAPDOUBLEPOSITION("","",glob_b_multilist,tlbond,0/*wrong, not important*/);
									}
									else
									{
										goto issuedrag_7_bond_setbondorder;
									}
								}
							}
							else
							{
								issuedrag_7_bond_setbondorder:;
								(*tlbond).Order=control_drawproperties.bond_multiplicity<<4;
							}
							(*tlbond).color=control_drawproperties.color;
							(*tlbond).Display=control_drawproperties.bond_Display1;
							goto ifertig;
						}
						(*tlbond).Z=(*tlatom2).Z;
						(*tlbond).Order=control_drawproperties.bond_multiplicity<<4;
					}
				}
			}
			break;
		}
		case 4:
		{
			intl tl_deltaback=0;
			float tl_deltax=control_coorsx-control_startx;
			float tl_deltay=control_coorsy-control_starty;
			if (MODIFIER_KEYS.SHIFT)
			{
				if (fabs(tl_deltax)>fabs(tl_deltay)) tl_deltay=0; else tl_deltax=0;
			}
			restoreundo(~0,((MODIFIER_KEYS.CTRL) || (MODIFIER_KEYS.ALT))?0:1);
			undo_undodirty=1;
			if (edit_singlepointselected==1)
			{
				if (MODIFIER_KEYS.ALT==0)
				{
					goto skip_snapping;
				}
			}
			if (MODIFIER_KEYS.ALT)
			{
				float bestdist=4000000000000;
				float tl_best_deltax=tl_deltax;
				float tl_best_deltay=tl_deltay;
				icompare=1<<STRUCTURE_OBJECTTYPE_n;
				for (int ilv0=0;ilv0<glob_n_multilist->filllevel;ilv0++)
				{
					if (selection_currentselection[ilv0] & icompare)
					{
						n_instance * tl_first_instance=glob_n_multilist->bufferlist()+ilv0;
						if (tl_first_instance->exist)
						{
							float ix=(*((n_instance*)tl_first_instance)).xyz.x;
							float iy=(*((n_instance*)tl_first_instance)).xyz.y;
							for (int ilv1=0;ilv1<glob_n_multilist->filllevel;ilv1++)
							{
								n_instance * tl_n_instance=glob_n_multilist->bufferlist()+ilv1;
								if ((*tl_n_instance).exist)
								{
									if (((selection_currentselection[ilv1] & icompare)==0) || (MODIFIER_KEYS.CTRL>0))
									{
										float idist=fsqr(tl_deltax+ix-(*tl_n_instance).xyz.x)+fsqr(tl_deltay+iy-(*tl_n_instance).xyz.y);
										if (idist<100)
										{
											if (idist<bestdist)
											{
												bestdist=idist;
												tl_best_deltax=((*tl_n_instance).xyz.x-ix);
												tl_best_deltay=((*tl_n_instance).xyz.y-iy);
											}
										}
									}
								}
							}
						}
					}
				}
				if (bestdist<3000000000000)
				{
					tl_deltax=tl_best_deltax;
					tl_deltay=tl_best_deltay;
				}
			}
			skip_snapping:;
			if (MODIFIER_KEYS.CTRL)
			{
				getatoms();
				janitor_getmaxid(STRUCTURE_OBJECTTYPE_n);
				edit_flexicopy(currentundostep,glob_n_multilist,glob_b_multilist,selection_currentselection,&tl_deltaback,tl_deltax,tl_deltay,0);
				getatoms();
			}
			else
			{
				edit_flexicopy(currentundostep,glob_n_multilist,glob_b_multilist,selection_currentselection,&tl_deltaback,tl_deltax,tl_deltay,1);
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
			undo_undodirty=1;
			arrow_instance * tl_arrow=edit_summonarrow();
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
				(*tl_arrow).ArrowheadType=1;
				(*tl_arrow).ArrowheadHead=2;
				(*tl_arrow).ArrowheadTail=0;
				(*tl_arrow).ArrowShaftSpacing=0;
				(*tl_arrow).Z=0;//TODO: pretty urgent
			}
			break;
		}
		case 10:
		{
			float tl_angle;
			Symbol_instance * tl_Symbol_instance=(Symbol_instance*)control_manipulatedinstance;
			n_instance * tl_n_instance=(n_instance*)control_manipulatedinstance2;
			tl_angle=getangle(control_coorsx-(*tl_n_instance).xyz.x,control_coorsy-(*tl_n_instance).xyz.y);
			(*tl_Symbol_instance).dxyz.x=cos(tl_angle)*10;
			(*tl_Symbol_instance).dxyz.y=sin(tl_angle)*10;
			break;
		}
		case 0x10000:
		{
			interpretkey(control_lastinterpret);
			break;
		}
		case 18:
		{
			float sx,sy,zx,zy;
			float mx,my,mz,dx,dy;
			float tl_height;
			retrievepoints_basic(control_manipulatedinstance,&sx,&sy,NULL,1,STRUCTURE_OBJECTTYPE_arrow);
			retrievepoints_basic(control_manipulatedinstance,&zx,&zy,NULL,2,STRUCTURE_OBJECTTYPE_arrow);
			float tl_length=sqrt(fsqr(zx-sx)+fsqr(zy-sy));
			retrievepoints_basic(control_manipulatedinstance,&mx,&my,&mz,0,STRUCTURE_OBJECTTYPE_arrow);
			float tl_angle=getangle(zx-sx,zy-sy);
			tl_angle+=Pi/2;
			dx=cos(tl_angle);dy=sin(tl_angle);
			tl_height=dx*(control_coorsx-mx)+dy*(control_coorsy-my);
			if (fabs(tl_height)<5)
			{
				(*(arrow_instance*)control_manipulatedinstance).AngularSize=0;
			}
			else
			{
				float aimangle=2*atan(2*tl_height/tl_length);
				(*(arrow_instance*)control_manipulatedinstance).AngularSize=-2*aimangle/Pi*180;
				float tl_radius=tl_length*0.5/sin(aimangle);//tl_radius signed to push center to either side of arrow
				(*(arrow_instance*)control_manipulatedinstance).Center3D.x=mx-cos(tl_angle)*tl_radius*cos(aimangle);
				(*(arrow_instance*)control_manipulatedinstance).Center3D.y=my-sin(tl_angle)*tl_radius*cos(aimangle);
				(*(arrow_instance*)control_manipulatedinstance).Center3D.z=0;
				(*(arrow_instance*)control_manipulatedinstance).MinorAxisEnd3D.x=(*(arrow_instance*)control_manipulatedinstance).Center3D.x+cos(tl_angle)*tl_radius;
				(*(arrow_instance*)control_manipulatedinstance).MinorAxisEnd3D.y=(*(arrow_instance*)control_manipulatedinstance).Center3D.y+sin(tl_angle)*tl_radius;
				(*(arrow_instance*)control_manipulatedinstance).MinorAxisEnd3D.z=0;
				(*(arrow_instance*)control_manipulatedinstance).MajorAxisEnd3D.x=(*(arrow_instance*)control_manipulatedinstance).Center3D.x+sin(tl_angle)*tl_radius;
				(*(arrow_instance*)control_manipulatedinstance).MajorAxisEnd3D.y=(*(arrow_instance*)control_manipulatedinstance).Center3D.y-cos(tl_angle)*tl_radius;
				(*(arrow_instance*)control_manipulatedinstance).MajorAxisEnd3D.z=0;
			}
			break;
		}
		case 19:
		{
			char whichofthem;
			float sx,sy,zx,zy;
			float mx,my,mz,dx,dy;
			float tl_height;
			float tl_angle1,tl_angle2;
			retrievepoints_basic(control_manipulatedinstance,&sx,&sy,NULL,1,STRUCTURE_OBJECTTYPE_arrow);
			retrievepoints_basic(control_manipulatedinstance,&zx,&zy,NULL,2,STRUCTURE_OBJECTTYPE_arrow);
			retrievepoints_basic(control_manipulatedinstance,&mx,&my,&mz,-3,STRUCTURE_OBJECTTYPE_arrow);
			whichofthem=((sx-control_coorsx)*(sx-control_coorsx)+(sy-control_coorsy)*(sy-control_coorsy)>(zx-control_coorsx)*(zx-control_coorsx)+(zy-control_coorsy)*(zy-control_coorsy));
			if ((*(arrow_instance*)control_manipulatedinstance).AngularSize==0)
			{
				retrievepoints_basic(control_manipulatedinstance,&mx,&my,&mz,0,STRUCTURE_OBJECTTYPE_arrow);
				float tl_angle=getangle(zx-sx,zy-sy);
				if (whichofthem==0)
				{
					float distance=sqrt((zx-control_coorsx)*(zx-control_coorsx)+(zy-control_coorsy)*(zy-control_coorsy));
					(*(arrow_instance*)control_manipulatedinstance).Head3D.x=zx-cos(tl_angle)*distance;
					(*(arrow_instance*)control_manipulatedinstance).Head3D.y=zy-sin(tl_angle)*distance;
				}
				else
				{
					float distance=sqrt((sx-control_coorsx)*(sx-control_coorsx)+(sy-control_coorsy)*(sy-control_coorsy));
					(*(arrow_instance*)control_manipulatedinstance).Tail3D.x=sx+cos(tl_angle)*distance;
					(*(arrow_instance*)control_manipulatedinstance).Tail3D.y=sy+sin(tl_angle)*distance;
				}
			}
			else
			{
				char angularside=(*(arrow_instance*)control_manipulatedinstance).AngularSize>0;
				retrievepoints_basic(control_manipulatedinstance,&mx,&my,&mz,-3,STRUCTURE_OBJECTTYPE_arrow);
				ellipsoid.create((*(arrow_instance*)control_manipulatedinstance).Center3D,(*(arrow_instance*)control_manipulatedinstance).MajorAxisEnd3D,(*(arrow_instance*)control_manipulatedinstance).MinorAxisEnd3D);
				ellipsoid.fill(control_coorsx-mx,control_coorsy-my);
				tl_angle2=ellipsoid.internalangle;
				if (whichofthem)
				{
					ellipsoid.fill(sx-mx,sy-my);
				}
				else
				{
					ellipsoid.fill(zx-mx,zy-my);
				}
				tl_angle1=ellipsoid.internalangle;
				float cosinus_x=(*(arrow_instance*)control_manipulatedinstance).MajorAxisEnd3D.x-mx;
				float cosinus_y=(*(arrow_instance*)control_manipulatedinstance).MajorAxisEnd3D.y-my;
				float sinus_x=(*(arrow_instance*)control_manipulatedinstance).MinorAxisEnd3D.x-mx;
				float sinus_y=(*(arrow_instance*)control_manipulatedinstance).MinorAxisEnd3D.y-my;
				(*(arrow_instance*)control_manipulatedinstance).AngularSize=((angularside^whichofthem)?fmod((tl_angle2-tl_angle1+2*Pi),(2*Pi)):-fmod((tl_angle1-tl_angle2+2*Pi),(2*Pi)))*(whichofthem?-1:1);
				if (whichofthem)
				{
					(*(arrow_instance*)control_manipulatedinstance).Tail3D.x=mx+sin(tl_angle2)*sinus_x+cos(tl_angle2)*cosinus_x;
					(*(arrow_instance*)control_manipulatedinstance).Tail3D.y=my+sin(tl_angle2)*sinus_y+cos(tl_angle2)*cosinus_y;
				}
				else
				{
					(*(arrow_instance*)control_manipulatedinstance).Head3D.x=mx+sin(tl_angle2)*sinus_x+cos(tl_angle2)*cosinus_x;
					(*(arrow_instance*)control_manipulatedinstance).Head3D.y=my+sin(tl_angle2)*sinus_y+cos(tl_angle2)*cosinus_y;
				}
			}
			break;
		}
	}
	ifertig:;
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
	int tlwhichtool=control_tool;
	int ioffset;
	char * ibufferpos;
	int internalpointcount;
	edit_singlepointselected=0;
	if (control_mousestate==2)
	{
		tlwhichtool=control_keycombotool;
		goto shiftshunt;
	}
	shiftshunt:
	switch (tlwhichtool)
	{
		case 2:
		{
			if (!rectifyselectionframe())
			{
				clickforthem();
				if (selection_currentselection_found)
				{
					KEYDEPENDENTSELECTION;
				}
				else
				{
					selection_clearselection(selection_currentselection);
					selection_currentselection_found=0;
				}
				break;
			}
			selection_clearselection(selection_clickselection);selection_clickselection_found=0;
			for (int ilv1=0;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
			{
				int follower=0;
				icompare=1<<ilv1;
				internalpointcount=retrieveprops_basic(1,ilv1);
				int isize= STRUCTURE_OBJECTTYPE_List[ilv1].size;
				basicmultilist * tlmultilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
				if (tlmultilist==NULL) goto i_control2_fertig;
				CDXMLREAD_functype tldummy;
				ibufferpos=(char*)((*tlmultilist).pointer);
				float tlpx,tlpy,tlpz;
				if (tlmultilist!=NULL)
				{
					for (int ilv2=0;ilv2<(*tlmultilist).filllevel;ilv2++)
					{
						if ((*((basic_instance*)(ibufferpos+isize*ilv2))).exist)
						{
							int ilv3=0;
							if (internalpointcount>0)
							{
								follower=ilv2*internalpointcount;
							}
							while (retrievepoints_basic((basic_instance*)(ibufferpos+isize*ilv2),&tlpx,&tlpy,&tlpz,ilv3,ilv1)>0)
							{
								if ((tlpx>=selection_frame.startx) && (tlpx<=selection_frame.endx))
								{
									if ((tlpy>=selection_frame.starty) && (tlpy<=selection_frame.endy))
									{
										if (ilv3>0)
										{
											if (selection_clickabilitymatrix.types2[2] & (1<<ilv1))
											{
												selection_clickselection[follower]|=(1<<(ilv1+STRUCTURE_OBJECTTYPE_ListSize));
												goto i_control2_skipthiscenter;
											}
										}
										selection_clickselection[ilv2]|=icompare;
										selection_clickselection_found|=icompare;
										i_control2_skipthiscenter:;
									}
								}
								if (ilv3>0)
								{
									follower++;
								}
								ilv3++;
							}
						}
					}
				}
				i_control2_fertig:;
			}
			KEYDEPENDENTSELECTION;
			break;
		}
		case 3:
		{
			//TODO: what if not moved
			selection_lassotrail(control_posx,control_posy,selection_lassostartx,selection_lassostarty);
			if (selection_lasso_up==2) selection_lasso_putpixel(selection_lassostartx,selection_lassostarty,1);
			selection_lassofill();
			selection_clearselection(selection_clickselection);selection_clickselection_found=0;
			for (int ilv1=1;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
			{
				int follower=0;
				icompare=1<<ilv1;
				internalpointcount=retrieveprops_basic(1,ilv1);
				int isize= STRUCTURE_OBJECTTYPE_List[ilv1].size;
				basicmultilist * tlmultilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
				if (tlmultilist==NULL) goto i_control3_fertig;
				ibufferpos=(char*)((*tlmultilist).pointer);
				float tlpx,tlpy,tlpz;
				int tl_x,tl_y,tl_z;
				if (tlmultilist!=NULL)
				{
					for (int ilv2=0;ilv2<(*tlmultilist).filllevel;ilv2++)
					{
						if ((*((basic_instance*)(ibufferpos+isize*ilv2))).exist)
						{
							int ilv3=0;
							int ilv4;
							char markstate=0;
							//goto considered helpful
							//I am not kidding. Mixing goto statements into if constructs means just:
							//continue exactly as you would have done in the other case.
							if (internalpointcount>0)
							{
								follower=ilv2*internalpointcount;
							}
							while (retrievepoints_basic((basic_instance*)(ibufferpos+isize*ilv2),&tlpx,&tlpy,&tlpz,ilv3,ilv1)>0)
							{
								tl_x=(tlpx-SDL_scrollx)*SDL_zoomx;
								tl_y=(tlpy-SDL_scrolly)*SDL_zoomy;
								tl_z=tlpz;
								if ((tl_x<0)|| (tl_y<0) || (tl_y>=gfx_canvassizey))
								{
									markstate=0;
									ilv4=0;
									i_control3_hub:;
									for (;ilv4<selection_lassohub[tl_y+10000].count;ilv4++)
									{
										if (selection_lassohub[tl_y+10000].items[ilv4]>tl_x)
										{
											goto i_control3_hubdone;
										}
										markstate^=1;
									}
									i_control3_hubdone:;
									if (markstate)
									{
										goto i_control3_yesselect;
									}
								}
								else
								{
									if (tl_x>=gfx_canvassizex) 
									{
										markstate=((selection_lassobuffer[tl_y*gfx_canvassizex+gfx_canvassizex-1]&0x80)!=0);
										for (ilv4=0;ilv4<selection_lassohub[tl_y+10000].count;ilv4++)
										{
											if (selection_lassohub[tl_y+10000].items[ilv4]>0)//right of canvas
											{
												goto i_control3_hub;
											}
										}
										goto i_control3_hub;
									}
									if (selection_lassobuffer[tl_y*gfx_canvassizex+tl_x]&0x80)
									{
										i_control3_yesselect:;
										if (ilv3>0)
										{
											if (selection_clickabilitymatrix.types2[2] & (1<<ilv1))
											{
												selection_clickselection[follower]|=(1<<(ilv1+STRUCTURE_OBJECTTYPE_ListSize));
												goto i_control3_skipthiscenter;
											}
										}
										selection_clickselection[ilv2]|=icompare;
										selection_clickselection_found|=icompare;
										i_control3_skipthiscenter:;
									}
								}
								if (ilv3>0)
								{
									follower++;
								}
								ilv3++;
							}
						}
					}
				}
				i_control3_fertig:;
			}
			KEYDEPENDENTSELECTION;
			break;
		}
		case 4:
		{
			for (int ilv1=0;ilv1<glob_n_multilist->filllevel;ilv1++)
			{
				if (selection_currentselection[ilv1] & (1<<STRUCTURE_OBJECTTYPE_n))
				{
					n_instance * iinstance=glob_n_multilist->bufferlist()+ilv1;
					if (iinstance->exist)
					{
						for (int ilv2=0;ilv2<glob_n_multilist->filllevel;ilv2++)
						{
							if (ilv1!=ilv2)
							{
//								if ((selection_currentselection[ilv2] & (1<<STRUCTURE_OBJECTTYPE_n))==0)
//TODO: Lock only selection to unselected and CLEAN THIS MESS
								{
									n_instance * iinstance2=glob_n_multilist->bufferlist()+ilv2;
									if (iinstance2->exist)
									{
										if (edit_atommatch(iinstance,iinstance2))
										{
											edit_joinatom(iinstance2,iinstance,ilv1);
											goto ifound;
										}
									}
								}
							}
						}
					}
					ifound:;
				}
			}
			checkupinconsistencies();
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
						control_hotatom=tlatom;
						edit_errichten(tlatom);
					}
				}
				else
				{
					n_instance * tl_atom=edit_summonatom();
					if (tl_atom)
					{
						(*tl_atom).xyz.x=control_startx;
						(*tl_atom).xyz.y=control_starty;
						(*tl_atom).xyz.z=0;
						control_id=(*tl_atom).id;
						goto madeitmyself1;
					}
				}
			}
			checkupinconsistencies();
			break;
		}
	}
	control_mousestate=0;
	return;
}
int issuemenuclick(AUTOSTRUCT_PULLOUTLISTING_ * ilisting,int icount,int posx,int posy,int button,int pixeloriginposx,int pixeloriginposy,int starthitnr=-1)
{
	int horzistart;
	int vertistart;
	int ihitnr=0;
	AUTOSTRUCT_PULLOUTLISTING_ * ipulloutlisting=NULL;
	if (starthitnr!=-1)
	{
		ihitnr=starthitnr;
		ipulloutlisting=ilisting+ihitnr;
		goto ifound;
	}
	for (int ilv1=0;ilv1<icount;ilv1++)
	{
	}
	for (int ilv1=0;ilv1<icount;ilv1++)
	{
		horzistart=ilisting[ilv1].x;
		vertistart=ilisting[ilv1].y;
		if (horzistart<0) horzistart+=gfx_screensizex/constants_buttonsize;
		if (vertistart<0) vertistart+=gfx_screensizey/constants_buttonsize;
		if ((horzistart==posx) && (vertistart==posy))
		{
			ipulloutlisting=&(ilisting[ilv1]);
			ihitnr=ilv1;
			goto ifound;
		}
		if (ilisting[ilv1].lmbmode==0x103)
		{
			if (vertistart==posy)
			{
				if ((horzistart<=posx) && (horzistart+(gfx_canvassizex/32)>posx))
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
			int tl_mbmode;
			case SDL_BUTTON_LEFT:
			{
				tl_mbmode=((*ipulloutlisting).lmbmode);
				switch (tl_mbmode)
				{
					case 1: 
					selecttool:;
					control_clickabilitymatrixes[control_tool]=selection_clickabilitymatrix;
					if (control_tool==(*ipulloutlisting).toolnr)
					{
						char istring[100];
						for (int ilv1=0;ilv1<100;ilv1++)
						{
							char ihv1;
							ihv1=(*ipulloutlisting).name[ilv1];
							if ((ihv1==0) || (ihv1==0x20))
							{
								istring[ilv1]=0;
								goto LMB1_finished;
							}
							else
							{
								istring[ilv1]=ihv1;
							}
						}
						LMB1_finished:;
						structenum * tl_structenum=searchreflectedstruct("toolbox");
						for (int ilv1=0;ilv1<tl_structenum->count;ilv1++)
						{
							if (strcmp((((AUTOSTRUCT_PULLOUTLISTING_*)(tl_structenum->pointer))+ilv1)->name,istring)==0)
							{
								control_tool=(((AUTOSTRUCT_PULLOUTLISTING_*)(tl_structenum->pointer))+ilv1)->toolnr;
								selection_clickabilitymatrix=control_clickabilitymatrixes[control_tool];
							}
						}
						break;
					}
					control_tool=(*ipulloutlisting).toolnr;
					selection_clickabilitymatrix=control_clickabilitymatrixes[control_tool];
					break;
					case 2: *((char*)(*ipulloutlisting).variable)^=1;break;
					case 3: 
					{
						char istring[100];
						sprintf(istring,"%i",(*ipulloutlisting).toolnr);
						(*ipulloutlisting).LMB_function("",istring);break;
					}
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
					case 0x103:
					{
						if ((pixeloriginposy-(posy*32))>=24)
						{
							int tl_angle;
							tl_angle=((((((pixeloriginposx-(ipulloutlisting->x*32))*360)/gfx_canvassizex)+7)/15)*15)-180;
							char istring[100];
							sprintf(istring,"%f",tl_angle/180.0*Pi);
							(*ipulloutlisting).LMB_function("",istring);
							break;
						}
						goto idefault;
					}
					case 0x201:
					{
						structenum * istructenum=(structenum*)(*ipulloutlisting).variable;
						int tl_clickindex=(((pixeloriginposy-ipulloutlisting->y)/16)+(*istructenum).scroll);
						if (tl_clickindex<istructenum->count)
						{
							(*ipulloutlisting).LMB_function(((char*)((*istructenum).pointer))+(*istructenum).size*tl_clickindex,"");
						}
						break;
					}
					case 0x301:
					{
						menu_selectedmenuelement=menu_itembyname((*ipulloutlisting).name);
						control_menutexteditcursor=(pixeloriginposx-(*ipulloutlisting).x)/8;
						break;
					}
					default:
					{
						goto idefault;
					}
				}
				break;
			}
			case SDL_BUTTON_RIGHT:
			{
				tl_mbmode=((*ipulloutlisting).rmbmode);
				switch (tl_mbmode)
				{
					case 1: goto selecttool;break;
					case 2: *((char*)(*ipulloutlisting).variable)&=~1;break;
					case 3: (*ipulloutlisting).RMB_function("","");break;
					case 4:
					{
						control_mousestate=8;
						menu_matrixsubmenuvariable=(*ipulloutlisting).name;
						break;
					}
					case 6: *((_i32*)((*ipulloutlisting).variable))=0;break;
					case 0x201: 
					{
						structenum * istructenum=(structenum*)(*ipulloutlisting).variable;
						int tl_clickindex=(((pixeloriginposy-ipulloutlisting->y)/16)+(*istructenum).scroll);
						menu_selectedmenuelement=menu_itembyname((*ipulloutlisting).name);
						if (tl_clickindex<istructenum->count)
						{
							istructenum->number=tl_clickindex;
						}
						break;
					}
					case 0x301:
					{
						menu_selectedmenuelement=menu_itembyname((*ipulloutlisting).name);
						break;
					}
					default:
					{
						idefault:;
						if ((tl_mbmode & (~0xFF))==0x100)
						{
							storeundo(~0);
							if (tl_mbmode==0x111)
							{
								pixeloriginposx=(pixeloriginposx>>5)<<5;
								pixeloriginposy=(pixeloriginposy>>5)<<5;
							}
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
			case SDL_BUTTON_WHEELUP:
			{
				switch ((*ipulloutlisting).lmbmode)
				{
					case 0x201: 
					{
						structenum * istructenum=(structenum*)(*ipulloutlisting).variable;
						istructenum->scroll-=1;
						if (istructenum->scroll<0) istructenum->scroll=0;
						break;
					}
				}
				break;
			}
			case SDL_BUTTON_WHEELDOWN:
			{
				switch ((*ipulloutlisting).lmbmode)
				{
					case 0x201: 
					{
						structenum * istructenum=(structenum*)(*ipulloutlisting).variable;
						istructenum->scroll+=1;
						if (istructenum->scroll>=istructenum->count) istructenum->scroll=istructenum->count-1;
						break;
					}
				}
				break;
			}
		}
		return 1;
	}
	return 0;
}
void sdl_commonmenucommon();
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
							if (((*glob_n_multilist).bufferlist()+ilv2)->exist)
							{
								edit_setelement(ilv1,(*glob_n_multilist).bufferlist()+ilv2,ilv2);
								//It is by reason that uninterpreted text labels are not overwritten!
							}
						}
					}
					return 1;
				}
				if ((control_tool==7) && (control_hotatom!=-1))
				{
					if (control_hotatom>(*glob_n_multilist).filllevel)
					{
						if (((*glob_n_multilist).bufferlist()+control_hotatom)->exist)
						{
							edit_setelement(ilv1,(*glob_n_multilist).bufferlist()+control_hotatom,control_hotatom);
							return 1;
						}
					}
				}
				control_tool=6;
				selection_clickabilitymatrix=control_clickabilitymatrixes[control_tool];
			}
			else
			{
				control_tool=6;
				selection_clickabilitymatrix=control_clickabilitymatrixes[control_tool];
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
int menu_unbound(int invalue,char xy);
int issuerectclick(AUTOSTRUCT_PULLOUTLISTING_ * ilisting,int icount,int iposx,int iposy,int ibutton)
{
	int ihitnr=0;
	AUTOSTRUCT_PULLOUTLISTING_ * ipulloutlisting=NULL;
	for (int ilv1=0;ilv1<icount;ilv1++)
	{
		if ((menu_unbound(ilisting[ilv1].x,0)<=iposx) && (menu_unbound(ilisting[ilv1].y,1)<=iposy) &&
		(menu_unbound(ilisting[ilv1].maxx,0)>iposx) && (menu_unbound(ilisting[ilv1].maxy,1)>iposy))
		{
			ipulloutlisting=&(ilisting[ilv1]);
			ihitnr=ilv1;
			goto ifound;
		}
	}
	return 0;
	ifound:;
	issuemenuclick(ilisting,icount,iposx,iposy,ibutton,iposx,iposy,ihitnr);
	return 1;
}
void issuemenuclicks(int iposx,int iposy,int ibutton)
{
	int tlsuccess=0;
	for (int ilv1=menu_list_count-1;ilv1>=0;ilv1--)
	{
		int tl_alignx=menu_list[ilv1].alignx;
		int tl_aligny=menu_list[ilv1].aligny;
		if (tl_alignx<0) tl_alignx+=gfx_screensizex;
		if (tl_aligny<0) tl_aligny+=gfx_screensizey;
		if (((iposx-tl_alignx)>=0) && ((iposy-tl_aligny)>=0))
		{
			switch (menu_list[ilv1].type)
			{
				case 0:tlsuccess|=issuemenuclick((AUTOSTRUCT_PULLOUTLISTING_*)menu_list[ilv1].what.pointer,menu_list[ilv1].what.count,(iposx-tl_alignx)/32,(iposy-tl_aligny)/32,ibutton,iposx,iposy);break;
				case 1:tlsuccess|=issuemenuclick((AUTOSTRUCT_PULLOUTLISTING_*)menu_list[ilv1].what.pointer,menu_list[ilv1].what.count,(iposx-tl_alignx)/192,(iposy-tl_aligny)/16,ibutton,iposx,iposy);break;
				case 2: tlsuccess|=issuepseclick((iposx-tl_alignx)/32,(iposy-tl_aligny)/48,ibutton);break;
				case 3: tlsuccess|=issuerectclick((AUTOSTRUCT_PULLOUTLISTING_*)menu_list[ilv1].what.pointer,menu_list[ilv1].what.count,iposx-tl_alignx,iposy-tl_aligny,ibutton);break;
				case 4: tlsuccess|=issuerectclick((AUTOSTRUCT_PULLOUTLISTING_*)menu_list[ilv1].what.pointer,menu_list[ilv1].what.count,iposx-tl_alignx,iposy-tl_aligny,ibutton);break;
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
		if (control_mousestate & 24)
		{
			control_mousestate=control_mousestate & (~8);
		}
	}
}
int textedit_left()
{
	int wert=1;
	int tl_length=1;
	int tl_backval=0;
	TELESCOPE_shrink(control_textedit_cursor,3);
	iback:;
	control_textedit_cursor-=1;
	if (((_iXX)control_textedit_cursor)<0)
	{
		control_textedit_cursor=0;
		if (control_textedit_telescope>0)
		{
			control_textedit_telescope-=1;
			tl_backval=TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_s);
			for (int ilv1=0;ilv1<control_textedit_telescope;ilv1++)
			{
				if (tl_backval<=0) {wert=0;goto skipfail;}
				tl_backval=TELESCOPE_searchthroughobject_next(TELESCOPE_ELEMENTTYPE_s);
			}
			if (tl_backval>0)
			{
				control_textedit_cursor=strlen((char*)TELESCOPE_getproperty_contents());
				goto iback;
			}
			else
			{
				wert=0;
			}
			skipfail:;
		}
		else
		{
			wert=0;
		}
	}
	utf8encompass((char*)TELESCOPE_getproperty_contents(),&control_textedit_cursor,&tl_length);
	TELESCOPE_insertintoproperties_offset("\uE000",3,control_textedit_cursor);
	return wert;
}
int textedit_right()
{
	int wert=1;
	int tl_length=1;
	int tl_backval=0;
	TELESCOPE_shrink(control_textedit_cursor,3);
	iback:;
	utf8encompass((char*)TELESCOPE_getproperty_contents(),&control_textedit_cursor,&tl_length);
	control_textedit_cursor+=tl_length;
	if (control_textedit_cursor>strlen((char*)TELESCOPE_getproperty_contents()))
	{
		control_textedit_cursor=strlen((char*)TELESCOPE_getproperty_contents());
		control_textedit_telescope+=1;
		tl_backval=TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_s);
		for (int ilv1=0;ilv1<control_textedit_telescope;ilv1++)
		{
			if (tl_backval<=0) {wert=0;goto skipfail;}
			tl_backval=TELESCOPE_searchthroughobject_next(TELESCOPE_ELEMENTTYPE_s);
		}
		if (tl_backval>0)
		{
			control_textedit_cursor=0;
			goto iback;
		}
		else
		{
			wert=0;
		}
		skipfail:;
	}
	TELESCOPE_insertintoproperties_offset("\uE000",3,control_textedit_cursor);
	return wert;
}
long long counter_SDL;
void issuemenudrag(int posx,int posy,char ifinal=0)
{
	int diffx,diffy;
	switch ((*control_menuitem).lmbmode)
	{
		case 0x111:
		{
			char istring[100];
			if ((posx>=control_firstmenux) && (posy>=control_firstmenuy) && (posy<control_firstmenuy+32))
			{
				if ((ifinal) && (posx>=control_firstmenux+32))
				{
					unsigned int palette=(posx-control_firstmenux-32)/32;
					(*(_u32*)(*control_menuitem).variable)=255*(palette & 1)+32640*(palette & 2)+4177920*(palette & 4);
				}
				goto apply_111;
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
			apply_111:
			if (control_mousestate & 0x40)
			{
				char * icursor;
				control_splitattextcursor();
				control_aggresstextcursor("\uE000");
				if (control_textedit_selectmode==1)
				{
					icursorback:
					icursor=(char*)TELESCOPE_getproperty_contents();
					if ((strstr(icursor,"\uE001")==NULL))
					{
						(*(s_instance*)TELESCOPE_getproperty()).color=*(int*)((*control_menuitem).variable);
						int tl_backvalue=TELESCOPE_searchthroughobject_next(TELESCOPE_ELEMENTTYPE_s);
						if (tl_backvalue>0) goto icursorback;
					}
				}
				else
				{
					icursor=(char*)TELESCOPE_getproperty_contents();
					if (icursor[3]!=0)
					{
						TELESCOPE_split(3,"",1);
						control_aggresstextcursor("\uE000");
					}
					(*(s_instance*)TELESCOPE_getproperty()).color=*(int*)((*control_menuitem).variable);
				}
			}
			else
			{
				cantinsert:;
				sprintf(istring,"%i",*(int*)((*control_menuitem).variable));
				if ((*control_menuitem).LMB_function)
				{
					(*control_menuitem).LMB_function((*control_menuitem).name,istring);
				}
			}
			break;
		}
		case 0x103:
		{
			restoreundo(~0,0);
			getatoms();
			undo_undodirty=1;
			diffx=posx-control_lastmenux;
			diffy=posy-control_lastmenuy;
			control_menudragint+=diffx;
			char istring[100];
			sprintf(istring,"%f",((float)(control_menudragint))/gfx_canvassizex*2.0*Pi);
			(*control_menuitem).LMB_function("",istring);
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
				control_doubleclickenergy=0;
				control_mousex=control_Event.motion.x;
				control_mousey=control_Event.motion.y;
				control_hotatom=-1;

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
				if (control_mousestate==0x42)
				{
					issuetextclick(control_Event.button.x-gfx_canvasminx, control_Event.button.y-gfx_canvasminy,"\uE001");
					break;
				}
				if (control_mousestate & 0x20)
				{
					sdl_commonmenucommon();
					idontrepeat=1;
					issuemenudrag(control_Event.motion.x,control_Event.motion.y);
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
			#ifdef SDL2
			case SDL_MOUSEWHEEL:
			{
				if (control_Event.wheel.y-control_Event.wheel.x!=0)
				{
					control_Event.button.type=SDL_MOUSEBUTTONDOWN;
					control_Event.button.button=(control_Event.wheel.y-control_Event.wheel.x>0)?SDL_BUTTON_WHEELUP:SDL_BUTTON_WHEELDOWN;
					control_Event.button.state=SDL_PRESSED;
					control_Event.button.clicks=1;
					control_Event.button.x=control_mousex;
					control_Event.button.y=control_mousey;
					goto SDL_MOUSEBUTTONDOWN_FROM_MOUSEWHEEL;
				}
				break;
			}
			#endif
			case SDL_MOUSEBUTTONDOWN:
			{
				SDL_MOUSEBUTTONDOWN_FROM_MOUSEWHEEL:;
				if ((control_mousestate & (~0x58))==0)
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
						control_doubleclickenergy=0;
						goto clickshunt;
					}
					case SDL_BUTTON_LEFT:
					{
						if (((control_mousestate & (~0x58))==0) && (control_doubleclickenergy>0) && ((control_tool==2) || (control_tool==3) || (control_tool==4)))
						{
							clickforthem();
							selection_clearselection(selection_fragmentselection);
							if (selection_clickselection_found & ((1<<STRUCTURE_OBJECTTYPE_n) | (1<<STRUCTURE_OBJECTTYPE_b)))
							{
								for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
								{
									if (selection_clickselection[ilv1] & (1<<STRUCTURE_OBJECTTYPE_n))
									{
										select_fragment_by_atom(ilv1);
										goto doubleclicktargetfound;
									}
								}
								for (int ilv1=0;ilv1<(*glob_b_multilist).filllevel;ilv1++)
								{
									if (selection_clickselection[ilv1] & (1<<STRUCTURE_OBJECTTYPE_b))
									{
										select_fragment_by_atom(bond_actual_node[ilv1].start);
										goto doubleclicktargetfound;
									}
								}
								doubleclicktargetfound:;
								KEYDEPENDENTSELECTION;
								break;
							}
						}
						control_lastmousebutton=SDL_BUTTON_LEFT;
						control_doubleclickenergy=50;
						clickshunt:
						if (control_mousestate & 0x20)
						{
							control_usingmousebutton=control_lastmousebutton;
						}
						if (control_mousestate==0)
						{
							issueclick(control_Event.button.x-gfx_canvasminx,control_Event.button.y-gfx_canvasminy);
						}
						else
						{
							if (control_mousestate==0x40)
							{
								issuetextclick(control_Event.button.x-gfx_canvasminx,control_Event.button.y-gfx_canvasminy,"\uE000");
							}
						}
						break;
					}
					case SDL_BUTTON_WHEELUP:
					{
						idirection=-1;
					}
					case SDL_BUTTON_WHEELDOWN:
					{
						control_doubleclickenergy=0;
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
						if (control_mousestate==0x42)
						{
							issuetextclick(control_Event.motion.x-gfx_canvasminx, control_Event.motion.y-gfx_canvasminy,"\uE001");
							if (control_aggresstextcursor(arbitrarycursorstring))
							{
								char * currenttextpointer=((char*)TELESCOPE_getproperty_contents())+control_textedit_cursor+3;
								if ((strncmp(currenttextpointer,"\uE001",3)==0) || (strncmp(currenttextpointer,"\uE000",3)==0))
								{
									if (control_aggresstextcursor("\uE001"))
									{
										TELESCOPE_shrink(control_textedit_cursor,3);
										control_textedit_selectmode=0;
									}
								}
							}
							control_mousestate=0x40;
						}
						control_lastmousebutton=SDL_BUTTON_LEFT;
						dragshunt:
						if ((control_mousestate==1) || ((control_mousestate==2) && (control_toolstartkeysym==SDLK_UNKNOWN)))
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
			#ifdef SDL2
			case SDL_TEXTINPUT:
			{
				if (control_aggresstextcursor())
				{
					TELESCOPE_insertintoproperties_offset(control_Event.text.text,strlen(control_Event.text.text),control_textedit_cursor);
				}
				break;
			}
			#endif
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
							#ifndef SDL2
							SDL_WM_ToggleFullScreen(video);
							#endif
						}
						break;
					}
					case SDLK_ESCAPE:
					{
						if (control_mousestate==0)
						{
							if (idirection==1)
							{
								LHENDRAW_leave=1;
							}
						}
						if (control_mousestate==2)
						{
							issuedrag(control_mousex-gfx_canvasminx, control_mousey-gfx_canvasminy);
							control_mousestate=0;
						}
						break;
					}
					case SDLK_SPACE:
					{
						if (idirection==1)
						{
							if (control_mousestate==0)
							{
								selection_clearselection(selection_clickselection);
								int ilv0=STRUCTURE_OBJECTTYPE_n;
								_u32 tlfound=clickfor((control_mousex-gfx_canvasminx)/SDL_zoomx+SDL_scrollx,(control_mousey-gfx_canvasminy)/SDL_zoomy+SDL_scrolly,STRUCTURE_OBJECTTYPE_n,constants_clickradius,1)>0;
								if (tlfound)
								{
									for (int ilv2=0;ilv2<(*glob_n_multilist).filllevel;ilv2++)
									{
										if (selection_clickselection[ilv2] & (1<<STRUCTURE_OBJECTTYPE_n))
										{
											if ((*glob_n_multilist)[ilv2].exist)
											{
												control_hotatom=ilv2;
											}
										}
									}
								}
								idontrepeat=1;
								issueshiftstart();
								control_keycombotool=1;
								control_mousestate=2;
							}
						}
						break;
					}
					case SDLK_BACKSPACE:
					{
						if (idirection)
						{
							if (control_hotatom!=-1)
							{
								if (((*glob_n_multilist).bufferlist()+control_hotatom)->exist)
								{
									if (atom_actual_node[control_hotatom].bondcount>0)
									{
										control_hotatom=getother(control_hotatom,atom_actual_node[control_hotatom].bonds[0]);
									}
								}
							}
						}
					}
					case SDLK_LEFT:
					{
						MODIFIER_KEYS.LEFT=idirection;
						break;
					}
					case SDLK_RIGHT:
					{
						MODIFIER_KEYS.RIGHT=idirection;
						break;
					}
					case SDLK_UP:
					{
						MODIFIER_KEYS.UP=idirection;
						break;
					}
					case SDLK_DOWN:
					{
						MODIFIER_KEYS.DOWN=idirection;
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
				if ((control_mousestate==0x40) && (idirection==1))
				{
					int tl_length;
					char tl_alternate=0;
					int tl_counter=0;
					switch (control_Event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
						if (control_aggresstextcursor())
						{
							TELESCOPE_shrink(control_textedit_cursor,3);
							if (control_aggresstextcursor("\uE001"))
							{
								TELESCOPE_shrink(control_textedit_cursor,3);
							}
							if (control_textedit_type==STRUCTURE_OBJECTTYPE_n)
							{
								edit_resortstring(glob_n_multilist,control_textedit_index);
								edit_interpretaselementwithimplicithydrogens(glob_n_multilist,control_textedit_index);
								edit_bondsum(control_textedit_index,1);
							}
							if (control_textedit_type==STRUCTURE_OBJECTTYPE_t)
							{
								edit_textlength(glob_t_multilist,control_textedit_index);
							}
						}
						control_mousestate=0;
						break;
						case SDLK_LEFT:
						if ((control_textedit_selectmode==0) && (MODIFIER_KEYS.SHIFT))
						{
							if (control_aggresstextcursor("\uE000"))
							{
								TELESCOPE_insertintoproperties_offset("\uE001",3,control_textedit_cursor+3);
								control_textedit_selectmode=1;
							}
						}
						if (control_aggresstextcursor())
						{
							textedit_left();
						}
						break;
						case SDLK_RIGHT:
						if ((control_textedit_selectmode==0) && (MODIFIER_KEYS.SHIFT))
						{
							if (control_aggresstextcursor("\uE000"))
							{
								TELESCOPE_insertintoproperties_offset("\uE001",3,control_textedit_cursor);
								control_textedit_selectmode=1;
							}
						}
						if (control_aggresstextcursor())
						{
							textedit_right();
						}
						break;
						case SDLK_DOWN:
						tl_alternate=3;
						tl_counter=0;
						goto iTEXT_HOME_RESTART;//searches for a BEGINNING of the line, first!
						case SDLK_UP:
						tl_alternate=2;
						tl_counter=0;
						//FALLTHROUGH
						case SDLK_HOME://FALLTHROUGH:
						{
							iTEXT_HOME_RESTART:;
							if (control_aggresstextcursor())
							{
								while (textedit_left()>0){if (((char*)TELESCOPE_getproperty_contents())[control_textedit_cursor+3]=='\n') {textedit_right();goto iTEXT_HOME_END;}if (tl_alternate>=2)tl_counter++;}
							}
							iTEXT_HOME_END:;
							if (tl_alternate==2) {tl_alternate=1;textedit_left();goto iTEXT_HOME_RESTART;}
							if (tl_alternate==3) {tl_alternate=2;goto iTEXT_END_RESTART;}//and then, forward
							if (tl_alternate)
							{
								for (int ilv1=0;ilv1<tl_counter;ilv1++)
								{
									if (((char*)TELESCOPE_getproperty_contents())[control_textedit_cursor+3]=='\n') {goto iTEXT_END_done;}
									textedit_right();
								}
							}
						}
						break;
						case SDLK_END:
						{
							if (control_aggresstextcursor())
							{
								while (textedit_right()>0){iTEXT_END_RESTART:;if (((char*)TELESCOPE_getproperty_contents())[control_textedit_cursor+3]=='\n') {goto iTEXT_END_END;}}
							}
							iTEXT_END_END:;
							if (tl_alternate)
							{
								for (int ilv1=0;ilv1<tl_counter+1;ilv1++)
								{
									textedit_right();
									if (((char*)TELESCOPE_getproperty_contents())[control_textedit_cursor+3]=='\n') {goto iTEXT_END_done;}
								}
							}
							iTEXT_END_done:;
						}
						break;
						case SDLK_BACKSPACE:
						if (control_aggresstextcursor())
						{
							textedit_left();
							control_textedit_cursor+=3;
							if (utf8encompass((char*)TELESCOPE_getproperty_contents(),&control_textedit_cursor,&tl_length)>0)
							{
								TELESCOPE_shrink(control_textedit_cursor,tl_length);
							}
						}
						break;
						case SDLK_DELETE:
						if (control_aggresstextcursor())
						{
							control_textedit_cursor+=3;
							while (control_textedit_cursor>=strlen((char*)TELESCOPE_getproperty_contents()))
							{
								if (TELESCOPE_searchthroughobject_next(TELESCOPE_ELEMENTTYPE_s)<=0)
								{
									goto control_textedit_KEY_abort;
								}
								else
								{
									control_textedit_cursor=0;
								}
							}
							if (utf8encompass((char*)TELESCOPE_getproperty_contents(),&control_textedit_cursor,&tl_length)>0)
							{
								TELESCOPE_shrink(control_textedit_cursor,tl_length);
							}
						}
						break;
						case SDLK_RETURN:
						if (control_aggresstextcursor())
						{
							if (control_textedit_type==STRUCTURE_OBJECTTYPE_n)
							{
								TELESCOPE_shrink(control_textedit_cursor,3);
								if (control_aggresstextcursor("\uE001"))
								{
									TELESCOPE_shrink(control_textedit_cursor,3);
								}
								edit_resortstring(glob_n_multilist,control_textedit_index);
								control_mousestate=0;
							}
							else
							{
								TELESCOPE_insertintoproperties_offset("\n",1,control_textedit_cursor);
							}
						}
						else
						{
							control_mousestate=0;
						}
						break;
						#ifndef SDL2
						default:;
						char * tl_unicode;
						if (control_aggresstextcursor())
						{
							unicodeinput:;
							utf8encode(getunicode(&control_Event),&tl_unicode);
							if (strcmp(tl_unicode,"")!=0)
							{
								if (control_textedit_selectmode==1)
								{
									control_squashselection();
								}
							}
							TELESCOPE_insertintoproperties_offset(tl_unicode,strlen(tl_unicode),control_textedit_cursor);
						}
						#endif
					}
					control_textedit_KEY_abort:;
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
		LHENDRAW_leave=userwarning("\n\n  Quit without saving?");
	}
	if (LHENDRAW_leave==1)
	{
		#ifndef SDL2
		if (video->flags & SDL_FULLSCREEN)
		{
			SDL_WM_ToggleFullScreen(video);
		}
		#endif
	}
}
