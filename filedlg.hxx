_u8 filedlg_devstring[]="/dev/";
catalogized_command_funcdef(CANCEL)
{
	if (LHENDRAW_warndlgmode)
	{
		LHENDRAW_userdecision=0;
		LHENDRAW_warndlgmode=0;
	}
	else
	{
		LHENDRAW_filedlgmode=0;
	}
	return 1;
}
void control_filedlg_datastorages()
{
	#ifndef NOPOSIX
	_u8 ihv1;
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
			ihv1=0;
			while (ihv1!=10)
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
					if (ihv1==10) {goto i_POSIX_linedone;}
					if (ihv1==' ')
					{
						tl_mode=2;
					}
					else
					{
						control_devicememory_buffer[control_devicememory.count][tl_counter]=ihv1;
						tl_counter++;
					}
				}
				if (tl_counter>63)
				{
					goodline=0;
					tl_mode=2;
				}
			}
			i_POSIX_linedone:;
			if (goodline) {control_devicememory_buffer[control_devicememory.count][tl_counter]=0;control_devicememory.count++;}
		}
		i_POSIX_done:;
		fclose(tl_POSIXFILE);
	}
	#endif
}
void control_filedlg()
{
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
					issuemenuclicks(control_Event.button.x,control_Event.button.y,control_Event.button.button);
					break;
				}
				break;
			}
			#ifdef SDL2
			case SDL_TEXTINPUT:
			{
				int tl_menunr,tl_index;
				menu_itemwadethrough(&menu_selectedmenuelement,&tl_menunr,&tl_index,0);
				AUTOSTRUCT_PULLOUTLISTING_ * tl_pulloutlisting=((AUTOSTRUCT_PULLOUTLISTING_*)(menu_list[tl_menunr].what.pointer))+tl_index;
				if (tl_pulloutlisting.lmbmode==0x301)
				{
					int ilength=strlen(control_Event.text.text);
					if (control_menutexteditcursor<=strlen(((char*)(tl_pulloutlisting->variable))))
					{
						for (int ilv1=strlen(((char*)(tl_pulloutlisting->variable)))+ilength-1;ilv1>=control_menutexteditcursor;ilv1--)
						{
							((char*)(tl_pulloutlisting->variable))[ilv1+ilength]=((char*)(tl_pulloutlisting->variable))[ilv1];
						}
						strncpy(((char*)(tl_pulloutlisting->variable))+control_menutexteditcursor,control_Event.text.text,ilength);
						control_menutexteditcursor=control_menutexteditcursor+=ilength;
					}
				}
				break;
			}
			#endif
			case SDL_QUIT:
			CANCEL(NULL,NULL);
			break;
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
							#ifndef SDL2
							SDL_WM_ToggleFullScreen(video);
							#endif
						}
						break;
					}
					case SDLK_ESCAPE:
					{
						if (idirection==1)
						{
							CANCEL(NULL,NULL);
						}
						break;
					}
					default:
					if (idirection)
					{
						int tl_menunr,tl_index;
						menu_itemwadethrough(&menu_selectedmenuelement,&tl_menunr,&tl_index,0);
						AUTOSTRUCT_PULLOUTLISTING_ * tl_pulloutlisting=((AUTOSTRUCT_PULLOUTLISTING_*)(menu_list[tl_menunr].what.pointer))+tl_index;
						switch (control_Event.key.keysym.sym)
						{
							case SDLK_TAB:
							{
								int tl_menunr,tl_index;
								if (MODIFIER_KEYS.SHIFT) menu_selectedmenuelement--; else menu_selectedmenuelement++;
								menu_itemwadethrough(&menu_selectedmenuelement,&tl_menunr,&tl_index,0);
								goto keyprocessed;
								break;
							}
							case SDLK_RETURN:
							{
								if ((*tl_pulloutlisting).LMB_function!=NULL)
								{
									if ((*tl_pulloutlisting).lmbmode==0x201)
									{
										structenum * tl_structenum=(structenum*)((*tl_pulloutlisting).variable);
										if ((*tl_structenum).number<tl_structenum->count)
										{
											(*tl_pulloutlisting).LMB_function(((char*)((*tl_structenum).pointer))+(*tl_structenum).size*(*tl_structenum).number,"");
										}
									}
									else
									{
										(*tl_pulloutlisting).LMB_function("","");
									}
								}
								goto keyprocessed;
								break;
							}
						}
						if (((*tl_pulloutlisting).lmbmode&(0xFF00))==0x200)
						{
							structenum * tl_structenum=(structenum*)((*tl_pulloutlisting).variable);
							switch (control_Event.key.keysym.sym)
							{
								case SDLK_PAGEUP:
								{
									tl_structenum->number-=20;
								}//FALLTHROUGH
								case SDLK_UP:
								{
									tl_structenum->number--;
									if (tl_structenum->number<0) tl_structenum->number=0;
									if (tl_structenum->number<tl_structenum->scroll) tl_structenum->scroll=tl_structenum->number;
									break;
								}
								case SDLK_HOME:
								{
									tl_structenum->number=0;
									if (tl_structenum->number<tl_structenum->scroll) tl_structenum->scroll=tl_structenum->number;
									break;
								}
								case SDLK_END:
								{
									tl_structenum->number=tl_structenum->count-1;
									int tl_max=(menu_addyifneg((*tl_pulloutlisting).maxy)-(*tl_pulloutlisting).y)/16;
									if (tl_structenum->number>=tl_structenum->scroll+tl_max) tl_structenum->scroll=tl_structenum->number-tl_max+1;
									break;
								}
								case SDLK_PAGEDOWN:
								{
									tl_structenum->number+=20;
								}//FALLTHROUGH
								case SDLK_DOWN://FALLTHROUGH
								{
									tl_structenum->number++;
									if (tl_structenum->number>=tl_structenum->count) tl_structenum->number=tl_structenum->count-1;
									int tl_max=(menu_addyifneg((*tl_pulloutlisting).maxy)-(*tl_pulloutlisting).y)/16;
									if (tl_structenum->number>=tl_structenum->scroll+tl_max) tl_structenum->scroll=tl_structenum->number-tl_max+1;
									break;
								}
							}
						}
						if (((*tl_pulloutlisting).lmbmode&(0xFF00))==0x300)
						{
							char tl_symboltype;
							char arrester=0;
							char * tl_editedstring=NULL;
							char numberstring[20];
							int maxruns=1000;
							iback:
							maxruns--;
							if (maxruns<=0) goto keyprocessed;
							if (((*tl_pulloutlisting).lmbmode&(0xFFFF))==0x301)
							{
								tl_editedstring=((char*)(tl_pulloutlisting->variable));
							}
							if (((*tl_pulloutlisting).lmbmode&(0xFFFF))==0x302)
							{
								tl_editedstring=numberstring;
								sprintf(tl_editedstring,"%i",*(int*)(tl_pulloutlisting[ilv1].variable));
							}
							if (control_menutexteditcursor>strlen(tl_editedstring)) control_menutexteditcursor=strlen(tl_editedstring);
							tl_symboltype=sentenumeric(tl_editedstring[control_menutexteditcursor]);
							arrester=0;
							if (((*tl_pulloutlisting).lmbmode&(0xFFFF))==0x302)
							{
								_i8 tl_numeral;
								switch (control_Event.key.keysym.sym)
								{
									case SDLK_KP_MINUS:
									case SDLK_MINUS:
									{
										(*((int*)tl_pulloutlisting->variable))=-(*((int*)tl_pulloutlisting->variable));
									}
									break;
									case SDLK_0:tl_0:tl_numeral=0;goto numeralfound;
									case SDLK_1:tl_1:tl_numeral=1;goto numeralfound;
									case SDLK_2:tl_2:tl_numeral=2;goto numeralfound;
									case SDLK_3:tl_3:tl_numeral=3;goto numeralfound;
									case SDLK_4:tl_4:tl_numeral=4;goto numeralfound;
									case SDLK_5:tl_5:tl_numeral=5;goto numeralfound;
									case SDLK_6:tl_6:tl_numeral=6;goto numeralfound;
									case SDLK_7:tl_7:tl_numeral=7;goto numeralfound;
									case SDLK_8:tl_8:tl_numeral=8;goto numeralfound;
									case SDLK_9:tl_9:tl_numeral=9;goto numeralfound;
									{
										numeralfound:;
										int position=strlen(tl_editedstring)-control_menutexteditcursor;
										if (position<0) position=0;
										_i32 lastval=(*((int*)tl_pulloutlisting->variable));
										_i32 mantisse=lastval/pow(10,max(position,0));
										_i32 rest=lastval%(_i32)pow(10,max(position,0));
										(*((int*)tl_pulloutlisting->variable))=mantisse*pow(10,max(position+1,0))+rest+pow(10,max(position,0))*tl_numeral;
										position++;
										control_menutexteditcursor++;
									}
									break;
									case SDLK_KP0:goto tl_0;
									case SDLK_KP1:goto tl_1;
									case SDLK_KP2:goto tl_2;
									case SDLK_KP3:goto tl_3;
									case SDLK_KP4:goto tl_4;
									case SDLK_KP5:goto tl_5;
									case SDLK_KP6:goto tl_6;
									case SDLK_KP7:goto tl_7;
									case SDLK_KP8:goto tl_8;
									case SDLK_KP9:goto tl_9;
									default:
									goto key_unprocessed;
								}
								goto keyprocessed;
							}
							key_unprocessed:;
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
									if (control_menutexteditcursor>strlen(tl_editedstring)) {control_menutexteditcursor=strlen(tl_editedstring);arrester=1;}
									if (control_menutextedithorziscroll+((tl_pulloutlisting->maxx-tl_pulloutlisting->x)/8)<control_menutexteditcursor) {control_menutextedithorziscroll=control_menutexteditcursor;arrester=1;}
									break;
								}
								case SDLK_END:
								{
									control_menutexteditcursor=strlen(tl_editedstring);
									arrester=1;
									if (control_menutexteditcursor>strlen(tl_editedstring)) {control_menutexteditcursor=strlen(tl_editedstring);arrester=1;}
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
									if (control_menutexteditcursor>=1)
									{
										if (sentenumeric(tl_editedstring[control_menutexteditcursor])!=sentenumeric(tl_editedstring[control_menutexteditcursor-1]))
										{
											arrester=1;
										}
									}
								}
								case SDLK_DELETE:
								{
									if (((*tl_pulloutlisting).lmbmode&(0xFFFF))==0x302)
									{
										int position=strlen(tl_editedstring)-control_menutexteditcursor;
										if (position<0) position=0;
										_i32 lastval=(*((int*)tl_pulloutlisting->variable));
										_i32 mantisse=lastval/pow(10,max(position,0));
										_i32 rest=lastval%(_i32)pow(10,max(position-1,0));
										position--;
										(*((int*)tl_pulloutlisting->variable))=mantisse*pow(10,max(position,0))+rest;
										if (abs(lastval)>=abs((*((int*)tl_pulloutlisting->variable))))
										{
											arrester=1;
										}
										break;
									}
									if (strlen(tl_editedstring)>control_menutexteditcursor)
									{
										for (int ilv1=control_menutexteditcursor;ilv1<strlen(tl_editedstring);ilv1++)
										{
											tl_editedstring[ilv1]=tl_editedstring[ilv1+1];
										}
									}
									else
									{
										arrester=1;
									}
									break;
								}
								#ifndef SDL2
								default:
								{
									_u16 ihv1=(getunicode(&control_Event));
									if (control_menutexteditcursor<=strlen(tl_editedstring))
									{
										for (int ilv1=strlen(tl_editedstring);ilv1>=control_menutexteditcursor;ilv1--)
										{
											tl_editedstring[ilv1+1]=tl_editedstring[ilv1];
										}
										tl_editedstring[control_menutexteditcursor++]=ihv1;
									}
									arrester=1;
								}
								#endif
							}
							if (MODIFIER_KEYS.CTRL)
							{
								if (arrester==0)
								{
									if (tl_symboltype==sentenumeric(tl_editedstring[control_menutexteditcursor]))
									{
										goto iback;
									}
								}
							}
						}
					}
					keyprocessed:;
				}
				break;
			}
		}
	}
}
int warndlg_loop(const char*warning)
{
	LHENDRAW_warndlgmode=1;
	menu_selectedmenuelement=0;
	while (LHENDRAW_warndlgmode)
	{
		control_filedlg();
		gfx_gfxstart();
		screenclear(0xFFFFFF);
		printmenutext(0,0,warning,strlen(warning),1);
		sdl_warndlgcommon();
		sdl_menudraw();
		sdl_menuframe();
		draw_reticle();
		gfx_gfxstop();
		usleep(1000);
		control_doubleclickenergy-=1;
		if (control_doubleclickenergy<0) control_doubleclickenergy=0;
	}
	return LHENDRAW_userdecision;
}
int keywarning(const char*warning)
{
	char ihv1;
	printf("\n");
	while ((*(warning))!=0)
	{
		if ((*warning)=='\n') goto ifertig;
		if ((*warning)=='\e') {while ((*warning)!=' ') warning++; goto ifertig;}
		printf("%c",(*warning));
		ifertig:;
		warning++;
	}
	printf("\n");
	while (fread(&ihv1,1,1,stdin)>0)
	{
		if ((ihv1=='y') || (ihv1=='Y'))
		{
			return 1;
		}
		if ((ihv1=='n') || (ihv1=='N'))
		{
			return 0;
		}
	}
	return 0;
}
int userwarning(const char*warning)
{
	if (control_force) return 1;
	if (control_GUI) return (warndlg_loop(warning));else return(keywarning(warning));
}
