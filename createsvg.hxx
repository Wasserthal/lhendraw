#include <math.h>
float Pi=3.141592654;
#define minfloat -1.0e20
#define maxfloat 1.0e20
#define _small int
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
float arrowdepth=10;
float arrowthickness=10;

multilist<color_instance> * glob_color_multilist;
multilist<n_instance> * glob_n_multilist;
multilist<b_instance> * glob_b_multilist;
multilist<t_instance> * glob_t_multilist;
multilist<s_instance> * glob_s_multilist;
char colorstring[7]="AABBCC";
char resortedstring[stringlength];
FILE * outfile;
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

void getcaptions(float * width,float * height)
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
}

struct atom_actual_node_
{
	_small bonds[10];
	_small bondcount;
	_small special;
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

void expressline(float ileft,float itop,float iright,float ibottom,char * icolorstring)
{
	fprintf(outfile,"<path d=\"M %f %f L %f %f \" style=\"stroke:#%s\"/>",ileft,itop,iright,ibottom,icolorstring);
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

void svg_main(const char * filename)
{
	float cangle;
	float langle;
	float width, height;
	float ibonddist;
	float ibonddist2;
	float ibonddist3;
	float ibonddist4;
	int colornr;
	_small owner;
	outfile=fopen(filename,"w+");
	fprintf(outfile,"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>");
	width+=10;
	height+=10;
	glob_color_multilist=retrievemultilist<color_instance>();
	glob_b_multilist=retrievemultilist<b_instance>();
	glob_n_multilist=retrievemultilist<n_instance>();
	glob_t_multilist=retrievemultilist<t_instance>();
	glob_s_multilist=retrievemultilist<s_instance>();
	getcaptions(&width,&height);
	fprintf(outfile,"<svg version=\"1.0\" width=\"%f\" height=\"%f\">\n",width,height);
	getatoms();
	n_instance * startnode, * endnode;
	multilist<graphic_instance> * i_graphic_multilist=retrievemultilist<graphic_instance>();
	for (int ilv1=0;ilv1<(*i_graphic_multilist).filllevel;ilv1++)
	{
		cdx_Rectangle iBBX=((*i_graphic_multilist).bufferlist)[ilv1].BoundingBox;
		colornr=((*i_graphic_multilist).bufferlist)[ilv1].color;
		get_colorstring(colornr);
		if (((*i_graphic_multilist).bufferlist)[ilv1].GraphicType==1)
		{
			expressline(iBBX.left,iBBX.top,iBBX.right,iBBX.bottom,colorstring);
			cangle=getangle(iBBX.right-iBBX.left,iBBX.top-iBBX.bottom)+Pi/2;
			langle=getangle(iBBX.right-iBBX.left,iBBX.top-iBBX.bottom);
			if (((*i_graphic_multilist).bufferlist)[ilv1].ArrowType==2)
			{
				fprintf(outfile,"<path d=\"M %f %f L %f %f \" style=\"stroke:#%s fill:#%s\"/>",iBBX.left,iBBX.top,iBBX.left+cos(langle)*arrowdepth+cos(cangle)*arrowthickness,iBBX.top+sin(langle)*arrowdepth+sin(cangle)*arrowthickness,colorstring,colorstring);
				fprintf(outfile,"<path d=\"M %f %f L %f %f \" style=\"stroke:#%s fill:#%s\"/>",iBBX.left,iBBX.top,iBBX.left+cos(langle)*arrowdepth-cos(cangle)*arrowthickness,iBBX.top+sin(langle)*arrowdepth-sin(cangle)*arrowthickness,colorstring,colorstring);
			}
		}
		if (((*i_graphic_multilist).bufferlist)[ilv1].GraphicType==3)
		{
			expressline(iBBX.left,iBBX.top,iBBX.right,iBBX.top,colorstring);
			expressline(iBBX.left,iBBX.bottom,iBBX.right,iBBX.bottom,colorstring);
			expressline(iBBX.left,iBBX.top,iBBX.left,iBBX.bottom,colorstring);
			expressline(iBBX.right,iBBX.top,iBBX.right,iBBX.bottom,colorstring);
		}
	}
	for (int ilv1=0;ilv1<(*glob_b_multilist).filllevel;ilv1++)
	{
		colornr=((*glob_b_multilist).bufferlist)[ilv1].color;
		get_colorstring(colornr);
		for (int ilv2=0;ilv2<(*glob_n_multilist).filllevel;ilv2++)
		{
			if (((*glob_n_multilist).bufferlist)[ilv2].id==((*glob_b_multilist).bufferlist)[ilv1].E)
			{
				endnode=&(((*glob_n_multilist).bufferlist)[ilv2]);
			}
			if (((*glob_n_multilist).bufferlist)[ilv2].id==((*glob_b_multilist).bufferlist)[ilv1].B)
			{
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
		int iDisplaytype1=((*glob_b_multilist).bufferlist[ilv1].Display);
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
			fprintf(outfile,"<path d=\"M %f %f L %f %f L %f %f L %f %f z \" style=\"stroke:#%s;fill:#%s\"/>",
(*startnode).p.x+ibonddist3*cos(cangle),(*startnode).p.y+ibonddist3*sin(cangle),(*startnode).p.x-ibonddist3*cos(cangle),(*startnode).p.y-ibonddist3*sin(cangle),
(*endnode).p.x+ibonddist4*cos(cangle),(*endnode).p.y+ibonddist4*sin(cangle),(*endnode).p.x-ibonddist4*cos(cangle),(*endnode).p.y-ibonddist4*sin(cangle),
colorstring,((iDisplaytype1==3) || (iDisplaytype1==4) || (iDisplaytype1==9) || (iDisplaytype1==10))?"FFFFFF":colorstring);//TODO: to work with white-on-black...
		}
		else
		{
			fprintf(outfile,"<path d=\"M %f %f L %f %f \" style=\"stroke:#%s%s\"/>",(*startnode).p.x+ibonddist2*cos(cangle),(*startnode).p.y+ibonddist2*sin(cangle),(*endnode).p.x+ibonddist2*cos(cangle),(*endnode).p.y+ibonddist2*sin(cangle),colorstring,((*glob_b_multilist).bufferlist[ilv1].Display==5) ? ";stroke-width:5;stroke-linecap:round":"");
		}
		if (((*glob_b_multilist).bufferlist)[ilv1].Order==2)
		{
			fprintf(outfile,"<path d=\"M %f %f L %f %f \" style=\"stroke:#%s%s\"/>",(*startnode).p.x+ibonddist*cos(cangle),(*startnode).p.y+ibonddist*sin(cangle),(*endnode).p.x+ibonddist*cos(cangle),(*endnode).p.y+ibonddist*sin(cangle),colorstring,((*glob_b_multilist).bufferlist[ilv1].Display2==1) ? ";stroke-dasharray:2,2":"");
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

		printf("!!%i!!",(*i_t_multilist).bufferlist[ilv1].LabelAlignment);
		fprintf(outfile,"<text fill=\"%s\" %s stroke=\"none\" transform=\"translate(%f,%f)\" font-size=\"18\">",colorstring,((*i_t_multilist).bufferlist[ilv1].LabelAlignment==-1) ? "text-anchor=\"end\" text-align=\"end\"" : "",((*i_t_multilist).bufferlist)[ilv1].p.x,((*i_t_multilist).bufferlist)[ilv1].p.y);
		intl start,end;
		start=(*(((*i_t_multilist).bufferlist)[ilv1].s)).start_in_it;
		end=start+(*(((*i_t_multilist).bufferlist)[ilv1].s)).count_in_it;
		printf("color=%s",colorstring);
		char string_resorted=0;
		if (owner!=-1)
		{
			if ((*((*i_t_multilist).bufferlist[ilv1].s)).count_in_it==1)
			{
				if (((*i_s_multilist).bufferlist[start].face & 0x60)>0)
				{
					if ((*i_t_multilist).bufferlist[ilv1].Justification==-1)
					{
						string_resorted=resortstring((((*i_s_multilist).bufferlist))[start].PCTEXT.a);
					}
				}
			}
		}
		for (int ilv2=start;ilv2<end;ilv2++)
		{
			colornr=((*i_s_multilist).bufferlist)[ilv2].color;
			if (colornr!=0)
			{
				get_colorstring(colornr);
			}
			printf("%s",(((*i_s_multilist).bufferlist))[ilv2].PCTEXT.a);	
			fprintf(outfile,"<tspan style=\"fill:#%s\">%s</tspan>",colorstring,string_resorted ? resortedstring:(((*i_s_multilist).bufferlist))[ilv2].PCTEXT.a);
		}
		fprintf(outfile,"</text>");
	}
	
//	for (int ilv1=0;ilv1<(*i_
	fprintf(outfile,"</svg>");
	fclose(outfile);
}
