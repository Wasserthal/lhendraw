#include <math.h>
float Pi=3.141592654;
#define minfloat -1.0e20
#define maxfloat 1.0e20
#define _small int
#include "definitionlist.h"
float getangle(float dx,float dy)
{
	float frac;
	float val;
	if (dx!=0)
	{
		frac=dy/dx;
	}
	else
	{
		if (dy>=0)
		{
			val=Pi/2;
		}
		else
		{
			val=3*Pi/2;
		}
		return val;
	}
	val=atan(frac);
	if (dx<0)
	{
		val+=Pi;
	}
	if (val<0)
	{
		val+=2*Pi;
	}
	return val;
}
float bonddist=4;
float boldwidth=5;
float arrowdepth=15;
float arrowthickness=5;

multilist<color_instance> * glob_color_multilist;
multilist<n_instance> * glob_n_multilist;
multilist<b_instance> * glob_b_multilist;
multilist<t_instance> * glob_t_multilist;
multilist<s_instance> * glob_s_multilist;
multilist<font_instance> * glob_font_multilist;
char colorstring[7]="AABBCC";
char resortedstring[stringlength];
FILE * outfile;
float currentbasex,currentbasey;
color_instance * get_color(int number)
{
	if (number==0)
	{
		return NULL;
	}
	if (number==1)
	{
		return NULL;
	}
	if (number-2>=(*glob_color_multilist).filllevel)
	{
		return NULL;
	}
	return &(((*glob_color_multilist).bufferlist)[number-2]);//is this really the best method? After all, colors have no IDs.
}

int get_colorstring(int number)
{
	if (number==0)
	{
		strcpy(colorstring,"000000");
		return 0;
	}
	if (number==1)
	{
		strcpy(colorstring,"FFFFFF");
		return 0;
	}
	if (number-2>=(*glob_color_multilist).filllevel)
	{	
		strcpy(colorstring,"000000");
		return -1;
	}
	snprintf(colorstring,7,"%02hhX%02hhX%02hhX",char(((*glob_color_multilist).bufferlist)[number-2].r*255),char(((*glob_color_multilist).bufferlist)[number-2].g*255),char(((*glob_color_multilist).bufferlist)[number-2].b*255)); //is this really the best method? After all, colors have no IDs.
	return 0;
}
inline int get_colorstring_passive(int number)
{
	if (number!=0)
	{
		get_colorstring(number);
	}
}

void getcaptions(float * width,float * height,float * left,float * top)
{
	float minx=maxfloat;
	float miny=maxfloat;
	float maxx=minfloat;
	float maxy=minfloat;
	n_instance * i_n_instance;
	graphic_instance * i_graphic_instance;
	multilist<n_instance> * i_n_multilist=retrievemultilist<n_instance>();
	multilist<graphic_instance> * i_graphic_multilist=retrievemultilist<graphic_instance>();
	
	for (int ilv1=0;ilv1<(*i_n_multilist).filllevel;ilv1++)
	{
		i_n_instance=(*i_n_multilist).bufferlist+ilv1;
		if ((*i_n_instance).p.x>maxx)
		{
			maxx=(*i_n_instance).p.x;
		}
		if ((*i_n_instance).p.y>maxy)
		{
			maxy=(*i_n_instance).p.y;
		}
		if ((*i_n_instance).p.x<minx)
		{
			minx=(*i_n_instance).p.x;
		}
		if ((*i_n_instance).p.y<miny)
		{
			miny=(*i_n_instance).p.y;
		}
	}
	for (int ilv1=0;ilv1<(*i_graphic_multilist).filllevel;ilv1++)
	{
		i_graphic_instance=(*i_graphic_multilist).bufferlist+ilv1;
		if ((*i_graphic_instance).BoundingBox.left>maxx)
		{
			maxx=(*i_graphic_instance).BoundingBox.left;
		}
		if ((*i_graphic_instance).BoundingBox.top>maxy)
		{
			maxy=(*i_graphic_instance).BoundingBox.top;
		}
		if ((*i_graphic_instance).BoundingBox.left<minx)
		{
			minx=(*i_graphic_instance).BoundingBox.left;
		}
		if ((*i_graphic_instance).BoundingBox.top<miny)
		{
			miny=(*i_graphic_instance).BoundingBox.top;
		}
		if ((*i_graphic_instance).BoundingBox.right>maxx)
		{
			maxx=(*i_graphic_instance).BoundingBox.right;
		}
		if ((*i_graphic_instance).BoundingBox.bottom>maxy)
		{
			maxy=(*i_graphic_instance).BoundingBox.bottom;
		}
		if ((*i_graphic_instance).BoundingBox.right<minx)
		{
			minx=(*i_graphic_instance).BoundingBox.right;
		}
		if ((*i_graphic_instance).BoundingBox.bottom<miny)
		{
			miny=(*i_graphic_instance).BoundingBox.bottom;
		}
	}
	(*width)=maxx;
	(*height)=maxy;
	if (left!=NULL)
	{
		*left=minx;
	}
	if (top!=NULL)
	{
		*top=miny;
	}
}

