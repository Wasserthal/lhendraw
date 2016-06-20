char help_menu[]="HELP MENU\n\nPress the corresponding number key to select topic\n\n1: Introduction\n2: other tools\n3: command line\n4: selection tools\n5: atom drawing tools\n6: Bond manipulation\na: About\npress ESCAPE to return to program";
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
"If you want to modify an atom, use the \e35  tool.";
char help_commandline[]="COMMANDLINE\n\nAdding filenames to the lhendraw command will open them.\nAdding several Filenames will drop the contents on top of each other.\nYou can also use the command line to search for files containing a certain structure.\nTo do this, enter\nlhendraw searchpattern.cdxml --search files_to_search.cdxml\nTo get full information about the command-line functions, enter lhendraw -h";
char help_selectiontools[]="SELECTION TOOLS\n\n"
"the selection tools are \e9  and \e6  .\n\n"
"With \e9  , you can select rectangular areas and also click single items in their pivot spot to select them\n\n"
"With \e6  , you can draw an arbitrary frame around the objects you want to select.\n"
"Selection tools are influenced by the CTRL and SHIFT key.\n"
"Holding down CTRL when selecting does a OR operation with the old and new selection.\n"
"Holding down SHIFT when selecting does a XOR operation with the old and new selection.\n"
"Holding down SHIFT and CTRL when selecting does a AND operation with the old and new selection.\n"
"CTRL+a selects anything\n"
"CTRL+SHIFT+A removes any selection\n"
"If you are in selection mode, by means of the \e9  or \e6  tool,\n"
"you can use the right mouse button to drag the selected shapes.\n\n";
char help_atomtools[]="DRAWING ATOMS\n\n"
"atoms are drawn with the \e11  tool\n"
"you will usually draw carbon atoms only\n\n"
"otherwise, you have to use the \e12  tool.\n\n"
"the \e12 tool can be used to drop atoms directly.\n"
"for this purpose, click the appearing periodic table with the left mouse button\n"
"you can replace the atoms you click on with that element and drop new atoms of that element\n\n"
"but if the periodic table is clicked with the right mouse button, the \e11  tool will draw bonds\n"
"With that atom at the end instead.\n\n"
"(click the lower \e11  symbol on the tool-specific options of that tool to return to implicit carbon)\n"
"In the periodic table, there is an implicit-carbon symbol above the Cobalt atom\n"
"Here, there are also symbols for lone pair, lone electron, and the red symbols denote\n"
"different wildcards.\n"
"It is also possible to enter arbitrary text by Typing two letters closely after earch other\n\n"
"Or to edit an atom text by using the text tool(\e40  )\n"
"With the Enter key, you can also turn the atom under the cursor to the currently selected element\n\n"
"This switches the element for the \e11  tool back to implicit carbon\n";
char help_bondtools[]="BONDS\n\n"
"bonds can also be drawn with the \e11  tool by dragging with it between two existing atoms.\n"
"While it is selected, you can also select the bond type from the Symbols to the right.\n"
"combinations of styles and multiplicity are available.\n"
"Bond multiplicities can also be changed by entering numbers 1-4\n"
"To change the side where the second one of a \"double\" bond is, click onto that double bond again.\n"
"To select the bond's properties more controllably, use the popup button.\n"
"Note that lhendraw stores the bond thickness internally in 1/16ths.\n"
"clicking the bond tool onto an atom will draw a new bond in 120° angle to one existing bond\n"
"trying to get it away as far as possible from other bonds\n"
"The lowest symbols in the tool-specific menu just select the type of atom drawn at the new end of a\n"
"bond.\n"
"If bonds are selected while changing bond attributes, present bonds will also change\n";
char help_arrows[]="ARROWS\n\n"
"The arrows(\e4  ) in lhendraw should correspond to the arrow object in cambridge-compatible files\n"
"also have their corresponding toolspecific menu.\n"
"The first two of them are a tool to bend the arrows and one to make a bent arrow shorter\n"
"Without modifying the circle which it is a part of\n"
"The following settings are some where you can select the arrow style\n"
"Just as in every tool, changing the tool style while items are selected changes their style, too.\n";
char help_about[]="ABOUT\n\n"
"CDX/CDXML compatible molecule editor\n"
"LHENDRAW version " LHENDRAW_VERSIONSTRING "(beta)\n"
"written by Lennard Wasserthal:\n\n"
"\e184  \e185  \n\n"
"\e186  \e187  \n\n"
"\e188  \e189  \n\n"
"License: AFFERO GNU GENERAL PUBLIC LICENSE v.3\n"
"www.lennardwasserthal.de\n"
"written in C++\n"
"I also want to thank the alpha-tester, Florian Fischer:\n\n"
"\e190  \e191  \n\n"
"\e192  \e193  \n\n"
"\e194  \e195  \n\n";
char * helpstring=help_introduction;
void printmenutext(int posx,int posy,const char * iinput,int end,char symbolmode=0);
void control_help()
{
	SDL_Event control_Event;
	helpstring=help_menu;
	iback:
	if (SDL_PollEvent(&control_Event))
	{
		if (control_Event.type==SDL_KEYDOWN)
		{
			switch (control_Event.key.keysym.sym)
			{
				case SDLK_F1:
				case SDLK_1: helpstring=help_introduction;break;
				case SDLK_2: helpstring=help_othertools;break;
				case SDLK_3: helpstring=help_commandline;break;
				case SDLK_4: helpstring=help_selectiontools;break;
				case SDLK_5: helpstring=help_atomtools;break;
				case SDLK_6: helpstring=help_bondtools;break;
				case SDLK_m: helpstring=help_menu;break;
				case SDLK_a: helpstring=help_about;break;
				default:
				return;
			}
		}
		if (control_Event.type==SDL_QUIT) return;
	}
	gfx_gfxstart();
	screenclear(0xFFFFFF);
	printmenutext(0,20,helpstring,strlen(helpstring),1);
	gfx_gfxstop();
	goto iback;
}
