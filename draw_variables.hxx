//variables to configure graphic drawing.
//Includes some help routines also.
//TODO:Includes the document data which is converted from cdxml, which belongs into another unit.

#ifdef LENNARD_HACK
char LENNARD_HACK_dokilltext;
char LENNARD_HACK_colormode;
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
