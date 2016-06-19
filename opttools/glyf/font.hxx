#define constants_maxglyph 1000
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
		else
		{
			for (int ilv1=0;ilv1<composite.count+1;ilv1++)
			{
				printf("#%4i:%04hX\n",composite.unit[ilv1].index,composite.unit[ilv1].flags);
			}
		}
	}
} glyph;
FILE * fontfile;
void insertfixpoint(int index,int posx,int posy)
{
	for (int ilv1=glyph.maxcount;ilv1>index;ilv1--)
	{
		glyph.simple.donecoordinates[ilv1]=glyph.simple.donecoordinates[ilv1-1];
	}
	glyph.simple.donecoordinates[index].flags=0x81;//DEBUG-80
	glyph.simple.donecoordinates[index].x=posx;
	glyph.simple.donecoordinates[index].y=posy;
	for (int ilv2=0;ilv2<glyph.units;ilv2++)
	{
		if (glyph.simple.endpoints[ilv2]>=index)glyph.simple.endpoints[ilv2]++;
	}
	glyph.maxcount++;
}
void insertmedipoints(char actually)
{
	int currentunit=0;
	for (int ilv1=0;ilv1<glyph.maxcount;ilv1++)
	{
		while (ilv1>glyph.simple.endpoints[currentunit])
		{
			currentunit++;
		}
		if ((glyph.simple.donecoordinates[ilv1].flags & 1)==0)
		{
			if (((glyph.simple.donecoordinates[ilv1+1].flags & 1)==0) && (ilv1+1<glyph.maxcount))
			{
				int startx=glyph.simple.donecoordinates[ilv1].x;
				int starty=glyph.simple.donecoordinates[ilv1].y;
				int endx=glyph.simple.donecoordinates[ilv1+1].x;
				int endy=glyph.simple.donecoordinates[ilv1+1].y;
				if (actually)
				{
					insertfixpoint(ilv1+1,startx+(endx-startx)*0.5,starty+(endy-starty)*0.5);
					ilv1++;
				}
				else
				{
					glyph.auxpointcount+=1;
				}
			}
		}
		glyph.offsetx=glyph.maxx;
		glyph.offsety=0;
	}
}
_i32 geti32()
{
	_i32 wert=0;
	fread(((char*)&wert)+3,1,1,fontfile);
	fread(((char*)&wert)+2,1,1,fontfile);
	fread(((char*)&wert)+1,1,1,fontfile);
	fread(((char*)&wert),1,1,fontfile);
	return wert;
}
_u32 getu32()
{
	_u32 wert=0;
	fread(((char*)&wert)+3,1,1,fontfile);
	fread(((char*)&wert)+2,1,1,fontfile);
	fread(((char*)&wert)+1,1,1,fontfile);
	fread(((char*)&wert),1,1,fontfile);
	return wert;
}
_u16 getu16()
{
	_u16 wert=0;
	fread(((char*)&wert)+1,1,1,fontfile);
	fread(((char*)&wert),1,1,fontfile);
	return wert;
}
_i16 geti16()
{
	_i16 wert=0;
	fread(((char*)&wert)+1,1,1,fontfile);
	fread(((char*)&wert),1,1,fontfile);
	return wert;
}
_u8 getu8()
{
	_u8 wert=0;
	fread(((char*)&wert),1,1,fontfile);
	return wert;
}
_i8 geti8()
{
	_i8 wert=0;
	fread(((char*)&wert),1,1,fontfile);
	return wert;
}
int font_glyfpos=0;
int font_cmappos=0;
int font_hmtxpos=0;
int font_hheapos=0;
int font_glyflength=0;
int font_cmaplength=0;
int font_hmtxlength=0;
int font_hhealength=0;
void font_initlist()
{
	fontfile=fopen("LiberationMono-Regular.ttf","r");
	getu32();
	int numtables=getu16();
	getu16();
	getu16();
	getu16();
	for (int ilv1=0;ilv1<numtables;ilv1++)
	{
		char tag[4];
		_u32 checksum;
		_u32 offset;
		_u32 length;
		fread(tag,4,1,fontfile);
		checksum=getu32();
		offset=getu32();
		length=getu32();
		if (strncmp(tag,"glyf",4)==0) {font_glyfpos=offset;font_glyflength=length;}
		if (strncmp(tag,"cmap",4)==0) {font_cmappos=offset;font_cmaplength=length;}
		if (strncmp(tag,"hmtx",4)==0) {font_hmtxpos=offset;font_hmtxlength=length;}
		if (strncmp(tag,"hhea",4)==0) {font_hheapos=offset;font_hhealength=length;}
	}
}
_u16 * font_cmap_endCount;
_u16 * font_cmap_startCount;
_u16 * font_cmap_idDelta;
_u16 * font_cmap_idRangeOffset;
_u16 * font_cmap_glyphIdArray;
_u16 font_cmap_segCount;
typedef struct 	_longHorMertric {
	_u16 advanceWidth;
	_i16	lsb;
}  longHorMetric;
_longHorMertric hmetrics[constants_maxglyph];
void font_hhea_hmtx(int glyfcount)
{
	int ilv1;
	fseek(fontfile,font_hheapos+34,SEEK_SET);
	int numberofmetrix=getu16();
	fseek(fontfile,font_hmtxpos,SEEK_SET);
		printf("ADVn%i\n",numberofmetrix);
	for (ilv1=0;ilv1<numberofmetrix;ilv1++)
	{
		hmetrics[ilv1].advanceWidth=getu16();
		hmetrics[ilv1].lsb=geti16();
		printf("ADV:%i\n",hmetrics[ilv1].advanceWidth);
	}
	for (;ilv1<glyfcount;ilv1++)//Brutal HACK, runs over the rim, guessing that the file end will come later.
	{
		hmetrics[ilv1].advanceWidth=hmetrics[numberofmetrix-1].advanceWidth;
		hmetrics[ilv1].lsb=geti16();
	}
}
void font_cmap()
{
	fseek(fontfile,font_cmappos,SEEK_SET);
	getu16();
	int numoftables=getu16();
	for (int ilv1=0;ilv1<numoftables;ilv1++)
	{
		int platform_id;
		int encoding_id;
		_u32 byteoffset;
		platform_id=getu16();
		encoding_id=getu16();
		byteoffset=getu32();
		printf("Plat:%i,ENC:%i,offset:%08X\n",platform_id,encoding_id,byteoffset+font_cmappos);
		if (((platform_id==3) && (encoding_id==1))||((platform_id==0) && (encoding_id==3)))
		{
			fseek(fontfile,byteoffset+font_cmappos,SEEK_SET);
			_u16 format=getu16();
			_u16 length=getu16();
			_u16 version=getu16();
			font_cmap_segCount=getu16()>>1;
			_u16 searchRange=getu16();
			_u16 entrySelector=getu16();
			_u16 rangeShift=getu16();

			font_cmap_endCount=(_u16*)malloc(2*font_cmap_segCount);
			font_cmap_startCount=(_u16*)malloc(2*font_cmap_segCount);
			font_cmap_idDelta=(_u16*)malloc(2*font_cmap_segCount);
			font_cmap_idRangeOffset=(_u16*)malloc(2*font_cmap_segCount);
			font_cmap_glyphIdArray=(_u16*)malloc(length);

			for (int ilv3=0;ilv3<font_cmap_segCount;ilv3++) {font_cmap_endCount[ilv3]=getu16();}
			_u16 reservedPad=getu16();
			for (int ilv3=0;ilv3<font_cmap_segCount;ilv3++) {font_cmap_startCount[ilv3]=getu16();}
			for (int ilv3=0;ilv3<font_cmap_segCount;ilv3++) {font_cmap_idDelta[ilv3]=getu16();}
			for (int ilv3=0;ilv3<font_cmap_segCount;ilv3++) {font_cmap_idRangeOffset[ilv3]=getu16();}
			for (int ilv3=0;ilv3<length/2;ilv3++) {font_cmap_glyphIdArray[ilv3]=getu16();}
			for (int ilv3=0;ilv3<font_cmap_segCount;ilv3++)
			{
				printf("font_cmap_endCount:%i\n",font_cmap_endCount[ilv3]);
				printf("font_cmap_startCount:%i\n",font_cmap_startCount[ilv3]);
				printf("font_cmap_idDelta:%i\n",font_cmap_idDelta[ilv3]);
				printf("font_cmap_idRangeOffset:%i\n",font_cmap_idRangeOffset[ilv3]);
				printf("font_cmap_glyphIdArray:%i\n",font_cmap_glyphIdArray[ilv3]);
				printf("---%7i---\n",ilv3);
			}
			return;
		}
	}
	exit(1);
}
int font_getcode(int incode)
{
	for (int ilv1=1;ilv1<65536;ilv1++)
	{
		for (int ilv2=0;ilv2<font_cmap_segCount;ilv2++)
		{
			if ((font_cmap_startCount[ilv2]<=ilv1) && (font_cmap_endCount[ilv2]>=ilv1))
			{
				if (font_cmap_idRangeOffset[ilv2]!=0)
				{
					int outcode=(_u16)(font_cmap_glyphIdArray[(font_cmap_idRangeOffset[ilv2]/2)-(font_cmap_segCount-ilv2)+ilv1-font_cmap_startCount[ilv2]]+font_cmap_idDelta[ilv2])&((_u16)0xFFFF);
					if (outcode==incode) return ilv1;
				}
				else
				{
					if (incode==((_u16)(ilv1+font_cmap_idDelta[ilv2]))&((_u16)0xFFFF))
					{
						return ilv1;
					}
				}
			}
		}
	}
	return 0;
}
void font_start()
{
	fseek(fontfile,font_glyfpos,SEEK_SET);
}
int font(char medipoints=1)
{
	if (ftell(fontfile)>=font_glyfpos+font_glyflength) return 0;
	glyph.units=geti16();
	glyph.auxpointcount=0;
	fprintf(stderr,"UNITS:%i\n",glyph.units);
	glyph.minx=geti16();
	glyph.miny=geti16();
	glyph.maxx=geti16();
	glyph.maxy=geti16();
	_i16 x=0;
	_i16 y=0;
	if (glyph.units<0)
	{
		printf("composite%i!\n",glyph.units);
		glyph.composite.count=0;
		goto icomposite;
	}
	for (int ilv1=0;ilv1<glyph.units;ilv1++)
	{
		glyph.simple.endpoints[ilv1]=getu16();
	}
	glyph.simple.instructionlength=getu16();
	for (int ilv1=0;ilv1<glyph.simple.instructionlength;ilv1++)
	{
		_u8 ihv1;
		fread(&ihv1,1,1,fontfile);
	}
	glyph.maxcount=glyph.simple.endpoints[glyph.units-1]+1;
	for (int currentcount=0;currentcount<glyph.maxcount;)
	{
		fread(&(glyph.simple.donecoordinates[currentcount].flags),1,1,fontfile);
		printf("%hhX\n",(glyph.simple.donecoordinates[currentcount].flags));
		if ((glyph.simple.donecoordinates[currentcount].flags) & 8)
		{
			_u8 ihv1;
			fread(&ihv1,1,1,fontfile);
			printf("x%hhi\n",ihv1);
			for (int ilv2=1;ilv2<=ihv1;ilv2++)
			{
				glyph.simple.donecoordinates[currentcount+ilv2].flags=glyph.simple.donecoordinates[currentcount].flags;//actually, this count is additive
			}
			currentcount+=ihv1+1;
		}
		else
		{
			currentcount++;
		}
	}
	x=0;
	y=0;
	for (int ilv1=0;ilv1<glyph.maxcount;ilv1++)
	{
		_u8 currentflags=glyph.simple.donecoordinates[ilv1].flags;
		if ((currentflags & 0x2)!=0)
		{
			if ((currentflags & 0x10)!=0)
			{
				x+=getu8();
			}
			else
			{
				x-=getu8();
			}
		}
		else
		{
			if ((currentflags & 0x10)==0)
			{
				x+=geti16();
			}
		}
		glyph.simple.donecoordinates[ilv1].x=x;
	}
	for (int ilv1=0;ilv1<glyph.maxcount;ilv1++)
	{
		_u8 currentflags=glyph.simple.donecoordinates[ilv1].flags;
		if ((currentflags & 0x4)!=0)
		{
			if ((currentflags & 0x20)!=0)
			{
				y+=getu8();
			}
			else
			{
				y-=getu8();
			}
		}
		else
		{
			if ((currentflags & 0x20)==0)
			{
				y+=getu16();
			}
		}
		glyph.simple.donecoordinates[ilv1].y=y;
	}
	glyph.printme();
	while(ftell(fontfile)&0x3) getu8();//The next element is aligned!
	insertmedipoints(medipoints);
	return 1;
	icomposite:;
	glyph.composite.unit[glyph.composite.count].flags=getu16();
//	glyph.composite.unit[glyph.composite.count].flags=(glyph.composite.unit[glyph.composite.count].flags>>8)+(glyph.composite.unit[glyph.composite.count].flags<<8);
	glyph.composite.unit[glyph.composite.count].index=getu16();
	if (glyph.composite.unit[glyph.composite.count].flags & 1)
	{
		glyph.composite.unit[glyph.composite.count].arg1=geti16();
		glyph.composite.unit[glyph.composite.count].arg2=geti16();
	}
	else
	{
		glyph.composite.unit[glyph.composite.count].arg1=geti8();
		glyph.composite.unit[glyph.composite.count].arg2=geti8();
	}
	if (glyph.composite.unit[glyph.composite.count].flags & 2)
	{
		glyph.composite.unit[glyph.composite.count].arg2=-glyph.composite.unit[glyph.composite.count].arg2;
	}
	glyph.composite.unit[glyph.composite.count].scalexx=1;
	glyph.composite.unit[glyph.composite.count].scalexy=0;
	glyph.composite.unit[glyph.composite.count].scaleyx=0;
	glyph.composite.unit[glyph.composite.count].scaleyy=1;
	if (glyph.composite.unit[glyph.composite.count].flags & 8)
	{
		glyph.composite.unit[glyph.composite.count].scalexx=geti16();
		glyph.composite.unit[glyph.composite.count].scaleyy=glyph.composite.unit[glyph.composite.count].scalexx;
	}
	else if (glyph.composite.unit[glyph.composite.count].flags & 0x40)
	{
		glyph.composite.unit[glyph.composite.count].scalexx=geti16();
		glyph.composite.unit[glyph.composite.count].scaleyy=geti16();
	}
	else if (glyph.composite.unit[glyph.composite.count].flags & 0x80)
	{
		glyph.composite.unit[glyph.composite.count].scalexx=geti16();
		glyph.composite.unit[glyph.composite.count].scalexy=geti16();
		glyph.composite.unit[glyph.composite.count].scaleyx=geti16();
		glyph.composite.unit[glyph.composite.count].scaleyy=geti16();
	}
	glyph.composite.count++;
	if (glyph.composite.unit[glyph.composite.count-1].flags & 0x20)
	{
		goto icomposite;
	}
	if (glyph.composite.unit[glyph.composite.count-1].flags & 0x100)
	{
		int instcount=getu16();
		for (int ilvx=0;ilvx<instcount;ilvx++)
		{
			getu8();
		}
	}
	glyph.printme();
	while(ftell(fontfile)&0x3) getu8();//The next element is aligned!
	return 1;
}
