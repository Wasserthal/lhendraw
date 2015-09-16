//reads in all files essential for the program

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
	#ifndef WITHOUT_FREETYPE
	resource_file=criticalfilename(idirectorystring,"/LiberationMono-Regular.ttf");
	if (resource_file)
	{
		fclose(resource_file);
		text_init(idirectorystring);
	}
	#else
	resource_file=criticalfilename(idirectorystring,"/LiberationMono-Regular.bin");
	if (resource_file)
	{
		text_load(resource_file);
	}
	#endif
	resource_file=criticalfilename(idirectorystring,"/LiberationMono-Regular.lennardfont");
	if (resource_file)
	{
		glyf_init(resource_file);
	}
}
