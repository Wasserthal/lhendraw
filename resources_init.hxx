//reads in all files essential for the program
//gfx/buttons.bmp relies on the external variable selection_maxbuttons
FILE * criticalfilename(char * iargv0,char * idirectorystring,const char * iname)
{
	FILE * thefile;
	strcpy(idirectorystring,"/usr/share/lhendraw");
	strcat(idirectorystring,iname);
	thefile=fopen(idirectorystring,"r");
	if (thefile==NULL)
	{
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
	}
	return thefile;
}
void resources_init(char * iargv0)
{
	char idirectorystring[1000];
	FILE * bitmapfile;
	char ihv1;
	int ioffset;
	bitmapfile=criticalfilename(iargv0,idirectorystring,"/gfx/buttons.bmp");
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
	fclose(bitmapfile);
	bitmapfile=criticalfilename(iargv0,idirectorystring,"/LiberationMono-Regular.ttf");
	if (bitmapfile)
	{
		fclose(bitmapfile);
		text_init(idirectorystring);
	}
}
