char control_mousestate=0;
int control_posx=0;
int control_posy=0;

void issueclick(int iposx,int iposy)
{
	control_posx=iposx;
	control_posy=iposy;
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
				sdl_outit();
				exit(0);
				break;
		}
	}
}
