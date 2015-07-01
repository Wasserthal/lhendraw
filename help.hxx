char helpstring[]="Usage\n"
"The first thing you do is to use the \n\n\e11 \n\n tool. Draw a carbon chain (which contains implicit hydrogens by default).\n"
"If you want to draw other elements, use the \n\n\e12 \n\n tool. Select an atom or more, then click this tool. A periodic table appears.\n"
"click any Element there and the selected atoms will be that element.\n"
"If you draw on and want to set the element again, hit enter while hovering over that atom\n"
"By the way, many atoms can be made when hovering over a carbon and pressing a letter key.\n"
"The Element corresponding to that letter will appear.\n"
"If thats not enough, just press two keys quickly after each other and you can enter text.\n"
"To select things, you can either use the \n"
"\n\e9  or \e6  tool.\n\n"
"To undo, click \e1 , to undo an undo, click \e0 .\n\n"
"Arrows can be drawn with the \e4  tool.\n\n"
"You draw from start to end.\n"
"If you want to bend them, use the arrow -\"Skipping\" tool \e72 .\n"
"which can be selected from arrows mode.";
void printmenutext(int posx,int posy,const char * iinput,int end,char symbolmode=0);
void control_help()
{
	SDL_Event control_Event;
	iback:
	SDL_PollEvent(&control_Event);
	if (control_Event.type==SDL_KEYDOWN) return;
	gfx_gfxstart();
	screenclear(0xFFFFFF);
	printmenutext(0,20,helpstring,sizeof(helpstring)-1,1);
	gfx_gfxstop();
	goto iback;
}
