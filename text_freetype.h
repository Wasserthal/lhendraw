//This unit accesses freetype

#ifndef WITHOUT_FREETYPE
#include <ft2build.h>
#include FT_FREETYPE_H
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
		fontpixbuffer[fontpixbuffer_count++]=(((_u8)(*islot).bitmap.buffer[ilv1])>65)?0x80:0;
	}
	fontpixinf_count++;
}
void fontpixinf_empty()
{
	int length=8;
	fontpixinf[fontpixinf_count].sizex=8;
	length*=16;
	fontpixinf[fontpixinf_count].sizey=16;
	fontpixinf[fontpixinf_count].deltax=0;
	fontpixinf[fontpixinf_count].deltay=0;
	fontpixinf[fontpixinf_count].pivotx=0;
	fontpixinf[fontpixinf_count].pivoty=-8;
	fontpixinf[fontpixinf_count].memstart=fontpixbuffer_count+fontpixbuffer;
	fontpixinf[fontpixinf_count].unicode=0;
	for (int ilv1=0;ilv1<length;ilv1++)
	{
		fontpixbuffer[fontpixbuffer_count++]=(((ilv1>>2)&1) ^ ((ilv1>>5)&1))*0x80;
	}
	fontpixinf_count++;
}
void putpixel(int iposx,int iposy);
	
int text_init(char * filename)
{
	FT_Library library;
	FT_Face face;
	FT_GlyphSlot slot;
//	FT_Matrix matrix;

	FT_Init_FreeType( &library );
	FT_New_Face(library, filename, 0, &face);
	FT_Set_Char_Size(face, 12*64*72, 0,1,0);
	slot= (*face).glyph;
	fontpixinf_count=0;
	fontpixbuffer_count=0;
	fontpixinf_empty();
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
#endif
