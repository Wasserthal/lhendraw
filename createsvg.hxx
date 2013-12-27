//Turns document data into graphics
//Intended to be included several times for different graphics outputs
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

#ifdef GFXOUT_SVG
int get_colorstringv(int number)
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
#endif

int get_colorstring(int number)
{
	get_colorstringv(number);
}

inline int get_colorstring_passive(int number)
{
	if (number!=0)
	{
		get_colorstringv(number);
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
	multilist<t_instance> * i_t_multilist=retrievemultilist<t_instance>();
	for (int ilv1=0;ilv1<(*i_t_multilist).filllevel;ilv1++)
	{
		#ifdef LENNARD_HACK
		if ((text_actual_node[ilv1].owner!=-1) || (LENNARD_HACK_dokilltext==0))
		{
		#endif
		t_instance * i_t_instance=(t_instance*)&((*i_t_multilist).bufferlist[ilv1]);
		if ((*i_t_instance).BoundingBox.top>maxy)
		{
			maxy=(*i_t_instance).BoundingBox.top;
		}
		if ((*i_t_instance).BoundingBox.bottom>maxy)
		{
			maxy=(*i_t_instance).BoundingBox.bottom;
		}
		#ifdef LENNARD_HACK
		}
		#endif
	}
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


int get_bond_between(int inatom1, int inatom2)
{
	int imax=(*glob_b_multilist).filllevel;
	for (int ilv1=0;ilv1<imax;ilv1++)
	{
		if ((bond_actual_node[ilv1].start==inatom1) && (bond_actual_node[ilv1].end==inatom2))
		{
			return ilv1;
		}
		if ((bond_actual_node[ilv1].end==inatom1) && (bond_actual_node[ilv1].start==inatom2))
		{
			return ilv1;
		}
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
	for (int ilv1=0;ilv1<(*glob_b_multilist).filllevel;ilv1++)//defines processable bonds
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
	for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)//defines processable atoms
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
		for (int ilv2=0;ilv2<atom_actual_node[ilv1].bondcount;ilv2++)//checks how atoms behave towards bonds and text
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
	for (int ilv1=0;ilv1<(*glob_b_multilist).filllevel;ilv1++)//refines undefined double bonds
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
#ifdef GFXOUT_SVG
void stylegenestring(int flags,unsigned int fillcolor=0)
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
		if ((fillcolor & 0xFF000000)==0)
		{
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
		}
		else
		{
			sprintf(stylestring+stylestringlength,"%sfill:#%06X%n",waspar,fillcolor & 0xFFFFFF,&formatreturnvalue);
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
#endif
int stylefromline(int input) {return 1|((input & 1)?8:0)|((input & 2)?4:0);}
int stylefromrectangle(int input) {return 1|((input & 0x10)?8:0)|((input & 0x20)?4:0)|((input & 0x8)?2:0);}

#ifdef GFXOUT_SVG
void expressline(float ileft,float itop,float iright,float ibottom)
{
	fprintf(outfile,"<path d=\"M %f %f L %f %f \" %s/>\n",ileft+SVG_currentshiftx,itop+SVG_currentshifty,iright+SVG_currentshiftx,ibottom+SVG_currentshifty,stylestring);
}
#endif
#ifdef GFXOUT_SVG
void expressspinellipse(float ix,float iy,float irx,float iry,float iangle)
{
	fprintf(outfile,"<ellipse cx=\"%f\" cy=\"%f\" rx=\"%f\" ry=\"%f\" transform=\"rotate(%f,%f,%f)\" %s/>\n",ix+SVG_currentshiftx,iy+SVG_currentshifty,irx,iry,(iangle*180)/Pi,ix+SVG_currentshiftx,iy+SVG_currentshifty,stylestring);
}
void expressellipse(float ix,float iy,float irx,float iry)
{
	fprintf(outfile,"<ellipse cx=\"%f\" cy=\"%f\" rx=\"%f\" ry=\"%f\" %s/>\n",ix+SVG_currentshiftx,iy+SVG_currentshifty,irx,iry,stylestring);
}
#endif
void expresscdxcircle(float ileft,float itop,float radius)
{
	expressellipse(ileft,itop,fabs(radius),fabs(radius));
}
#ifdef GFXOUT_SVG
void expresstetrangle(float ix1,float iy1,float ix2,float iy2,float ix3,float iy3,float ix4,float iy4)
{
	fprintf(outfile,"<path d=\"M %f %f L %f %f L %f %f L %f %f z \" %s/>\n",
	ix1+SVG_currentshiftx,iy1+SVG_currentshifty,ix2+SVG_currentshiftx,iy2+SVG_currentshifty,ix3+SVG_currentshiftx,iy3+SVG_currentshifty,ix4+SVG_currentshiftx,iy4+SVG_currentshifty,
stylestring);
}
#endif
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
#ifdef GFXOUT_SVG
void expresshexangle(float ix1,float iy1,float ix2,float iy2,float ix3,float iy3,float ix4,float iy4,float ix5,float iy5,float ix6,float iy6)
{
	fprintf(outfile,"<path d=\"M %f %f L %f %f L %f %f L %f %f L %f %f L %f %f z \" %s/>\n",
	ix1+SVG_currentshiftx,iy1+SVG_currentshifty,ix2+SVG_currentshiftx,iy2+SVG_currentshifty,ix3+SVG_currentshiftx,iy3+SVG_currentshifty,ix4+SVG_currentshiftx,iy4+SVG_currentshifty,ix5+SVG_currentshiftx,iy5+SVG_currentshifty,ix6+SVG_currentshiftx,iy6+SVG_currentshifty,
stylestring);
}
#endif
#ifdef GFXOUT_SVG
void expressarc(float centerx,float centery,float radiusx,float radiusy,float startangle,float endangle)
{
	float startx,starty;
	float endx,endy;
	startx=centerx+radiusx*cos(startangle)+SVG_currentshiftx;
	starty=centery+radiusy*sin(startangle)+SVG_currentshifty;
	endx=centerx+radiusx*cos(endangle)+SVG_currentshiftx;
	endy=centery+radiusy*sin(endangle)+SVG_currentshifty;
	fprintf(outfile,"<path d=\"M %f,%f A %f,%f %i %i %i %f %f\" %s />",startx,starty,radiusx,radiusy,(int)0,(int)(fabs(startangle-endangle)>=Pi),(int)(startangle-endangle)<0,endx,endy,stylestring);
}
void expressarc_enhanced(float centerx,float centery,float radiusx,float radiusy,float startangle,float endangle,float tiltangle)
{
	float startx,starty;
	float endx,endy;
	startx=centerx+(radiusx*cos(startangle)*cos(tiltangle)-radiusy*sin(startangle)*sin(tiltangle))+SVG_currentshiftx;
	starty=centery+(radiusy*sin(startangle)*cos(tiltangle)+radiusx*cos(startangle)*sin(tiltangle))+SVG_currentshifty;
	endx=centerx+(radiusx*cos(endangle)*cos(tiltangle)-radiusy*sin(endangle)*sin(tiltangle))+SVG_currentshiftx;
	endy=centery+(radiusy*sin(endangle)*cos(tiltangle)+radiusx*cos(endangle)*sin(tiltangle))+SVG_currentshifty;
	fprintf(outfile,"<path d=\"M %f,%f A %f,%f %i %i %i %f %f\" %s />",startx,starty,radiusx,radiusy,(int)(tiltangle*180.0/Pi),(int)(fabs(startangle-endangle)>=Pi),(int)(startangle-endangle)<0,endx,endy,stylestring);
}
#endif

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
	if ((fsm==1) || (fsm==4))
	{
		for (int ilv2=start;ilv2<=end;ilv2++)//write remaining numbers
		{
			resortedstring[resortedstringpos++]=input[ilv2];
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
	glob_arrow_multilist=retrievemultilist<arrow_instance>();
	glob_moleculefill_multilist=retrievemultilist<moleculefill_instance>();
	#ifdef LENNARD_HACK
	LENNARD_HACK_dokilltext=0;
	{
		multilist<LENHACK_ANNOTATION_instance> * tl_LENHACK_ANNOTATION_MULTILIST=retrievemultilist<LENHACK_ANNOTATION_instance>();
		for (int ilv1=0;ilv1<(*tl_LENHACK_ANNOTATION_MULTILIST).filllevel;ilv1++)
		{
			if ((*tl_LENHACK_ANNOTATION_MULTILIST).bufferlist[ilv1].IgnoreTextboxes)
			{
				LENNARD_HACK_dokilltext=1;
			}
		}
	}
	#endif
	/*Piece from getatoms information needed also when there are no graphics form bound calculation*/
	for (int ilv1=0;ilv1<bufferlistsize;ilv1++)
	{
		atom_actual_node[ilv1].bondcount=0;
		atom_actual_node[ilv1].special=-1;
	}
	for (int ilv1=0;ilv1<bufferlistsize;ilv1++)
	{
		text_actual_node[ilv1].owner=-1;
	}
	for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)//defines processable atoms
	{
		n_instance * tlatominstance=&((*glob_n_multilist).bufferlist[ilv1]);
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
	/*This was a piece from getatoms information needed also when there are no graphics form bound calculation*/
	getcaptions(&SVG_width,&SVG_height,&SVG_ileft,&SVG_itop);
	SVG_ileft-=10;
	SVG_itop-=10;
	SVG_width+=20;
	SVG_height+=20;

}

void expressbezier(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4)
{
	fprintf(outfile,"<path d=\"M %f %f C %f %f %f %f %f %f \" %s/>\n",x1+SVG_currentshiftx,y1+SVG_currentshifty,x2+SVG_currentshiftx,y2+SVG_currentshifty,x3+SVG_currentshiftx,y3+SVG_currentshifty,x4+SVG_currentshiftx,y4+SVG_currentshifty,stylestring);
}

int currentLineType;//0: normal 2: Bold 0x100: Double
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
				if (currentArrowHeadType==0)
				{
					stylegenestring(2);
					expresstetrangle(tlArrowTopx,tlArrowTopy,
tlArrowTopx+cos(tllangle)*arrowheadlength+cos(tlcangle)*arrowthickness,tlArrowTopy+sin(tllangle)*arrowheadlength+sin(tlcangle)*arrowthickness,
tlArrowTopx+cos(tllangle)*arrowheadlength-cos(tlcangle)*arrowthickness,tlArrowTopy+sin(tllangle)*arrowheadlength-sin(tlcangle)*arrowthickness,
tlArrowTopx,tlArrowTopy);
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
				tlcangle=tlcangle+Pi;
			}
			case 2 :
			{
				stylegenestring(2);
				if (currentArrowHeadType==0)
				{
					expresstetrangle(tlArrowTopx+tllinedist*cos(tlcangle),tlArrowTopy+tllinedist*sin(tlcangle),
tlArrowTopx+cos(tllangle)*arrowheadlength+(arrowthickness+tllinedist)*cos(tlcangle),tlArrowTopy+sin(tllangle)*arrowheadlength+(arrowthickness+tllinedist)*sin(tlcangle),
tlArrowTopx+cos(tllangle)*arrowheadlength+tllinedist*cos(tlcangle),tlArrowTopy+sin(tllangle)*arrowheadlength+tllinedist*sin(tlcangle),
tlArrowTopx+tllinedist*cos(tlcangle),tlArrowTopy+tllinedist*sin(tlcangle));
				}
				break;
			}
		}
	}
}

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
int index_in_buffer;
int ilv1;
int currentArrowHeadType;
int currentArrowHeadTail;
int currentArrowHeadHead;
char currentEllipsemode;
float tllinedist;
float tllefttan;
float tlrighttan;
float tllefttan2;
float tlrighttan2;
struct ellipsoid_
{
	float majx,majy;
	float minx,miny;
	float normalizedmajx,normalizedmajy;
	float normalizedminx,normalizedminy;
	float radiusx,radiusy;
	float internal1,internal2;
	float internalrad,internalangle;
	float axangle;
	char shearsharepositive;
	void create(cdx_Point3D iCenter,cdx_Point3D iMajor,cdx_Point3D iMinor)
	{
		majx=iMajor.x-iCenter.x;
		majy=iMajor.y-iCenter.y;
		minx=iMinor.x-iCenter.x;
		miny=iMinor.y-iCenter.y;
		radiusx=sqrt(fsqr(majx)+fsqr(majy));
		radiusy=sqrt(fsqr(minx)+fsqr(miny));
/*		if (radiusx<radiusy)
		{
			float swap;
			swap=majx;
			majx=majy;
			majy=swap;
			swap=minx;
			minx=miny;
			miny=swap;
			swap=radiusx;
			radiusx=radiusy;
			radiusy=swap;
		}*/
		if (radiusx>0.0000001)
		{
			normalizedmajx=majx/radiusx;
			normalizedmajy=majy/radiusx;
		}
		if (radiusy>0.0000001)
		{
			normalizedminx=minx/radiusy;
			normalizedminy=miny/radiusy;
		}
		axangle=getangle(majx,majy);
		float shearshare=(normalizedmajx*minx+normalizedmajy*miny);
		radiusy=radiusy*sqrt(1-fsqr(shearshare/radiusy));
		float oldvol=fabs(radiusx*radiusy);
		shearsharepositive=(shearshare>0);
		shearshare=fabs(shearshare/radiusy);
		if (fabs(shearshare)>=1e-6)
		{
			float tiltangle;
			float tilttangens;
			tiltangle=acos(NORMALIZEELLIPSE(1,radiusy/radiusx,shearshare))/2;
			tilttangens=tan(tiltangle);
			float flatness=(radiusy/radiusx);
			float criticaliks=sqrt(fabs(1/((1/(flatness*(shearshare-(1/tilttangens))))-1)));
			float criticalyps=sqrt(1-fsqr(criticaliks));
			radiusx=radiusx*sqrt(fsqr(criticalyps*shearshare*flatness+criticaliks)+fsqr(criticalyps*flatness));
			radiusy=oldvol/radiusx;
			axangle+=shearsharepositive?tiltangle:-tiltangle;
		}
	}
	void reset()
	{
		normalizedmajx=1;
		normalizedminx=0;
		normalizedmajy=0;
		normalizedminy=1;
		axangle=0;
	}
	void fill(float x,float y)
	{
		internal1=(normalizedmajx*x+normalizedmajy*y)/radiusx;
		internal2=(normalizedminx*x+normalizedminy*y)/radiusy;
		internalrad=sqrt(fsqr(internal1)+fsqr(internal2));
		internalangle=getangle(internal1,internal2);
	}
} ellipsoid;
float textdeltax,textdeltay;
char specialS;
char specialE;
int iDisplaytype1;
int tllast;
char string_resorted;
char ifsmat;//0: nothing //1: on a subscript number; 2: on text; 3: on a superscript
curve_instance * i_curve_instance;
graphic_instance * i_graphic_instance;
arrow_instance * i_arrow_instance;
b_instance * i_b_instance;
n_instance * startnode, * endnode;
char STRINGOUTPUT_emptyformat[]="";
char STRINGOUTPUT_bold[]="font-weight=\"bold\" ";
char STRINGOUTPUT_LENNARDBOLD[]="font-weight=\"bold\"";
char iparmsstring[stringlength+1];
float currentsetfontsize=18;
void printformatted(const char * iinput,const char * parms,int imode,int start,int end)
{
	int ilv4=start;
	char linebreak;
	thatwasatemporaryskip:
	linebreak=0;
	if (imode==1)
	{
		fprintf(outfile,"<tspan dy=\"+%f\" %s font-size=\"%f\" style=\"fill:#%s\">",currentsetfontsize/6.0,parms,currentsetfontsize*0.77777,colorstring);
	}
	if ((imode==2) || (imode==0))
	{
		fprintf(outfile,"<tspan %s font-size=\"%f\" style=\"fill:#%s\">",parms,currentsetfontsize,colorstring);
	}
	if (imode==4)
	{
		fprintf(outfile,"<tspan dy=\"-%f\" %s font-size=\"%f\" style=\"fill:#%s\">",currentsetfontsize/6.0,parms,currentsetfontsize*0.77777,colorstring);
	}
	for (;ilv4<end;ilv4++)
	{
		if (iinput[ilv4]==10)
		{
			ilv4++;
			linebreak=1;
			goto skipfornow;
		}
		fprintf(outfile,"%c",iinput[ilv4]);
	}
	skipfornow:
	fprintf(outfile,"</tspan>",colorstring);
	if (imode==1)
	{
		fprintf(outfile,"<tspan %s dy=\"-%f\">&#8288;</tspan>",parms,currentsetfontsize/6.0);
	}
	if (imode==4)
	{
		fprintf(outfile,"<tspan %s dy=\"%f\">&#8288;</tspan>",parms,currentsetfontsize/6.0);
	}
	fprintf(outfile,"\n");
	if (linebreak) {fprintf(outfile,"<tspan dy=\"%f\" x=\"0\">&#8288;</tspan>",20.0/18.0*currentsetfontsize);if (ilv4<end) goto thatwasatemporaryskip;}//a line break;
}

#ifdef LENNARD_HACK
void LENNARD_HACK_killtext()
{
	int tl_t_number=-1;
	for (int ilv1=0;ilv1<multilist_count;ilv1++)
	{
		if (multilistlist.instances[ilv1]==glob_t_multilist)
		{
			tl_t_number=ilv1;
		}
	}
	for (int ilv1=0;ilv1<(*glob_t_multilist).filllevel;ilv1++)
	{
		if (text_actual_node[ilv1].owner==-1)
		{
			deletefromZlist(tl_t_number,ilv1);
		}
	}
}
#endif

void svg_head(const char * filename,float width,float height)
{
	outfile=fopen(filename,"w+");
	getatoms();
	fprintf(outfile,"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>");
	fprintf(outfile,"<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">");
//	fprintf(outfile,"<svg version=\"1.0\" width=\"%f\" height=\"%f\">\n",SVG_width-SVG_ileft,SVG_height-SVG_itop);
	fprintf(outfile,"<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"%f\" height=\"%f\">\n",width,height);
}
void svg_tail()
{
	fprintf(outfile,"</svg>");
	fclose(outfile);
}
void svg_controlprocedure(bool irestriction=0,bool hatches=0)
{
	void * dummy;
	cdx_Rectangle tlBoundingBox;
	int tlGraphicType;
	int tlAngularSize;
	for (ilv1=0;ilv1<bufferlistsize*multilistZcount;ilv1++)
	{
		if (objectZorderlist[ilv1].listnr!=-1)
		{
			basicmultilist * tlcurrentmultilist=(basicmultilist*)multilistlist.instances[objectZorderlist[ilv1].listnr];
			index_in_buffer=objectZorderlist[ilv1].nr;
			void * tlcurrentinstance=((char*)((*tlcurrentmultilist).pointer))+((*tlcurrentmultilist).itemsize)*index_in_buffer;
			{//TODO**** delete instance in findaround after implementing delete routines
				int ipropertyoffset=(tlcurrentmultilist)->getproperties("SupersededBy",(CDXMLREAD_functype*)&dummy);
				if (ipropertyoffset!=-1)
				{
					if ((*(int*)(((char*)tlcurrentinstance)+ipropertyoffset))>0)
					{
						goto svg_main_loop;
					}
				}
			}
			if ((tlcurrentmultilist==glob_moleculefill_multilist) ^ (hatches==1))
			{
				goto svg_main_loop;
			}
			if (irestriction)
			{
				if (tlcurrentmultilist==glob_curve_multilist)
				{
					cdx_Bezierpoints * tlBez=&((*glob_curve_multilist).bufferlist[bond_actual_node[index_in_buffer].start].CurvePoints);
					for (int ilv2=1;ilv2<(*tlBez).count-1;ilv2+=3)
					{
						cdx_Point2D tlp2d=(*tlBez).a[ilv2];
						if (tlp2d.x<SVG_currentbasex) {goto svg_main_loop;}
						if (tlp2d.y<SVG_currentbasey) {goto svg_main_loop;}
						if (tlp2d.x>SVG_currentfringex) {goto svg_main_loop;}
						if (tlp2d.y>SVG_currentfringey) {goto svg_main_loop;}
					}
				}
				int ipropertyoffset=(tlcurrentmultilist)->getproperties("BoundingBox",(CDXMLREAD_functype*)&dummy);
				if (ipropertyoffset!=-1)
				{
					tlBoundingBox=*((cdx_Rectangle*)(((char*)tlcurrentinstance)+ipropertyoffset));
					canonicalizeboundingbox(&tlBoundingBox);
					if (tlBoundingBox.left<SVG_currentbasex) {goto svg_main_loop;}
					if (tlBoundingBox.top<SVG_currentbasey) {goto svg_main_loop;}
					if (tlBoundingBox.left>SVG_currentfringex) {goto svg_main_loop;}
					if (tlBoundingBox.top>SVG_currentfringey) {goto svg_main_loop;}
				}
				else
				{
					if (tlcurrentmultilist==glob_b_multilist)
					{
						cdx_Point2D tlp2d=(*glob_n_multilist).bufferlist[bond_actual_node[index_in_buffer].start].p;
						if (tlp2d.x<SVG_currentbasex) {goto svg_main_loop;}
						if (tlp2d.y<SVG_currentbasey) {goto svg_main_loop;}
						if (tlp2d.x>SVG_currentfringex) {goto svg_main_loop;}
						if (tlp2d.y>SVG_currentfringey) {goto svg_main_loop;}
					}
				}
			}
			if (multilistlist.instances[objectZorderlist[ilv1].listnr]==glob_curve_multilist) goto svg_main_curve;
			if (multilistlist.instances[objectZorderlist[ilv1].listnr]==glob_graphic_multilist) goto svg_main_graphic;
			if (multilistlist.instances[objectZorderlist[ilv1].listnr]==glob_b_multilist) goto svg_main_b;
			if (multilistlist.instances[objectZorderlist[ilv1].listnr]==glob_t_multilist) goto svg_main_t;
			if (multilistlist.instances[objectZorderlist[ilv1].listnr]==glob_arrow_multilist) goto svg_main_arrow;
			if (multilistlist.instances[objectZorderlist[ilv1].listnr]==glob_moleculefill_multilist) goto svg_main_moleculefill;
		}
		svg_main_loop:
		;
	}
	goto svg_main_end;
	svg_main_curve:
	tllinedist=0;//TODO****
	i_curve_instance=(curve_instance*)&((*glob_curve_multilist).bufferlist[index_in_buffer]);
	colornr=(*i_curve_instance).color;
	get_colorstring(colornr);
	stylegenestring(1|(((*i_curve_instance).CurveType & 128)?2:0)|(((*i_curve_instance).CurveType & 4)?4:0));
	tllast=1;
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
		currentArrowHeadType=0;
		currentArrowHeadTail=0;
		currentArrowHeadHead=0;
		tllinedist=0;
		tllefttan=0;
		tlrighttan=0;
		tllefttan2=0;
		tlrighttan2=0;
		langle=0;cangle=0;
		currentLineType=0;//0: normal 2: Bold 0x100: Double
		currentArrowHeadType=(*i_curve_instance).ArrowheadType;
		currentArrowHeadHead=(*i_curve_instance).ArrowheadHead;
		currentArrowHeadTail=(*i_curve_instance).ArrowheadTail;
		iBBX.left=(*i_curve_instance).CurvePoints.a[(*i_curve_instance).CurvePoints.count-2].x;
		iBBX.top=(*i_curve_instance).CurvePoints.a[(*i_curve_instance).CurvePoints.count-2].y;
		iBBX.right=(*i_curve_instance).CurvePoints.a[1].x;
		iBBX.bottom=(*i_curve_instance).CurvePoints.a[1].y;

		langle=getangle((*i_curve_instance).CurvePoints.a[(*i_curve_instance).CurvePoints.count-3].x-(*i_curve_instance).CurvePoints.a[(*i_curve_instance).CurvePoints.count-2].x,(*i_curve_instance).CurvePoints.a[(*i_curve_instance).CurvePoints.count-3].y-(*i_curve_instance).CurvePoints.a[(*i_curve_instance).CurvePoints.count-2].y);
		otherlangle=getangle((*i_curve_instance).CurvePoints.a[2].x-(*i_curve_instance).CurvePoints.a[1].x,(*i_curve_instance).CurvePoints.a[2].y-(*i_curve_instance).CurvePoints.a[1].y);
		cangle=langle+Pi/2;
		othercangle=otherlangle+Pi/2;
		drawarrheads(iBBX,langle,cangle,otherlangle,othercangle,currentArrowHeadType,currentArrowHeadTail,currentArrowHeadHead,tllinedist);
	}
	goto svg_main_loop;
	svg_main_graphic:
/* HOW ARROW MUST BE DONE:
1. The arrow type determiner determines arrow type and bond spacing.
2. The lines/arc are drawn. if it was something else, the next two points are skipped. 
3. Otherwise, the arrow angles are calculated
4. And the arrows are drawn.*/
	i_graphic_instance=&((*glob_graphic_multilist).bufferlist)[index_in_buffer];
	iBBX=(*i_graphic_instance).BoundingBox;
	colornr=(*i_graphic_instance).color;
	tlGraphicType=(*i_graphic_instance).GraphicType;
	tlAngularSize=(*i_graphic_instance).AngularSize;
	currentLineType=(*i_graphic_instance).LineType;
	get_colorstring(colornr);
	ellipsoid.reset();
	currentArrowHeadType=0;
	currentArrowHeadTail=0;
	currentArrowHeadHead=0;
	tllinedist=0;
	tllefttan=0;
	tlrighttan=0;
	tllefttan2=0;
	tlrighttan2=0;
	langle=0;cangle=0;
	currentLineType=0;//0: normal 2: Bold 0x100: Double
	currentEllipsemode=0;
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
		currentArrowHeadHead=2;
		currentArrowHeadTail=2;
	}
	if ((*i_graphic_instance).ArrowType & 48)
	{
		tllinedist=8;
		currentArrowHeadHead=1;//only valid when called from graphic
		if ((*i_graphic_instance).ArrowType & 16) {currentArrowHeadType=1;}
		if ((*i_graphic_instance).ArrowType & 32) {currentArrowHeadType=2;}
		currentLineType=0x100;
	}
	svg_graphic_arrow_shunt:
	if (currentArrowHeadType==1) {if (currentArrowHeadHead&1){tllefttan2=2;}if (currentArrowHeadHead&2){tlrighttan2=2;}if (currentArrowHeadTail&1){tllefttan=2;}if (currentArrowHeadTail&2){tlrighttan=2;}}
	if (currentArrowHeadType==2) {if (currentArrowHeadHead&1){tllefttan2=1;}if (currentArrowHeadHead&2){tlrighttan2=1;}if (currentArrowHeadTail&1){tllefttan=1;}if (currentArrowHeadTail&2){tlrighttan=1;}}
	if (tlGraphicType==1)
	{
		stylegenestring(stylefromline(currentLineType));
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
	if (tlGraphicType==2)
	{
		float deltax,deltay;
		float tlradius;
		deltax=iBBX.left-iBBX.right;
		deltay=iBBX.top-iBBX.bottom;
		stylegenestring(stylefromline(currentLineType));
		if (currentLineType &0x100)
		{
//TODO****				expressarc(iBBX.right,iBBX.bottom
		}
		if (currentEllipsemode)
		{
			float tla,tlb,tlc,tld,tle;
			double ellipticx[4];
			double ellipticy[8];
			ellipsoid.fill(deltax,deltay);
			float tlangle;
			expressarc_enhanced(iBBX.right,iBBX.bottom,ellipsoid.radiusx,ellipsoid.radiusy,ellipsoid.internalangle,ellipsoid.internalangle+((tlAngularSize/180.0)*Pi),ellipsoid.axangle);
			for (int ilv0=0;ilv0<2;ilv0++)
			{
				double tlbest=1e20;
				int tlbestone=-1;
				tlangle=ellipsoid.internalangle+((ilv0)?((tlAngularSize/180.0)*Pi):0);
				ARROW_ELLIPTIC(ellipsoid.radiusx/arrowheadlength,ellipsoid.radiusy/arrowheadlength,cos(tlangle),sin(tlangle),tla,tlb,tlc,tld,tle);
				if (fabs(tla)<=1e-3) {goto stillacircle;}
				QUARTIC_quartic(tla,tlb,tlc,tld,tle,&(ellipticx[0]),&(ellipticx[1]),&(ellipticx[2]),&(ellipticx[3]));
				for (int ilv1=0;ilv1<4;ilv1++)
				{
					if (!(isnan(ellipticx[ilv1])))
					{
						ellipticy[ilv1]=sqrt(1.0-fsqr(ellipticx[ilv1]))*ellipsoid.radiusy;
						ellipticy[ilv1+4]=-sqrt(1.0-fsqr(ellipticx[ilv1]))*ellipsoid.radiusy;
						if (abs(ellipticx[ilv1])>1) {ellipticy[ilv1]=ellipticx[ilv1]*ellipsoid.radiusy;ellipticy[ilv1]=-ellipticx[ilv1+4]*ellipsoid.radiusy;}
						ellipticx[ilv1]*=ellipsoid.radiusx;
					}
				}
				float tlsinus=sin(tlangle+(((tlAngularSize>0)^(ilv0))?(Pi/2):(-Pi/2)));
				float tlcosinus=cos(tlangle+(((tlAngularSize>0)^(ilv0))?(Pi/2):(-Pi/2)));
				for (int ilv1=0;ilv1<8;ilv1++)
				{
					if (!((isnan(ellipticx[ilv1%4]))||(isnan(ellipticy[ilv1]))))
					{
						float tlhorz=ellipticx[ilv1%4]-cos(tlangle)*ellipsoid.radiusx;
						float tlvert=ellipticy[ilv1]-sin(tlangle)*ellipsoid.radiusy;
						float tltemp=fabs(sqrt(fsqr(tlhorz)+fsqr(tlvert))-arrowheadlength);
						if ((tlhorz*tlcosinus+tlvert*tlsinus)<0)
						{
							tltemp+=arrowheadlength*2;
						}
						if (tltemp<tlbest)
						{
							tlbestone=ilv1;
							tlbest=tltemp;
						}
					}
				}
				if (tlbestone!=-1)
				{
					(ilv0?otherlangle:langle)=getangle(ellipticx[tlbestone%4]-cos(tlangle)*ellipsoid.radiusx,ellipticy[tlbestone]-sin(tlangle)*ellipsoid.radiusy)+ellipsoid.axangle;
					(ilv0?othercangle:cangle)=(ilv0?otherlangle:langle)+Pi/2;
				}
				else
				{
					goto stillacircle;
				}
				if (ilv0==1)
				{
					iBBX.right+=cos(tlangle)*ellipsoid.radiusx*cos(ellipsoid.axangle)-sin(tlangle)*ellipsoid.radiusy*sin(ellipsoid.axangle);
					iBBX.bottom+=+sin(tlangle)*ellipsoid.radiusy*cos(ellipsoid.axangle)+cos(tlangle)*ellipsoid.radiusx*sin(ellipsoid.axangle);
				}
			}
		}
		else 
		if (tlradius>arrowheadlength/2)
		{
			stillacircle:
			float tlangle;
			tlangle=getangle(deltax,deltay);
			if (tlAngularSize>0)
			{
				 langle=(tlangle+Pi/2.0);
			}
			else
			{
				 langle=(tlangle-Pi/2.0);
			}
			cangle=langle+Pi/2.0;
			tlradius=sqrt(deltax*deltax+deltay*deltay);
			expressarc(iBBX.right,iBBX.bottom,tlradius,tlradius,tlangle,tlangle+((tlAngularSize/180.0)*Pi));
			otherlangle=langle+((tlAngularSize/180.0)*Pi)+Pi;
			othercangle=otherlangle+Pi/2.0;
			float dturn=asin(arrowheadlength/tlradius/2);
			if (tlAngularSize>0)
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
			iBBX.right+=tlradius*cos(tlangle+((tlAngularSize/180.0)*Pi));
			iBBX.bottom+=tlradius*sin(tlangle+((tlAngularSize/180.0)*Pi));
		}
	}
	else goto skiparrows;
	drawarrheads(iBBX,langle,cangle,otherlangle,othercangle,currentArrowHeadType,currentArrowHeadTail,currentArrowHeadHead,tllinedist);
	goto skipthisgraphic;
	skiparrows:
	if ((*i_graphic_instance).GraphicType==3)
	{
		int tlisfound;
		multilistreference<annotation_instance> * tlannotationmultilistreference=dynamic_cast<multilistreference<annotation_instance>*>((*glob_graphic_multilist).bufferlist[index_in_buffer].annotation);
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
		expresstetrangle(iBBX.left,iBBX.top,iBBX.right,iBBX.top,iBBX.right,iBBX.bottom,iBBX.left,iBBX.bottom);
	}
	if ((*i_graphic_instance).GraphicType==4)
	{
		stylegenestring(1|(((*i_graphic_instance).OvalType & 4)?2:0));
		if ((*i_graphic_instance).OvalType & 1)
		{
			float tldistance=sqrt((iBBX.right-iBBX.left)*(iBBX.right-iBBX.left)+(iBBX.bottom-iBBX.top)*(iBBX.bottom-iBBX.top));
			iBBX.left-=tldistance;
			iBBX.right=iBBX.left+tldistance;
			iBBX.bottom=iBBX.top+tldistance;
			expresscdxcircle(iBBX.left,iBBX.top,tldistance);
		}
		else
		{
			ellipsoid.create((*i_graphic_instance).Center3D,(*i_graphic_instance).MajorAxisEnd3D,(*i_graphic_instance).MinorAxisEnd3D);
			expressspinellipse((*i_graphic_instance).Center3D.x,(*i_graphic_instance).Center3D.y,ellipsoid.radiusx,ellipsoid.radiusy,ellipsoid.axangle);
		}
	}
	if ((*i_graphic_instance).GraphicType==7)
	{
		stylegenestring(1);
		switch((*i_graphic_instance).SymbolType)
		{
			case 4 : strcpy(colorstring2,"FFFFFF");goto charge;break;
			case 5 : strcpy(colorstring2,"FFFFFF");goto charge;break;
			case 1 : strcpy(colorstring2,colorstring);goto radical;break;
			case 6 : strcpy(colorstring2,colorstring);goto dagger;break;
			case 7 : strcpy(colorstring2,colorstring);goto dagger2;break;
		}
		charge:
		stylegenestring(1,0xFFFFFFFF);
		expressellipse(iBBX.left,iBBX.top,6,6);
		expressline(iBBX.left-3,iBBX.top,iBBX.left+3,iBBX.top);
		if ((*i_graphic_instance).SymbolType==4)
		{
			expressline(iBBX.left,iBBX.top-3,iBBX.left,iBBX.top+3);
		}
		goto GraphicType7done;
		radical:
		stylegenestring(3);
		expressellipse(iBBX.left,iBBX.top,4,4);
		goto GraphicType7done;
		dagger2:
		expressline(iBBX.left-5,iBBX.top+10,iBBX.right+5,iBBX.top+10);
		//PASSTHROUGH
		dagger:
		expressline(iBBX.left-5,iBBX.top,iBBX.right+5,iBBX.top);
		expressline(iBBX.left,iBBX.top-5,iBBX.right,iBBX.top+15);
		goto GraphicType7done;
		GraphicType7done:
		;
	}
	if ((*i_graphic_instance).GraphicType==6)
	{
		stylegenestring(1);
		cangle=getangle(iBBX.right-iBBX.left,iBBX.bottom-iBBX.top)+Pi/2;
		langle=getangle(iBBX.right-iBBX.left,iBBX.bottom-iBBX.top);
		switch ((*i_graphic_instance).BracketType)
		{
			case 3:
			{
				expressline(iBBX.left,iBBX.top,iBBX.right,iBBX.bottom);
				expressline(iBBX.left,iBBX.top,iBBX.left+13.4*cos(cangle),iBBX.top+13.4*sin(cangle));
				expressline(iBBX.right,iBBX.bottom,iBBX.right+13.4*cos(cangle),iBBX.bottom+13.4*sin(cangle));
				break;
			}
			case 4:
			{
				expressline(iBBX.left+13.4*cos(langle),iBBX.top+13.4*sin(langle),(iBBX.left+iBBX.right)/2-13.4*cos(langle),(iBBX.top+iBBX.bottom)/2-13.4*sin(langle));
				expressline((iBBX.left+iBBX.right)/2+13.4*cos(langle),(iBBX.top+iBBX.bottom)/2+13.4*sin(langle),iBBX.right-13.4*cos(langle),iBBX.bottom-13.4*sin(langle));
				expressbezier(iBBX.left+13.4*cos(langle),iBBX.top+13.4*sin(langle),
				iBBX.left+6.7*cos(langle),iBBX.top+6.7*sin(langle),
				iBBX.left+6.7*cos(cangle),iBBX.top+6.7*sin(cangle),
				iBBX.left+13.4*cos(cangle),iBBX.top+13.4*sin(cangle));
				expressbezier((iBBX.left+iBBX.right)/2+13.4*cos(langle),(iBBX.top+iBBX.bottom)/2+13.4*sin(langle),
				(iBBX.left+iBBX.right)/2+6.7*cos(langle),(iBBX.top+iBBX.bottom)/2+6.7*sin(langle),
				(iBBX.left+iBBX.right)/2-6.7*cos(cangle),(iBBX.top+iBBX.bottom)/2-6.7*sin(cangle),
				(iBBX.left+iBBX.right)/2-13.4*cos(cangle),(iBBX.top+iBBX.bottom)/2-13.4*sin(cangle));
				expressbezier(iBBX.right-13.4*cos(langle),iBBX.bottom-13.4*sin(langle),
				iBBX.right-6.7*cos(langle),iBBX.bottom-6.7*sin(langle),
				iBBX.right+6.7*cos(cangle),iBBX.bottom+6.7*sin(cangle),
				iBBX.right+13.4*cos(cangle),iBBX.bottom+13.4*sin(cangle));
				expressbezier((iBBX.left+iBBX.right)/2-13.4*cos(langle),(iBBX.top+iBBX.bottom)/2-13.4*sin(langle),
				(iBBX.left+iBBX.right)/2-6.7*cos(langle),(iBBX.top+iBBX.bottom)/2-6.7*sin(langle),
				(iBBX.left+iBBX.right)/2-6.7*cos(cangle),(iBBX.top+iBBX.bottom)/2-6.7*sin(cangle),
				(iBBX.left+iBBX.right)/2-13.4*cos(cangle),(iBBX.top+iBBX.bottom)/2-13.4*sin(cangle));
				break;
			}
			case 5:
			{
				float tldist=sqrt(fsqr(iBBX.right-iBBX.left)+fsqr(iBBX.top-iBBX.bottom));
				fprintf(outfile,"<path d=\"M %f,%f A %f,%f %i %i %i %f %f\" %s />",iBBX.left+SVG_currentshiftx,iBBX.top+SVG_currentshifty,tldist*1.8,tldist*1.8,0,0,1,iBBX.right+SVG_currentshiftx,iBBX.bottom+SVG_currentshifty,stylestring);
				break;
			}
		}
	}
	skipthisgraphic:
	;//always run over here. One might need this.
	goto svg_main_loop;
	svg_main_arrow:
	i_arrow_instance=&((*glob_arrow_multilist).bufferlist)[index_in_buffer];
	colornr=(*i_arrow_instance).color;
	get_colorstring(colornr);
	currentArrowHeadType=(*i_arrow_instance).ArrowheadType;
	currentArrowHeadTail=(*i_arrow_instance).ArrowheadTail;
	currentArrowHeadHead=(*i_arrow_instance).ArrowheadHead;
	tllinedist=0;
	tllefttan=0;
	tlrighttan=0;
	tllefttan2=0;
	tlrighttan2=0;
	langle=0;cangle=0;
	currentLineType=(*i_arrow_instance).LineType;//0: normal 2: Bold 0x100: Double
	currentEllipsemode=0;
	if (((*i_arrow_instance).ArrowShaftSpacing)>0)
	{
		tllinedist=4;
		currentLineType|=0x100;
	}
	if (currentArrowHeadType & 3)
	{
		tllinedist=8;
		currentLineType|=0x100;
	}
	tlAngularSize=(*i_arrow_instance).AngularSize;
	if (tlAngularSize!=0)
	{
		tlGraphicType=2;
		iBBX.right=(*i_arrow_instance).Center3D.x;
		iBBX.bottom=(*i_arrow_instance).Center3D.y;
		ellipsoid.create((*i_arrow_instance).Center3D,(*i_arrow_instance).MajorAxisEnd3D,(*i_arrow_instance).MinorAxisEnd3D);
		ellipsoid.fill((*i_arrow_instance).Head3D.x-(*i_arrow_instance).Center3D.x,(*i_arrow_instance).Head3D.y-(*i_arrow_instance).Center3D.y);
		currentEllipsemode=1;
	}
	else
	{
		tlGraphicType=1;
		iBBX.right=(*i_arrow_instance).Tail3D.x;
		iBBX.bottom=(*i_arrow_instance).Tail3D.y;
	}
	iBBX.left=(*i_arrow_instance).Head3D.x;
	iBBX.top=(*i_arrow_instance).Head3D.y;
	goto svg_graphic_arrow_shunt;
	goto svg_main_loop;
	svg_main_b:
	_small inr_E,inr_S;
	i_b_instance=&(((*glob_b_multilist).bufferlist)[index_in_buffer]);
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
		if ((*i_b_instance).Order>2.1)
		{
			ibonddist=bonddist;ibonddist2=0;
			if ((*i_b_instance).Order>3.1)
			{
				ibonddist=(bonddist/2)*3;ibonddist2=(bonddist/2);
			}
		}
	}
	langle=getangle((*endnode).p.x-(*startnode).p.x,(*endnode).p.y-(*startnode).p.y);
	cangle=langle+Pi/2;
	calcdelta(&textdeltax,&textdeltay,(*endnode).p.x-(*startnode).p.x,(*endnode).p.y-(*startnode).p.y);
	specialS=(atom_actual_node[inr_S].special!=-1);
	specialE=(atom_actual_node[inr_E].special!=-1);
	iBBX.left=(*startnode).p.x+textdeltax*specialS;
	iBBX.top=(*startnode).p.y+textdeltay*specialS;
	iDisplaytype1=((*glob_b_multilist).bufferlist[index_in_buffer].Display);
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
			if ((bond_actual_node[index_in_buffer]).numberleft[0]!=-1) {tllefttan=tan(Pi/2-bond_actual_node[index_in_buffer].cotanleft[0]);}else{if ((bond_actual_node[index_in_buffer]).numberright[0]!=-1){tllefttan=tan(bond_actual_node[index_in_buffer].xcotanright[0]/2);}}
			if ((bond_actual_node[index_in_buffer]).numberright[0]!=-1) {tlrighttan=tan(Pi/2-bond_actual_node[index_in_buffer].cotanright[0]);}else{if ((bond_actual_node[index_in_buffer]).numberleft[0]!=-1){tlrighttan=tan(bond_actual_node[index_in_buffer].xcotanleft[0]/2);}}
		}
		if (specialS==0)
		{
			if ((bond_actual_node[index_in_buffer]).numberleft[1]!=-1) {tllefttan2=tan(Pi/2-bond_actual_node[index_in_buffer].cotanleft[1]);}else{if ((bond_actual_node[index_in_buffer]).numberright[1]!=-1){tllefttan2=tan(bond_actual_node[index_in_buffer].xcotanright[1]/2);}}
			if ((bond_actual_node[index_in_buffer]).numberright[1]!=-1) {tlrighttan2=tan(Pi/2-bond_actual_node[index_in_buffer].cotanright[1]);}else{if ((bond_actual_node[index_in_buffer]).numberleft[1]!=-1){tlrighttan2=tan(bond_actual_node[index_in_buffer].xcotanleft[1]/2);}}
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
			ibonddist+=fabs(bonddist)*((ibonddist>0) ? 1 : -1);
		}
	}
	else
	{
		stylegenestring(((iDisplaytype1==1) ?8:0) | 1);
		expressline(iBBX.left+ibonddist2*cos(cangle),iBBX.top+ibonddist2*sin(cangle),iBBX.right+ibonddist2*cos(cangle),iBBX.bottom+ibonddist2*sin(cangle));
	}
	if ((*i_b_instance).Order>1.1)
	{
		stylegenestring((((*glob_b_multilist).bufferlist[index_in_buffer].Display2==1)?8:0)|1);
		expressline(iBBX.left+ibonddist*cos(cangle),iBBX.top+ibonddist*sin(cangle),iBBX.right+ibonddist*cos(cangle),iBBX.bottom+ibonddist*sin(cangle));
		if ((*i_b_instance).Order>2.1)
		{
			expressline(iBBX.left-ibonddist*cos(cangle),iBBX.top-ibonddist*sin(cangle),iBBX.right-ibonddist*cos(cangle),iBBX.bottom-ibonddist*sin(cangle));
			if ((*i_b_instance).Order>3.1)
			{
				expressline(iBBX.left-ibonddist2*cos(cangle),iBBX.top-ibonddist2*sin(cangle),iBBX.right-ibonddist2*cos(cangle),iBBX.bottom-ibonddist2*sin(cangle));
			}
		}
	}
	for (int ilv0=0;ilv0<4;ilv0++)
	{
		n_instance * tlnode=(ilv0/2)?startnode:endnode;
		get_colorstring((*tlnode).color);
		stylegenestring(1);
		if ((*tlnode).ExternalConnectionType==4)
		{
			float tlposx=(*tlnode).p.x+SVG_currentshiftx;
			float tlposy=(*tlnode).p.y+SVG_currentshifty;
			float tlsin=sin(langle+((ilv0%2)?(Pi/2):(-Pi/2)))*4;
			float tlcos=cos(langle+((ilv0%2)?(Pi/2):(-Pi/2)))*4;
			fprintf(outfile,"<path d=\" M %f %f C %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f \" %s />",tlposx,tlposy,tlposx+tlsin,tlposy-tlcos,tlposx+tlsin+tlcos,tlposy-tlcos+tlsin,tlposx+tlcos,tlposy+tlsin,
tlposx+tlcos-tlsin,tlposy+tlsin+tlcos,tlposx+2*tlcos-tlsin,tlposy+2*tlsin+tlcos,tlposx+2*tlcos,tlposy+2*tlsin
,tlposx+2*tlcos+0.25*tlsin,tlposy+2*tlsin-0.25*tlcos,tlposx+2.25*tlcos+0.5*tlsin,tlposy+2.25*tlsin-0.5*tlcos,tlposx+2.5*tlcos+0.5*tlsin,tlposy+2.5*tlsin-0.5*tlcos,stylestring);
		}
	}
	goto svg_main_loop;
	svg_main_t:
	owner=text_actual_node[index_in_buffer].owner;
	colornr=0;
	get_colorstring(colornr);
	if (owner!=-1)
	{
		colornr=(*glob_n_multilist).bufferlist[text_actual_node[index_in_buffer].owner].color;
		get_colorstring(colornr);
	}
	colornr=((*glob_t_multilist).bufferlist)[index_in_buffer].color;
	get_colorstring_passive(colornr);

	fprintf(outfile,"<text fill=\"%s\" %s stroke=\"none\" transform=\"translate(%f,%f)\" font-size=\"%f\">",colorstring,((*glob_t_multilist).bufferlist[index_in_buffer].LabelAlignment==-1) ? "text-anchor=\"end\" text-align=\"end\"" : "",((*glob_t_multilist).bufferlist)[index_in_buffer].p.x+SVG_currentshiftx,((*glob_t_multilist).bufferlist)[index_in_buffer].p.y+SVG_currentshifty,atomfontheight);
	intl start,end;
	start=(*(((*glob_t_multilist).bufferlist)[index_in_buffer].s)).start_in_it;
	end=start+(*(((*glob_t_multilist).bufferlist)[index_in_buffer].s)).count_in_it;
	string_resorted=0;
	ifsmat=0;//0: nothing //1: on a subscript number; 2: on text; 3: on a superscript
	for (int ilv2=start;ilv2<end;ilv2++)
	{
		colornr=((*glob_s_multilist).bufferlist)[ilv2].color;
		fontnr=((*glob_s_multilist).bufferlist)[ilv2].font;
		font_instance * i_font_instance=getfont(fontnr);
		finalstring=(((*glob_s_multilist).bufferlist))[ilv2].PCTEXT.a;
		char * iparms;
		iparms=STRINGOUTPUT_emptyformat;
		{
			int tlformlabeltype=((*glob_s_multilist).bufferlist)[ilv2].face;
			currentsetfontsize=(((*glob_s_multilist).bufferlist))[ilv2].size;
			#ifdef LENNARD_HACK
			if ((currentsetfontsize>20.0) || (tlformlabeltype & 0x10))
			{
				if (currentsetfontsize>99.0)
				{
					iparms=STRINGOUTPUT_emptyformat;
				}
				else
				{
					if (currentsetfontsize!=29)
					{
						iparms=STRINGOUTPUT_bold;
						if (currentsetfontsize!=38)
						{
							currentsetfontsize=24;
						}
					}
					else
					{
						currentsetfontsize=29;
					}
				}
			}
			else
			{
				currentsetfontsize=18;
			}
			#endif
			#ifdef CAMBRIDGESOFT_CONFORMING
			sprintf(iparmsstring,"%s",((tlformlabeltype & 0x10) ? STRINGOUTPUT_bold : STRINGOUTPUT_emptyformat),currentsetfontsize);
			iparms=iparmsstring;
			#endif
		}
		if (owner!=-1)
		{
			if ((*((*glob_t_multilist).bufferlist[index_in_buffer].s)).count_in_it==1)
			{
				if (((*glob_s_multilist).bufferlist[start].face & 0x60)==0x60)
				{
					if ((*glob_t_multilist).bufferlist[index_in_buffer].Justification==-1)
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
		if ((((*glob_s_multilist).bufferlist)[ilv2].face & 0x60)==0x60)
		{
			int tlmax=strlen(finalstring);
			int tlstart,tlend;
			tlstart=0;tlend=0;
			for (int ilv3=0;ilv3<tlmax;ilv3++)
			{
				if ((finalstring[ilv3]>='0') && (finalstring[ilv3]<='9'))
				{
					if (ifsmat==2)
					{
						printformatted(finalstring,iparms,ifsmat,tlstart,tlend);
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
							printformatted(finalstring,iparms,ifsmat,tlstart,tlend);
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
							printformatted(finalstring,iparms,ifsmat,tlstart,tlend);
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
							printformatted(finalstring,iparms,ifsmat,tlstart,tlend);
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
								printformatted(finalstring,iparms,ifsmat,tlstart,tlend);
								tlstart=ilv3;
								tlend=ilv3+1;
								ifsmat=4;
								goto trivial;
							}
							if (ifsmat==2)
							{
								printformatted(finalstring,iparms,ifsmat,tlstart,tlend);
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
									printformatted(finalstring,iparms,ifsmat,tlstart,tlend);
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
									printformatted(finalstring,iparms,ifsmat,tlstart,tlend);
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
			printformatted(finalstring,iparms,ifsmat,tlstart,tlend);
		}
		else
		{
			int tlformlabeltype=((*glob_s_multilist).bufferlist)[ilv2].face;
			currentsetfontsize=(((*glob_s_multilist).bufferlist))[ilv2].size;
			#ifdef LENNARD_HACK
			if ((currentsetfontsize>20.0) || (tlformlabeltype & 0x10))
			{
				if (currentsetfontsize>99.0)
				{
					iparms=STRINGOUTPUT_emptyformat;
				}
				else
				{
					if (currentsetfontsize!=29)
					{
						iparms=STRINGOUTPUT_bold;
						if (currentsetfontsize!=38)
						{
							currentsetfontsize=24;
						}
					}
					else
					{
						currentsetfontsize=29;
					}
				}
			}
			else
			{
				currentsetfontsize=18;
			}
			#endif
			#ifdef CAMBRIDGESOFT_CONFORMING
			sprintf(iparmsstring,"%s",((tlformlabeltype & 0x10) ? STRINGOUTPUT_bold : STRINGOUTPUT_emptyformat),currentsetfontsize);
			iparms=iparmsstring;
			#endif
			/*fprintf(outfile,"<tspan %s style=\"fill:#%s\">%s</tspan>%s\n",
			(tlformlabeltype & 0x20) ? "dy=\"+3\" font-size=\"14\"" : ((tlformlabeltype & 0x40) ? "dy=\"-3\" font-size=\"14%\"":""),
			colorstring,finalstring,(tlformlabeltype & 0x20)?"<tspan dy=\"-3\"/>":((tlformlabeltype & 0x40)?"<tspan dy=\"3\"/>":""));*/
			printformatted(finalstring,iparms,((tlformlabeltype & 0x20) ? 1 : 0) | ((tlformlabeltype & 0x40) ? 4 : 0),0,strlen(finalstring));
		}
	}
	fprintf(outfile,"</text>\n");
	goto svg_main_loop;
	svg_main_moleculefill:
	{
		moleculefill_instance * tlmoleculefill=&((*glob_moleculefill_multilist).bufferlist[index_in_buffer]);
		fprintf(outfile,"<path d=\" ");
		for (int ilv2=0;ilv2<(*tlmoleculefill).Points.count;ilv2++)
		{
			int iid=(*tlmoleculefill).Points.a[ilv2];
			int ilv3;
			for (ilv3=0;ilv3<(*glob_n_multilist).filllevel;ilv3++)
			{
				if ((*glob_n_multilist).bufferlist[ilv3].id==iid)
				{
					goto ifertig;
				}
			}
			goto dontputthispoint;
			ifertig:
			;
			{
				n_instance * i_n_instance=&((*glob_n_multilist).bufferlist[ilv3]);
				
				fprintf(outfile,"%c %f,%f ",(ilv2==0)?'M':'L',(*i_n_instance).p.x+SVG_currentshiftx,(*i_n_instance).p.y+SVG_currentshifty);
			}
			dontputthispoint:
			;
		}
		fprintf(outfile,"z \" style=\"fill:#%06X;color:none;\" opacity=\"0.9\"/>\n",(*tlmoleculefill).RGB);
	}
	goto svg_main_loop;
	svg_main_end:
	;
}
void Lennard_hatch();
void svg_main(const char * filename)
{
	svg_findaround();
	svg_head(filename,SVG_width-SVG_ileft,SVG_height-SVG_itop);
	SVG_currentshiftx=-SVG_ileft;
	SVG_currentshifty=-SVG_itop;
	SVG_currentfringex=((unsigned int)-1)>>1;
	SVG_currentfringey=((unsigned int)-1)>>1;
	#ifdef LENNARD_HACK
	Lennard_hatch();
	#endif
	initZlist();
	#ifdef LENNARD_HACK
	if (LENNARD_HACK_dokilltext)
	{
		LENNARD_HACK_killtext();
	}
	#endif
	svg_controlprocedure(0,1);
	svg_controlprocedure(0,0);
	svg_tail();
}