struct atom_actual_node_
{
	_small bonds[10];
	_small bondcount;
	_small special;//offset in text list. TODO**** for other purposes (if applicable)
	inline char operator += (_small input)
	{
		if (bondcount<10)
		{
			bonds[bondcount++]=input;
			return 1;
		}
		else
		{
			return 0;
		}
	}
};

atom_actual_node_ atom_actual_node[bufferlistsize];
struct bond_actual_node_
{
	_small start,end;
};

bond_actual_node_ bond_actual_node[bufferlistsize];
struct text_actual_node_
{
	_small owner;
};
text_actual_node_ text_actual_node[bufferlistsize];

char getleftof(cdx_Point2D * istart,cdx_Point2D * iend,cdx_Point2D * ikink)
{
	float diff1x,diff1y,diff2x,diff2y;
	float iresult;
	diff1x=(*iend).x-(*istart).x;
	diff1y=(*iend).y-(*istart).y;
	diff2x=(*ikink).x-(*iend).x;
	diff2y=(*ikink).y-(*iend).y;
	iresult=(diff1x*diff2y-diff1y*diff2x);
	if (iresult==0) return 0;
	return (diff1x*diff2y-diff1y*diff2x>0) ? 1 : 2;//then, it is right of=> return1, otherwise, it is left of=>return2;
}

_small getother(_small inatom, _small inbond)
{
	if (bond_actual_node[inbond].end==inatom)
	{
		return (bond_actual_node[inbond].start);
	}
	if (bond_actual_node[inbond].start==inatom)
	{
		return (bond_actual_node[inbond].end);
	}
}

