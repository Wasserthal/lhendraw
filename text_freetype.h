//This unit accesses freetype

#ifndef WITHOUT_FREETYPE
#include <ft2build.h>
#include FT_FREETYPE_H
#endif

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
_i32 utf8resolve(_u8 * input,_i32 * backcount)
{
	intl icounter;
	_u8 ihv1;
	_i32 currentunicode;
	icounter=0;
	_i32 followers;
	if (input[0]<128)	//utf-8 to UCS16
	{
		(*backcount)=1;
		return input[0];
	}
	else
	{
		if ((input[icounter]&0xC0)!=0xC0)
		{
			(*backcount)=1;
			return -20;
		}
		else
		{
			ihv1=input[icounter];
			ihv1=ihv1<<2;
			followers=1;
			while ((ihv1 & 0x80)!=0)
			{
				followers++;
				ihv1=ihv1<<1;
			}
			currentunicode=ihv1>>followers+1;
			icounter++;
			for (_i32 ilv2=0;ilv2<followers;ilv2++)
			{
				if ((input[icounter] & 0xC0)!=0x80)
				{
					(*backcount)=icounter;
					return -20;
				}
				currentunicode=(currentunicode<<6)+(input[icounter] & 0x3F);
			}
			icounter++;
		}
	}
	(*backcount)=icounter;
	return currentunicode;
}
_u8 unicodeoutputbuffer[8];
int utf8encode(_u32 input,char ** outputbuffer)
{
	_u32 headmask=0xFFFFFFE0;
	unicodeoutputbuffer[7]=0;
	if ((input & 0x80)==0)
	{
		unicodeoutputbuffer[0]=input;
		unicodeoutputbuffer[1]=0;
		*outputbuffer=(char*)unicodeoutputbuffer;
		return 1;
	}
	int cursor=6;
	iback:
	if (input & headmask)
	{
		unicodeoutputbuffer[cursor]=(input & 0x3F) | 0x80;
		input=input>>6;
		headmask=headmask>>1;
		cursor--;
		if (cursor<0) return -1;
		goto iback;
	}
	else
	{
		unicodeoutputbuffer[cursor]=((headmask<<2) & 0xFF) | input;
	}
	*outputbuffer=(char*)(unicodeoutputbuffer+cursor);
	return 7-cursor;
}
int utf8encompass(const char * string,int * position,int * length)
{
	int start=*position;
	if ((((_u8*)string)[start]&0xC0)==0xC0)
	{
		start++;
	}
	while ((((_u8*)string)[start]&0xC0)==0x80)
	{
		start++;
	}
	iback:;
	if (position<0) {position=0;length=0;return -1;}
	if ((string[*position]& 0x80)==0)
	{
		start++;
		if (string[*position]==0) {(*length)=1;return 0;}
		goto ifound;
	}
	if ((((_u8*)string)[*position]&0xC0)==0x80)
	{
		(*position)-=1;
	}
	else
	{
		ifound:;
		(*length)=start-(*position);
		return 1;
	}
	goto iback;
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
	if (searchpos<scanner_start) return 0;
	if (searchpos>scanner_end) return 0;
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
#ifndef WITHOUT_FREETYPE
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
#else
int text_load(FILE * ifile)
{
	fread(&fontpixinf_count,4,1,ifile);
	fread(&fontpixbuffer_count,4,1,ifile);
	fread(fontpixinf,sizeof(fontpixinf_),fontpixinf_count,ifile);
	fread(fontpixinf,sizeof(char),fontpixbuffer_count,ifile);
	for (int ilv1=0;ilv1<fontpixinf_count;ilv1++)
	{
		fontpixinf[ilv1].memstart=+fontpixbuffer;
	}
	fclose(ifile);
}
#endif
