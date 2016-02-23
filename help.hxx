char help_menu[]="HELP MENU\n\nPress the corresponding number key to select topic\n\n1: Introduction\n2: other tools\n3: command line\n\npress ESCAPE to return to program";
char help_introduction[]="Usage\n"
"The first thing you do is to use the \n\n\e11  tool.\nDraw a carbon chain (which contains implicit hydrogens by default).\n"
"If you want to draw other elements, use the \n\n\e12  tool.\nSelect an atom or more, then click this tool. A periodic table appears.\n"
"click any Element there and the selected atoms will be that element.\n"
"If you draw on and want to set the element again, hit enter while hovering over that atom.\n"
"By the way, many atoms can be made when hovering over a carbon and pressing a letter key.\n"
"The Element corresponding to that letter will appear.\n"
"If thats not enough, just press two keys quickly after each other and you can enter text.\n"
"To select things, you can either use the \n"
"\n\e9  or \e6  tool.\n\n"
"To undo, click \e1 , to undo an undo, click \e0 .\n\n"
"Arrows can be drawn with the \e4  tool.\n\n"
"You draw the arrow from start to end.\n"
"If you want to bend them, use the arrow -\"Skipping\" tool \e72 .\n"
"which can be selected from arrows mode.";
char help_othertools[]="OTHER TOOLS\n\n"
"When a tool is selected, right of the main display,\nconfiguration parameters for that tool appear.\n\n"
"If you are in selection mode, by means of the \e9  or \e6  tool,\n"
"you can use the right mouse button to drag the selected shapes.\n\n"
"If you want to modify an atom, use the \e35  tool.";
char help_commandline[]="COMMANDLINE\n\nAdding filenames to the lhendraw command will open them.\nAdding several Filenames will drop the contents on top of each other.\nYou can also use the command line to search for files containing a certain structure.\nTo do this, enter\nlhendraw searchpattern.cdxml --search files_to_search.cdxml\nTo get full information about the command-line functions, enter lhendraw -h";
char * helpstring=help_introduction;
void printmenutext(int posx,int posy,const char * iinput,int end,char symbolmode=0);
void control_help()
{
	SDL_Event control_Event;
	helpstring=help_menu;
	iback:
	SDL_PollEvent(&control_Event);
	if (control_Event.type==SDL_KEYDOWN)
	{
		switch (control_Event.key.keysym.sym)
		{
			case SDLK_F1:
			case SDLK_1: helpstring=help_introduction;break;
			case SDLK_2: helpstring=help_othertools;break;
			case SDLK_3: helpstring=help_commandline;break;
			case SDLK_m: helpstring=help_menu;break;
			default:
			return;
		}
	}
	if (control_Event.type==SDL_QUIT) return;
	gfx_gfxstart();
	screenclear(0xFFFFFF);
	printmenutext(0,20,helpstring,strlen(helpstring),1);
	gfx_gfxstop();
	goto iback;
}
