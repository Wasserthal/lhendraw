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

multilist<n_instance> * glob_n_multilist;
multilist<b_instance> * glob_b_multilist;
multilist<t_instance> * glob_t_multilist;
multilist<graphic_instance> * glob_graphic_multilist;
multilist<curve_instance> * glob_curve_multilist;
multilist<arrow_instance> * glob_arrow_multilist;
multilist<moleculefill_instance> * glob_moleculefill_multilist;
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
