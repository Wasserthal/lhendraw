typedef int selection_[20000];//a 32bit int gives selection masks for 32 layers. TODO: make this grow to the current buffer size.
selection_ selection_currentselection;//The ordinary selection, more precisely the current state of it.
_u32 selection_currentselection_found;
selection_ selection_clickselection;//All elements caught with the current click, or another likewise action of the mouse.
_u32 selection_clickselection_found;
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
