#define selection_max 20000
typedef int selection_[selection_max];//a 32bit int gives selection masks for 32 layers. TODO: make this grow to the current buffer size.
selection_ selection_currentselection;//The ordinary selection, more precisely the current state of it.
_u32 selection_currentselection_found;
selection_ selection_clickselection;//All elements caught with the current click, or another likewise action of the mouse.
_u32 selection_clickselection_found;
selection_ selection_fragmentselection;//A fragment
_u32 selection_fragmentselection_found;
typedef struct clickabilitymatrix_
{
	int mode;//0: groups //1 whole objects 2: manipulators(default) 3: fragments, such as bezierpoints
	int level0;//zero: selects outermost groups. positive: enters groups to n'th stage. negative: groups over this object by n, 0 would be the object.
	int types1;//0x1: structures 0x2: graphics 0x4: curves 0x8: beziers 0x10: texts 0x20: pictures 0x40: tlc plates 0x80: NMR spectra
	//list of object groups to process click following. For bit fields, see internal_enum.hxx
	_u32 types2[8];//indexed as: 0x0: other 0x1: centers 0x2: auxpoints, e.g. endpoints 0x3: shape-specific graphic manipulators, like mid-arrow manipulators or corner roundness selectionlers 0x4: lines 0x5: area 0x6: virtual scaling handles 
}clickabilitymatrix_;
clickabilitymatrix_ selection_clickabilitymatrix={2,0,0,
{0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF}};
void selection_clearselection(selection_ iselection)
{
	for (int ilv1=0;ilv1<selection_max;ilv1++)
	{
		iselection[ilv1]=0;
	}
}
void selection_copyselection(selection_ iselection,selection_ iselection2)
{
	for (int ilv1=0;ilv1<selection_max;ilv1++)
	{
		iselection[ilv1]=iselection2[ilv1];
	}
}
void selection_ORselection(selection_ iselection,selection_ iselection2)
{
	for (int ilv1=0;ilv1<selection_max;ilv1++)
	{
		iselection[ilv1]|=iselection2[ilv1];
	}
}
void selection_XORselection(selection_ iselection,selection_ iselection2)
{
	for (int ilv1=0;ilv1<selection_max;ilv1++)
	{
		iselection[ilv1]^=iselection2[ilv1];
	}
}
void selection_ANDselection(selection_ iselection,selection_ iselection2)
{
	for (int ilv1=0;ilv1<selection_max;ilv1++)
	{
		iselection[ilv1]&=iselection2[ilv1];
	}
}
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
typedef struct selection_frame_
{
	float startx,starty;
	float endx,endy;
	float left,top;
	float right,bottom;
}selection_frame_;
selection_frame_ selection_frame;
char selection_lassobuffer[gfx_canvassizex*gfx_canvassizey];//The selection_lasso buffer corresponds to the pixels, not to the datastructure units!
struct selection_lassoline_
{
	int items[40];
	char itemm[40];
	int count;
};
selection_lassoline_ selection_lassohub[20000];//off-screen selection_lasso passes 10000 at upper rim of selection frame
int selection_lassostartx,selection_lassostarty;//TODO: end one pixel before the startpoint!
char selection_lasso_up;
void selection_lassoclear()
{
	for (int ilv1=0;ilv1<20000;ilv1++)
	{
		selection_lassohub[ilv1].count=0;
	}
	for (int ilv1=0;ilv1<gfx_canvassizex*gfx_canvassizey;ilv1++)
	{
		selection_lassobuffer[ilv1]=0;
	}
	selection_lasso_up=1;
	return;
}
inline void offscreenlasso(selection_lassoline_ * ilassoline,int value,char up)
{
	int ilv1;
	for (ilv1=0;ilv1<(*ilassoline).count;ilv1++)
	{
		if ((value<(*ilassoline).items[ilv1]) || ((value==(*ilassoline).items[ilv1]) && (up<(*ilassoline).itemm[ilv1])))
		{
			goto fillline;
		}
		else
		{
			if (value==(*ilassoline).items[ilv1])
			{
				if (up>(*ilassoline).itemm[ilv1])
				{
					if ((*ilassoline).count>ilv1+1)
					{
						if ((*ilassoline).items[ilv1+1]==value)//up will then definitely be correct
						{
							goto scrapline;
						}
					}
				}
				else
				{
					scrapline:;//goto considered helpful
					for (int ilv2=ilv1;ilv2<(*ilassoline).count-1;ilv2++)
					{
						(*ilassoline).items[ilv2]=(*ilassoline).items[ilv2+1];
					}
					(*ilassoline).count--;
					return;
				}
			}
		}
	}
	fillline:;
	if ((*ilassoline).count>=40)return;
	for (int ilv2=(*ilassoline).count;ilv2>ilv1;ilv2--)
	{
		(*ilassoline).items[ilv2]=(*ilassoline).items[ilv2-1];
	}
	(*ilassoline).count++;
	(*ilassoline).items[ilv1]=value;
	(*ilassoline).itemm[ilv1]=up;
}
inline void selection_lasso_putpixel(int tl_x,int tl_y,char up)
{
	if ((tl_x>=0) && (tl_x<gfx_canvassizex))
	{
		if ((tl_y>=0) && (tl_y<gfx_canvassizey))
		{
			selection_lassobuffer[tl_y*gfx_canvassizex+tl_x]^=up;
			return;
		}
	}
	if (tl_y<-10000) return;
	if (tl_y>=10000) return;
	offscreenlasso(selection_lassohub+10000+tl_y,tl_x,up);
}
void selection_lassotrail(int x,int y,int x2,int y2)
{
	float m;
	float t;
	int tl_x;
	int tl_y;
	char up;
	if (y==y2) return;
	if (y>y2)
	{
		int swap;
		swap=y2;y2=y;y=swap;
		swap=x2;x2=x;x=swap;
		up=2;
		if (selection_lasso_up & up)
		{
			selection_lasso_putpixel(x2,y2,up);
		}
	}
	else
	{
		up=1;
		if (selection_lasso_up & up)
		{
			selection_lasso_putpixel(x,y,up);
		}
	}
	selection_lasso_up=up;
	m=((float)(x2-x))/(y2-y);
	t=x-m*y;
	for (tl_y=y;tl_y<=y2;tl_y++)
	{
		tl_x=t+m*tl_y;
		selection_lasso_putpixel(tl_x,tl_y,up);
	}
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
int findunfragmentedatom()
{
	_u32 icompare=1<<STRUCTURE_OBJECTTYPE_n;
	for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
	{
		if ((*glob_n_multilist).bufferlist[ilv1].exist)
		{
			if ((selection_clickselection[ilv1] & icompare)==0)
			{
				return ilv1;
			}
		}
	}
	return -1;
}
void select_fragment_by_atom(int start)
{
	char ichanged;
	char winetoggle=0;
	_u32 icompare_n,icompare_b;
	icompare_n=1<<STRUCTURE_OBJECTTYPE_n;
	icompare_b=1<<STRUCTURE_OBJECTTYPE_b;
	selection_fragmentselection[start]|=icompare_n;
	selection_clickselection[start]|=icompare_n;
	iback:
	ichanged=0;
	for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
	{
		int ilv1_wine;
		if (winetoggle) ilv1_wine=(*glob_n_multilist).filllevel-1-ilv1; else ilv1_wine=ilv1;
		if ((*glob_n_multilist).bufferlist[ilv1_wine].exist)
		{
			if ((selection_fragmentselection[ilv1_wine] & icompare_n))
			{
				for (int ilv2=0;ilv2<atom_actual_node[ilv1_wine].bondcount;ilv2++)
				{
					int tlbondnr=atom_actual_node[ilv1_wine].bonds[ilv2];
					int tlatomnr=getother(ilv1_wine,tlbondnr);
					if ((*glob_n_multilist).bufferlist[tlatomnr].exist)
					{
						if ((selection_fragmentselection[tlatomnr] & icompare_n)==0)
						{
							if ((selection_clickselection[tlatomnr] & icompare_n)==0)
							{
								selection_fragmentselection[tlatomnr]|=icompare_n;
								selection_fragmentselection[tlbondnr]|=icompare_b;
								selection_clickselection[tlatomnr]|=icompare_n;
								selection_clickselection[tlbondnr]|=icompare_b;
								selection_fragmentselection_found|=icompare_n;
								selection_fragmentselection_found|=icompare_b;
								ichanged=1;
							}
						}
					}
				}
			}
		}
	}
	winetoggle^=1;
	if (ichanged) goto iback;
	for (int ilv1=0;ilv1<(*glob_b_multilist).filllevel;ilv1++)
	{
		b_instance * tl_b_instance=&((*glob_b_multilist).bufferlist[ilv1]);
		if ((*tl_b_instance).exist)
		{
			if (selection_fragmentselection[bond_actual_node[ilv1].start] & icompare_n)
			{
				selection_fragmentselection[ilv1]|=icompare_b;
			}
		}
	}
}
