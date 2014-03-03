char control_mousestate=0;
int control_posx=0;
int control_posy=0;
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

void issueclick(int iposx,int iposy)
{
	control_posx=iposx;
	control_posy=iposy;
	if (true){}
	control_mousestate=1;
}
void issuedrag(int iposx,int iposy)
{
	int tldeltax=iposx-control_posx;
	int tldeltay=iposy-control_posy;
	SDL_scrollx-=tldeltax;
	SDL_scrolly-=tldeltay;
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
	while ( SDL_PollEvent(&tlEvent) ) 
	{
		switch (tlEvent.type) 
		{
			case SDL_MOUSEMOTION:
				if (control_mousestate>0)
				{
					issuedrag(tlEvent.motion.x, tlEvent.motion.y);
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				issueclick(tlEvent.button.x, tlEvent.button.y);
				break;
			case SDL_MOUSEBUTTONUP:
				issuedrag(tlEvent.motion.x, tlEvent.motion.y);
				issuerelease();
				break;
			case SDL_QUIT:
				LHENDRAW_leave=1;
				break;
		}
	}
}
