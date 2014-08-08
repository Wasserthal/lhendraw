//reads in all files essential for the program

FILE * criticalfilename(char * iargv0,char * idirectorystring,const char * iname)
{
	FILE * thefile;
	strcpy(idirectorystring,iargv0);
	for (int ilv1=strlen(idirectorystring);ilv1>0;ilv1--)
	{
		if (idirectorystring[ilv1]=='/')
		{
			idirectorystring[ilv1]=0;
			strcat(idirectorystring,iname);
			goto ifilenamefertig;
		}
	}
	ifilenamefertig:;
	thefile=fopen(idirectorystring,"r");
	if (thefile==NULL)
	{
		strcpy(idirectorystring,"/usr/share/lhendraw");
		strcat(idirectorystring,iname);
		
		thefile=fopen(idirectorystring,"r");
		if (thefile==NULL){printf("File not found: %s\n",idirectorystring);}
	}
	return thefile;
}
void bitmap_init(FILE * bitmapfile)
{
	char ihv1;
	int ioffset;
	for (int ilv1=0;ilv1<10;ilv1++)
	{
		fread(&ihv1,1,1,bitmapfile);
	}
	fread(&ioffset,4,1,bitmapfile);
	fseek(bitmapfile,ioffset,SEEK_SET);
	for (int ilv1=32*selection_maxbuttons-1;ilv1>=0;ilv1--)
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
void resources_init(char * iargv0)
{
	char idirectorystring[1000];
	FILE * resource_file;
	resource_file=criticalfilename(iargv0,idirectorystring,"/gfx/buttons.bmp");
	bitmap_init(resource_file);
	fclose(resource_file);
	resource_file=criticalfilename(iargv0,idirectorystring,"/hotkeys.xml");
	config_init(resource_file);
	fclose(resource_file);
	resource_file=criticalfilename(iargv0,idirectorystring,"/LiberationMono-Regular.ttf");
	if (resource_file)
	{
		fclose(resource_file);
		text_init(idirectorystring);
	}
}
