//This unit transforms user input into commands to the program
char control_mousestate=0;//0: inactive; 1: from tool; 2: on menu 3: from tool, resuming
int control_toolaction=0;//1: move 2: move selection 3: tool specific
int control_tool=0;//1: Hand 2: 2coordinate Selection 3: Lasso, no matter which 4: Shift tool 5: Magnifying glass 6: Element draw 7: chemdraw draw 8: eraser 9: Arrows 10: graphic
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
	{(_u32)~0,3,-1},//eraser
};
typedef struct clickabilitymatrix_
{
	int mode;//0: groups //1 whole objects 2: manipulators(default) 3: fragments, such as bezierpoints
	int level0;//zero: selects outermost groups. positive: enters groups to n'th stage. negative: groups over this object by n, 0 would be the object.
	int types1;//0x1: structures 0x2: graphics 0x4: curves 0x8: beziers 0x10: texts 0x20: pictures 0x40: tlc plates 0x80: NMR spectra
	_u32 types2;//0x1: atoms 0x2: bonds 0x4: graphic lines 0x8: graphic plains 0x4: curve end/start points 0x8: curve bodies 0x10: bezier start/end points 0x20: bezier bodies 0x40: bezier fill 0x80: tlc spots 0x100: NMR curves
	int types3;//0x1: special scaling handles 0x2: shape-specific graphic manipulators 0x4: bezier cores 0x8: bezier manipulators 0x10: bezier mid-curve manipulating 0x20: arrow mid manipulators 0x20:
}clickabilitymatrix_;
clickabilitymatrix_ clickabilitymatrix={2,0,0,0xFFFFFFFF,0};
typedef struct MODIFIER_KEYS_
{
	char CTRL;
	char ALT;
	char SHIFT;
	char SUPER;
}MODIFIER_KEYS_;
MODIFIER_KEYS_ MODIFIER_KEYS={0,0,0,0};

