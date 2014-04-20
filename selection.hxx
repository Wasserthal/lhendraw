typedef int selection_[20000];//a 32bit int gives selection masks for 32 layers. TODO: make this grow to the current buffer size.
selection_ selection_currentselection;//The ordinary selection, more precisely the current state of it.
_u32 selection_currentselection_found;
selection_ selection_clickselection;//All elements caught with the current click, or another likewise action of the mouse.
_u32 selection_clickselection_found;
typedef struct clickabilitymatrix_
{
	int mode;//0: groups //1 whole objects 2: manipulators(default) 3: fragments, such as bezierpoints
	int level0;//zero: selects outermost groups. positive: enters groups to n'th stage. negative: groups over this object by n, 0 would be the object.
	int types1;//0x1: structures 0x2: graphics 0x4: curves 0x8: beziers 0x10: texts 0x20: pictures 0x40: tlc plates 0x80: NMR spectra
	_u32 types2;//list of object groups to process click. See internal_enum.hxx
	_u32 types3[8];//indexed as: 0x0: other 0x1: centers 0x2: auxpoints, e.g. endpoints 0x3: virtual scaling handles 0x4: shape-specific graphic manipulators, like mid-arrow manipulators or corner roundness selectionlers 0x5: lines 0x6: area
}clickabilitymatrix_;
clickabilitymatrix_ selection_clickabilitymatrix={2,0,0,0xFFFFFFFF,
{0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF}};
void selection_clearselection(selection_ iselection)
{
	for (int ilv1=0;ilv1<20000;ilv1++)
	{
		iselection[ilv1]=0;
	}
}
typedef struct selection_frame_
{
	float startx,starty;
	float endx,endy;
	float left,top;
	float right,bottom;
}selection_frame_;
selection_frame_ selection_frame;
void selection_recheck(selection_ iselection,_u32 * matrix)
{
	(*matrix)=0;
	_u32 icompare=0;
	int isize=0;
	for (int ilv1=1;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
	{
		icompare=1<<ilv1;
		basicmultilist * tlmultilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
		isize=STRUCTURE_OBJECTTYPE_List[ilv1].size;
		for (int ilv2=0;ilv2<(*tlmultilist).filllevel;ilv2++)
		{
			if (iselection[ilv2] & icompare)
			{
				if ((*(basic_instance*)(((char*)(*tlmultilist).pointer)+ilv2*isize)).exist)
				{
					(*matrix)|=icompare;
					goto ifertig;
				}
			}
		}
		ifertig:;
	}
}