void getatoms()//makes some preprocessing
{
	for (int ilv1=0;ilv1<bufferlistsize;ilv1++)
	{
		atom_actual_node[ilv1].bondcount=0;
		atom_actual_node[ilv1].special=-1;
	}
	for (int ilv1=0;ilv1<bufferlistsize;ilv1++)
	{
		text_actual_node[ilv1].owner=-1;
	}
	for (int ilv1=0;ilv1<(*glob_b_multilist).filllevel;ilv1++)
	{
		for (int ilv2=0;ilv2<(*glob_n_multilist).filllevel;ilv2++)
		{
			if (((*glob_n_multilist).bufferlist)[ilv2].id==((*glob_b_multilist).bufferlist)[ilv1].E)
			{
				bond_actual_node[ilv1].end=ilv2;
				atom_actual_node[ilv2]+=ilv1;
			}
			if (((*glob_n_multilist).bufferlist)[ilv2].id==((*glob_b_multilist).bufferlist)[ilv1].B)
			{
				bond_actual_node[ilv1].start=ilv2;
				atom_actual_node[ilv2]+=ilv1;
			}
		}
	}
	for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
	{
		for (int ilv2=0;ilv2<atom_actual_node[ilv1].bondcount;ilv2++)
		{
			_small partner=getother(ilv1,(atom_actual_node[ilv1]).bonds[ilv2]);
		}
		for (int ilv2=(*((*glob_n_multilist).bufferlist)[ilv1].t).start_in_it;ilv2<(*((*glob_n_multilist).bufferlist)[ilv1].t).start_in_it+(*((*glob_n_multilist).bufferlist)[ilv1].t).count_in_it;ilv2++)
		{
			text_actual_node[ilv2].owner=ilv1;
			atom_actual_node[ilv1].special=ilv2;
		}
	}
	for (int ilv1=0;ilv1<(*glob_b_multilist).filllevel;ilv1++)
	{
		b_instance * currentbondinstance=&((*glob_b_multilist).bufferlist[ilv1]);
		int i_side_orvariable;
		int i_side_orvariable2;
		i_side_orvariable=0;
		i_side_orvariable2=0;
		if (((*currentbondinstance).DoublePosition & 0x100)==0)
		{
			for (int ilv2=0;ilv2<atom_actual_node[bond_actual_node[ilv1].start].bondcount;ilv2++)//or operation
			{
				i_side_orvariable|=getleftof(&((*glob_n_multilist).bufferlist[bond_actual_node[ilv1].start].p),&((*glob_n_multilist).bufferlist[bond_actual_node[ilv1].end].p),&((*glob_n_multilist).bufferlist[getother((bond_actual_node[ilv1].start),atom_actual_node[bond_actual_node[ilv1].start].bonds[ilv2])].p));
			}
			if (i_side_orvariable==3)
			{
				i_side_orvariable=0;
			}
			for (int ilv2=0;ilv2<atom_actual_node[bond_actual_node[ilv1].end].bondcount;ilv2++)//or operation
			{
				i_side_orvariable2|=getleftof(&((*glob_n_multilist).bufferlist[bond_actual_node[ilv1].start].p),&((*glob_n_multilist).bufferlist[bond_actual_node[ilv1].end].p),&((*glob_n_multilist).bufferlist[getother((bond_actual_node[ilv1].end),atom_actual_node[bond_actual_node[ilv1].end].bonds[ilv2])].p));
			}
			if (i_side_orvariable2==3)
			{
				i_side_orvariable2=0;
			}
			i_side_orvariable|=i_side_orvariable2;
			if (i_side_orvariable==3)
			{
				i_side_orvariable=2;
			}
			(*currentbondinstance).DoublePosition=i_side_orvariable;
		}
		
	}
}

char stylestring[1024];
char emptystring[1]="";
char semicolonstring[2]=";";
void stylegenestring(int flags)
{
	int stylestringlength=0;
	stylestring[0]=0;//redundant
	int formatreturnvalue;
	char * waspar=emptystring;
	if (flags!=0)
	{
		sprintf(stylestring,"style=\"%n",&formatreturnvalue);
		stylestringlength+=formatreturnvalue;
		if (flags & 1)
		{
			sprintf(stylestring+stylestringlength,"%sstroke:#%s%n",waspar,colorstring,&formatreturnvalue);
			stylestringlength+=formatreturnvalue;
			waspar=semicolonstring;
		}
		if (flags & 2)
		{
			sprintf(stylestring+stylestringlength,"%sfill:#%s%n",waspar,colorstring,&formatreturnvalue);
			stylestringlength+=formatreturnvalue;
			waspar=semicolonstring;
		}
		else
		{
			sprintf(stylestring+stylestringlength,"%sfill:none%n",waspar,&formatreturnvalue);
			stylestringlength+=formatreturnvalue;
			waspar=semicolonstring;
		}
		if (flags & 4)
		{
			printf("is on");
			sprintf(stylestring+stylestringlength,"%sstroke-width:5;stroke-linecap:round%n",waspar,&formatreturnvalue);
			stylestringlength+=formatreturnvalue;
			waspar=semicolonstring;
		}
		if (flags & 8)
		{
			sprintf(stylestring+stylestringlength,"%sstroke-dasharray:2,2%n",waspar,&formatreturnvalue);
			stylestringlength+=formatreturnvalue;
			waspar=semicolonstring;
		}
		sprintf(stylestring+stylestringlength,"\"%n",&formatreturnvalue);
		stylestringlength+=formatreturnvalue;
	}
	stylestring[stylestringlength]=0;
}

