void resources_init()
{
	FILE * bitmapfile;
	char ihv1;
	int ioffset;
	bitmapfile=fopen("gfx/buttons.bmp","r");
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
}
