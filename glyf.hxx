struct glyf_coordinate_
{
	_u8 flags;
	_i16 x;
	_i16 y;
	float modx;
	float mody;
};
struct glyf_
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
		glyf_coordinate_ donecoordinates[65536];
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
	glyf_()
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
glyf_ * glyphmemory;
int glyphmemory_count;
int glyphmemory_max;
void glyf_modglyph(int ino,int mode,float iposx,float iposy,int * horzx,int * horzy,float xx,float yx,float xy,float yy,int * xmin=NULL,int * xmax=NULL,int * ymin=NULL,int * ymax=NULL)
{
	if (ymax!=NULL)
	{
		(*xmin)=2147483647;
		(*xmax)=-2147483648;
		(*ymin)=2147483647;
		(*ymax)=-2147483648;
	}
	if (glyphmemory[ino].units<=0) return;
	for (int ilv1=0;ilv1<glyphmemory[ino].maxcount;ilv1++)
	{
		float tl_in_x,tl_in_y,tl_out_x,tl_out_y;
		tl_in_x=glyphmemory[ino].simple.donecoordinates[ilv1].x;
		tl_in_y=glyphmemory[ino].simple.donecoordinates[ilv1].y;
		tl_out_x=(tl_in_x+(*horzx))*xx+(tl_in_y+(*horzy))*xy+iposx;
		tl_out_y=(tl_in_x+(*horzx))*yx+(tl_in_y+(*horzy))*yy+iposy;
		if (ymax!=NULL)
		{
			if (tl_out_x<(*xmin)) (*xmin)=tl_out_x;	
			if (tl_out_y<(*ymin)) (*ymin)=tl_out_y;	
			if (tl_out_x>(*xmax)) (*xmax)=tl_out_x;	
			if (tl_out_y>(*ymax)) (*ymax)=tl_out_y;	
		}
		glyphmemory[ino].simple.donecoordinates[ilv1].modx=tl_out_x;
		glyphmemory[ino].simple.donecoordinates[ilv1].mody=tl_out_y;
	}
	(*horzx)+=glyphmemory[ino].offsetx;//1200
	//TODO: add points for underline, only after output is not in glyf memory any longer
	//also add a square around the letter, depending on mode, to show selection
}
void do_inglyph(glyf_ * inglyph,FILE * infile)
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
int glyf_init(FILE * infile)
{
	fread(&glyphmemory_max,4,1,infile);
	glyphmemory=(glyf_*)malloc(glyphmemory_max*sizeof(glyf_));
	glyphmemory_count=0;
	for (int ilv1=0;ilv1<glyphmemory_max;ilv1++)
	{
		do_inglyph(glyphmemory+ilv1,infile);
	}
	fclose(infile);
	return 0;
}
