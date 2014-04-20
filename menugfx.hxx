//any graphics output which is not data of the document
struct menuref_
{
	char type;//0: buttons 1: popup and the like
	structenum what;//The structenum
	int alignx,aligny;//The position
};
menuref_ menu_list[20];
int menu_list_count=0;
int sdl_toolboxitemdraw(int posx,int posy,int gfxno,_u32 state)
{
	_u32 * ibutton=resources_bitmap_buttons[0][0];
	_u32 * ihgd=resources_bitmap_buttons[1][0];
	int idelta=(state & 0xD)>>1;
	for (int ilv2=0;ilv2<32-idelta;ilv2++)
	{
		for (int ilv3=0;ilv3<32-idelta;ilv3++)
		{
			_u32 tlgfx=resources_bitmap_buttons[selection_maxbuttons-gfxno][ilv2][ilv3];
			_u8 tlgfr=(tlgfx>>16) & 0xFF;
			_u8 tlgfg=(tlgfx>>8) & 0xFF;
			_u8 tlgfb=tlgfx & 0xFF;
			if ((state & 1)==1)
			{
				tlgfr=((1+(((tlgfx>>24) & 0xFF)))*tlgfr+((state & 0xFF000000)>>24)*(0xFF-((tlgfx>>24) & 0xFF)))>>8;
				tlgfg=((1+(((tlgfx>>24) & 0xFF)))*tlgfg+((state & 0xFF0000)>>16)*(0xFF-((tlgfx>>24) & 0xFF)))>>8;
				tlgfb=((1+(((tlgfx>>24) & 0xFF)))*tlgfb+((state & 0xFF00)>>8)*(0xFF-((tlgfx>>24) & 0xFF)))>>8;
			}
			screen[gfx_screensizex*(posy+ilv2+idelta)+posx+ilv3+idelta]=((tlgfx & 0xFF000000)==0x00000000) ? resources_bitmap_buttons[selection_maxbuttons-17][ilv2][ilv3]:((tlgfr<<16)+(tlgfg<<8)+(tlgfb));
		}
	}
	if (idelta>0)
	{
		for (int ilv1=0;ilv1<idelta;ilv1++)
		{
			for (int ilv2=0;ilv2<32;ilv2++)
			{
				screen[gfx_screensizex*(posy+ilv1)+posx+ilv2]=resources_bitmap_buttons[selection_maxbuttons-16][ilv1][ilv2];
				screen[gfx_screensizex*(posy+ilv2)+posx+ilv1]=resources_bitmap_buttons[selection_maxbuttons-16][ilv2][ilv1];
			}
		}
	}
}
int quersum(_u32 input,int max=32)
{
	char wert=0;
	_u32 iinput=input;
	for (int ilv1=0;ilv1<max;ilv1++)
	{
		wert+=((iinput & 1)==1);
		iinput=iinput>>1;
	}
	return wert;
}
int sdl_buttonmenudraw(AUTOSTRUCT_PULLOUTLISTING_ * ilisting,int count,int xpos=0,int ypos=0)
{
	for (int ilv1=0;ilv1<count;ilv1++)
	{
		_u32 state=ilisting[ilv1].bgcolor;
		state=(*ilisting).bgcolor<<8;
		switch (ilisting[ilv1].lmbmode)
		{
			case 1: if (control_tool==ilisting[ilv1].toolnr) state|=9;break;
			case 2: state|=ilisting[ilv1].getflag(2,0); break;
			case 4: 
			{
				int tlval=quersum(*(_u32*)(ilisting[ilv1].variable),STRUCTURE_OBJECTTYPE_ListSize);
				state=(((ilisting[ilv1].bgcolor & 0xFF)*tlval*256/STRUCTURE_OBJECTTYPE_ListSize)&0xFF00)+(((ilisting[ilv1].bgcolor & 0xFF00)*tlval*256/STRUCTURE_OBJECTTYPE_ListSize)&0xFF0000)+(((ilisting[ilv1].bgcolor & 0xFF0000)*tlval*256/STRUCTURE_OBJECTTYPE_ListSize)&0xFF000000);
				state|=1;state|=((((tlval*8)-1)/STRUCTURE_OBJECTTYPE_ListSize)&7)<<1; break;
			}
		}
		sdl_toolboxitemdraw(ilisting[ilv1].x*32+xpos,ilisting[ilv1].y*32+ypos,ilisting[ilv1].picno,state);
	}
}
int sdl_canvasframedraw()
{
	for (int ilv1=gfx_canvasminx;ilv1<gfx_canvasmaxx;ilv1++)
	{
		screen[(gfx_screensizex*gfx_canvasminy-1)+ilv1]=0;
	}
	for (int ilv1=gfx_canvasminx;ilv1<gfx_canvasmaxx;ilv1++)
	{
		screen[(gfx_screensizex*gfx_canvasmaxy)+ilv1]=0;
	}
	for (int ilv1=gfx_canvasminy;ilv1<gfx_canvasmaxy;ilv1++)
	{
		screen[(gfx_screensizex*ilv1-1)+gfx_canvasminx]=0;
	}
	for (int ilv1=gfx_canvasminy;ilv1<gfx_canvasmaxy;ilv1++)
	{
		screen[(gfx_screensizex*ilv1)+gfx_canvasmaxx]=0;
	}
}
structenum * searchreflectedstruct(const char * input);
int addmenu(const char * name,int type,int alignx=0,int aligny=0)
{
	structenum * tlstructenum;
	tlstructenum=searchreflectedstruct(name);
	if (tlstructenum)
	{
		menu_list[menu_list_count].type=type;
		menu_list[menu_list_count].what=*tlstructenum;
		menu_list[menu_list_count].alignx=alignx;
		menu_list[menu_list_count].aligny=aligny;
		menu_list_count++;
	}
}
extern int control_tool;
int sdl_commonmenudraw()
{
	char tlstring[60];
	menu_list_count=0;
	addmenu("toolbox",0);
	for (int ilv1=0;ilv1<AUTOSTRUCT_PULLOUTLISTING_toolbox_Size;ilv1++)
	{
		if (AUTOSTRUCT_PULLOUTLISTING_toolbox[ilv1].toolnr==control_tool)
		{
			snprintf(tlstring,60,"toolspecific_%s",AUTOSTRUCT_PULLOUTLISTING_toolbox[ilv1].name);
			tlstring[59]=0;
			addmenu(tlstring,0,gfx_screensizex-160,128);
		}
	}
	for (int ilv1=0;ilv1<menu_list_count;ilv1++)
	{
		if (menu_list[ilv1].type==0)
		{
			sdl_buttonmenudraw((AUTOSTRUCT_PULLOUTLISTING_*)menu_list[ilv1].what.pointer,menu_list[ilv1].what.count,menu_list[ilv1].alignx,menu_list[ilv1].aligny);
		}
	}
}
extern int control_hotatom;
int sdl_selectiondraw()
{
	_u32 icompare;
	int isize;
	char * ibufferpos;
	if (control_mousestate==1)
	{
		if (control_tool==2)
		{
			expressline(selection_frame.startx,selection_frame.starty,selection_frame.endx,selection_frame.starty);
			expressline(selection_frame.startx,selection_frame.starty,selection_frame.startx,selection_frame.endy);
			expressline(selection_frame.startx,selection_frame.endy,selection_frame.endx,selection_frame.endy);
			expressline(selection_frame.endx,selection_frame.starty,selection_frame.endx,selection_frame.endy);
		}
	}
	for (int ilv1=0;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
	{
		icompare=1<<ilv1;
		int isize= STRUCTURE_OBJECTTYPE_List[ilv1].size;
		basicmultilist * tlmultilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
		if (tlmultilist==NULL) goto i_fertig;
		CDXMLREAD_functype tldummy;
		ibufferpos=(char*)((*tlmultilist).pointer);
		float tlpx,tlpy;
		SDL_color=0x00FF00;
		for (int ilv2=0;ilv2<(*tlmultilist).filllevel;ilv2++)
		{
			if ((selection_currentselection[ilv2]) & icompare)
			{
				if ((*((basic_instance*)(ibufferpos+isize*ilv2))).exist)
				{
					int ilv3=0;
					iback:
					if (retrievepoints_basic(((basic_instance*)(ibufferpos+isize*ilv2)),&tlpx,&tlpy,ilv3,ilv1)>0)
					{
						putpixel((tlpx-SDL_scrollx)*SDL_zoomx+1,(tlpy-SDL_scrolly)*SDL_zoomy+1);
						putpixel((tlpx-SDL_scrollx)*SDL_zoomx+2,(tlpy-SDL_scrolly)*SDL_zoomy);
						putpixel((tlpx-SDL_scrollx)*SDL_zoomx-2,(tlpy-SDL_scrolly)*SDL_zoomy);
						putpixel((tlpx-SDL_scrollx)*SDL_zoomx-1,(tlpy-SDL_scrolly)*SDL_zoomy+1);
						putpixel((tlpx-SDL_scrollx)*SDL_zoomx+1,(tlpy-SDL_scrolly)*SDL_zoomy-1);
						putpixel((tlpx-SDL_scrollx)*SDL_zoomx,(tlpy-SDL_scrolly)*SDL_zoomy+2);
						putpixel((tlpx-SDL_scrollx)*SDL_zoomx,(tlpy-SDL_scrolly)*SDL_zoomy-2);
						putpixel((tlpx-SDL_scrollx)*SDL_zoomx-1,(tlpy-SDL_scrolly)*SDL_zoomy-1);
						ilv3++;
						goto iback;
					}
				}
			}
		}
		i_fertig:;
	}
		float tlpx,tlpy;
		SDL_color=0x3F3F00;
					if (control_hotatom!=-1)
					{
					int ilv3=0;
					iback2:
						if (retrievepoints_basic(((basic_instance*)(((char*)(*glob_n_multilist).pointer)+sizeof(n_instance)*control_hotatom)),&tlpx,&tlpy,ilv3,STRUCTURE_OBJECTTYPE_n)>0)
						{
							putpixel((tlpx-SDL_scrollx)*SDL_zoomx+1,(tlpy-SDL_scrolly)*SDL_zoomy+1);
							putpixel((tlpx-SDL_scrollx)*SDL_zoomx+2,(tlpy-SDL_scrolly)*SDL_zoomy);
							putpixel((tlpx-SDL_scrollx)*SDL_zoomx-2,(tlpy-SDL_scrolly)*SDL_zoomy);
							putpixel((tlpx-SDL_scrollx)*SDL_zoomx-1,(tlpy-SDL_scrolly)*SDL_zoomy+1);
							putpixel((tlpx-SDL_scrollx)*SDL_zoomx+1,(tlpy-SDL_scrolly)*SDL_zoomy-1);
							putpixel((tlpx-SDL_scrollx)*SDL_zoomx,(tlpy-SDL_scrolly)*SDL_zoomy+2);
							putpixel((tlpx-SDL_scrollx)*SDL_zoomx,(tlpy-SDL_scrolly)*SDL_zoomy-2);
							putpixel((tlpx-SDL_scrollx)*SDL_zoomx-1,(tlpy-SDL_scrolly)*SDL_zoomy-1);
							ilv3++;
							goto iback2;
						}
					}
	if (control_reticle)
	{
		if ((control_mousey<gfx_screensizey) && (control_mousey>=0))
		{
			for (int ilv1=0;ilv1<gfx_screensizex;ilv1++)
			{
				screen[gfx_screensizex*(control_mousey)+ilv1]^=0xFFFFFF;
			}
		}
		if ((control_mousex<gfx_screensizex) && (control_mousex>=0))
		{
			for (int ilv1=0;ilv1<gfx_screensizey;ilv1++)
			{
				screen[gfx_screensizex*(ilv1)+control_mousex]^=0xFFFFFF;
			}
		}
	}
}
