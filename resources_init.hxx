//reads in all files essential for the program
_u32 resources_icon[32][8]={{0x8C30000},{0xAEB0000},{0xAEB0000},{0x8C30000},{0x300C000},{0x300C000},{0xC003000},{0xC003000},
{0x30000C00},{0x35555C00},{0x35555C00},{0x35555C00},{0x35555C00},{0x35555C00},{0x35555C00},{0x35555C00},
{0x35555C00},{0x35555C00},{0x35555F00},{0x355555C0},{0x35555570},{0xF555457C},{0xD555415C},{0xD555505C},
{0xD5555517},{0xF5555557},{0x35555557},{0x35555557},{0x3D55557C},{0xF5555F0},{0x3F55FC0},{0x03FFC00}};
SDL_Color resources_icon_palette[5]={{255,255,255},{0,0,255},{127,127,127},{0,0,0},{255,0,0}};
void resources_set_icon()
{
	for (int hoehe=0;hoehe<32;hoehe++)
	{
		_u32 px=resources_icon[hoehe][0];
		_u8 * pos=(_u8*)&(resources_icon[hoehe][0]);
		for (int ilv1=0;ilv1<16;ilv1++)
		{
			pos[ilv1]=px&3;
			if (pos[ilv1]==1) pos[ilv1+16]=4; else pos[ilv1+16]=pos[ilv1];
			px=px>>2;
		}
		_u32*fix=(_u32*)(pos+14);
		if ((hoehe==12)||(hoehe==15)) {*(fix++)=0x03030303;*fix=0x03030303;}
		if ((hoehe==13)||(hoehe==14)) {*(fix++)=0x02010101;*fix=0x04040404;}
	}
	SDL_Surface * iSurface=SDL_CreateRGBSurfaceFrom(resources_icon,32,32,8,32,0,0,0,0);
	#ifndef SDL2
	SDL_SetPalette(iSurface,SDL_LOGPAL|SDL_PHYSPAL,resources_icon_palette,0,5);
	SDL_WM_SetIcon(iSurface,NULL);
	#else
	SDL_Palette * lost_iconpalette=SDL_AllocPalette(5);
	SDL_SetPaletteColors(lost_iconpalette,resources_icon_palette,0,5);
	SDL_SetSurfacePalette(iSurface,lost_iconpalette);
	SDL_SetWindowIcon(window,iSurface);
	#endif
}
FILE * criticalfilename(char * idirectorystring,const char * iname)
{
	FILE * thefile=NULL;
	strcpy(idirectorystring,"/usr/share/lhendraw");
	strcat(idirectorystring,iname);
	
	thefile=fopen(idirectorystring,"r");
	if (thefile==NULL){printf("File not found: %s\n",idirectorystring);}
	return thefile;
}
void bitmap_init(FILE * bitmapfile)
{
	_u32 dummy;
	char ihv1;
	int ioffset;
	int vsize;
	for (int ilv1=0;ilv1<10;ilv1++)
	{
		fread(&ihv1,1,1,bitmapfile);
	}
	fread(&ioffset,4,1,bitmapfile);
	fread(&dummy,4,1,bitmapfile);
	fread(&dummy,4,1,bitmapfile);
	fread(&vsize,4,1,bitmapfile);
	LHENDRAW_maxbuttons=vsize/32;
	resources_bitmap_buttons=(resources_button_*)malloc(LHENDRAW_maxbuttons*sizeof(resources_button_));//intended PERMANENT malloc
	fseek(bitmapfile,ioffset,SEEK_SET);
	for (int ilv1=(32*LHENDRAW_maxbuttons)-1;ilv1>=0;ilv1--)
	{
		fread(&(resources_bitmap_buttons[0][ilv1][0]),128,1,bitmapfile);
		for (int ilv2=0;ilv2<32;ilv2++)
		{
			resources_bitmap_buttons[0][ilv1][ilv2]=(resources_bitmap_buttons[0][ilv1][ilv2]>>8) | (resources_bitmap_buttons[0][ilv1][ilv2]<<24);
		}
	}
}
void config_init(FILE * configfile)
{
	CONFIGBRIDGE_Total_Document_instance firstcurrentinstance;
	currentinstance=&firstcurrentinstance;
	input_fsm(configfile);
}
void resources_init()
{
	char idirectorystring[1000];
	FILE * resource_file;
	resource_file=criticalfilename(idirectorystring,"/gfx/buttons.bmp");
	bitmap_init(resource_file);
	fclose(resource_file);
	resource_file=criticalfilename(idirectorystring,"/hotkeys.xml");
	config_init(resource_file);
	fclose(resource_file);
	resource_file=criticalfilename(idirectorystring,"/LiberationMono-Regular.bin");
	if (resource_file)
	{
		text_load(resource_file);
	}
	resource_file=criticalfilename(idirectorystring,"/LiberationMono-Regular.lennardfont");
	if (resource_file)
	{
		glyf_init(resource_file);
	}
}