void expressline(float ileft,float itop,float iright,float ibottom)
{
	fprintf(outfile,"<path d=\"M %f %f L %f %f \" %s/>\n",ileft-currentbasex,itop-currentbasey,iright-currentbasex,ibottom-currentbasey,stylestring);
}
void expresstetrangle(float ix1,float iy1,float ix2,float iy2,float ix3,float iy3,float ix4,float iy4,char * istylestring)
{
	fprintf(outfile,"<path d=\"M %f %f L %f %f L %f %f L %f %f z \" %s/>\n",
        ix1-currentbasex,iy1-currentbasey,ix2-currentbasex,iy2-currentbasey,ix3-currentbasex,iy3-currentbasey,ix4-currentbasex,iy4-currentbasey,
istylestring);
}

char iswedgenr(_small input)
{
	if (input==3) return 1;
	if (input==4) return 2;
	if (input==6) return 1;
	if (input==7) return 2;
	if (input==9) return 1;
	if (input==10) return 2;
	if (input==11) return 1;
	if (input==12) return 2;
	return 0;
}

char resortstring(char * input) //TODO: what about brackets?
{
	int ilength=strlen(input);
	int fsm=0;
	int start,end;
	resortedstring[0]=0;
	int resortedstringpos=0;
	for (int ilv1=ilength-1;ilv1>=0;ilv1--)
	{
		if (input[ilv1]==' ') {return 0;}
		if ((input[ilv1]>='0') && (input[ilv1]<='9'))
		{
			if (fsm==1)
			{
				start=ilv1;
			}
			else
			{
				if (fsm==2)
				for (int ilv2=start;ilv2<=end;ilv2++)
				{
					resortedstring[resortedstringpos++]=input[ilv2];
				}
				end=ilv1;
				start=ilv1;
				fsm=1;
			}
		}
		if ((input[ilv1]>='a') && (input[ilv1]<='z'))
		{
			if ((fsm==2) || (fsm==1))
			{
				start=ilv1;
				fsm=2;
			}
			else
			{
				if (fsm==1)
				{
					for (int ilv2=start;ilv2<=end;ilv2++)//write down numbers
					{
						resortedstring[resortedstringpos++]=input[ilv2];
					}
				}
				end=ilv1;
				start=ilv1;
				fsm=2;
			}
		}
		if ((input[ilv1]>='A') && (input[ilv1]<='Z'))
		{
			if ((fsm!=2) && (fsm!=1))
			{
				if (fsm==1)
				{
					for (int ilv2=start;ilv2<=end;ilv2++)//write down numbers
					{
						resortedstring[resortedstringpos++]=input[ilv2];
					}
				}
				end=ilv1;
			}
			start=ilv1;
			fsm=0;
			for (int ilv2=start;ilv2<=end;ilv2++)
			{
				resortedstring[resortedstringpos++]=input[ilv2];
			}
		}
	}
	return 1;
}

font_instance * getfont(_small iid)
{
	for (int ilv1=0;ilv1<(*glob_font_multilist).filllevel;ilv1++)
	{
		if (((*glob_font_multilist).bufferlist[ilv1]).id==iid)
		{
			return &((*glob_font_multilist).bufferlist[ilv1]);
		}
	}
	return NULL;
}

