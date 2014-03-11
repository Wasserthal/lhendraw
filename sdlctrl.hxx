char control_mousestate=0;//0: inactive; 1: from tool; 2: on menu 3: from tool, resuming
int control_toolaction=0;//1: move 2: move selection 3: tool specific
int tool=0;//1: Hand 2: 2coordinate Selection 3: Lasso, no matter which 4: 
int control_posx=0;
int control_posy=0;
float control_coorsx=0;
float control_coorsy=0;
char LHENDRAW_leave;
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

void issueclick(int iposx,int iposy)
{
	int ibackval;
	control_posx=iposx;
	control_posy=iposy;
	control_coorsx=control_posx*SDL_zoomx+SDL_scrollx;
	control_coorsy=control_posy*SDL_zoomy+SDL_scrolly;
	clearselection(clickselection);
	if (clickabilitymatrix.mode==1)
	{
		if (clickabilitymatrix.types1 & 1)
		{
			clickfor<n_instance>(control_coorsx,control_coorsy);
			clickfor<b_instance>(control_coorsx,control_coorsy);
			for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
			{
				
			}
//			selectwholestructure
		}
	}
	control_mousestate=1;
}
void issuedrag(int iposx,int iposy)
{
	int tldeltax=iposx-control_posx;
	int tldeltay=iposy-control_posy;
	SDL_scrollx-=tldeltax/SDL_zoomx;
	SDL_scrolly-=tldeltay/SDL_zoomy;
	control_posx=iposx;
	control_posy=iposy;
}
void issuerelease()
{
	control_mousestate=0;
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
				if (control_mousestate==1)
				{
					issuedrag(tlEvent.motion.x-gfx_canvasminx, tlEvent.motion.y-gfx_canvasminy);
				}
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
			{
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
