//any graphics output which is not data of the document
int sdl_toolboxitemdraw(int posx,int posy,int gfxno,char state)
{
	_u32 * ibutton=resources_bitmap_buttons[0][0];
	_u32 * ihgd=resources_bitmap_buttons[1][0];
	int idelta=(state & 2)?4:0;
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
				tlgfr=((1+(((tlgfx>>24) & 0xFF)))*tlgfr+0)>>8;
				tlgfg=((1+(((tlgfx>>24) & 0xFF)))*tlgfg+0x100*(0xFF-((tlgfx>>24) & 0xFF)))>>8;
				tlgfb=((1+(((tlgfx>>24) & 0xFF)))*tlgfb+0)>>8;
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
int sdl_toolboxdraw()
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
	for (int ilv1=0;ilv1<sizeof(AUTOSTRUCT_PULLOUTLISTING_toolbox)/sizeof(AUTOSTRUCT_PULLOUTLISTING_);ilv1++)
	{
		char state=0;
		switch (AUTOSTRUCT_PULLOUTLISTING_toolbox[ilv1].lmbmode)
		{
			case 1: if (control_tool==AUTOSTRUCT_PULLOUTLISTING_toolbox[ilv1].toolnr) state=3;break;
			case 2: state=AUTOSTRUCT_PULLOUTLISTING_toolbox[ilv1].getflag(2,0); break;
		}
		sdl_toolboxitemdraw(AUTOSTRUCT_PULLOUTLISTING_toolbox[ilv1].x*32,AUTOSTRUCT_PULLOUTLISTING_toolbox[ilv1].y*32,AUTOSTRUCT_PULLOUTLISTING_toolbox[ilv1].picno,state);
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
}