#define outputstringlength 4096
char geek_greek_string[outputstringlength];
void converttogreek(char * input)
{
	char substring[8];
	geek_greek_string[0]=0;
	int greekstring_length=0;
	char * backvalue;
	int ilv1,ilv2;
	for (ilv1=0;input[ilv1]!=0;ilv1++)
	{
		for (ilv2=0;input[ilv1+ilv2] & 0x80;ilv2++)
		{
			substring[ilv2]=input[ilv1+ilv2];
		}
		if (ilv2==0)
		{
			substring[0]=input[ilv1];
			ilv2++;
		}
		if (ilv2<7)//important! without this restriction, it is a super intrusion vector!
		{
			substring[ilv2]=0;
			if (ilv2>0)
			{
				backvalue=get_list_greeklist(substring);
				if (backvalue!=NULL)
				{
					_small backvaluelength=strlen(backvalue);
					if (greekstring_length+backvaluelength<outputstringlength-1)
					{
						strcpy(&geek_greek_string[greekstring_length],backvalue);
						ilv1+=ilv2-1;//one less, because it is incremented anyway
						greekstring_length+=backvaluelength;
						goto ifertig;
					}
				}
			}
		}
		geek_greek_string[greekstring_length++]=input[ilv1];
		ifertig:
		;
	}
	geek_greek_string[greekstring_length]=0;
	return;
}

void calcdelta(float * x1,float * y1,float inputx,float inputy)
{
	float iquotient;
	if (fabs(inputx)>fabs(inputy))
	{
		iquotient=fabs(inputy/inputx);
		(*x1)=5.0*((inputx>0) ? 1.0 :-1.0);
		(*y1)=5.0*iquotient*((inputy>0) ? 1.0 :-1.0);
	}
	else
	{
		if ((inputy)!=0)
		{
			iquotient=fabs(inputx/inputy);
		}
		(*x1)=8.0*iquotient*((inputx>0) ? 1.0 :-1.0);
		(*y1)=8.0*((inputy>0) ? 1.0 :-1.0);
	}
}

