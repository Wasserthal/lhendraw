#include <math.h>
float Pi=3.141592654;
#define minfloat -1.0e20
#define maxfloat 1.0e20
#define _small int
#include "definitionlist.h"
#include "janitor.hxx"
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
float xboldwidth=15;
float arrowheadlength=15;
float arrowthickness=5;
float atomfontheight=18.0;

multilist<color_instance> * glob_color_multilist;
multilist<n_instance> * glob_n_multilist;
multilist<b_instance> * glob_b_multilist;
multilist<t_instance> * glob_t_multilist;
multilist<s_instance> * glob_s_multilist;
multilist<graphic_instance> * glob_graphic_multilist;
multilist<annotation_instance> * glob_annotation_multilist;
multilist<font_instance> * glob_font_multilist;
multilist<curve_instance> * glob_curve_multilist;
char colorstring[7]="AABBCC";
char colorstring2[7]="AABBCC";
char resortedstring[stringlength];
FILE * outfile;
float SVG_currentbasex,SVG_currentbasey;
float SVG_width,SVG_height;
float SVG_ileft,SVG_itop;
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

int get_colorstringv(int number,char * icolorstring)
{
	if (number==0)
	{
		strcpy(icolorstring,"000000");
		return 0;
	}
	if (number==1)
	{
		strcpy(icolorstring,"FFFFFF");
		return 0;
	}
	if (number-2>=(*glob_color_multilist).filllevel)
	{
		strcpy(icolorstring,"000000");
		return -1;
	}
	snprintf(icolorstring,7,"%02hhX%02hhX%02hhX",char(((*glob_color_multilist).bufferlist)[number-2].r*255),char(((*glob_color_multilist).bufferlist)[number-2].g*255),char(((*glob_color_multilist).bufferlist)[number-2].b*255)); //is this really the best method? After all, colors have no IDs.
	return 0;
}
int get_colorstring(int number)
{
	get_colorstringv(number,colorstring);
}

