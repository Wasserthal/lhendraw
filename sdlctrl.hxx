//This unit transforms user input into commands to the program
SDL_Event control_Event;
char control_mousestate=0;//0: inactive; 1: from tool; 2: on menu 3: from tool, resuming
int control_toolaction=0;//1: move 2: move selection 3: tool specific
int control_tool=0;//1: Hand 2: 2coordinate Selection 3: Lasso, no matter which 4: Shift tool 5: Magnifying glass 6: Element draw 7: chemdraw draw 8: eraser 9: Arrows 10: graphic
int control_keycombotool=0;//as above, but only valid if (mousestate & 2)
SDLKey control_toolstartkeysym;
int control_lastinterpret=-1;
int control_posx=0;
int control_posy=0;
long long control_id=-1;
float control_coorsx=0;
float control_coorsy=0;
int control_mousex,control_mousey;
char LHENDRAW_leave;
char control_reticle=1;
char control_dragged=0;
char control_lastmousebutton=0;
char control_usingmousebutton=0;
char control_clickforpriority=0;//0: frontmost 1: rearmost 2: nearest 3: nearest3d
char control_ambiguousity=0;
float control_startx=0;
float control_starty=0;
typedef struct contro_toolinfo_
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
};
typedef struct clickabilitymatrix_
{
	int mode;//0: groups //1 whole objects 2: manipulators(default) 3: fragments, such as bezierpoints
	int level0;//zero: selects outermost groups. positive: enters groups to n'th stage. negative: groups over this object by n, 0 would be the object.
	int types1;//0x1: structures 0x2: graphics 0x4: curves 0x8: beziers 0x10: texts 0x20: pictures 0x40: tlc plates 0x80: NMR spectra
	_u32 types2;//list of object groups to process click. See internal_enum.hxx
	int types3[32];//indexed like types2.//0x1: centers 0x2: auxpoints, e.g. endpoints 0x4: virtual scaling handles 0x8: shape-specific graphic manipulators, like mid-arrow manipulators or corner roundness controllers 0x10: lines 0x20: area
}clickabilitymatrix_;
clickabilitymatrix_ clickabilitymatrix={2,0,0,0xFFFFFFFF,
{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,}};
typedef struct MODIFIER_KEYS_
{
	char CTRL;
	char ALT;
	char SHIFT;
	char SUPER;
}MODIFIER_KEYS_;
MODIFIER_KEYS_ MODIFIER_KEYS={0,0,0,0};

