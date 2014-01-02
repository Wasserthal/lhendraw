//This unit accesses freetype

#include <ft2build.h>
#include FT_FREETYPE_H

//TODO: append the same prefix as in createsvg.hxx here!
char fontpixbuffer[1000000];
int fontpixbuffer_count;
typedef struct fontpixinf_
{
	int deltax,deltay;
	int sizex,sizey;
	int pivotx,pivoty;
	char * memstart;
	int unicode;
} fontpixinf_;
int fontpixinf_count;
fontpixinf_ fontpixinf[10000];
void fontpixinf_add(FT_GlyphSlot islot,int iunicode)
{
	int length=((*islot).bitmap).width;
	fontpixinf[fontpixinf_count].sizex=((*islot).bitmap).width;
	length*=((*islot).bitmap).rows;
	fontpixinf[fontpixinf_count].sizey=((*islot).bitmap).rows;
	fontpixinf[fontpixinf_count].deltax=(*islot).advance.x>>6;
	fontpixinf[fontpixinf_count].deltay=(*islot).advance.y>>6;
	fontpixinf[fontpixinf_count].pivotx=(*islot).bitmap_left;
	fontpixinf[fontpixinf_count].pivoty=-(*islot).bitmap_top;
	fontpixinf[fontpixinf_count].memstart=fontpixbuffer_count+fontpixbuffer;
	fontpixinf[fontpixinf_count].unicode=iunicode;
	for (int ilv1=0;ilv1<length;ilv1++)
	{
		fontpixbuffer[fontpixbuffer_count++]=(*islot).bitmap.buffer[ilv1];
	}
	fontpixinf_count++;
}
int indexfromunicode(int input)//Weighed approximation
{
	int scanner_start=0;
	int scanner_end=fontpixinf_count-1;
	int codeat_start=fontpixinf[0].unicode;
	int codeat_end=fontpixinf[scanner_end].unicode;
	int searchpos;
	iback:
	if (scanner_end==scanner_start)
	{
		goto heutemuszligdieglockewerden;
	}
	if (codeat_end==codeat_start)
	{
		goto heutemuszligdieglockewerden;
	}
	searchpos=((input-codeat_start)*(scanner_end-scanner_start)/(codeat_end-codeat_start))+scanner_start;
	if (searchpos>=fontpixinf_count) {goto to_smaller;}
	if (fontpixinf[searchpos].unicode==input)
	{
		return searchpos;
	}
	if (fontpixinf[searchpos].unicode<input)
	{
//		to_bigger:
		scanner_start=searchpos+1;
		codeat_start=fontpixinf[scanner_start].unicode;
		goto iback;
	}
	if (fontpixinf[searchpos].unicode>input)
	{
		to_smaller:
		scanner_end=searchpos-1;
		codeat_end=fontpixinf[scanner_start].unicode;
		goto iback;
	}
	heutemuszligdieglockewerden:
	if (fontpixinf[searchpos].unicode==input)
	{
		return searchpos;
	}
	return 0;
}
void putpixel(int iposx,int iposy);
	
int text_init(void)
{
	FT_Library library;
	FT_Face face;
	FT_GlyphSlot slot;
//	FT_Matrix matrix;

	FT_Init_FreeType( &library );
	FT_New_Face(library, "/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf", 0, &face);
	FT_Set_Char_Size(face, 12*64*72, 0,1,0);
	slot= (*face).glyph;
	fontpixinf_count=0;
	fontpixbuffer_count=0;
	for (int ilv1=0;ilv1<65535;ilv1++)
	{
		if (FT_Get_Char_Index(face,ilv1)!=0)
		{
			if (FT_Load_Char(face, ilv1, FT_LOAD_RENDER)==0)
			{
				fontpixinf_add(slot,ilv1);
			}
		}
	}
	return 0;
}