inline int get_colorstring_passive(int number)
{
	if (number!=0)
	{
		get_colorstringv(number,colorstring);
	}
}
void canonicalizeboundingbox(cdx_Rectangle * iBBX)
{
	float swap;
	if ((*iBBX).left>(*iBBX).right)
	{
		swap=(*iBBX).left;
		(*iBBX).left=(*iBBX).right;
		(*iBBX).right=swap;
	}
	if ((*iBBX).top>(*iBBX).bottom)
	{
		swap=(*iBBX).top;
		(*iBBX).top=(*iBBX).bottom;
		(*iBBX).bottom=swap;
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
	multilist<curve_instance> * i_curve_multilist=retrievemultilist<curve_instance>();
	for (int ilv1=0;ilv1<(*i_curve_multilist).filllevel;ilv1++)
	{
		curve_instance * i_curve_instance=(curve_instance*)&((*i_curve_multilist).bufferlist[ilv1]);
		for (int ilv2=0;ilv2<(*i_curve_instance).CurvePoints.count;ilv2++)
		{
			if ((*i_curve_instance).CurvePoints.a[ilv2].x>maxx)
			{
				maxx=(*i_curve_instance).CurvePoints.a[ilv2].x;
			}
			if ((*i_curve_instance).CurvePoints.a[ilv2].y>maxy)
			{
				maxy=(*i_curve_instance).CurvePoints.a[ilv2].y;
			}
			if ((*i_curve_instance).CurvePoints.a[ilv2].x<minx)
			{
				minx=(*i_curve_instance).CurvePoints.a[ilv2].x;
			}
			if ((*i_curve_instance).CurvePoints.a[ilv2].y<miny)
			{
				miny=(*i_curve_instance).CurvePoints.a[ilv2].y;
			}
		}
	}
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
	float cotanleft[2],cotanright[2];//fw,bw==end,start NOTE: when [1], right and left are also seen from the other side.
	float xcotanleft[2],xcotanright[2];//fw,bw==end,start NOTE: when [1], right and left are also seen from the other side.
	float numberleft[2],numberright[2];
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

void getforwardity()
{
	for (int ilv1=0;ilv1<(*glob_b_multilist).filllevel;ilv1++)
	{
	}
}

float angle_between[10][10];
_small number_between[10];
float compangle(float first,float second)
{
	float wert;
	wert=second-first;
	while (wert<-2*Pi) {wert+=2*Pi;}
	while (wert>2*Pi) {wert-=2*Pi;}
	if (wert>Pi) {wert-=2*Pi;}
	if (wert<-Pi) {wert+=2*Pi;}
	return wert;
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
		bond_actual_node[ilv1].cotanleft[0]=Pi/4;
		bond_actual_node[ilv1].cotanleft[1]=Pi/4;
		bond_actual_node[ilv1].cotanright[0]=Pi/4;
		bond_actual_node[ilv1].cotanright[1]=Pi/4;
		bond_actual_node[ilv1].xcotanleft[0]=Pi/2;
		bond_actual_node[ilv1].xcotanleft[1]=Pi/2;
		bond_actual_node[ilv1].xcotanright[0]=Pi/2;
		bond_actual_node[ilv1].xcotanright[1]=Pi/2;
		bond_actual_node[ilv1].numberleft[0]=-1;
		bond_actual_node[ilv1].numberleft[1]=-1;
		bond_actual_node[ilv1].numberright[0]=-1;
		bond_actual_node[ilv1].numberright[1]=-1;
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
		n_instance * tlatominstance=&((*glob_n_multilist).bufferlist[ilv1]);
		for (int ilv2=0;ilv2<atom_actual_node[ilv1].bondcount;ilv2++)
		{
			_small partner=getother(ilv1,(atom_actual_node[ilv1]).bonds[ilv2]);
			angle_between[ilv2][ilv2]=getangle((*glob_n_multilist).bufferlist[partner].p.x-(*tlatominstance).p.x,(*glob_n_multilist).bufferlist[partner].p.y-(*tlatominstance).p.y);//TODO:not good at real-time!!!
			number_between[ilv2]=(atom_actual_node[ilv1]).bonds[ilv2];
		}
		for (int ilv2=0;ilv2<atom_actual_node[ilv1].bondcount;ilv2++)//TODO:not good at real-time!!!
		{
			float intermediate;
			for (int ilv3=0;ilv3<atom_actual_node[ilv1].bondcount;ilv3++)
			{
				if (ilv3<ilv2)
				{
					intermediate=compangle(angle_between[ilv2][ilv2],angle_between[ilv3][ilv3]);
					angle_between[ilv2][ilv3]=intermediate;
					angle_between[ilv3][ilv2]=-intermediate;
				}
			}
		}
		for (int ilv2=0;ilv2<atom_actual_node[ilv1].bondcount;ilv2++)
		{
			bond_actual_node_ * i_bond_actual_node=&(bond_actual_node[atom_actual_node[ilv1].bonds[ilv2]]);
			float tlleftest,tlrightest;
			float tlxleftest,tlxrightest;
			int tlleftnr,tlrightnr;
			tlleftnr=-1;tlrightnr=-1;
			tlleftest=Pi;tlrightest=-Pi;
			tlxleftest=Pi;tlxrightest=-Pi;
			for (int ilv3=0;ilv3<atom_actual_node[ilv1].bondcount;ilv3++)
			{
				if (ilv3!=ilv2)
				{
					float iangle=angle_between[ilv2][ilv3];
					int adequate;
					adequate=1;
					if (fabs(iangle)<Pi/6){adequate=0;}
					if (fabs(iangle)>5*Pi/6){adequate=0;}
					if (iangle>0) if (iangle<tlleftest) {if (adequate){tlleftest=iangle;tlleftnr=number_between[ilv3];}if (iangle<tlxleftest){tlxleftest=iangle;tlleftnr=number_between[ilv3];}}
					if (iangle<0) if (iangle>tlrightest){if (adequate){tlrightest=iangle;tlrightnr=number_between[ilv3];}if (iangle>tlxrightest){tlxrightest=iangle;tlrightnr=number_between[ilv3];}}
					notfound:
					;
				}
			}
			//if the atom is a start atom, the bond has to be treated backwards
			if (tlleftnr!=-1)
			{
				(*i_bond_actual_node).numberleft[(*i_bond_actual_node).start==ilv1]=tlleftnr;
			}
			if (tlrightnr!=-1)
			{
				(*i_bond_actual_node).numberright[(*i_bond_actual_node).start==ilv1]=tlrightnr;
			}
			if (tlleftest<Pi)
			{
				(*i_bond_actual_node).cotanleft[(*i_bond_actual_node).start==ilv1]=tlleftest;
			}
			if (tlrightest>-Pi)
			{
				(*i_bond_actual_node).cotanright[(*i_bond_actual_node).start==ilv1]=-tlrightest;
			}
			if (tlxleftest<Pi/2)
			{
				(*i_bond_actual_node).xcotanleft[(*i_bond_actual_node).start==ilv1]=tlxleftest;
			}
			if (tlxrightest>-Pi/2)
			{
				(*i_bond_actual_node).xcotanright[(*i_bond_actual_node).start==ilv1]=-tlxrightest;
			}
		}
		for (int ilv2=(*((*glob_n_multilist).bufferlist)[ilv1].t).start_in_it;ilv2<(*((*glob_n_multilist).bufferlist)[ilv1].t).start_in_it+(*((*glob_n_multilist).bufferlist)[ilv1].t).count_in_it;ilv2++)//allows for multiple text items on one atom. Nonsense.
		{
			if ((*glob_t_multilist).bufferlist[ilv2].LabelAlignment==-1)
			{
				(*glob_t_multilist).bufferlist[ilv2].p.x=((*glob_n_multilist).bufferlist)[ilv1].p.x+7;
			}
			else
			{
				(*glob_t_multilist).bufferlist[ilv2].p.x=((*glob_n_multilist).bufferlist)[ilv1].p.x-7;
			}
			(*glob_t_multilist).bufferlist[ilv2].p.y=((*glob_n_multilist).bufferlist)[ilv1].p.y+atomfontheight/3;
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
int stylefromline(int input) {return 1|((input & 1)?8:0)|((input & 2)?4:0);}
int stylefromrectangle(int input) {return 1|((input & 0x10)?8:0)|((input & 0x20)?4:0)|((input & 0x8)?2:0);}

void expressline(float ileft,float itop,float iright,float ibottom)
{
	fprintf(outfile,"<path d=\"M %f %f L %f %f \" %s/>\n",ileft-SVG_currentbasex,itop-SVG_currentbasey,iright-SVG_currentbasex,ibottom-SVG_currentbasey,stylestring);
}
void expresscircle(float ileft,float itop,float iright,float ibottom)
{
	fprintf(outfile,"<ellipse cx=\"%f\" cy=\"%f\" rx=\"%f\" ry=\"%f\" %s/>\n",ileft-SVG_currentbasex,itop-SVG_currentbasey,iright-ileft,ibottom-itop,stylestring);
}
void expresstetrangle(float ix1,float iy1,float ix2,float iy2,float ix3,float iy3,float ix4,float iy4,char * istylestring)
{
	fprintf(outfile,"<path d=\"M %f %f L %f %f L %f %f L %f %f z \" %s/>\n",
	ix1-SVG_currentbasex,iy1-SVG_currentbasey,ix2-SVG_currentbasex,iy2-SVG_currentbasey,ix3-SVG_currentbasex,iy3-SVG_currentbasey,ix4-SVG_currentbasex,iy4-SVG_currentbasey,
istylestring);
}
#define dashdist 3
void expresshashangle(float langle,float cangle,float ix1,float iy1,float ix2,float iy2,float bonddist3,float bonddist4)
{
	float ilength=sqrt((ix2-ix1)*(ix2-ix1)+(iy2-iy1)*(iy2-iy1));
	for (float ilv1=0;ilv1<ilength;ilv1+=dashdist)
	{
		float share=(ilv1/ilength);
		float share2=share*bonddist3+(1-share)*bonddist4;
		expressline(ix1*(1-share)+ix2*share+cos(cangle)*share2,iy1*(1-share)+iy2*share+sin(langle)*share2,ix1*(1-share)+ix2*share-cos(cangle)*share2,iy1*(1-share)+iy2*share-sin(cangle)*share2);
	}
}
void expresshexangle(float ix1,float iy1,float ix2,float iy2,float ix3,float iy3,float ix4,float iy4,float ix5,float iy5,float ix6,float iy6)
{
	fprintf(outfile,"<path d=\"M %f %f L %f %f L %f %f L %f %f L %f %f L %f %f z \" %s/>\n",
	ix1-SVG_currentbasex,iy1-SVG_currentbasey,ix2-SVG_currentbasex,iy2-SVG_currentbasey,ix3-SVG_currentbasex,iy3-SVG_currentbasey,ix4-SVG_currentbasex,iy4-SVG_currentbasey,ix5-SVG_currentbasex,iy5-SVG_currentbasey,ix6-SVG_currentbasex,iy6-SVG_currentbasey,
stylestring);
}
void expressarc(float centerx,float centery,float radiusx,float radiusy,float startangle,float endangle)
{
	float startx,starty;
	float endx,endy;
	startx=centerx+radiusx*cos(startangle)-SVG_currentbasex;
	starty=centery+radiusy*sin(startangle)-SVG_currentbasey;
	endx=centerx+radiusx*cos(endangle)-SVG_currentbasex;
	endy=centery+radiusy*sin(endangle)-SVG_currentbasey;
	fprintf(outfile,"<path d=\"M %f,%f A %f,%f %i %i %i %f %f\" %s />",startx,starty,radiusx,radiusy,(int)0,(int)(fabs(startangle-endangle)>=Pi),(int)(startangle-endangle)<0,endx,endy,stylestring);
}

char iswedgenr(_small input)
{
	if (input==3) return 1;
	if (input==4) return 2;
	if (input==5) return 3;
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
	int fsm=0;//0=done 1: number 2: little letter 3: Capital letter 4: plus/minus
	int start,end;
	resortedstring[0]=0;
	int resortedstringpos=0;
	for (int ilv1=ilength-1;ilv1>=0;ilv1--)
	{
		if (input[ilv1]==' ') {return 0;}
		if ((input[ilv1]=='+') || (input[ilv1]=='-'))
		{
			if (fsm!=0)
			{
				for (int ilv2=start;ilv2<=end;ilv2++)
				{
					resortedstring[resortedstringpos++]=input[ilv2];
				}
			}
			fsm=4;
		}
		if ((input[ilv1]>='0') && (input[ilv1]<='9'))
		{
			if ((fsm==1) || (fsm==4))
			{
				start=ilv1;
			}
			else
			{
				if (fsm==2)
				{
					for (int ilv2=start;ilv2<=end;ilv2++)
					{
						resortedstring[resortedstringpos++]=input[ilv2];
					}
				}
				end=ilv1;
				start=ilv1;
				fsm=1;
			}
		}
		if ((input[ilv1]>='a') && (input[ilv1]<='z'))
		{
			if ((fsm==2) || (fsm==1) || (fsm==4))
			{
				start=ilv1;
				fsm=2;
			}
			else
			{
				if (fsm==1)//never executed -> obsolete
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
			if ((fsm!=2) && (fsm!=1) && (fsm!=4))
			{
				if (fsm==1)//never executed _>obsolete
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
	resortedstring[resortedstringpos]=0;
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
void printformatted(const char * iinput,int imode,int start,int end)
{
	if (imode==1)
	{
		fprintf(outfile,"<tspan dy=\"+3\" font-size=\"14\" style=\"fill:#%s\">",colorstring);
	}
	if ((imode==2) || (imode==0))
	{
		fprintf(outfile,"<tspan style=\"fill:#%s\">",colorstring);
	}
	if (imode==4)
	{
		fprintf(outfile,"<tspan dy=\"-3\" font-size=\"14\" style=\"fill:#%s\">",colorstring);
	}
	for (int ilv4=start;ilv4<end;ilv4++)
	{
		fprintf(outfile,"%c",iinput[ilv4]);
	}
	fprintf(outfile,"</tspan>",colorstring);
	if (imode==1)
	{
		fprintf(outfile,"<tspan dy=\"-3\">&#8288;</tspan>");
	}
	if (imode==4)
	{
		fprintf(outfile,"<tspan dy=\"3\">&#8288;</tspan>");
	}
	fprintf(outfile,"\n");
}

void svg_findaround()
{
	glob_color_multilist=retrievemultilist<color_instance>();
	glob_b_multilist=retrievemultilist<b_instance>();
	glob_n_multilist=retrievemultilist<n_instance>();
	glob_t_multilist=retrievemultilist<t_instance>();
	glob_s_multilist=retrievemultilist<s_instance>();
	glob_graphic_multilist=retrievemultilist<graphic_instance>();
	glob_annotation_multilist=retrievemultilist<annotation_instance>();
	glob_font_multilist=retrievemultilist<font_instance>();
	glob_curve_multilist=retrievemultilist<curve_instance>();
	getcaptions(&SVG_width,&SVG_height,&SVG_ileft,&SVG_itop);
	SVG_ileft-=10;
	SVG_itop-=10;
	SVG_width+=20;
	SVG_height+=20;
}

void expressbezier(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4)
{
	fprintf(outfile,"<path d=\"M %f %f C %f %f %f %f %f %f \" %s/>\n",x1-SVG_currentbasex,y1-SVG_currentbasey,x2-SVG_currentbasex,y2-SVG_currentbasey,x3-SVG_currentbasex,y3-SVG_currentbasey,x4-SVG_currentbasex,y4-SVG_currentbasey,stylestring);
}

void drawarrheads(cdx_Rectangle iBBX,float langle,float cangle,float otherlangle,float othercangle,int currentArrowHeadType,int currentArrowHeadTail,int currentArrowHeadHead,float tllinedist)
{
	for (int tlarrowside=-1;tlarrowside<2;tlarrowside+=2)
	{
		int tlarrowHead=(tlarrowside==-1) ? currentArrowHeadTail : currentArrowHeadHead;
		float tlArrowTopx=(tlarrowside==-1) ? iBBX.right : iBBX.left;
		float tlArrowTopy=(tlarrowside==-1) ? iBBX.bottom : iBBX.top;
		float tlcangle,tllangle;
		tllangle=(tlarrowside==-1) ? otherlangle : langle;
		tlcangle=(tlarrowside==-1) ? othercangle : cangle;
		switch (tlarrowHead)
		{
			case 0 :
			{
				if (currentArrowHeadType==1)
				{
					expressline(tlArrowTopx+tllinedist*cos(tlcangle),tlArrowTopy+tllinedist*sin(tlcangle),tlArrowTopx-tllinedist*cos(tlcangle),tlArrowTopy-tllinedist*sin(tlcangle));
				}
				break;
			}
			case 1 : 
			{
				stylegenestring(3);
				if (currentArrowHeadType==0)
				{
					expresstetrangle(tlArrowTopx,tlArrowTopy,
tlArrowTopx+cos(tllangle)*arrowheadlength+cos(tlcangle)*arrowthickness,tlArrowTopy+sin(tllangle)*arrowheadlength+sin(tlcangle)*arrowthickness,
tlArrowTopx+cos(tllangle)*arrowheadlength-cos(tlcangle)*arrowthickness,tlArrowTopy+sin(tllangle)*arrowheadlength-sin(tlcangle)*arrowthickness,
tlArrowTopx,tlArrowTopy,
stylestring);
				}
				if ((currentArrowHeadType==2) || (currentArrowHeadType==1))
				{
					expressline(tlArrowTopx,tlArrowTopy,
tlArrowTopx+cos(tllangle)*tllinedist*2+cos(tlcangle)*tllinedist*2,tlArrowTopy+sin(tllangle)*tllinedist*2+sin(tlcangle)*tllinedist*2);
					expressline(tlArrowTopx,tlArrowTopy,
tlArrowTopx+cos(tllangle)*tllinedist*2-cos(tlcangle)*tllinedist*2,tlArrowTopy+sin(tllangle)*tllinedist*2-sin(tlcangle)*tllinedist*2);

				}
				if (currentArrowHeadType==1)
				{
					expressline(tlArrowTopx+cos(tllangle)*tllinedist*2+cos(tlcangle)*tllinedist,tlArrowTopy+sin(tllangle)*tllinedist*2+sin(tlcangle)*tllinedist,
tlArrowTopx+cos(tllangle)*tllinedist*2+cos(tlcangle)*tllinedist*2,tlArrowTopy+sin(tllangle)*tllinedist*2+sin(tlcangle)*tllinedist*2);
					expressline(tlArrowTopx+cos(tllangle)*tllinedist*2-cos(tlcangle)*tllinedist,tlArrowTopy+sin(tllangle)*tllinedist*2-sin(tlcangle)*tllinedist,
tlArrowTopx+cos(tllangle)*tllinedist*2-cos(tlcangle)*tllinedist*2,tlArrowTopy+sin(tllangle)*tllinedist*2-sin(tlcangle)*tllinedist*2);
				}
				break;
			}
			case 3 :
			{
				tllangle=tllangle+Pi;
			}
			case 2 :
			{
				stylegenestring(3);
				if (currentArrowHeadType==0)
				{
					expresstetrangle(tlArrowTopx,tlArrowTopy,
tlArrowTopx+cos(tllangle)*arrowheadlength+cos(tlcangle)*arrowthickness,tlArrowTopy+sin(tllangle)*arrowheadlength+sin(tlcangle)*arrowthickness,
tlArrowTopx+cos(tllangle)*arrowheadlength,tlArrowTopy+sin(tllangle)*arrowheadlength,
tlArrowTopx,tlArrowTopy,
stylestring);
				}
				if (currentArrowHeadType==55)
				{
					expresstetrangle(tlArrowTopx+tllinedist*cos(tlcangle),tlArrowTopy+tllinedist*sin(tlcangle),
tlArrowTopx+cos(tllangle)*arrowheadlength+(arrowthickness+tllinedist)*cos(tlcangle),tlArrowTopy+sin(tllangle)*arrowheadlength+(arrowthickness+tllinedist)*sin(tlcangle),
tlArrowTopx+cos(tllangle)*arrowheadlength+tllinedist*cos(tlcangle),tlArrowTopy+sin(tllangle)*arrowheadlength+tllinedist*sin(tlcangle),
tlArrowTopx+tllinedist*cos(tlcangle),tlArrowTopy+tllinedist*sin(tlcangle),
stylestring);
				}
				break;
			}
		}
	}
}

void svg_main(const char * filename)
{
	float cangle;
	float langle;
	float ibonddist;
	float ibonddist2;
	float ibonddist3;
	float ibonddist4;
	int colornr;
	int fontnr;
	char * finalstring;
	cdx_Rectangle iBBX;
	_small owner;
	float othercangle;
	float otherlangle;
	outfile=fopen(filename,"w+");

	svg_findaround();
	getatoms();
	fprintf(outfile,"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>");
	fprintf(outfile,"<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">");
//	fprintf(outfile,"<svg version=\"1.0\" width=\"%f\" height=\"%f\">\n",SVG_width-SVG_ileft,SVG_height-SVG_itop);
	fprintf(outfile,"<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"%f\" height=\"%f\">\n",SVG_width-SVG_ileft,SVG_height-SVG_itop);
	SVG_currentbasex=SVG_ileft;
	SVG_currentbasey=SVG_itop;
	n_instance * startnode, * endnode;
	multilist<graphic_instance> * i_graphic_multilist=retrievemultilist<graphic_instance>();
	initZlist();
	for (int ilv1=0;ilv1<(*glob_curve_multilist).filllevel;ilv1++)
	{
		float tllinedist=0;//TODO****
		curve_instance * i_curve_instance=(curve_instance*)&((*glob_curve_multilist).bufferlist[ilv1]);
		colornr=(*i_curve_instance).color;
		get_colorstring(colornr);
		stylegenestring(1|(((*i_curve_instance).CurveType & 128)?2:0)|(((*i_curve_instance).CurveType & 4)?4:0));
		int tllast=1;
		for (int ilv2=1;ilv2<(*i_curve_instance).CurvePoints.count-3;ilv2+=3)
		{
			expressbezier((*i_curve_instance).CurvePoints.a[ilv2].x,(*i_curve_instance).CurvePoints.a[ilv2].y,(*i_curve_instance).CurvePoints.a[ilv2+1].x,(*i_curve_instance).CurvePoints.a[ilv2+1].y,(*i_curve_instance).CurvePoints.a[ilv2+2].x,(*i_curve_instance).CurvePoints.a[ilv2+2].y,(*i_curve_instance).CurvePoints.a[ilv2+3].x,(*i_curve_instance).CurvePoints.a[ilv2+3].y);
			tllast=ilv2+3;
		}
		if ((*i_curve_instance).Closed)
		{
			expressbezier((*i_curve_instance).CurvePoints.a[tllast].x,(*i_curve_instance).CurvePoints.a[tllast].y,(*i_curve_instance).CurvePoints.a[tllast+1].x,(*i_curve_instance).CurvePoints.a[tllast+1].y,(*i_curve_instance).CurvePoints.a[0].x,(*i_curve_instance).CurvePoints.a[0].y,(*i_curve_instance).CurvePoints.a[1].x,(*i_curve_instance).CurvePoints.a[1].y);
		}
		else
		{
			int currentArrowHeadType=0;
			int currentArrowHeadTail=0;
			int currentArrowHeadHead=0;
			float tllinedist=0;
			float tllefttan=0;
			float tlrighttan=0;
			float tllefttan2=0;
			float tlrighttan2=0;
			langle=0;cangle=0;
			int currentLineType=0;//0: normal 5: Bold 0x100: Double
			currentArrowHeadType=(*i_curve_instance).ArrowheadType;
			currentArrowHeadHead=(*i_curve_instance).ArrowheadHead;
			currentArrowHeadTail=(*i_curve_instance).ArrowheadTail;
			iBBX.left=(*i_curve_instance).CurvePoints.a[1].x;
			iBBX.top=(*i_curve_instance).CurvePoints.a[1].y;

			iBBX.right=(*i_curve_instance).CurvePoints.a[(*i_curve_instance).CurvePoints.count-2].x;
			iBBX.bottom=(*i_curve_instance).CurvePoints.a[(*i_curve_instance).CurvePoints.count-2].y;
			langle=getangle((*i_curve_instance).CurvePoints.a[(*i_curve_instance).CurvePoints.count-1].x-(*i_curve_instance).CurvePoints.a[(*i_curve_instance).CurvePoints.count-2].x,(*i_curve_instance).CurvePoints.a[(*i_curve_instance).CurvePoints.count-1].y-(*i_curve_instance).CurvePoints.a[(*i_curve_instance).CurvePoints.count-2].y);
			otherlangle=getangle((*i_curve_instance).CurvePoints.a[0].x-(*i_curve_instance).CurvePoints.a[1].x,(*i_curve_instance).CurvePoints.a[0].y-(*i_curve_instance).CurvePoints.a[1].y);
			cangle=langle+Pi/2;
			othercangle=otherlangle+Pi/2;
			drawarrheads(iBBX,langle,cangle,otherlangle,othercangle,currentArrowHeadType,currentArrowHeadTail,currentArrowHeadHead,tllinedist);
		}
	}
	for (int ilv1=0;ilv1<(*i_graphic_multilist).filllevel;ilv1++)
	{
/* HOW ARROW MUST BE DONE:
1. The arrow type determiner determines arrow type and bond spacing.
2. The lines/arc are drawn. if it was something else, the next two points are skipped. 
3. Otherwise, the arrow angles are calculated
4. And the arrows are drawn.*/
		graphic_instance * i_graphic_instance=&((*i_graphic_multilist).bufferlist)[ilv1];
		iBBX=(*i_graphic_instance).BoundingBox;
		colornr=(*i_graphic_instance).color;
		get_colorstring(colornr);
		int currentArrowHeadType=0;
		int currentArrowHeadTail=0;
		int currentArrowHeadHead=0;
		float tllinedist=0;
		float tllefttan=0;
		float tlrighttan=0;
		float tllefttan2=0;
		float tlrighttan2=0;
		langle=0;cangle=0;
		int currentLineType=0;//0: normal 5: Bold 0x100: Double
		if ((*i_graphic_instance).ArrowType & 1)
		{
			currentArrowHeadHead=2;
		}
		if ((*i_graphic_instance).ArrowType & 2)
		{
			currentArrowHeadHead=1;
		}
		if ((*i_graphic_instance).ArrowType & 4)
		{
			currentArrowHeadHead=1;
			currentArrowHeadTail=1;
		}
		if ((*i_graphic_instance).ArrowType & 8)
		{
			currentLineType=0x100;
			tllinedist=4;
			currentArrowHeadType=55;
			currentArrowHeadHead=2;
			currentArrowHeadTail=2;
		}
		if ((*i_graphic_instance).ArrowType & 48)
		{
			tllinedist=8;
			currentArrowHeadHead=1;//only valid when called from graphic
			if ((*i_graphic_instance).ArrowType & 16) {currentArrowHeadType=1;if (currentArrowHeadHead&1){tllefttan2=2;}if (currentArrowHeadHead&2){tlrighttan2=2;}if (currentArrowHeadTail&1){tllefttan=2;}if (currentArrowHeadTail&2){tlrighttan=2;}}
			if ((*i_graphic_instance).ArrowType & 32) {currentArrowHeadType=2;if (currentArrowHeadHead&1){tllefttan2=1;}if (currentArrowHeadHead&2){tlrighttan2=1;}if (currentArrowHeadTail&1){tllefttan=1;}if (currentArrowHeadTail&2){tlrighttan=1;}}
			currentLineType=0x100;
		}
		if ((*i_graphic_instance).GraphicType==1)
		{
			stylegenestring(stylefromline((*i_graphic_instance).LineType));
			cangle=getangle(iBBX.right-iBBX.left,iBBX.bottom-iBBX.top)+Pi/2;
			langle=getangle(iBBX.right-iBBX.left,iBBX.bottom-iBBX.top);
			if (currentLineType &0x100)
			{
				expressline(iBBX.left+tllinedist*(cos(cangle)+(cos(langle)*tllefttan2)),iBBX.top+tllinedist*(sin(cangle)+(sin(langle)*tllefttan2)),iBBX.right+tllinedist*(cos(cangle)-(cos(langle)*tllefttan)),iBBX.bottom+tllinedist*(sin(cangle)-(sin(langle)*tllefttan)));
				expressline(iBBX.left-tllinedist*(cos(cangle)-(cos(langle)*tllefttan2)),iBBX.top-tllinedist*(sin(cangle)-(sin(langle)*tllefttan2)),iBBX.right-tllinedist*(cos(cangle)+(cos(langle)*tllefttan)),iBBX.bottom-tllinedist*(sin(cangle)+(sin(langle)*tllefttan)));
			}
			else
			{
				expressline(iBBX.left,iBBX.top,iBBX.right,iBBX.bottom);
			}
			othercangle=cangle+Pi;
			otherlangle=langle+Pi;
		}
		else
		if ((*i_graphic_instance).GraphicType==2)
		{
			float deltax,deltay;
			deltax=iBBX.left-iBBX.right;
			deltay=iBBX.top-iBBX.bottom;
			float tlradius=sqrt(deltax*deltax+deltay*deltay);
			float tlangle=getangle(deltax,deltay);
			if ((*i_graphic_instance).AngularSize>0)
			{
				 langle=(tlangle+Pi/2.0);
			}
			else
			{
				 langle=(tlangle-Pi/2.0);
			}
			cangle=langle+Pi/2.0;
			stylegenestring(stylefromline((*i_graphic_instance).LineType));
			if (currentLineType &0x100)
			{
//TODO****				expressarc(iBBX.right,iBBX.bottom
			}
			else
			expressarc(iBBX.right,iBBX.bottom,tlradius,tlradius,tlangle,tlangle+(((*i_graphic_instance).AngularSize/180.0)*Pi));
			otherlangle=langle+(((*i_graphic_instance).AngularSize/180.0)*Pi)+Pi;
			othercangle=otherlangle+Pi/2.0;
			iBBX.right+=tlradius*cos(tlangle+(((*i_graphic_instance).AngularSize/180.0)*Pi));
			iBBX.bottom+=tlradius*sin(tlangle+(((*i_graphic_instance).AngularSize/180.0)*Pi));
			if (tlradius>arrowheadlength/2)
			{
				float dturn=asin(arrowheadlength/tlradius/2);
				if ((*i_graphic_instance).AngularSize>0)
				{
					langle+=dturn;
					cangle+=dturn;
					otherlangle-=dturn;
					othercangle-=dturn;
				}
				else
				{
					langle-=dturn;
					cangle-=dturn;
					otherlangle+=dturn;
					othercangle+=dturn;
				}
			}
		}
		else goto skiparrows;
		drawarrheads(iBBX,langle,cangle,otherlangle,othercangle,currentArrowHeadType,currentArrowHeadTail,currentArrowHeadHead,tllinedist);
		skiparrows:
		if ((*i_graphic_instance).GraphicType==3)
		{
			int tlisfound;
			multilistreference<annotation_instance> * tlannotationmultilistreference=dynamic_cast<multilistreference<annotation_instance>*>((*glob_graphic_multilist).bufferlist[ilv1].annotation);
			tlisfound=0;
			for (int ilv2=0;ilv2<(*tlannotationmultilistreference).count_in_it;ilv2++)
			{
				annotation_instance * tlannotation=&((*glob_annotation_multilist).bufferlist[(*tlannotationmultilistreference).start_in_it+ilv2]);
				if (strcmp((*tlannotation).Keyword.a,"Name")==0)
				{
					if ((strncmp((*tlannotation).Content.a,"Frame",5))==0)
					{
						tlisfound=1;
					}
				}
			}
			if (tlisfound==1)
			{
				goto skipthisgraphic;
			}
			stylegenestring(stylefromrectangle((*i_graphic_instance).RectangleType));
			expresstetrangle(iBBX.left,iBBX.top,iBBX.right,iBBX.top,iBBX.right,iBBX.bottom,iBBX.left,iBBX.bottom,stylestring);
		}
		if ((*i_graphic_instance).GraphicType==4)
		{
			stylegenestring(1);
			if ((*i_graphic_instance).OvalType==1)
			{
				float tldistance=sqrt((iBBX.right-iBBX.left)*(iBBX.right-iBBX.left)+(iBBX.bottom-iBBX.top)*(iBBX.bottom-iBBX.top));
				iBBX.left-=tldistance;
				iBBX.right=iBBX.left+tldistance;
				iBBX.bottom=iBBX.top+tldistance;
				(*i_graphic_instance).BoundingBox.right=iBBX.right;
				(*i_graphic_instance).BoundingBox.bottom=iBBX.bottom;
			}
			expresscircle(iBBX.left,iBBX.top,iBBX.right,iBBX.bottom);
		}
		if ((*i_graphic_instance).GraphicType==7)
		{
			stylegenestring(1);
			switch((*i_graphic_instance).SymbolType)
			{
				case 4 : strcpy(colorstring2,"FF0000");goto charge;break;
				case 5 : strcpy(colorstring2,"0000FF");goto charge;break;
				case 1 : strcpy(colorstring2,colorstring);goto radical;break;
			}
			charge:
			fprintf(outfile,"<circle cx=\"%f\" cy=\"%f\" r=\"6\" stroke=\"#%s\" fill=\"#%s\"/>",iBBX.left-SVG_currentbasex,iBBX.top-SVG_currentbasey,colorstring,colorstring2);
			expressline(iBBX.left-3,iBBX.top,iBBX.left+3,iBBX.top);
			if ((*i_graphic_instance).SymbolType==4)
			{
				expressline(iBBX.left,iBBX.top-3,iBBX.left,iBBX.top+3);
			}
			goto GraphicType7done;
			radical:
			fprintf(outfile,"<circle cx=\"%f\" cy=\"%f\" r=\"4\" stroke=\"#%s\" fill=\"#%s\"/>",iBBX.left-SVG_currentbasex,iBBX.top-SVG_currentbasey,colorstring,colorstring2);
			GraphicType7done:
			;
		}
		if ((*i_graphic_instance).GraphicType==6)
		{
			stylegenestring(1);
			cangle=getangle(iBBX.right-iBBX.left,iBBX.bottom-iBBX.top)+Pi/2;
			langle=getangle(iBBX.right-iBBX.left,iBBX.bottom-iBBX.top);
			expressline(iBBX.left-6.7*cos(cangle),iBBX.top-6.7*sin(cangle),iBBX.right-6.7*cos(cangle),iBBX.bottom-6.7*sin(cangle));
			expressline(iBBX.left,iBBX.top,iBBX.left-6.7*cos(cangle),iBBX.top-6.7*sin(cangle));
			expressline(iBBX.right,iBBX.bottom,iBBX.right-6.7*cos(cangle),iBBX.bottom-6.7*sin(cangle));
		}
		skipthisgraphic:
		;
	}
	for (int ilv1=0;ilv1<(*glob_b_multilist).filllevel;ilv1++)
	{
		_small inr_E,inr_S;
		b_instance * i_b_instance=&(((*glob_b_multilist).bufferlist)[ilv1]);
		colornr=(*i_b_instance).color;
		get_colorstring(colornr);
		for (int ilv2=0;ilv2<(*glob_n_multilist).filllevel;ilv2++)//TODO: base on pre-calcd. values
		{
			if (((*glob_n_multilist).bufferlist)[ilv2].id==(*i_b_instance).E)
			{
				inr_E=ilv2;
				endnode=&(((*glob_n_multilist).bufferlist)[ilv2]);
			}
			if (((*glob_n_multilist).bufferlist)[ilv2].id==(*i_b_instance).B)
			{
				inr_S=ilv2;
				startnode=&(((*glob_n_multilist).bufferlist)[ilv2]);
			}
		}
		ibonddist=0;ibonddist2=0;
		if ((*i_b_instance).Order>1.1)//floats, after all....
		{
			switch((*i_b_instance).DoublePosition & 0xFF)
			{
				case 0 : ibonddist=bonddist/2;ibonddist2=-bonddist/2;break;
				case 1 : ibonddist=bonddist;break;
				case 2 : ibonddist=-bonddist;break;
			}
		}
		langle=getangle((*endnode).p.x-(*startnode).p.x,(*endnode).p.y-(*startnode).p.y);
		cangle=langle+Pi/2;
		float textdeltax,textdeltay;
		calcdelta(&textdeltax,&textdeltay,(*endnode).p.x-(*startnode).p.x,(*endnode).p.y-(*startnode).p.y);
		char specialS=(atom_actual_node[inr_S].special!=-1);
		char specialE=(atom_actual_node[inr_E].special!=-1);
		iBBX.left=(*startnode).p.x+textdeltax*specialS;
		iBBX.top=(*startnode).p.y+textdeltay*specialS;
		int iDisplaytype1=((*glob_b_multilist).bufferlist[ilv1].Display);
		iBBX.right=(*endnode).p.x-textdeltax*specialE;
		iBBX.bottom=(*endnode).p.y-textdeltay*specialE;
		if (iswedgenr(iDisplaytype1)>0)
		{
			ibonddist3=0;
			ibonddist4=0;
			if (iswedgenr(iDisplaytype1) & 2)
			{
				ibonddist3=boldwidth/2;
			}
			if (iswedgenr(iDisplaytype1) & 1)
			{
				ibonddist4=boldwidth/2;
			}
			stylegenestring(
(((iDisplaytype1==5) || (iDisplaytype1==6) || (iDisplaytype1==7))?2:0) |
1);
			float tllefttan=0;
			float tlrighttan=0;
			float tllefttan2=0;
			float tlrighttan2=0;
			if (specialE==0)
			{
				if ((bond_actual_node[ilv1]).numberleft[0]!=-1) {tllefttan=tan(Pi/2-bond_actual_node[ilv1].cotanleft[0]);}else{if ((bond_actual_node[ilv1]).numberright[0]!=-1){tllefttan=tan(bond_actual_node[ilv1].xcotanright[0]/2);}}
				if ((bond_actual_node[ilv1]).numberright[0]!=-1) {tlrighttan=tan(Pi/2-bond_actual_node[ilv1].cotanright[0]);}else{if ((bond_actual_node[ilv1]).numberleft[0]!=-1){tlrighttan=tan(bond_actual_node[ilv1].xcotanleft[0]/2);}}
			}
			if (specialS==0)
			{
				if ((bond_actual_node[ilv1]).numberleft[1]!=-1) {tllefttan2=tan(Pi/2-bond_actual_node[ilv1].cotanleft[1]);}else{if ((bond_actual_node[ilv1]).numberright[1]!=-1){tllefttan2=tan(bond_actual_node[ilv1].xcotanright[1]/2);}}
				if ((bond_actual_node[ilv1]).numberright[1]!=-1) {tlrighttan2=tan(Pi/2-bond_actual_node[ilv1].cotanright[1]);}else{if ((bond_actual_node[ilv1]).numberleft[1]!=-1){tlrighttan2=tan(bond_actual_node[ilv1].xcotanleft[1]/2);}}
			}
			if ((iDisplaytype1==2) || (iDisplaytype1==3) || (iDisplaytype1==4))
			{
				expresshashangle(langle,cangle,
				iBBX.right+ibonddist2*cos(cangle),iBBX.bottom+ibonddist2*sin(cangle),
				iBBX.left+ibonddist2*cos(cangle),iBBX.top+ibonddist2*sin(cangle),
				ibonddist3,ibonddist4
				);
			}
			else
			{
				expresshexangle(
iBBX.right+ibonddist2*cos(cangle),iBBX.bottom+ibonddist2*sin(cangle),
iBBX.right+ibonddist2*cos(cangle)+ibonddist4*(-cos(cangle)-(cos(langle)*tllefttan)),iBBX.bottom+ibonddist2*sin(cangle)+ibonddist4*(-sin(cangle)-(sin(langle)*tllefttan)),
iBBX.left+ibonddist2*cos(cangle)+ibonddist3*(-cos(cangle)+(cos(langle)*tlrighttan2)),iBBX.top+ibonddist2*sin(cangle)+ibonddist3*(-sin(cangle)+(sin(langle)*tlrighttan2)),
iBBX.left+ibonddist2*cos(cangle),iBBX.top+ibonddist2*sin(cangle),
iBBX.left+ibonddist2*cos(cangle)+ibonddist3*(cos(cangle)+(cos(langle)*tllefttan2)),iBBX.top+ibonddist2*sin(cangle)+ibonddist3*(sin(cangle)+(sin(langle)*tllefttan2)),
iBBX.right+ibonddist2*cos(cangle)+ibonddist4*(cos(cangle)-(cos(langle)*tlrighttan)),iBBX.bottom+ibonddist2*sin(cangle)+ibonddist4*(sin(cangle)-(sin(langle)*tlrighttan))
				);
				ibonddist+=abs(bonddist)*((ibonddist>0) ? 1 : -1);
			}
		}
		else
		{
			stylegenestring(((iDisplaytype1==1) ?8:0) | 1);
			expressline(iBBX.left+ibonddist2*cos(cangle),iBBX.top+ibonddist2*sin(cangle),iBBX.right+ibonddist2*cos(cangle),iBBX.bottom+ibonddist2*sin(cangle));
		}
		if ((*i_b_instance).Order>1.1)
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
		get_colorstring(colornr);
		if (owner!=-1)
		{
			colornr=(*glob_n_multilist).bufferlist[text_actual_node[ilv1].owner].color;
			get_colorstring(colornr);
		}
		colornr=((*i_t_multilist).bufferlist)[ilv1].color;
		get_colorstring_passive(colornr);

		fprintf(outfile,"<text fill=\"%s\" %s stroke=\"none\" transform=\"translate(%f,%f)\" font-size=\"%f\">",colorstring,((*i_t_multilist).bufferlist[ilv1].LabelAlignment==-1) ? "text-anchor=\"end\" text-align=\"end\"" : "",((*i_t_multilist).bufferlist)[ilv1].p.x-SVG_currentbasex,((*i_t_multilist).bufferlist)[ilv1].p.y-SVG_currentbasey,atomfontheight);
		intl start,end;
		start=(*(((*i_t_multilist).bufferlist)[ilv1].s)).start_in_it;
		end=start+(*(((*i_t_multilist).bufferlist)[ilv1].s)).count_in_it;
		char string_resorted=0;
		char ifsmat=0;//0: nothing //1: on a subscript number; 2: on text; 3: on a superscript
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
					if (((*i_s_multilist).bufferlist[start].face & 0x60)==0x60)
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
			if ((((*i_s_multilist).bufferlist)[ilv2].face & 0x60)==0x60)
			{
				printf("Active:%s",finalstring);
				int tlmax=strlen(finalstring);
				int tlstart,tlend;
				tlstart=0;tlend=0;
				for (int ilv3=0;ilv3<tlmax;ilv3++)
				{
					if ((finalstring[ilv3]>='0') && (finalstring[ilv3]<='9'))
					{
						if (ifsmat==2)
						{
							printformatted(finalstring,ifsmat,tlstart,tlend);
							tlstart=ilv3;
							tlend=ilv3+1;
							ifsmat=1;
						}
						else
						{
							if (ifsmat==0)
							{
								tlend=ilv3+1;
								goto trivial;
							}
							if (ifsmat==1)
							{
								tlend=ilv3+1;
								goto trivial;
							}
							if (ifsmat==4)
							{
								printformatted(finalstring,ifsmat,tlstart,tlend);
								tlstart=ilv3;
								tlend=ilv3+1;
								ifsmat=0;
								goto trivial;
							}
						}
					}
					else
					{
						if (sentenumeric(finalstring[ilv3]))
						{
							treatasbookstave:
							if (ifsmat==0)
							{
								tlend=ilv3+1;
								ifsmat=2;
								goto trivial;
							}
							if (ifsmat==1)
							{
								printformatted(finalstring,ifsmat,tlstart,tlend);
								tlstart=ilv3;
								tlend=ilv3+1;
								ifsmat=2;
								goto trivial;
							}
							if (ifsmat==2)
							{
								tlend=ilv3+1;
								goto trivial;
							}
							if (ifsmat==4)
							{
								printformatted(finalstring,ifsmat,tlstart,tlend);
								tlstart=ilv3;
								tlend=ilv3+1;
								ifsmat=2;
								goto trivial;
							}
						}
						else
						{
							if ((finalstring[ilv3]=='+') || (finalstring[ilv3]=='-'))
							{
								if (ifsmat==0)
								{
									tlend=ilv3+1;
									goto trivial;
								}
								if (ifsmat==4)
								{
									tlend=ilv3+1;
									goto trivial;
								}
								if (ifsmat==1)
								{
									printformatted(finalstring,ifsmat,tlstart,tlend);
									tlstart=ilv3;
									tlend=ilv3+1;
									ifsmat=4;
									goto trivial;
								}
								if (ifsmat==2)
								{
									printformatted(finalstring,ifsmat,tlstart,tlend);
									tlstart=ilv3;
									tlend=ilv3+1;
									ifsmat=4;
									goto trivial;
								}
							}
							else
							{
								if (spaciatic(finalstring[ilv3]))
								{
									treatasspace:
									if (ifsmat==0)
									{
										tlend=ilv3+1;
										goto trivial;
									}
									if (ifsmat==1)
									{
										printformatted(finalstring,ifsmat,tlstart,tlend);
										tlstart=ilv3;
										tlend=ilv3+1;
										ifsmat=0;
										goto trivial;
									}
									if (ifsmat==2)
									{
										tlend=ilv3+1;
										ifsmat=0;
										goto trivial;
									}
									if (ifsmat==4)
									{
										printformatted(finalstring,ifsmat,tlstart,tlend);
										tlstart=ilv3;
										tlend=ilv3+1;
										ifsmat=0;
										goto trivial;
									}
								}
								else
								{
									if (finalstring[ilv3] & 0x80)
									{
										goto treatasbookstave;
									}
									goto treatasspace;
								}
							}
						}
					}
					trivial:
					;
				}
				printformatted(finalstring,ifsmat,tlstart,tlend);
			}
			else
			{
				printf("Passive:%s",finalstring);
				int tlformlabeltype=((*i_s_multilist).bufferlist)[ilv2].face;
				fprintf(outfile,"<tspan %s style=\"fill:#%s\">%s</tspan>%s\n",
				(tlformlabeltype & 0x20) ? "dy=\"+3\" font-size=\"14\"" : ((tlformlabeltype & 0x40) ? "dy=\"-3\" font-size=\"14%\"":""),
				colorstring,finalstring,(tlformlabeltype & 0x20)?"<tspan dy=\"-3\"/>":((tlformlabeltype & 0x40)?"<tspan dy=\"3\"/>":""));
			}
		}
		fprintf(outfile,"</text>\n");
	}
//	for (int ilv1=0;ilv1<(*i_
	fprintf(outfile,"</svg>");
	fclose(outfile);
}
