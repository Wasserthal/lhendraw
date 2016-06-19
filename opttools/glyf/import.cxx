#include <stdio.h>
#include <stdlib.h>
#include "lendefs.h"
#include <unistd.h>
FILE * infile;
struct coordinate_
{
	_u8 flags;
	_i16 x;
	_i16 y;
};
struct glyph_
{
	int maxcount;
	int auxpointcount;
	int unicode;
	_i16 units;
	_i16 xoffset;
	_i16 yoffset;
	_i16 minx;
	_i16 miny;
	_i16 maxx;
	_i16 maxy;
	_i16 offsetx;
	_i16 offsety;
	struct
	{
		_u16 endpoints[255];
		_i16 instructionlength;
//		_u8 instructions[65536];
		coordinate_ donecoordinates[65536];
	} simple;
	struct
	{
		_i16 count;
		struct unit
		{
			_u16 flags;
			_i32 index;
			_i16 arg1,arg2;
			_i16 scalexx;
			_i16 scalexy;
			_i16 scaleyx;
			_i16 scaleyy;
		} unit[10];
	} composite;
	glyph_()
	{
		units=0;
	}
	void printme()
	{
		printf("Glyph:\
units:%i\
minx:%i\
miny:%i\
maxx:%i\
maxy:%i\
\n",units,minx,miny,maxx,maxy);
		if (units>0)
		{
			for (int ilv1=0;ilv1<units;ilv1++)
			{
				printf("ep%i: %i\n",ilv1,simple.endpoints[ilv1]);
			}
			printf("simple\
instructionLength:%i\
\n",simple.instructionlength);
		}
	}
};
glyph_ * glyphmemory;
int glyphmemory_count;
int glyphmemory_max;
void do_inglyph(glyph_ * inglyph)
{
	_u32 bytes;
	_u32 debug_expectedbytes;
	debug_expectedbytes=ftell(infile);
	fread(&bytes,4,1,infile);
	fread(&((*inglyph).unicode),4,1,infile);
	fread(&((*inglyph).maxcount),2,1,infile);
	fread(&((*inglyph).offsetx),2,1,infile);
	fread(&((*inglyph).offsety),2,1,infile);
	fread(&((*inglyph).units),2,1,infile);
	printf(">!!%i!!<",(*inglyph).units);
	if ((*inglyph).units==-1)
	{
		fread(&((*inglyph).composite.count),2,1,infile);
		for (int ilv1=0;ilv1<(*inglyph).composite.count;ilv1++)
		{
			fread(&((*inglyph).composite.unit[ilv1].flags),2,1,infile);
			fread(&((*inglyph).composite.unit[ilv1].index),4,1,infile);
			fread(&((*inglyph).composite.unit[ilv1].arg1),2,1,infile);
			fread(&((*inglyph).composite.unit[ilv1].arg2),2,1,infile);
			fread(&((*inglyph).composite.unit[ilv1].scalexx),2,1,infile);
			fread(&((*inglyph).composite.unit[ilv1].scalexy),2,1,infile);
			fread(&((*inglyph).composite.unit[ilv1].scaleyx),2,1,infile);
			fread(&((*inglyph).composite.unit[ilv1].scaleyy),2,1,infile);
		}
		return;
	}
	int ibasecount=0;
	int lastoff=0;
	debug_expectedbytes+=bytes;
	printf("bytes:%i\n",bytes);
	printf("!%i!\n",(*inglyph).maxcount);
	if ((*inglyph).maxcount>300) {printf("SLEEP!\n");sleep(10);exit(0);}
	_i16 forelastx=0;
	_i16 forelasty=0;
	_i16 lastx=0;
	_i16 lasty=0;
	for (int ilv2=0;ilv2<(*inglyph).maxcount;ilv2++)
	{
		_u8 ihv0;
		fread(&ihv0,1,1,infile);
		(*inglyph).simple.donecoordinates[ilv2].flags=(ihv0>>4);
		printf("___%08hhx:%i:%i\n",ihv0,ilv2,(*inglyph).maxcount);
		for (int ilv1=0;ilv1<2;ilv1++)
		{
			_i16 * target=(ilv1)?(&((*inglyph).simple.donecoordinates[ilv2].y)):&(((*inglyph).simple.donecoordinates[ilv2].x));
			(*target)=((ilv1)?lasty:lastx);
			int mode=(ilv1)?((ihv0 & 0xF)/4):((ihv0 & 0xF)%4);
			switch (mode)
			{
				case 0:
				break;
				case 1:
				{
					_i8 ihv1;
					fread(&ihv1,1,1,infile);
					(*target)+=ihv1;
					break;
				}
				case 2:
				{
					_u16 ishv1;
					fread(&ishv1,2,1,infile);//ENDIAN
					(*target)=ishv1;
					break;
				}
				case 3:
				{
					(*target)=(ilv1)?((2*lasty)-forelasty):((2*lastx)-forelastx);
					_i8 ihv1;
					fread(&ihv1,1,1,infile);
					(*target)+=ihv1;
					break;
				}
				break;
			}
			((ilv1)?lasty:lastx)=(*target);
		}
		lastoff=(ihv0 & 0x10)?1:0;
		forelastx=lastx;
		forelasty=lasty;
	}
	if (ftell(infile)!=debug_expectedbytes) exit(1);
}
#define bezierpointmax 65536
int gfx_bks[bezierpointmax];//Worst case, every curve making an s
float gfx_type;//1: line 3: cubic bezier 0x100: bezier part, order depending on the last preceding
float gfx_m[bezierpointmax];//Only for polygons
int ibrakelist_count;
int gfx_canvassizex=1376;
int gfx_canvassizey=768;
int gfx_screensizex=1376;
struct inficorn_
{
	float x,y;
	int ix,iy;
	char flags;
};
inficorn_ LHENDRAW_inficorn[bezierpointmax];
#define SDL_zoomx 0.1
#define SDL_zoomy 0.1
#define SDL_scrollx (-3200)
#define SDL_scrolly (-1600)
FILE * framebuffer;
int gfx_expressinfinityangle(int count)
{
	intl iminx,imaxx,iminy,imaxy;
	iminx=10000;
	iminy=10000;
	imaxx=-10000;
	imaxy=-10000;
	int debugcounter=0;
	for (int ilv1=0;ilv1<count;ilv1++)
	{
		LHENDRAW_inficorn[ilv1].ix=(LHENDRAW_inficorn[ilv1].x-SDL_scrollx)*SDL_zoomx;
		LHENDRAW_inficorn[ilv1].iy=(LHENDRAW_inficorn[ilv1].y-SDL_scrolly)*SDL_zoomy;
		if (iminx>LHENDRAW_inficorn[ilv1].ix)iminx=LHENDRAW_inficorn[ilv1].ix;
		if (iminy>LHENDRAW_inficorn[ilv1].iy)iminy=LHENDRAW_inficorn[ilv1].iy;
		if (imaxx<LHENDRAW_inficorn[ilv1].ix)imaxx=LHENDRAW_inficorn[ilv1].ix;
		if (imaxy<LHENDRAW_inficorn[ilv1].iy)imaxy=LHENDRAW_inficorn[ilv1].iy;
	}
	if (iminx<0)iminx=0;
	if (imaxx>gfx_canvassizex)imaxx=gfx_canvassizex;
	if (iminy<0)iminy=0;
	if (imaxy>gfx_canvassizey)imaxy=gfx_canvassizey;
	/*	if (imaxx<iminx){return 0;}
		if (imaxy<iminy){return 0;}*/
	iminx=0;imaxx=1376;iminy=0;imaxy=768;
	int currentstartpoint=0;
	for (int ilv1=0;ilv1<count;ilv1++)
	{
		int tlvert;
		int otherno=(LHENDRAW_inficorn[ilv1].flags & 0x2)?(currentstartpoint):(ilv1+1);
		tlvert=(LHENDRAW_inficorn[(otherno)%count].iy-LHENDRAW_inficorn[ilv1].iy);
		if (abs(tlvert)>=1)
		gfx_m[ilv1]=
		(float(LHENDRAW_inficorn[(otherno)%count].ix)-LHENDRAW_inficorn[ilv1].ix)/tlvert;
		if (otherno==currentstartpoint) {currentstartpoint=ilv1+1;}
	}
	currentstartpoint=0;
	for (int ilv1=iminy;ilv1<imaxy;ilv1++)
	{
		ibrakelist_count=0;
		for (int ilv2=0;ilv2<count;ilv2++)
		{
			int otherno=(LHENDRAW_inficorn[ilv2].flags & 0x2)?(currentstartpoint):(ilv2+1);
			if (((LHENDRAW_inficorn[ilv2].iy<ilv1) && (LHENDRAW_inficorn[(otherno)%count].iy>=ilv1)) || ((LHENDRAW_inficorn[(otherno)%count].iy<ilv1) && (LHENDRAW_inficorn[ilv2].iy>=ilv1)))
			{
				gfx_bks[ibrakelist_count++]=gfx_m[ilv2]*(ilv1-LHENDRAW_inficorn[ilv2].iy)+LHENDRAW_inficorn[ilv2].ix;
			}
			if (otherno==currentstartpoint) {currentstartpoint=ilv2+1;}
		}
		char tlchanged;
		tlsortback:
		tlchanged=0;
		for (int ilv2=0;ilv2<ibrakelist_count;ilv2++)
		{
			for (int ilv3=ilv2+1;ilv3<ibrakelist_count;ilv3++)
			{
				if (gfx_bks[ilv2]>gfx_bks[ilv3])
				{
					float temp=gfx_bks[ilv2];
					gfx_bks[ilv2]=gfx_bks[ilv3];
					gfx_bks[ilv3]=temp;
					tlchanged=1;
				}
			}
		}
		if (tlchanged) goto tlsortback;
		char tlonness;
		tlonness=0;
		int tlbrakesthrough=0;
		int ilv2=0;
		if (ibrakelist_count==0) goto ilinefertig;
		for (ilv2=0;ilv2<gfx_canvassizex;ilv2++)
		{
			while (gfx_bks[tlbrakesthrough]<ilv2)
			{
				tlbrakesthrough++;
				if (tlbrakesthrough>=ibrakelist_count)
				{
					goto ilinefertig;
				}
				tlonness=tlbrakesthrough & 1;
			}
			_u32 icolor=tlonness?0xFFFFFFFF:0;
			fwrite(&icolor,4,1,framebuffer);
			debugcounter++;
		}
		ilinefertig:
		;
		for (;ilv2<gfx_canvassizex;ilv2++)
		{
			_u32 icolor=0;
			fwrite(&icolor,4,1,framebuffer);
			debugcounter++;
		}
	}
	return 1;
}
void drawglyph(int ino,int deltax,int deltay)
{
	system("clear\n");
	framebuffer=fopen("/dev/fb0","w");
	int sizex=1376;
	_u32 icolor=0;
	for (int ilv2=0;ilv2<glyphmemory[ino].maxcount;ilv2++)
	{
		LHENDRAW_inficorn[ilv2].x=glyphmemory[ino].simple.donecoordinates[ilv2].x+deltax;
		LHENDRAW_inficorn[ilv2].y=glyphmemory[ino].simple.donecoordinates[ilv2].y+deltay;
		LHENDRAW_inficorn[ilv2].flags=glyphmemory[ino].simple.donecoordinates[ilv2].flags;
	}
	gfx_expressinfinityangle(glyphmemory[ino].maxcount);
	fclose(framebuffer);
}
int main(int argc,char ** argv)
{
	infile=fopen(argv[1],"r");
	fread(&glyphmemory_max,4,1,infile);
	glyphmemory=(glyph_*)malloc(glyphmemory_max*sizeof(glyph_));
	glyphmemory_count=0;
	for (int ilv1=0;ilv1<glyphmemory_max;ilv1++)
	{
		do_inglyph(glyphmemory+ilv1);
		if (ilv1>105)
		{
			if (glyphmemory[ilv1].units>0)
			{
				drawglyph(ilv1,0,100);
			}
		}
	}
	for (int ilv1=0;ilv1<glyphmemory_max;ilv1++)
	{
		if (glyphmemory[ilv1].units==-1)
		{
			for (int ilv2=0;ilv2<glyphmemory[ilv1].composite.count;ilv2++)
			{
				int deltax=0;int deltay=0;
				if (glyphmemory[ilv1].composite.unit[ilv2].flags & 2)
				{
					deltax+=glyphmemory[ilv1].composite.unit[ilv2].arg1;
					deltay+=glyphmemory[ilv1].composite.unit[ilv2].arg2;
				}
				drawglyph(glyphmemory[ilv1].composite.unit[ilv2].index-3,deltax,deltay+100);
				printf("unic:%i\n",glyphmemory[ilv1].unicode);
				sleep(1);
			}
		}
	}
	fclose(infile);
	drawglyph(0,0,100);
	return 0;
}
