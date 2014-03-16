typedef int selection_[20000];//a 32bit int gives selection masks for 32 layers. TODO: make this grow to the current buffer size.
selection_ currentselection;//The ordinary selection, more precisely the current state of it.
selection_ clickselection;//All elements caught with the current click, or another likewise action of the mouse.
void clearselection(selection_ iselection)
{
	for (int ilv1=0;ilv1<20000;ilv1++)
	{
		iselection[ilv1]=0;
	}
}
typedef struct selection_frame_
{
	int startx,starty;
	int endx,endy;
	float left,top;
	float right,bottom;
}selection_frame_;
selection_frame_ selection_frame;
int sdl_selectiondraw()
{
	_u32 icompare;
	int isize;
	int ioffset;
	char * ibufferpos;
	for (int ilv1=0;ilv1<sizeof(STRUCTURE_OBJECTTYPE_List)/sizeof(trienum);ilv1++)
	{
		icompare=1<<ilv1;
		int isize= STRUCTURE_OBJECTTYPE_List[ilv1].size;
		basicmultilist * tlmultilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
		if (tlmultilist==NULL) goto i_control4_fertig;
		CDXMLREAD_functype tldummy;
		ioffset=(*tlmultilist).getproperties("xyz",&tldummy);
		if (ioffset<0) goto i_control4_fertig;
		ibufferpos=(char*)((*tlmultilist).pointer);
		cdx_Point2D * tlpoint2d;
		SDL_color=0x00FF00;
		for (int ilv2=0;ilv2<(*tlmultilist).filllevel;ilv2++)
		{
			if ((currentselection[ilv2]) & icompare)
			{
				if ((*((basic_instance*)(ibufferpos+isize*ilv2))).exist)
				{
					tlpoint2d = ((cdx_Point2D*)(ibufferpos+isize*ilv2+ioffset));
					putpixel(((*tlpoint2d).x-SDL_scrollx)*SDL_zoomx+1,((*tlpoint2d).y-SDL_scrolly)*SDL_zoomy+1);
					putpixel(((*tlpoint2d).x-SDL_scrollx)*SDL_zoomx+2,((*tlpoint2d).y-SDL_scrolly)*SDL_zoomy);
					putpixel(((*tlpoint2d).x-SDL_scrollx)*SDL_zoomx-2,((*tlpoint2d).y-SDL_scrolly)*SDL_zoomy);
					putpixel(((*tlpoint2d).x-SDL_scrollx)*SDL_zoomx-1,((*tlpoint2d).y-SDL_scrolly)*SDL_zoomy+1);
					putpixel(((*tlpoint2d).x-SDL_scrollx)*SDL_zoomx+1,((*tlpoint2d).y-SDL_scrolly)*SDL_zoomy-1);
					putpixel(((*tlpoint2d).x-SDL_scrollx)*SDL_zoomx,((*tlpoint2d).y-SDL_scrolly)*SDL_zoomy+2);
					putpixel(((*tlpoint2d).x-SDL_scrollx)*SDL_zoomx,((*tlpoint2d).y-SDL_scrolly)*SDL_zoomy-2);
					putpixel(((*tlpoint2d).x-SDL_scrollx)*SDL_zoomx-1,((*tlpoint2d).y-SDL_scrolly)*SDL_zoomy-1);
				}
			}
		}
		i_control4_fertig:;
	}
}
