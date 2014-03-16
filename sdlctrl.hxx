char control_mousestate=0;//0: inactive; 1: from tool; 2: on menu 3: from tool, resuming
int control_toolaction=0;//1: move 2: move selection 3: tool specific
int control_tool=0;//1: Hand 2: 2coordinate Selection 3: Lasso, no matter which 4: Shift tool 5: Magnifying glass 6: Element draw 7: chemdraw draw 8: eraser 9: 
int control_posx=0;
int control_posy=0;
float control_coorsx=0;
float control_coorsy=0;
int control_mousex,control_mousey;
char LHENDRAW_leave;
char control_reticle=1;
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

int issueclick(int iposx,int iposy)
{
	int ibackval;
	control_posx=iposx;
	control_posy=iposy;
	control_coorsx=control_posx/SDL_zoomx+SDL_scrollx;
	control_coorsy=control_posy/SDL_zoomy+SDL_scrolly;
	clearselection(clickselection);
	switch (control_tool)
	{
		case 2:
		{
			selection_frame.startx=control_coorsx;
			selection_frame.starty=control_coorsy;
			break;
		}
	}
	if (clickabilitymatrix.mode==1)
	{
		if (clickabilitymatrix.types1 & 1)
		{
			clickfor(control_coorsx,control_coorsy,STRUCTURE_OBJECTTYPE_n);
			clickfor(control_coorsx,control_coorsy,STRUCTURE_OBJECTTYPE_b);
			for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
			{
				
			}
//			selectwholestructure
		}
	}
	control_mousestate=1;
	return 0;
}
char storeundo(_u32 flags)
{
	return 1;
}
void issuedrag(int iposx,int iposy)
{
	int ideltax=iposx-control_posx;
	int ideltay=iposy-control_posy;
	control_coorsx=iposx/SDL_zoomx+SDL_scrollx;
	control_coorsy=iposy/SDL_zoomy+SDL_scrolly;
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
		case 4:
		{
			if (storeundo(~0))//TODO Urgent: NEIN, eben NICHT! Immer nur am Anfang!
			{
				for (int ilv1=0;ilv1<sizeof(STRUCTURE_OBJECTTYPE_List)/sizeof(trienum);ilv1++)
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
			for (int ilv1=0;ilv1<sizeof(STRUCTURE_OBJECTTYPE_List)/sizeof(trienum);ilv1++)
			{
				icompare=1<<ilv1;
				int isize= STRUCTURE_OBJECTTYPE_List[ilv1].size;
				basicmultilist * tlmultilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
				if (tlmultilist==NULL) goto i_control2_fertig;
				CDXMLREAD_functype tldummy;
				ioffset=(*tlmultilist).getproperties("xyz",&tldummy);
				if (ioffset<0) goto i_control2_fertig;
				ibufferpos=(char*)((*tlmultilist).pointer);
				cdx_Point2D * tlpoint2d;
				if (tlmultilist!=NULL)
				{
					for (int ilv2=0;ilv2<(*tlmultilist).filllevel;ilv2++)
					{
						if ((*((basic_instance*)(ibufferpos+isize*ilv2))).exist)
						{
							tlpoint2d = ((cdx_Point2D*)(ibufferpos+isize*ilv2+ioffset));
							if (((*tlpoint2d).x>=selection_frame.startx) && ((*tlpoint2d).x<=selection_frame.endx))
							{
								if (((*tlpoint2d).y>=selection_frame.starty) && ((*tlpoint2d).y<=selection_frame.endy))
								{
									currentselection[ilv2]|=icompare;
								}
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
void sdl_control()
{
	SDL_Event tlEvent;
	LHENDRAW_leave=0;
	char idirection=1;
	while ( SDL_PollEvent(&tlEvent) ) 
	{
		switch (tlEvent.type) 
		{
			case SDL_MOUSEMOTION:
			{
				control_mousex=tlEvent.motion.x;
				control_mousey=tlEvent.motion.y;
				if (control_mousestate==1)
				{
					issuedrag(tlEvent.motion.x-gfx_canvasminx, tlEvent.motion.y-gfx_canvasminy);
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
					case SDL_BUTTON_LEFT:
					{
						issueclick(tlEvent.button.x-gfx_canvasminx, tlEvent.button.y-gfx_canvasminy);
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
					case SDL_BUTTON_LEFT:
					{
						if (control_mousestate==1)
						{
							issuedrag(tlEvent.motion.x-gfx_canvasminx, tlEvent.motion.y-gfx_canvasminy);
							issuerelease();
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
				}
				break;
			}
			case SDL_QUIT:
			{
				LHENDRAW_leave=1;
				break;
			}
		}
	}
}
