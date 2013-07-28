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
float arrowheadlength=15;
float arrowthickness=5;

multilist<color_instance> * glob_color_multilist;
multilist<n_instance> * glob_n_multilist;
multilist<b_instance> * glob_b_multilist;
multilist<t_instance> * glob_t_multilist;
multilist<s_instance> * glob_s_multilist;
multilist<font_instance> * glob_font_multilist;
char colorstring[7]="AABBCC";
char colorstring2[7]="AABBCC";
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
	float cotanleft[2],cotanright[2];//fw,bw==end,start NOTE: when [1], right and left are also seen from the other side.
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
		bond_actual_node[ilv1].cotanleft[0]=Pi/2;
		bond_actual_node[ilv1].cotanleft[1]=Pi/2;
		bond_actual_node[ilv1].cotanright[0]=-Pi/2;
		bond_actual_node[ilv1].cotanright[1]=-Pi/2;
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
			if (ilv1==65)
			{
				if (((atom_actual_node[ilv1]).bonds[ilv2]==57) || ((atom_actual_node[ilv1]).bonds[ilv2]==58))
				{
					printf("??%f;%i??",angle_between[ilv2][ilv2],(atom_actual_node[ilv1]).bonds[ilv2]);
					printf(">>>>%f",compangle(5.76,1.32));
				}
			}
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
			int tlleftnr,tlrightnr;
			tlleftnr=0;tlrightnr=0;
			tlleftest=Pi;tlrightest=-Pi;
			for (int ilv3=0;ilv3<atom_actual_node[ilv1].bondcount;ilv3++)
			{
				if (ilv3!=ilv2)
				{
					float iangle=angle_between[ilv2][ilv3];
					if ((atom_actual_node[ilv1].bonds[ilv2]==57) && (ilv1==65))
					{
						printf("!!%f,%i,%i!!",iangle,atom_actual_node[ilv1].bonds[ilv2],atom_actual_node[ilv1].bonds[ilv3]);
					}
					if (fabs(iangle)<Pi/6){goto notfound;}
					if (fabs(iangle)>5*Pi/6){goto notfound;}
					if (iangle>0) if (iangle<tlleftest){tlleftest=iangle;tlleftnr=number_between[ilv3];}
					if (iangle<0) if (iangle>tlrightest){tlrightest=iangle;tlrightnr=number_between[ilv3];}
					notfound:
					;
				}
			}
