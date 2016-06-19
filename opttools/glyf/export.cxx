#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lendefs.h"
#include "font.hxx"
glyph_ glyphlist[constants_maxglyph];

size_t glyphnumberpos=0;
size_t glyphbytecountpos=0;
size_t glyphbytecountpos2=0;
FILE * outfile;
inline void nout(_i32 val,int bytes)
{
	fwrite(&val,bytes,1,outfile);
}
int main(int argc,char ** argv)
{
	outfile=fopen(argv[2],"w");
	int ilv0=0;
	int lastendpoint;
	font_initlist();
	font_cmap();
	font_start();
	nout(0,4);
	while (font(0))
	{
		glyphlist[ilv0]=glyph;
		ilv0++;
	}
	int glyfcount=ilv0;
	font_hhea_hmtx(glyfcount);
	ilv0=0;
	for (ilv0=0;ilv0<glyfcount;ilv0++)
	{
		glyph=glyphlist[ilv0];
		glyph.offsetx=hmetrics[ilv0].advanceWidth;
		glyphbytecountpos=ftell(outfile);
		nout(0,4);
		nout(font_getcode(ilv0+3),4);//regarding the nonlisted three glyphs, nondefined, invisible and return
		if (glyph.units==-1)
		{
			nout(0,2);
			nout(glyph.offsetx,2);
			nout(glyph.offsety,2);
			nout(-1,2);
			nout(glyph.composite.count,2);
			for (int ilv1=0;ilv1<glyph.composite.count;ilv1++)
			{
				nout(glyph.composite.unit[ilv1].flags,2);
				nout(glyph.composite.unit[ilv1].index,4);
				nout(glyph.composite.unit[ilv1].arg1,2);
				nout(glyph.composite.unit[ilv1].arg2,2);
				nout(glyph.composite.unit[ilv1].scalexx,2);
				nout(glyph.composite.unit[ilv1].scalexy,2);
				nout(glyph.composite.unit[ilv1].scaleyx,2);
				nout(glyph.composite.unit[ilv1].scaleyy,2);
			}
			goto finish;
		}
		nout(glyph.simple.endpoints[glyph.units-1]+1,2);
		nout(glyph.offsetx,2);
		nout(glyph.offsety,2);
		nout(glyph.units,2);
		lastendpoint=-1;
		_i16 lastx,lasty;
		lastx=0;lasty=0;
		for (int ilv1=0;ilv1<glyph.simple.endpoints[glyph.units-1]+1;ilv1++)
		{
			char type[2];
			char appendix[4];
			int appendix_count=0;
			for (int ilv2=0;ilv2<2;ilv2++)
			{
				type[ilv2]=0;
				_i16 ishv1=(ilv2)?(-glyph.simple.donecoordinates[ilv1].y):(glyph.simple.donecoordinates[ilv1].x);
				_u16 ishv2=(ilv2)?(-glyph.simple.donecoordinates[ilv1].y-lasty):(glyph.simple.donecoordinates[ilv1].x-lastx);
				if (ishv2!=0) type[ilv2]=2;
				_i8 cut=ishv1;
				if ((ilv1>0) && (ishv2!=0))
				{
					if (((ishv2 & 0xFF80)==0xFF80) || ((ishv2 & 0xFF80)==0x0000))
					{
						type[ilv2]=1;
						cut=ishv2;
					}
				}
				if ((type[ilv2]==1) || (type[ilv2]==3))
				{
					appendix[appendix_count]=cut;
					appendix_count++;
				}
				if (type[ilv2]==2)
				{
					appendix[appendix_count]=ishv1&0xFF;
					appendix_count++;
					appendix[appendix_count]=ishv1>>8;
					appendix_count++;
				}
			}
			_u8 endpoint_on=0;
			for (int ilv3=0;ilv3<glyph.units;ilv3++)
			{
				endpoint_on|=(glyph.simple.endpoints[ilv3]==ilv1);
			}
			nout(type[0]+4*type[1] | ((glyph.simple.donecoordinates[ilv1].flags & 1)?0x0:0x10) | (endpoint_on?0x20:0),1);
			fwrite(appendix,appendix_count,1,outfile);
			lastx=glyph.simple.donecoordinates[ilv1].x;
			lasty=-glyph.simple.donecoordinates[ilv1].y;
		}
		finish:;
		glyphbytecountpos2=ftell(outfile);
		fseek(outfile,glyphbytecountpos,SEEK_SET);
		nout(glyphbytecountpos2-glyphbytecountpos,4);
		fseek(outfile,glyphbytecountpos2,SEEK_SET);
	}
	fseek(outfile,glyphnumberpos,SEEK_SET);
	nout(ilv0,4);
	fseek(outfile,0,SEEK_END);
	fclose(outfile);
}
