//variables to configure graphic drawing.
//Includes some help routines also.
//TODO:Includes the document data which is converted from cdxml, which belongs into another unit.

#ifdef LENNARD_HACK
char LENNARD_HACK_dokilltext;
char LENNARD_HACK_colormode;
char * current_hack_annotation;
#endif
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
float boldwidth=3;
float xboldwidth=15;
float arrowheadlength=15;
float arrowthickness=5;
float atomfontheight=18.0;

char colorstring[7]="AABBCC";
char colorstring2[7]="AABBCC";
char resortedstring[stringlength];
FILE * outfile;
float SVG_currentbasex,SVG_currentbasey;
float SVG_width,SVG_height;
float SVG_ileft,SVG_itop;
float SVG_currentfringex,SVG_currentfringey;
float SVG_currentshiftx,SVG_currentshifty;

struct atom_actual_node_
{
	_small bonds[10];
	_small bondcount;
	_small labelside;
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
		//TODO SUBJECT to new memory structure
	}
	for (int ilv1=0;ilv1<(*i_n_multilist).filllevel;ilv1++)
	{
		i_n_instance=(*i_n_multilist).bufferlist+ilv1;
		if ((*i_n_instance).xyz.x>maxx)
		{
			maxx=(*i_n_instance).xyz.x;
		}
		if ((*i_n_instance).xyz.y>maxy)
		{
			maxy=(*i_n_instance).xyz.y;
		}
		if ((*i_n_instance).xyz.x<minx)
		{
			minx=(*i_n_instance).xyz.x;
		}
		if ((*i_n_instance).xyz.y<miny)
		{
			miny=(*i_n_instance).xyz.y;
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
	float tlangle=getangle(inputx,inputy);
	(*x1)=9.0*cos(tlangle);
	(*y1)=9.0*sin(tlangle);
	return;
	if (fabs(inputx)>fabs(inputy))
	{
		iquotient=fabs(inputy/inputx);
		(*x1)=9.0*((inputx>0) ? 1.0 :-1.0);
		(*y1)=9.0*iquotient*((inputy>0) ? 1.0 :-1.0);
	}
	else
	{
		if ((inputy)!=0)
		{
			iquotient=fabs(inputx/inputy);
		}
		(*x1)=9.0*iquotient*((inputx>0) ? 1.0 :-1.0);
		(*y1)=9.0*((inputy>0) ? 1.0 :-1.0);
	}
}

void svg_findaround()
{
	#ifdef LENNARD_HACK
	LENNARD_HACK_dokilltext=0;
	LENNARD_HACK_colormode=0;
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
	}
	for (int ilv1=0;ilv1<bufferlistsize;ilv1++)
	{
		text_actual_node[ilv1].owner=-1;
	}
	getcaptions(&SVG_width,&SVG_height,&SVG_ileft,&SVG_itop);
	SVG_ileft-=10;
	SVG_itop-=10;
	SVG_width+=20;
	SVG_height+=20;

}


int currentLineType;//0: normal 2: Bold 0x100: Double
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
	char ellipsereversed;
	void create(cdx_Point3D iCenter,cdx_Point3D iMajor,cdx_Point3D iMinor)
	{
		ellipsereversed=0;
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
		if ((normalizedmajx*normalizedminx+normalizedmajy*normalizedminy)<0)
		{
			ellipsereversed=1;
		}
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
			normalizedmajx=radiusx*cos(axangle);
			normalizedmajy=radiusx*sin(axangle);
			normalizedminx=radiusy*cos(axangle+Pi/2);
			normalizedminy=radiusy*sin(axangle+Pi/2);
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
n_instance * i_n_instance;
n_instance * startnode, * endnode;
char STRINGOUTPUT_emptyformat[]="";
char STRINGOUTPUT_bold[]="font-weight=\"bold\" ";
char STRINGOUTPUT_LENNARDBOLD[]="font-weight=\"bold\"";
char iparmsstring[stringlength+1];
float currentsetfontsize=18;
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


char stylestring[1024];
char emptystring[1]="";
char semicolonstring[2]=";";
int stylefromline(int input) {return 1|((input & 1)?8:0)|((input & 2)?4:0);}
int stylefromrectangle(int input) {return 1|((input & 0x10)?8:0)|((input & 0x20)?4:0)|((input & 0x8)?2:0);}
#ifdef SDL2
_u16 getunicode(SDL_Event * input)
{
	return SDL_GetKeyName(input->key.keysym.sym)[0];
}
#define SDL_BUTTON_WHEELUP 99999
#define SDL_BUTTON_WHEELDOWN 99998
#else
_u16 getunicode(SDL_Event * input)
{
	return input->key.keysym.unicode;
}
#endif
