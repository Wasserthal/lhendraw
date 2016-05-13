//variables to configure graphic drawing.
//Includes some help routines also.
//TODO:Includes the document data which is converted from cdxml, which belongs into another unit.

struct inficorn_
{
	float x,y;
	int ix,iy;
};
inficorn_ LHENDRAW_inficorn[bezierpointmax];

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
float boldwidth=6;
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

atom_actual_node_ * atom_actual_node;
struct bond_actual_node_
{
	_small start,end;
	float cotanleft[2],cotanright[2];//fw,bw==end,start NOTE: when [1], right and left are also seen from the other side.
	float xcotanleft[2],xcotanright[2];//fw,bw==end,start NOTE: when [1], right and left are also seen from the other side.
	char leftdefined,rightdefined;
};

bond_actual_node_ * bond_actual_node;
struct text_actual_node_
{
	_small owner;
};
text_actual_node_ * text_actual_node;
_u32 * janitor_atomsbyid;
_u32 * janitor_atomsbyid_other;
int janitor_atomsbyid_count;
_u32 getbigger(_u32 * source,_u32 no)
{
	if (no>=janitor_atomsbyid_count) return 0xFFFFFFFF;
	else
	{
		return glob_n_multilist->bufferlist()[*(source+no)].id;
	}
}
int janitor_listatomsbyid_recursion(_u32 * target,_u32 * source,int span,int max)
{
	_u32 value1,value2;
	int leftstack,rightstack;
	if (span>max/2)
	{
		return 0;
	}
	for (int ilv1=0;ilv1<max;ilv1+=span*2)
	{
		leftstack=0;
		rightstack=0;
		for (int ilv2=0;ilv2<span*2;ilv2++)
		{
			if (leftstack<span)
			{
				value1=getbigger(source,ilv1+leftstack);
			}
			else
			{
				value1=0xFFFFFFFF;
			}
			if (rightstack<span)
			{
				value2=getbigger(source,ilv1+span+rightstack);
			}
			else
			{
				value2=0xFFFFFFFF;
			}
			if ((value1==0xFFFFFFFF)&&(value2==0xFFFFFFFF))
			{
				goto we_are_at_the_end;
			}
			if (value1<value2)
			{
				target[ilv1+ilv2]=source[ilv1+leftstack];
				leftstack++;
			}
			else
			{
				target[ilv1+ilv2]=source[ilv1+span+rightstack];
				rightstack++;
			}
		}
		we_are_at_the_end:;
	}
	return 1;
}
void janitor_listatomsbyid()//creates a list that lists atom indexes. This index list is sorted by the atom's id's. Not-existing atoms come at the end.
{
	janitor_atomsbyid_count=0;
	_u32 * tl_janitor_atomsbyid=janitor_atomsbyid;
	_u32 * tl_janitor_atomsbyid_other=janitor_atomsbyid_other;
	for (int ilv1=0;ilv1<glob_n_multilist->filllevel;ilv1++)
	{
		if (glob_n_multilist->bufferlist()[ilv1].exist)
		{
			janitor_atomsbyid[janitor_atomsbyid_count]=ilv1;
			janitor_atomsbyid_count++;
		}
	}
	int tl_max=LHENDRAW_buffersize/sizeof(_u32);
	int tl_max2;
	ibitsearchback:;
	if (tl_max==1) return;
	tl_max2=tl_max>>1;
	if (tl_max2>=janitor_atomsbyid_count) { tl_max=tl_max2; goto ibitsearchback;}
	int span=1;
	int swapped=0;
	while (janitor_listatomsbyid_recursion(tl_janitor_atomsbyid_other,tl_janitor_atomsbyid,span,tl_max))
	{
		_u32 * tl_swap;
		tl_swap=tl_janitor_atomsbyid_other;
		tl_janitor_atomsbyid_other=tl_janitor_atomsbyid;
		tl_janitor_atomsbyid=tl_swap;
		span*=2;
		swapped^=1;
	}
	if (swapped)
	{
		for (int ilv1=0;ilv1<janitor_atomsbyid_count;ilv1++)
		{
			janitor_atomsbyid[ilv1]=janitor_atomsbyid_other[ilv1];
		}
	}
}
int edit_getatombyid(_u32 id)//Works only after a recent call of janitor_listatomsbyid. recent means that the atoms may not be relevantly changed in between.
{
	int upperrim=janitor_atomsbyid_count;
	int lowerrim=0;
	int middle;
	iback:;
	if ((upperrim<0) || (lowerrim>=janitor_atomsbyid_count)) return -1;
	middle=(upperrim+lowerrim)>>1;
	if (id==getbigger(janitor_atomsbyid,middle)) return janitor_atomsbyid[middle];
	if (id<getbigger(janitor_atomsbyid,middle))
	{
		if (upperrim<=middle) return -1;
		upperrim=middle;
	}
	else
	{
		if (lowerrim>=middle) return -1;
		lowerrim=middle+1;
	}
	goto iback;
	return -1;
}
void getcaptions(float * right,float * bottom,float * left,float * top)
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
		if (((*i_t_multilist)[ilv1].exist) && ((edit_fileoperationrefersonlytopartofdocument==0)||(selection_currentselection[ilv1]&(1<<STRUCTURE_OBJECTTYPE_t))))
		{
			t_instance * i_t_instance=(*i_t_multilist).bufferlist()+ilv1;
			if ((*i_t_instance).BoundingBox.top<miny)
			{
				miny=(*i_t_instance).BoundingBox.top;
			}
			if ((*i_t_instance).BoundingBox.bottom>maxy)
			{
				maxy=(*i_t_instance).BoundingBox.bottom;
			}
			if ((*i_t_instance).BoundingBox.left<minx)
			{
				minx=(*i_t_instance).BoundingBox.left;
			}
			if ((*i_t_instance).BoundingBox.right>maxx)
			{
				maxx=(*i_t_instance).BoundingBox.right;
			}
		}
	}
	for (int ilv1=0;ilv1<(*i_curve_multilist).filllevel;ilv1++)
	{
		curve_instance * i_curve_instance=(*i_curve_multilist).bufferlist()+ilv1;
		if (i_curve_instance->exist)
		{
			float tl_x,tl_y,tl_z;
			for (int ilv2=0;ilv2<i_curve_instance->CurvePoints.count;ilv2++)
			{
				tl_x=i_curve_instance->CurvePoints.a[ilv2].x;
				tl_y=i_curve_instance->CurvePoints.a[ilv2].y;
				if (tl_y<miny)
				{
					miny=tl_y;
				}
				if (tl_y>maxy)
				{
					maxy=tl_y;
				}
				if (tl_x<minx)
				{
					minx=tl_x;
				}
				if (tl_x>maxx)
				{
					maxx=tl_x;
				}
			}
		}
	}
	for (int ilv1=0;ilv1<(*i_n_multilist).filllevel;ilv1++)
	{
		if (((*i_n_multilist)[ilv1].exist) && ((edit_fileoperationrefersonlytopartofdocument==0)||(selection_currentselection[ilv1]&(1<<STRUCTURE_OBJECTTYPE_n))))
		{
			i_n_instance=(*i_n_multilist).bufferlist()+ilv1;
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
	}
	for (int ilv1=0;ilv1<(*i_graphic_multilist).filllevel;ilv1++)
	{
		if (((*i_graphic_multilist)[ilv1].exist) && ((edit_fileoperationrefersonlytopartofdocument==0)||(selection_currentselection[ilv1]&(1<<STRUCTURE_OBJECTTYPE_graphic))))

		{
			i_graphic_instance=(*i_graphic_multilist).bufferlist()+ilv1;
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
	}
	(*right)=maxx;
	(*bottom)=maxy;
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
	/*Piece from getatoms information needed also when there are no graphics form bound calculation*/
	for (int ilv1=0;ilv1<glob_n_multilist->filllevel;ilv1++)
	{
		atom_actual_node[ilv1].bondcount=0;
	}
	for (int ilv1=0;ilv1<glob_b_multilist->filllevel;ilv1++)
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
int currentFillType;
float cangle;
float langle;
float ibonddist;
float ibonddist2;
float ibonddist3;
float ibonddist4;
int colornr;
int basecolornr;
int fontnr;
char * finalstring;
cdx_Rectangle iBBX;
cdx_Point3D currentCenter3D;
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
_small draw_relevantothernr;
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
float draw_arrowmatrix[2][2];
char draw_begun=0;
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
void draw_coors_arc_enhanced(double centerx,double centery,double radiusx,double radiusy,double startangle,double endangle,double tiltangle,double * xstart,double * ystart,double * xend,double * yend)
{
	int medix,mediy;
	int lastx,lasty;
	lasty=centery+fabs(radiusy)+fabs(radiusx)+5;//so there won't be a "first" point equal to it
	double tlsaxx=cos(tiltangle);double tlsaxy=sin(tiltangle);
	*xstart=centerx+tlsaxx*radiusx*cos(startangle)-tlsaxy*radiusy*sin(startangle);
	*ystart=centery+tlsaxy*radiusx*cos(startangle)+tlsaxx*radiusy*sin(startangle);
	*xend=centerx+tlsaxx*radiusx*cos(endangle)-tlsaxy*radiusy*sin(endangle);
	*yend=centery+tlsaxy*radiusx*cos(endangle)+tlsaxx*radiusy*sin(endangle);
}


char stylestring[1024];
char emptystring[1]="";
char semicolonstring[2]=";";
int stylefromline(int input) {return 1|((input & 1)?8:0)|((input & 2)?4:0);}
int stylefromrectangle(int input) {return 1|((input & 0x10)?8:0)|((input & 0x20)?4:0)|((input & 0x8)?2:0);}
int stylefromattrs(_i32 LineType,_i32 FillType)
{
	_i32 wert=((FillType==0)?1:0)|((LineType & 1)?8:0)|((LineType & 2)?4:0)|((FillType==1)?2:0);
	//0x1: line 0x2: filled 0x4: bold 0x8 dashed
	return wert;
}
void draw_getposintlcplate(double * outx,double * outy,tlcplate_instance * i_tlcplate_instance,double inx,double iny)
{
	double horzipos=(1.0-inx)*(*i_tlcplate_instance).TopLeft.x*(1-iny)+(1.0-inx)*(*i_tlcplate_instance).BottomLeft.x*iny+inx*(*i_tlcplate_instance).TopRight.x*(1-iny)+inx*(*i_tlcplate_instance).BottomRight.x*iny;
	double vertipos=(1.0-inx)*(*i_tlcplate_instance).TopLeft.y*(1-iny)+(1.0-inx)*(*i_tlcplate_instance).BottomLeft.y*iny+inx*(*i_tlcplate_instance).TopRight.y*(1-iny)+inx*(*i_tlcplate_instance).BottomRight.y*iny;
	*outx=horzipos;
	*outy=vertipos;
	return;
}