void checkupinconsistencies()
{
	for (int ilv1=0;ilv1<(*glob_b_multilist).filllevel;ilv1++)
	{
		b_instance * tl_b_instance=&((*glob_b_multilist).bufferlist[ilv1]);
		if ((*tl_b_instance).exist)
		{
			for (char ilv2=0;ilv2<2;ilv2++)
			{
				if (!edit_locatebyid(STRUCTURE_OBJECTTYPE_n,ilv2?((*tl_b_instance).B):((*tl_b_instance).E),NULL))
				{
					(*tl_b_instance).exist=0;
					goto destroyed;
				}
			}
		}
		destroyed:;
	}
	getatoms();
	for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
	{
		n_instance * tl_n_instance=&((*glob_n_multilist).bufferlist[ilv1]);
		if ((*tl_n_instance).exist)
		{
			if (atom_actual_node[ilv1].bondcount==0)
			{
				(*tl_n_instance).exist=0;
			}
		}
	}
	for (int ilv2=0;ilv2<(*glob_b_multilist).filllevel;ilv2++)
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
							(*glob_b_multilist).bufferlist[ilv2].Order+=1;//TODO: *16
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
	int ilv1;
	_u32 tltype;
	char keystring[4]={0,0,0,0};
	char erledigt=0;
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
		default:;
		_u16 ihv1=(control_Event.key.keysym.unicode);
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
	for (ilv1=0;ilv1<hotkeylist_count;ilv1++)
	{
		tltype=hotkeylist[ilv1].type;
		if (((1<<(tltype & 0xFFFF)) & selection_currentselection_found) || ((tltype & 0xFFFF)==0))
		{
			if (((selection_currentselection_found==0) || ((tltype & 0x10000)==0)) && ((selection_currentselection_found) || ((tltype & 0x20000)==0)))
			{
				if (strcmp(keystring,hotkeylist[ilv1].key)==0)
				{
					if (tltype & 0x40000)
					{
						issueshiftstart();
						control_keycombotool=0x10000;
						control_lastinterpret=ilv1;
					}
					interpreted:
					erledigt=0;
					if (hotkeylist[ilv1].command!=NULL)
					{
						if (hotkeylist[ilv1].command(hotkeylist[ilv1].variable,hotkeylist[ilv1].value)){erledigt=1;};
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
	if (clickabilitymatrix.mode==1)
	{
		if (clickabilitymatrix.types1 & 1)
		{
			selection_clickselection_found|=clickfor(control_coorsx,control_coorsy,STRUCTURE_OBJECTTYPE_n,iclickradius)<<STRUCTURE_OBJECTTYPE_n;
			selection_clickselection_found|=clickfor(control_coorsx,control_coorsy,STRUCTURE_OBJECTTYPE_b,iclickradius)<<STRUCTURE_OBJECTTYPE_b;
			for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
			{
				
			}
//			selectwholestructure
		}
	}
	if (clickabilitymatrix.mode==2)
	{
		for (int ilv1=1;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
		{
			if (clickabilitymatrix.types2 & (1<<ilv1))
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
		if (control_hotatom!=-1)
		{
			if (control_hotatom<(*glob_n_multilist).filllevel)
			{
				ibufferpos=(char*)(*glob_n_multilist).pointer;
				isize=STRUCTURE_OBJECTTYPE_List[STRUCTURE_OBJECTTYPE_n].size;
				if ((*glob_n_multilist).bufferlist[control_hotatom].exist)
				{
					TELESCOPE_aggressobject(glob_n_multilist,control_hotatom);
					TELESCOPE_clear();
					(*((basic_instance*)(ibufferpos+isize*control_hotatom))).exist=0;
					isuccessful=1;
				}
			}
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
					(*tlbond).Order+=1;//TODO: *16
					if ((*tlbond).Order>4)//TODO
					{
						(*tlbond).Order=1;
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
	}
	control_mousestate=1;
	control_usingmousebutton=control_lastmousebutton;
	return 0;
}
void issuedrag(int iposx,int iposy)
{
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
		case 7:
		{
			restoreundo(~0,1);
			n_instance * tlatom,*tlatom2;
			b_instance * tlbond;
			tlatom=NULL;
			tlatom2=NULL;
			tlbond=NULL;
			if (control_id!=-1)
			{
				tlatom=(n_instance*)edit_locatebyid(STRUCTURE_OBJECTTYPE_n,control_id,NULL);
			}
			if (!tlatom)
			{
				tlatom=summonatom();
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
					tlatom2=snapatom(control_coorsx,control_coorsy);
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
				tlatom2=snapatom_short(control_coorsx,control_coorsy,NULL,10);
			}
			if (!tlatom2)
			{
				tlatom2=summonatom();
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
					tlbond=summonbond((*tlatom).id,(*tlatom2).id);
					if (tlbond)
					{
						(*tlbond).color=0xFF;
						(*tlbond).Z=0;
						(*tlbond).Order=1;
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
		case 0x10000:
		{
			interpretkey(control_lastinterpret);
		}
	}
	control_posx=iposx;
	control_posy=iposy;
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
			float tlswap;
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
			float ix,iy;
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
}
void issuemenuclick(int posx,int posy,int button)
{
	AUTOSTRUCT_PULLOUTLISTING_ * ipulloutlisting;
	for (int ilv1=0;ilv1<sizeof(AUTOSTRUCT_PULLOUTLISTING_toolbox)/sizeof(AUTOSTRUCT_PULLOUTLISTING_);ilv1++)
	{
		if ((AUTOSTRUCT_PULLOUTLISTING_toolbox[ilv1].x==posx) && (AUTOSTRUCT_PULLOUTLISTING_toolbox[ilv1].y==posy))
		{
			ipulloutlisting=&(AUTOSTRUCT_PULLOUTLISTING_toolbox[ilv1]);
		}
	}
	switch(button)
	{
		case SDL_BUTTON_LEFT:
		{
			switch ((*ipulloutlisting).lmbmode)
			{
				case 1: control_tool=(*ipulloutlisting).toolnr;break;
				case 2: (*ipulloutlisting).getflag(4,0);break;
			}
			break;
		}
		case SDL_BUTTON_RIGHT:
		{
			switch ((*ipulloutlisting).rmbmode)
			{
				case 1: control_tool=(*ipulloutlisting).toolnr;break;
				case 2: (*ipulloutlisting).getflag(4,0);break;
			}
			break;
		}
	}
}
long long counter_SDL;
void sdl_control()
{
	LHENDRAW_leave=0;
	char idirection=1;
	control_lastmousebutton=0;
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
				if (control_mousestate!=1)
				{
					_u32 tlfound=clickfor((control_Event.motion.x-gfx_canvasminx)/SDL_zoomx+SDL_scrollx,(control_Event.motion.y-gfx_canvasminy)/SDL_zoomy+SDL_scrolly,STRUCTURE_OBJECTTYPE_n,constants_clickradius)>0;
					if (tlfound)
					{
						for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
						{
							if (selection_clickselection[ilv1] & (1<<STRUCTURE_OBJECTTYPE_n))
							{
								if ((*glob_n_multilist).bufferlist[ilv1].exist)
								{
									control_hotatom=ilv1;
								}
							}
						}
					}
				}
				if (control_mousestate & 3)
				{
					float tlx=control_Event.motion.x-gfx_canvasminx;
					float tly=control_Event.motion.y-gfx_canvasminy;
					if (SDL_PollEvent(&control_Event))
					{
						if (control_Event.type==SDL_MOUSEMOTION)
						{
							goto irepeatlabel;
						}
						else
						{
							irepeat=1;
						}
					}
					idontrepeat=1;
					issuedrag(tlx,tly);
				}
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
			{
				if (control_mousestate==0)
				{
					if ((control_Event.button.x<gfx_canvasminx) || (control_Event.button.y<gfx_canvasminy) || (control_Event.button.x>=gfx_canvasmaxx) || (control_Event.button.y>=gfx_canvasmaxy))
					{
						issuemenuclick(control_Event.button.x/32,control_Event.button.y/32,control_Event.button.button);
						break;
					}
				}
				switch (control_Event.button.button)
				{
					case SDL_BUTTON_RIGHT:
					{
						control_lastmousebutton=2;
						goto clickshunt;
					}
					case SDL_BUTTON_LEFT:
					{
						control_lastmousebutton=1;
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
							float ifactor=1.414213562;
							if (MODIFIER_KEYS.ALT)
							{
								ifactor=1.090507733;
							}
							if (idirection==1)
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
						control_lastmousebutton=2;
						goto dragshunt;
					}
					case SDL_BUTTON_LEFT:
					{
						control_lastmousebutton=1;
						dragshunt:
						if (control_mousestate==1)
						{
							if (control_usingmousebutton==control_lastmousebutton)//Only when same button up as down
							{
								issuedrag(control_Event.motion.x-gfx_canvasminx, control_Event.motion.y-gfx_canvasminy);
								issuerelease();
							}
						}
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
						SDL_WM_ToggleFullScreen(video);
						break;
					}
					case SDLK_ESCAPE:
					{
						LHENDRAW_leave=1;
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
			goto irepeatlabel;
		}
		if (idontrepeat)
		{
			goto iloopendlabel;
		}
	}
	iloopendlabel:;
}
