_u8 filedlg_devstring[]="/dev/";
int control_filedlg()
{
	_u8 ihv1;
	#ifndef NOPOSIX
	FILE * tl_POSIXFILE=fopen("/proc/mounts","r");
	control_devicememory_buffer[0][0]='/';control_devicememory_buffer[0][1]=0;
	control_devicememory.count=1;
	if (tl_POSIXFILE)
	{
		for (int ilv1=1;ilv1<255;ilv1++)
		{
			int tl_mode=0;
			int tl_counter=0;
			int tl_precounter=0;
			int goodline=1;
			for (int ilv2=0;ilv2<63;ilv2++)
			{
				int backval=fread(&ihv1,1,1,tl_POSIXFILE);
				if (backval==0) { goto i_POSIX_done;}
				if (tl_mode==0)
				{
					if (ihv1==10) {tl_mode=2; goodline=0;}
					if (ihv1==' ')
					{
						tl_mode=1;
					}
					if (tl_precounter<5)
					{
						if (ihv1!=filedlg_devstring[tl_precounter]) {tl_mode=2;goodline=0;}
						tl_precounter++;
					}
				}
				else if (tl_mode==1)
				{
					if (ihv1==10) {control_devicememory_buffer[control_devicememory.count][tl_counter]=0;goto i_POSIX_linedone;}
					if (ihv1==' ')
					{
						tl_mode=2;
						control_devicememory_buffer[control_devicememory.count][tl_counter]=0;
					}
					else
					{
						control_devicememory_buffer[control_devicememory.count][tl_counter]=ihv1;
						tl_counter++;
					}
				}
				else if (tl_mode==2)
				{
					if (ihv1==10) {goto i_POSIX_linedone;}
				}
			}
			i_POSIX_linedone:;
			if (goodline) control_devicememory.count++;
		}
		i_POSIX_done:;
		fclose(tl_POSIXFILE);
	}
	#endif
	char idirection=1;
	control_lastmousebutton=0;
	while ( SDL_PollEvent(&control_Event) ) 
	{
		char irepeat=0;
		char idontrepeat=0;
		char idirection=1;
		switch (control_Event.type) 
		{
			case SDL_MOUSEMOTION:
			{
				control_mousex=control_Event.motion.x;
				control_mousey=control_Event.motion.y;
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
			{
				if ((control_mousestate & (~0x18))==0)
				{
					sdl_filemenucommon();
					issuemenuclicks(control_Event.button.x,control_Event.button.y,control_Event.button.button);
					break;
				}
				break;
			}
			case SDL_KEYUP:
			{
				idirection=0;
				//FALLTHROUGH
			}
			case SDL_KEYDOWN://FALLTRHOUGH
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
							SDL_WM_ToggleFullScreen(video);
						}
						break;
					}
					case SDLK_ESCAPE:
					{
						LHENDRAW_filedlgmode=0;
						break;
					}
					case SDLK_UP:
					{
						break;
					}
					case SDLK_DOWN:
					{
						break;
					}
					default:
					if (idirection)
					{
						switch (control_Event.key.keysym.sym)
						{
							case SDLK_TAB:
							{
								int tl_menunr,tl_index;
								if (MODIFIER_KEYS.SHIFT) menu_selectedmenuelement--; else menu_selectedmenuelement++;
								menu_itemwadethrough(&menu_selectedmenuelement,&tl_menunr,&tl_index,0);
								printf("%i\n",menu_selectedmenuelement);
								break;
							}
						}
						int tl_menunr,tl_index;
						menu_itemwadethrough(&menu_selectedmenuelement,&tl_menunr,&tl_index,0);
						AUTOSTRUCT_PULLOUTLISTING_ * tl_pulloutlisting=((AUTOSTRUCT_PULLOUTLISTING_*)(menu_list[tl_menunr].what.pointer))+tl_index;
						if (((*tl_pulloutlisting).lmbmode&(0xFF00))==0x300)
						{
							char tl_symboltype;
							char arrester=0;
							tl_symboltype=sentenumeric(control_filenamehead[control_menutexteditcursor]);
							iback:
							arrester=0;
							switch (control_Event.key.keysym.sym)
							{
								case SDLK_LEFT:
								{
									control_menutexteditcursor--;
									if (control_menutexteditcursor<0) {control_menutexteditcursor=0;arrester=1;}
									if (control_menutextedithorziscroll>control_menutexteditcursor) {control_menutextedithorziscroll=control_menutexteditcursor;arrester=1;}
									break;
								}
								case SDLK_RIGHT:
								{
									control_menutexteditcursor++;
									if (control_menutexteditcursor>strlen(control_filenamehead)) {control_menutexteditcursor=strlen(control_filenamehead);arrester=1;}
									if (control_menutextedithorziscroll+((tl_pulloutlisting->maxx-tl_pulloutlisting->x)/8)<control_menutexteditcursor) {control_menutextedithorziscroll=control_menutexteditcursor;arrester=1;}
									break;
								}
								case SDLK_END:
								{
									control_menutexteditcursor=strlen(control_filenamehead);
									arrester=1;
									if (control_menutexteditcursor>strlen(control_filenamehead)) {control_menutexteditcursor=strlen(control_filenamehead);arrester=1;}
									if (control_menutextedithorziscroll+((tl_pulloutlisting->maxx-tl_pulloutlisting->x)/8)<control_menutexteditcursor) {control_menutextedithorziscroll=control_menutexteditcursor;arrester=1;}
									break;
								}
								case SDLK_HOME:
								{
									control_menutexteditcursor=0;
									arrester=1;
									if (control_menutexteditcursor<0) {control_menutexteditcursor=0;arrester=1;}
									if (control_menutextedithorziscroll>control_menutexteditcursor) {control_menutextedithorziscroll=control_menutexteditcursor;arrester=1;}
									break;
								}
								case SDLK_BACKSPACE:
								{
									control_menutexteditcursor--;
									if (control_menutexteditcursor<0) {control_menutexteditcursor=0;arrester=1;}
									if (control_menutextedithorziscroll>control_menutexteditcursor) {control_menutextedithorziscroll=control_menutexteditcursor;arrester=1;}
								}
								case SDLK_DELETE:
								{
									if (strlen(control_filenamehead)>control_menutexteditcursor)
									{
										for (int ilv1=control_menutexteditcursor;ilv1<strlen(control_filenamehead);ilv1++)
										{
											control_filenamehead[ilv1]=control_filenamehead[ilv1+1];
										}
									}
									else
									{
										arrester=1;
									}
									break;
								}
							}
							if (MODIFIER_KEYS.CTRL)
							{
								if (arrester==0)
								{
									if (tl_symboltype==sentenumeric(control_filenamehead[control_menutexteditcursor]))
									{
										goto iback;
									}
								}
							}
						}
					}
				}
				break;
			}
		}
	}
}
