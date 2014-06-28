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
		for (int ilv1=1;ilv1<64;ilv1++)
		{
			int tl_mode=0;
			int tl_counter=0;
			int tl_precounter=0;
			int goodline=1;
			for (int ilv2=0;ilv2<255;ilv2++)
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
					sdl_commonmenucommon();
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
					case SDLK_ESCAPE:
					{
						LHENDRAW_filedlgmode=0;
						break;
					}
				}
				break;
			}
		}
	}
}