char storeundo(_u32 flags)
{
	int ilv0;
	for (int ilv1=0;ilv1<sizeof(multilist<n_instance>);ilv1++)
	{
		glob_n_undo_buffer[ilv1]=((char*)(glob_n_multilist))[ilv1];
	}
	for (int ilv1=0,ilv0=sizeof(multilist<n_instance>);ilv1<sizeof(n_instance)*bufferlistsize;ilv1++,ilv0++)
	{
		glob_n_undo_buffer[ilv0]=((char*)((*glob_n_multilist).bufferlist))[ilv1];
	}
	for (int ilv1=0;ilv1<1000000;ilv1++)
	{
		glob_n_undo_contentbuffer[ilv1]=internalstructure_n_buffer.buffer[ilv1];
	}
	internalstructure_n_undobuffer.max=internalstructure_n_buffer.max;
	internalstructure_n_undobuffer.count=internalstructure_n_buffer.count;
	for (int ilv1=0;ilv1<sizeof(multilist<b_instance>);ilv1++)
	{
		glob_b_undo_buffer[ilv1]=((char*)(glob_b_multilist))[ilv1];
	}
	for (int ilv1=0,ilv0=sizeof(multilist<b_instance>);ilv1<sizeof(b_instance)*bufferlistsize;ilv1++,ilv0++)
	{
		glob_b_undo_buffer[ilv0]=((char*)((*glob_b_multilist).bufferlist))[ilv1];
	}
	for (int ilv1=0;ilv1<1000000;ilv1++)
	{
		glob_b_undo_contentbuffer[ilv1]=internalstructure_b_buffer.buffer[ilv1];
	}
	internalstructure_b_undobuffer.max=internalstructure_b_buffer.max;
	internalstructure_b_undobuffer.count=internalstructure_b_buffer.count;
/*	for (int ilv1=0;ilv1<sizeof(multilist<text_instance>);ilv1++)
	{
		glob_text_undo_buffer[ilv1]=((char*)(glob_text_multilist))[ilv1];
	}
	for (int ilv1=0,ilv0=sizeof(multilist<text_instance>);ilv1<sizeof(text_instance)*bufferlistsize;ilv1++,ilv0++)
	{
		glob_text_undo_buffer[ilv0]=((char*)((*glob_text_multilist).bufferlist))[ilv1];
	}
	for (int ilv1=0;ilv1<1000000;ilv1++)
	{
		glob_text_undo_contentbuffer[ilv1]=internalstructure_text_buffer.buffer[ilv1];
	}
	internalstructure_text_undobuffer.max=internalstructure_text_buffer.max;
	internalstructure_text_undobuffer.count=internalstructure_text_buffer.count;*/
	return 1;
}
char restoreundo(_u32 flags)//doesn't discard the old undo state, loads ONLY
{
	int ilv0;
	for (int ilv1=0;ilv1<sizeof(multilist<n_instance>);ilv1++)
	{
		((char*)(glob_n_multilist))[ilv1]=glob_n_undo_buffer[ilv1];
	}
	for (int ilv1=0,ilv0=sizeof(multilist<n_instance>);ilv1<sizeof(n_instance)*bufferlistsize;ilv1++,ilv0++)
	{
		((char*)((*glob_n_multilist).bufferlist))[ilv1]=glob_n_undo_buffer[ilv0];
	}
	for (int ilv1=0;ilv1<1000000;ilv1++)
	{
		internalstructure_n_buffer.buffer[ilv1]=glob_n_undo_contentbuffer[ilv1];
	}
	internalstructure_n_buffer.max=internalstructure_n_undobuffer.max;
	internalstructure_n_buffer.count=internalstructure_n_undobuffer.count;
	for (int ilv1=0;ilv1<sizeof(multilist<b_instance>);ilv1++)
	{
		((char*)(glob_b_multilist))[ilv1]=glob_b_undo_buffer[ilv1];
	}
	for (int ilv1=0,ilv0=sizeof(multilist<b_instance>);ilv1<sizeof(b_instance)*bufferlistsize;ilv1++,ilv0++)
	{
		((char*)((*glob_b_multilist).bufferlist))[ilv1]=glob_b_undo_buffer[ilv0];
	}
	for (int ilv1=0;ilv1<1000000;ilv1++)
	{
		internalstructure_b_buffer.buffer[ilv1]=glob_b_undo_contentbuffer[ilv1];
	}
	internalstructure_b_buffer.max=internalstructure_b_undobuffer.max;
	internalstructure_b_buffer.count=internalstructure_b_undobuffer.count;
}
void clickforthem()
{
	selection_found=0;
	if (clickabilitymatrix.mode==1)
	{
		if (clickabilitymatrix.types1 & 1)
		{
			selection_found|=clickfor(control_coorsx,control_coorsy,STRUCTURE_OBJECTTYPE_n)<<STRUCTURE_OBJECTTYPE_n;
			selection_found|=clickfor(control_coorsx,control_coorsy,STRUCTURE_OBJECTTYPE_b)<<STRUCTURE_OBJECTTYPE_b;
			for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
			{
				
			}
//			selectwholestructure
		}
	}
	if (clickabilitymatrix.mode==2)
	{
		selection_found|=(clickfor(control_coorsx,control_coorsy,STRUCTURE_OBJECTTYPE_n)>0)<<STRUCTURE_OBJECTTYPE_n;
	}
}
int issueclick(int iposx,int iposy)
{
	int ibackval;
	n_instance * tlatom;
	control_posx=iposx;
	control_posy=iposy;
	control_coorsx=control_posx/SDL_zoomx+SDL_scrollx;
	control_coorsy=control_posy/SDL_zoomy+SDL_scrolly;
	clearselection(clickselection);
	storeundo(~0);
	clickforthem();
	switch (control_tool)
	{
		case 2:
		{
			selection_frame.startx=control_coorsx;
			selection_frame.starty=control_coorsy;
			break;
		}
		case 7:
		{
			control_startx=control_coorsx;
			control_starty=control_coorsy;
			control_id=-1;
			tlatom=NULL;
			if (!MODIFIER_KEYS.ALT)
			{
				if (selection_found & (1<<STRUCTURE_OBJECTTYPE_n))
				{
					tlatom=snapatom(control_coorsx,control_coorsy);
					if (tlatom)
					{
						control_id=(*tlatom).id;
						control_startx=(*tlatom).xyz.x;
						control_starty=(*tlatom).xyz.y;
					}
				}
				else
				{
					if (selection_found & (1<<STRUCTURE_OBJECTTYPE_b))
					{
						//TODO: doublify bond
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
	int ideltax=iposx-control_posx;
	int ideltay=iposy-control_posy;
	control_coorsx=iposx/SDL_zoomx+SDL_scrollx;
	control_coorsy=iposy/SDL_zoomy+SDL_scrolly;
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
	switch (control_tool)
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
			restoreundo(~0);
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
				}
			}
			clickforthem();
			if (selection_found & (1<<STRUCTURE_OBJECTTYPE_n))
			{
				tlatom2=snapatom(control_coorsx,control_coorsy);
			}
			if (!tlatom2)
			{
				tlatom2=summonatom();
				if (tlatom2)
				{
					(*tlatom2).xyz.x=control_coorsx;
					(*tlatom2).xyz.y=control_coorsy;
				}
			}
			if ((tlatom) && (tlatom2))
			{
				tlbond=summonbond();
				if (tlbond)
				{
					(*tlbond).color=0xFF;
					(*tlbond).Z=0;
					(*tlbond).Order=1;
					(*tlbond).B=(*tlatom).id;
					(*tlbond).E=(*tlatom2).id;
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
					if ((currentselection[ilv2]) & icompare)
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
			clearselection(currentselection);
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
										currentselection[ilv2]|=icompare;
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
}
void issuedelete()
{
	_u32 icompare;
	int isize;
	int ioffset;
	char * ibufferpos;
	if (control_mousestate==0)
	{
		if (storeundo(~0))
		{
			for (int ilv1=0;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
			{
				icompare=1<<ilv1;
				int isize= STRUCTURE_OBJECTTYPE_List[ilv1].size;
				basicmultilist * tlmultilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
				if (tlmultilist==NULL) goto i_delete_fertig;
				CDXMLREAD_functype tldummy;
				ibufferpos=(char*)((*tlmultilist).pointer);
				cdx_Point2D * tlpoint2d;
				for (int ilv2=0;ilv2<(*tlmultilist).filllevel;ilv2++)
				{
					if ((currentselection[ilv2]) & icompare)
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
		}
	}
	checkupinconsistencies();
}
#ifdef GFXOUT_SDL
int interpretkey(SDL_Event & tlEvent)
{
	char keystring[4]={0,0,0,0};

	switch (tlEvent.key.keysym.sym)
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
		default:;
		_u16 ihv1=(tlEvent.key.keysym.unicode);
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
	for (int ilv1=0;ilv1<hotkeylist_count;ilv1++)
	{
		if (strcmp(keystring,hotkeylist[ilv1].key)==0)
		{
			printf("%s:%s\n",hotkeylist[ilv1].variable,hotkeylist[ilv1].value);
		}
	}
}
#endif
void sdl_control()
{
	SDL_Event tlEvent;
	LHENDRAW_leave=0;
	char idirection=1;
	control_lastmousebutton=0;
	while ( SDL_PollEvent(&tlEvent) ) 
	{
		irepeatlabel:;
		char irepeat=0;
		switch (tlEvent.type) 
		{
			case SDL_MOUSEMOTION:
			{
				control_mousex=tlEvent.motion.x;
				control_mousey=tlEvent.motion.y;
				if (control_mousestate==1)
				{
					float tlx=tlEvent.motion.x-gfx_canvasminx;
					float tly=tlEvent.motion.y-gfx_canvasminy;
					if (SDL_PollEvent(&tlEvent))
					{
						if (tlEvent.type==SDL_MOUSEMOTION)
						{
							goto irepeatlabel;
						}
						else
						{
							irepeat=1;
						}
					}
					issuedrag(tlx,tly);
				}
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
			{
				if (control_mousestate==0)
				{
					if ((tlEvent.button.x<gfx_canvasminx) || (tlEvent.button.y<gfx_canvasminy) || (tlEvent.button.x>=gfx_canvasmaxx) || (tlEvent.button.y>=gfx_canvasmaxy))
					{
						issuemenuclick(tlEvent.button.x/32,tlEvent.button.y/32,tlEvent.button.button);
						break;
					}
				}
				switch (tlEvent.button.button)
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
							issueclick(tlEvent.button.x-gfx_canvasminx, tlEvent.button.y-gfx_canvasminy);
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
								SDL_scrollx+=(tlEvent.button.x-gfx_canvasminx)*((1/ifactor)-1)/SDL_zoomx;
								SDL_scrolly+=(tlEvent.button.y-gfx_canvasminy)*((1/ifactor)-1)/SDL_zoomy;
							}
							else
							{
								SDL_zoomx*=ifactor;
								SDL_zoomy*=ifactor;
								SDL_scrollx+=(tlEvent.button.x-gfx_canvasminx)*(ifactor-1)/SDL_zoomx;
								SDL_scrolly+=(tlEvent.button.y-gfx_canvasminy)*(ifactor-1)/SDL_zoomy;
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
				switch (tlEvent.button.button)
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
								issuedrag(tlEvent.motion.x-gfx_canvasminx, tlEvent.motion.y-gfx_canvasminy);
								issuerelease();
							}
						}
					}
				}
				break;
			}
			case SDL_KEYUP:
			{
				idirection=0;
			}
			case SDL_KEYDOWN://FALLTHROUGH
			{
				switch (tlEvent.key.keysym.sym)
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
					case SDLK_DELETE:
					{
						issuedelete();
						break;
					}
					default:
					interpretkey(tlEvent);
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
	}
}