//{printf("Achtung:%i;%i;%f;",(*i_bond_actual_node).start,ilv1,tlleftest);tlleftest=Pi/4;tlleftnr=0;}
			//if the atom is a start atom, the bond has to be treated backwards
			if (tlleftest<Pi)
			{
				(*i_bond_actual_node).cotanleft[(*i_bond_actual_node).start==ilv1]=tlleftest;
				(*i_bond_actual_node).numberleft[(*i_bond_actual_node).start==ilv1]=tlleftnr;
			}
			if (tlrightest>-Pi)
			{
				(*i_bond_actual_node).cotanright[(*i_bond_actual_node).start==ilv1]=-tlrightest;
				(*i_bond_actual_node).numberright[(*i_bond_actual_node).start==ilv1]=tlrightnr;
			}
		}
		for (int ilv2=(*((*glob_n_multilist).bufferlist)[ilv1].t).start_in_it;ilv2<(*((*glob_n_multilist).bufferlist)[ilv1].t).start_in_it+(*((*glob_n_multilist).bufferlist)[ilv1].t).count_in_it;ilv2++)//allows for multiple text items on one atom. Nonsense.
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
void expresshexangle(float ix1,float iy1,float ix2,float iy2,float ix3,float iy3,float ix4,float iy4,float ix5,float iy5,float ix6,float iy6)
{
	fprintf(outfile,"<path d=\"M %f %f L %f %f L %f %f L %f %f L %f %f L %f %f z \" %s/>\n",
        ix1-currentbasex,iy1-currentbasey,ix2-currentbasex,iy2-currentbasey,ix3-currentbasex,iy3-currentbasey,ix4-currentbasex,iy4-currentbasey,ix5-currentbasex,iy5-currentbasey,ix6-currentbasex,iy6-currentbasey,
stylestring);
}
void expressarc(float centerx,float centery,float radiusx,float radiusy,float startangle,float endangle)
{
	float startx,starty;
	float endx,endy;
	startx=centerx+radiusx*cos(startangle)-currentbasex;
	starty=centery+radiusy*sin(startangle)-currentbasey;
	endx=centerx+radiusx*cos(endangle)-currentbasex;
	endy=centery+radiusy*sin(endangle)-currentbasey;
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
		fprintf(outfile,"<tspan dy=\"+3\" font-size=\"80%\" style=\"fill:#%s\">",colorstring);
	}
	if ((imode==2) || (imode==0))
	{
		fprintf(outfile,"<tspan style=\"fill:#%s\">",colorstring);
	}
	if (imode==4)
	{
		fprintf(outfile,"<tspan dy=\"-3\" font-size=\"80%\" style=\"fill:#%s\">",colorstring);
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
		graphic_instance * i_graphic_instance=&((*i_graphic_multilist).bufferlist)[ilv1];
		iBBX=(*i_graphic_instance).BoundingBox;
		colornr=(*i_graphic_instance).color;
		get_colorstring(colornr);
		if ((*i_graphic_instance).GraphicType==1)
		{
			stylegenestring(1);
			expressline(iBBX.left,iBBX.top,iBBX.right,iBBX.bottom);
			cangle=getangle(iBBX.right-iBBX.left,iBBX.top-iBBX.bottom)+Pi/2;
			langle=getangle(iBBX.right-iBBX.left,iBBX.top-iBBX.bottom);
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
			stylegenestring(1);
			expressarc(iBBX.right,iBBX.bottom,tlradius,tlradius,tlangle,tlangle+(((*i_graphic_instance).AngularSize/180.0)*Pi));
			stylegenestring(3);
			expresstetrangle(iBBX.left,iBBX.top,
iBBX.left+cos(langle)*arrowheadlength+cos(cangle)*arrowthickness,iBBX.top+sin(langle)*arrowheadlength+sin(cangle)*arrowthickness,
iBBX.left+cos(langle)*arrowheadlength-cos(cangle)*arrowthickness,iBBX.top+sin(langle)*arrowheadlength-sin(cangle)*arrowthickness,
iBBX.left,iBBX.top,
stylestring);
		}
		else goto skiparrows;
		if ((*i_graphic_instance).ArrowType==2)
		{
			stylegenestring(3);
			expresstetrangle(iBBX.left,iBBX.top,
iBBX.left+cos(langle)*arrowheadlength+cos(cangle)*arrowthickness,iBBX.top+sin(langle)*arrowheadlength+sin(cangle)*arrowthickness,
iBBX.left+cos(langle)*arrowheadlength-cos(cangle)*arrowthickness,iBBX.top+sin(langle)*arrowheadlength-sin(cangle)*arrowthickness,
iBBX.left,iBBX.top,
stylestring);
		}
		skiparrows:
		if ((*i_graphic_instance).GraphicType==3)
		{
			stylegenestring(1);
			expressline(iBBX.left,iBBX.top,iBBX.right,iBBX.top);
			expressline(iBBX.left,iBBX.bottom,iBBX.right,iBBX.bottom);
			expressline(iBBX.left,iBBX.top,iBBX.left,iBBX.bottom);
			expressline(iBBX.right,iBBX.top,iBBX.right,iBBX.bottom);
		}
 		if ((*i_graphic_instance).GraphicType==7)
 		{
			stylegenestring(1);
			switch((*i_graphic_instance).SymbolType)
			{
				case 4 : strcpy(colorstring2,"FF0000");break;
				case 5 : strcpy(colorstring2,"0000FF");break;
			}
 			fprintf(outfile,"<circle cx=\"%f\" cy=\"%f\" r=\"6\" stroke=\"#%s\" fill=\"#%s\"/>",iBBX.left-currentbasex,iBBX.top-currentbasey,colorstring,colorstring2);
			expressline(iBBX.left-3,iBBX.top,iBBX.left+3,iBBX.top);
			if ((*i_graphic_instance).SymbolType==4)
			{
				expressline(iBBX.left,iBBX.top-3,iBBX.left,iBBX.top+3);
			}
 		}
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
		iBBX.left=(*startnode).p.x+textdeltax*(atom_actual_node[inr_S].special!=-1);
		iBBX.top=(*startnode).p.y+textdeltay*(atom_actual_node[inr_S].special!=-1);
		int iDisplaytype1=((*glob_b_multilist).bufferlist[ilv1].Display);
		iBBX.right=(*endnode).p.x-textdeltax*(atom_actual_node[inr_E].special!=-1);
		iBBX.bottom=(*endnode).p.y-textdeltay*(atom_actual_node[inr_E].special!=-1);
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
			if ((bond_actual_node[ilv1]).numberleft[0]!=-1) {tllefttan=tan(Pi/2-bond_actual_node[ilv1].cotanleft[0]);}
			if ((bond_actual_node[ilv1]).numberright[0]!=-1) {tlrighttan=tan(Pi/2-bond_actual_node[ilv1].cotanright[0]);}
			if ((bond_actual_node[ilv1]).numberleft[1]!=-1) {tllefttan2=tan(Pi/2-bond_actual_node[ilv1].cotanleft[1]);}
			if ((bond_actual_node[ilv1]).numberright[1]!=-1) {tlrighttan2=tan(Pi/2-bond_actual_node[ilv1].cotanright[1]);}
			expresshexangle(
			iBBX.right+ibonddist2*cos(cangle),iBBX.bottom+ibonddist2*sin(cangle),
iBBX.right+ibonddist2*cos(cangle)+ibonddist4*(-cos(cangle)-(cos(langle)*tllefttan)),iBBX.bottom+ibonddist2*sin(cangle)+ibonddist4*(-sin(cangle)-(sin(langle)*tllefttan)),
iBBX.left+ibonddist2*cos(cangle)+ibonddist3*(-cos(cangle)+(cos(langle)*tlrighttan2)),iBBX.top+ibonddist2*sin(cangle)+ibonddist3*(-sin(cangle)+(sin(langle)*tlrighttan2)),
			iBBX.left+ibonddist2*cos(cangle),iBBX.top+ibonddist2*sin(cangle),
iBBX.left+ibonddist2*cos(cangle)+ibonddist3*(cos(cangle)+(cos(langle)*tllefttan2)),iBBX.top+ibonddist2*sin(cangle)+ibonddist3*(sin(cangle)+(sin(langle)*tllefttan2)),
iBBX.right+ibonddist2*cos(cangle)+ibonddist4*(cos(cangle)-(cos(langle)*tlrighttan)),iBBX.bottom+ibonddist2*sin(cangle)+ibonddist4*(sin(cangle)-(sin(langle)*tlrighttan))
			);
/*			expresstetrangle(
iBBX.left+ibonddist3*cos(cangle),iBBX.top+ibonddist3*sin(cangle),
iBBX.left-ibonddist3*cos(cangle),iBBX.top-ibonddist3*sin(cangle),
iBBX.right+ibonddist4*cos(cangle),iBBX.bottom+ibonddist4*sin(cangle),
iBBX.right-ibonddist4*cos(cangle),iBBX.bottom-ibonddist4*sin(cangle),
stylestring);*/
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

		fprintf(outfile,"<text fill=\"%s\" %s stroke=\"none\" transform=\"translate(%f,%f)\" font-size=\"18\">",colorstring,((*i_t_multilist).bufferlist[ilv1].LabelAlignment==-1) ? "text-anchor=\"end\" text-align=\"end\"" : "",((*i_t_multilist).bufferlist)[ilv1].p.x-currentbasex,((*i_t_multilist).bufferlist)[ilv1].p.y-currentbasey);
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
				(tlformlabeltype & 0x20) ? "dy=\"+3\" font-size=\"80%\"" : ((tlformlabeltype & 0x40) ? "dy=\"-3\" font-size=\"80%\"":""),
				colorstring,finalstring,(tlformlabeltype & 0x20)?"<tspan dy=\"-3\"/>":((tlformlabeltype & 0x40)?"<tspan dy=\"3\"/>":""));
			}
		}
		fprintf(outfile,"</text>\n");
	}
	
//	for (int ilv1=0;ilv1<(*i_
	fprintf(outfile,"</svg>");
	fclose(outfile);
}