void svg_main(const char * filename)
{
	float cangle;
	float langle;
	float width, height;
	float ileft,itop;
	float ibonddist;
	float ibonddist2;
	float ibonddist3;
	float ibonddist4;
	int colornr;
	int fontnr;
	char * finalstring;
	cdx_Rectangle iBBX;
	_small owner;
	outfile=fopen(filename,"w+");
	fprintf(outfile,"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>");
	glob_color_multilist=retrievemultilist<color_instance>();
	glob_b_multilist=retrievemultilist<b_instance>();
	glob_n_multilist=retrievemultilist<n_instance>();
	glob_t_multilist=retrievemultilist<t_instance>();
	glob_s_multilist=retrievemultilist<s_instance>();
	glob_font_multilist=retrievemultilist<font_instance>();
	getcaptions(&width,&height,&ileft,&itop);
	ileft-=10;
	itop-=10;
	width+=20;
	height+=20;
	getatoms();
	fprintf(outfile,"<svg version=\"1.0\" width=\"%f\" height=\"%f\">\n",width-ileft,height-itop);
	currentbasex=ileft;
	currentbasey=itop;
	n_instance * startnode, * endnode;
	multilist<graphic_instance> * i_graphic_multilist=retrievemultilist<graphic_instance>();
	for (int ilv1=0;ilv1<(*i_graphic_multilist).filllevel;ilv1++)
	{
		iBBX=((*i_graphic_multilist).bufferlist)[ilv1].BoundingBox;
		colornr=((*i_graphic_multilist).bufferlist)[ilv1].color;
		get_colorstring(colornr);
		if (((*i_graphic_multilist).bufferlist)[ilv1].GraphicType==1)
		{
			stylegenestring(1);
			expressline(iBBX.left,iBBX.top,iBBX.right,iBBX.bottom);
			cangle=getangle(iBBX.right-iBBX.left,iBBX.top-iBBX.bottom)+Pi/2;
			langle=getangle(iBBX.right-iBBX.left,iBBX.top-iBBX.bottom);
			if (((*i_graphic_multilist).bufferlist)[ilv1].ArrowType==2)
			{
				stylegenestring(3);
				expresstetrangle(iBBX.left,iBBX.top,
iBBX.left+cos(langle)*arrowdepth+cos(cangle)*arrowthickness,iBBX.top+sin(langle)*arrowdepth+sin(cangle)*arrowthickness,
iBBX.left+cos(langle)*arrowdepth-cos(cangle)*arrowthickness,iBBX.top+sin(langle)*arrowdepth-sin(cangle)*arrowthickness,
iBBX.left,iBBX.top,
stylestring);
			}
		}
		if (((*i_graphic_multilist).bufferlist)[ilv1].GraphicType==3)
		{
			stylegenestring(1);
			expressline(iBBX.left,iBBX.top,iBBX.right,iBBX.top);
			expressline(iBBX.left,iBBX.bottom,iBBX.right,iBBX.bottom);
			expressline(iBBX.left,iBBX.top,iBBX.left,iBBX.bottom);
			expressline(iBBX.right,iBBX.top,iBBX.right,iBBX.bottom);
		}
	}
	for (int ilv1=0;ilv1<(*glob_b_multilist).filllevel;ilv1++)
	{
		_small inr_E,inr_S;
		colornr=((*glob_b_multilist).bufferlist)[ilv1].color;
		get_colorstring(colornr);
		for (int ilv2=0;ilv2<(*glob_n_multilist).filllevel;ilv2++)//TODO: base on pre-calcd. values
		{
			if (((*glob_n_multilist).bufferlist)[ilv2].id==((*glob_b_multilist).bufferlist)[ilv1].E)
			{
				inr_E=ilv2;
				endnode=&(((*glob_n_multilist).bufferlist)[ilv2]);
			}
			if (((*glob_n_multilist).bufferlist)[ilv2].id==((*glob_b_multilist).bufferlist)[ilv1].B)
			{
				inr_S=ilv2;
				startnode=&(((*glob_n_multilist).bufferlist)[ilv2]);
			}
		}
		ibonddist=0;ibonddist2=0;
		if (((*glob_b_multilist).bufferlist)[ilv1].Order>1.1)//floats, after all....
		{
			switch(((*glob_b_multilist).bufferlist)[ilv1].DoublePosition & 0xFF)
			{
				case 0 : ibonddist=bonddist/2;ibonddist2=-bonddist/2;break;
				case 1 : ibonddist=bonddist;break;
				case 2 : ibonddist=-bonddist;break;
			}
		}
		cangle=getangle((*endnode).p.x-(*startnode).p.x,(*endnode).p.y-(*startnode).p.y)+Pi/2;
		float textdeltax,textdeltay;
		calcdelta(&textdeltax,&textdeltay,(*endnode).p.x-(*startnode).p.x,(*endnode).p.y-(*startnode).p.y);
		iBBX.left=(*startnode).p.x+textdeltax*(atom_actual_node[inr_S].special!=-1);
		iBBX.top=(*startnode).p.y+textdeltay*(atom_actual_node[inr_S].special!=-1);
		int iDisplaytype1=((*glob_b_multilist).bufferlist[ilv1].Display);
		iBBX.right=(*endnode).p.x-textdeltax*(atom_actual_node[inr_E].special!=-1);
		iBBX.bottom=(*endnode).p.y-textdeltay*(atom_actual_node[inr_E].special!=-1);
		if (iswedgenr(iDisplaytype1)>0)
		{
			ibonddist3=0;
			ibonddist4=0;
			if (iswedgenr(iDisplaytype1)==2)
			{
				ibonddist3=boldwidth/2;
			}
			if (iswedgenr(iDisplaytype1)==1)
			{
				ibonddist4=boldwidth/2;
			}
			stylegenestring(
(((iDisplaytype1==6) || (iDisplaytype1==7))?2:0) |
1);
			expresstetrangle(
iBBX.left+ibonddist3*cos(cangle),iBBX.top+ibonddist3*sin(cangle),
iBBX.left-ibonddist3*cos(cangle),iBBX.top-ibonddist3*sin(cangle),
iBBX.right+ibonddist4*cos(cangle),iBBX.bottom+ibonddist4*sin(cangle),
iBBX.right-ibonddist4*cos(cangle),iBBX.bottom-ibonddist4*sin(cangle),
stylestring);
		}
		else
		{
			stylegenestring(((iDisplaytype1==5) ?4:0) | ((iDisplaytype1==1) ?8:0) | 1);
			expressline(iBBX.left+ibonddist2*cos(cangle),iBBX.top+ibonddist2*sin(cangle),iBBX.right+ibonddist2*cos(cangle),iBBX.bottom+ibonddist2*sin(cangle));
		}
		if (((*glob_b_multilist).bufferlist)[ilv1].Order==2)
		{
			stylegenestring((((*glob_b_multilist).bufferlist[ilv1].Display2==1)?8:0)|1);
			expressline(iBBX.left+ibonddist*cos(cangle),iBBX.top+ibonddist*sin(cangle),iBBX.right+ibonddist*cos(cangle),iBBX.bottom+ibonddist*sin(cangle));
		}
	}
	multilist<t_instance> * i_t_multilist=retrievemultilist<t_instance>();
	multilist<s_instance> * i_s_multilist=retrievemultilist<s_instance>();
	for (int ilv1=0;ilv1<(*i_t_multilist).filllevel;ilv1++)
	{
		owner=text_actual_node[ilv1].owner;
		colornr=0;
		if (owner!=-1)
		{
			colornr=(*glob_n_multilist).bufferlist[text_actual_node[ilv1].owner].color;
			get_colorstring(colornr);
		}
		colornr=((*i_t_multilist).bufferlist)[ilv1].color;
		get_colorstring_passive(colornr);

		fprintf(outfile,"<text fill=\"%s\" %s stroke=\"none\" transform=\"translate(%f,%f)\" font-size=\"18\">",colorstring,((*i_t_multilist).bufferlist[ilv1].LabelAlignment==-1) ? "text-anchor=\"end\" text-align=\"end\"" : "",((*i_t_multilist).bufferlist)[ilv1].p.x-currentbasex,((*i_t_multilist).bufferlist)[ilv1].p.y-currentbasey);
		intl start,end;
		start=(*(((*i_t_multilist).bufferlist)[ilv1].s)).start_in_it;
		end=start+(*(((*i_t_multilist).bufferlist)[ilv1].s)).count_in_it;
		char string_resorted=0;
		for (int ilv2=start;ilv2<end;ilv2++)
		{
			colornr=((*i_s_multilist).bufferlist)[ilv2].color;
			fontnr=((*i_s_multilist).bufferlist)[ilv2].font;
			font_instance * i_font_instance=getfont(fontnr);
			finalstring=(((*i_s_multilist).bufferlist))[ilv2].PCTEXT.a;
			if (owner!=-1)
			{
				if ((*((*i_t_multilist).bufferlist[ilv1].s)).count_in_it==1)
				{
					if (((*i_s_multilist).bufferlist[start].face & 0x60)>0)
					{
						if ((*i_t_multilist).bufferlist[ilv1].Justification==-1)
						{
							string_resorted=resortstring(finalstring);
							if (string_resorted)
							{
								finalstring=resortedstring;
							}
						}
					}
				}
			}
			if (strcmp((*i_font_instance).name.a,"Symbol")==0)
			{
				converttogreek(finalstring);
				finalstring=geek_greek_string;
			}
			if (colornr!=0)
			{
				get_colorstring(colornr);
			}
			fprintf(outfile,"<tspan style=\"fill:#%s\">%s</tspan>\n",colorstring,finalstring);
		}
		fprintf(outfile,"</text>\n");
	}
	
//	for (int ilv1=0;ilv1<(*i_
	fprintf(outfile,"</svg>");
	fclose(outfile);
}
