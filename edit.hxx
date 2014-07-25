//This unit does most processing of the internal data
#define LOCALMACROES\
		LOCALMACRO_1(n)\
		LOCALMACRO_1(b)\
		LOCALMACRO_1(graphic)\
		LOCALMACRO_1(arrow)
struct drawproperties_
{
	_i32 bond_multiplicity;
	_i32 bond_Display1;
	_i32 attribute_tool;
	int color;
	_i32 Element;
};
drawproperties_ control_drawproperties={1,0,4,0,constants_Element_implicitcarbon};
int control_hot[32]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,};
int getbondsum(intl inumber)
{
	float i_bond_sum=0;
	for (int ilv2=0;ilv2<atom_actual_node[inumber].bondcount;ilv2++)
	{
		i_bond_sum+=(*glob_b_multilist).bufferlist[atom_actual_node[inumber].bonds[ilv2]].Order/16.0;
	}
	if (fmod(i_bond_sum,1.0)>0.4)
	{
		i_bond_sum=trunc(i_bond_sum)+1;
	}
	return i_bond_sum;
}
int janitor_getmaxid(_u32 ino)
{
	_u32 iid=0;
	WORKIFIX_REGISTERED_TRADEMARK_workthrough_variables
	icompare=1<<ino;

	basicmultilist * tlmultilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ino].name);
	if (tlmultilist==NULL) return -1;
	ibufferpos=(char*)((*tlmultilist).pointer);
	isize=STRUCTURE_OBJECTTYPE_List[ino].size;
	CDXMLREAD_functype tldummy;
	ioffset=(*tlmultilist).getproperties("id",&tldummy);
	if (ioffset<0) return -1;
	
	for (int ilv2=0;ilv2<(*tlmultilist).filllevel;ilv2++)
	{
		if ((*((basic_instance*)(ibufferpos+isize*ilv2))).exist)
		{
			_u32 tlid=((*((_u32*)(ibufferpos+isize*ilv2+ioffset))));
			if (tlid>=iid)
			{
				iid=tlid+1;
			}
		}
	}
	(*tlmultilist).maxid=iid;
	return 0;
}
int edit_getnewZ()
{
	return 0;//TODO pretty urgent :)(!)
}
int for_all_objects(catalogized_command_iterated_functype ifunc,char selectiononly=0)
{
	char erledigt=0;
	for (int ilv1=0;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
	{
		basicmultilist * tl_multilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
		_u32  icompare=(1<<ilv1);
		int tl_size=(*tl_multilist).itemsize;
		int ifilllevel=(*tl_multilist).filllevel;//separately, so it doesn't grow while executing the loop
		for (int ilv2=0;ilv2<ifilllevel;ilv2++)
		{
			if ((selection_currentselection[ilv2] & icompare) || (selectiononly==0))
			{
				if ((*tl_multilist)[ilv2].exist)
				{
					if (((catalogized_command_iterated_functype)(ifunc))(hotkeylist[ilv1].variable,hotkeylist[ilv1].value,tl_multilist,&((*tl_multilist)[ilv2]),ilv2)){erledigt=1;};
				}
			}
		}
	}
	return erledigt;
}
char getleftof(cdx_Point3D * istart,cdx_Point3D * iend,cdx_Point3D * ikink)
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
	return -3;
}
basic_instance * gethot(int ino,int * nr=NULL)
{
	basicmultilist * tl_multilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ino].name);
	if ((control_hot[ino]>=0) && (control_hot[ino]<(*tl_multilist).filllevel))
	{
		if ((*(basic_instance*)(((char*)((*tl_multilist).pointer))+(control_hot[ino]*STRUCTURE_OBJECTTYPE_List[ino].size))).exist)
		{
			if (nr!=NULL)
			{
				(*nr)=control_hot[ino];
			}
		}
	}
	if (nr!=NULL)
	{
		*(nr)=-1;
	}
	return NULL;
}
inline int retrieveprops_n(int what)
{
	return 0;
}
inline int retrieveprops_b(int what)
{
	return 0;
}
inline int retrieveprops_graphic(int what)
{
	if (what==-1)
	{
		return 5;
	}
	if (what==1)
	{
		return 2;
	}
	return 0;
}
inline int retrieveprops_arrow(int what)
{
	if (what==-1)
	{
		return 5;
	}
	if (what==1)
	{
		return 2;
	}
	return 0;
}
inline int retrievepoints(n_instance * iinstance,float * ix,float * iy,int inumber)
{
	if (inumber>0) return 0;
	if (inumber<0) return 0;
	(*ix)=(*iinstance).xyz.x;
	(*iy)=(*iinstance).xyz.y;
	return 1;
}
inline int retrievepoints(b_instance * iinstance,float * ix,float * iy,int inumber)
{
	if (inumber>0) return 0;
	if (inumber<0) return 0;
	n_instance *iinstance1=NULL;
	n_instance *iinstance2=NULL;
	for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
	{
		n_instance * tlinstance=&((*glob_n_multilist).bufferlist[ilv1]);
		if ((*tlinstance).exist)
		{
			if ((*tlinstance).id==(*iinstance).B)
			{
				iinstance1=tlinstance;
			}
			if ((*tlinstance).id==(*iinstance).E)
			{
				iinstance2=tlinstance;
			}
		}
	}
	if (iinstance1==NULL) {return -3;}
	if (iinstance2==NULL) {return -3;}
	(*ix)=((*iinstance1).xyz.x+(*iinstance2).xyz.x)*0.5;
	(*iy)=((*iinstance1).xyz.y+(*iinstance2).xyz.y)*0.5;
	return 1;
}
inline int retrievepoints(graphic_instance * iinstance,float * ix,float * iy,int inumber)
{
	if (inumber>2) return 0;
	if (inumber<-5) return 0;
	if (inumber==-3)
	{
		*ix=(*iinstance).Center3D.x;
		*iy=(*iinstance).Center3D.y;
		return 1;
	}
	if (inumber==-4)
	{
		*ix=(*iinstance).MajorAxisEnd3D.x;
		*iy=(*iinstance).MajorAxisEnd3D.y;
		return 1;
	}
	if (inumber==-5)
	{
		*ix=(*iinstance).MinorAxisEnd3D.x;
		*iy=(*iinstance).MinorAxisEnd3D.y;
		return 1;
	}
	if ((inumber==1) || (inumber==-1))
	{
		(*ix)=(*iinstance).BoundingBox.left;
		(*iy)=(*iinstance).BoundingBox.top;
		return 1;
	}
	if ((inumber==2) || (inumber==-2))
	{
		(*ix)=(*iinstance).BoundingBox.right;
		(*iy)=(*iinstance).BoundingBox.bottom;
		return 1;
	}
	(*ix)=((*iinstance).BoundingBox.right+(*iinstance).BoundingBox.left)/2;
	(*iy)=((*iinstance).BoundingBox.bottom+(*iinstance).BoundingBox.top)/2;
	return 1;
}
inline int retrievepoints(arrow_instance * iinstance,float * ix,float * iy,int inumber)
{
	if (inumber>2) return 0;
	if (inumber<-5) return 0;
	if (inumber==-1)
	{
		*ix=(*iinstance).Head3D.x;
		*iy=(*iinstance).Head3D.y;
		return 1;
	}
	if (inumber==-2)
	{
		*ix=(*iinstance).Tail3D.x;
		*iy=(*iinstance).Tail3D.y;
		return 1;
	}
	if (inumber==-3)
	{
		*ix=(*iinstance).Center3D.x;
		*iy=(*iinstance).Center3D.y;
		return 1;
	}
	if (inumber==-4)
	{
		*ix=(*iinstance).MajorAxisEnd3D.x;
		*iy=(*iinstance).MajorAxisEnd3D.y;
		return 1;
	}
	if (inumber==-5)
	{
		*ix=(*iinstance).MinorAxisEnd3D.x;
		*iy=(*iinstance).MinorAxisEnd3D.y;
		return 1;
	}
	if (inumber==1)
	{
		*ix=(*iinstance).Head3D.x;
		*iy=(*iinstance).Head3D.y;
		return 1;
	}
	if (inumber==2)
	{
		*ix=(*iinstance).Tail3D.x;
		*iy=(*iinstance).Tail3D.y;
		return 1;
	}
	(*ix)=((*iinstance).Head3D.x+(*iinstance).Tail3D.x)/2;
	(*iy)=((*iinstance).Head3D.y+(*iinstance).Tail3D.y)/2;
	return 1;
}
int hit(n_instance * iinstance,float ix,float iy,int ino)
{
	if ((sqr(ix-(*iinstance).xyz.x)+sqr(iy-(*iinstance).xyz.y))<glob_clickradius)
	{
		return 1;
	}
	return 0;
}
int hit(b_instance * iinstance,float ix,float iy,int ino)
{
	n_instance *iinstance1=NULL;
	n_instance *iinstance2=NULL;
	for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
	{
		n_instance * tlinstance=&((*glob_n_multilist).bufferlist[ilv1]);
		if ((*tlinstance).exist)
		{
			if ((*tlinstance).id==(*iinstance).B)
			{
				iinstance1=tlinstance;
			}
			if ((*tlinstance).id==(*iinstance).E)
			{
				iinstance2=tlinstance;
			}
		}
	}
	if (iinstance1==NULL) {return -3;}
	if (iinstance2==NULL) {return -3;}
	if ((sqr(ix-((*iinstance1).xyz.x+(*iinstance2).xyz.x)*0.5)+sqr(iy-((*iinstance1).xyz.y+(*iinstance2).xyz.y)*0.5))<40)
	{
		return 1;
	}
	return 0;
}
int hit(graphic_instance * iinstance,float ix,float iy,int ino)
{
	float tl_x,tl_y;
	retrievepoints(iinstance,&tl_x,&tl_y,ino);
	if ((sqr(ix-tl_x)+sqr(iy-tl_y))<glob_clickradius)
	{
		return 1;
	}
	return 0;
}
int hit(arrow_instance * iinstance,float ix,float iy,int ino)
{
	float tl_x,tl_y;
	retrievepoints(iinstance,&tl_x,&tl_y,ino);
	if ((sqr(ix-tl_x)+sqr(iy-tl_y))<glob_clickradius)
	{
		return 1;
	}
	return 0;
}
inline int placepoints(n_instance * iinstance,float ix,float iy,int inumber)
{
	if (inumber>0) return 0;
	if (inumber<0) return 0;
	(*iinstance).xyz.x=ix;
	(*iinstance).xyz.y=iy;
	return 1;
}
inline int placepoints(b_instance * iinstance,float ix,float iy,int inumber)
{
	if (inumber>0) return 0;
	if (inumber<0) return 0;
	float tl_x,tl_y;
	retrievepoints(iinstance,&tl_x,&tl_y,0);
	tl_x-=ix;
	tl_y-=iy;
	n_instance *iinstance1=NULL;
	n_instance *iinstance2=NULL;
	for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
	{
		n_instance * tlinstance=&((*glob_n_multilist).bufferlist[ilv1]);
		if ((*tlinstance).exist)
		{
			if ((*tlinstance).id==(*iinstance).B)
			{
				if (selection_currentselection[ilv1] & (1<<STRUCTURE_OBJECTTYPE_n)) return 0;
				iinstance1=tlinstance;
			}
			if ((*tlinstance).id==(*iinstance).E)
			{
				if (selection_currentselection[ilv1] & (1<<STRUCTURE_OBJECTTYPE_n)) return 0;
				iinstance2=tlinstance;
			}
		}
	}
	if (iinstance1==NULL) {return -3;}
	if (iinstance2==NULL) {return -3;}
	(*iinstance1).xyz.x-=tl_x;
	(*iinstance1).xyz.y-=tl_y;
	return 1;
}
inline int placepoints(graphic_instance * iinstance,float ix,float iy,int inumber)
{
	if (inumber>2) return 0;
	if (inumber<-5) return 0;
	if (inumber==-3)
	{
		(*iinstance).Center3D.x=ix;
		(*iinstance).Center3D.y=iy;
		return 1;
	}
	if (inumber==-4)
	{
		(*iinstance).MajorAxisEnd3D.x=ix;
		(*iinstance).MajorAxisEnd3D.y=iy;
		return 1;
	}
	if (inumber==-5)
	{
		(*iinstance).MinorAxisEnd3D.x=ix;
		(*iinstance).MinorAxisEnd3D.y=iy;
		return 1;
	}
	if ((inumber==1) || (inumber==-1))
	{
		(*iinstance).BoundingBox.left=ix;
		(*iinstance).BoundingBox.top=iy;
		return 1;
	}
	if ((inumber==2) || (inumber==-2))
	{
		(*iinstance).BoundingBox.right=ix;
		(*iinstance).BoundingBox.bottom=iy;
		return 1;
	}
	float tl_x,tl_y;
	retrievepoints(iinstance,&tl_x,&tl_y,inumber);
	tl_x-=ix;
	tl_y-=iy;
	(*iinstance).BoundingBox.left-=tl_x;
	(*iinstance).BoundingBox.top-=tl_y;
	(*iinstance).BoundingBox.right-=tl_x;
	(*iinstance).BoundingBox.bottom-=tl_y;
	return 1;
}
inline int placepoints(arrow_instance * iinstance,float ix,float iy,int inumber)
{
	if (inumber>2) return 0;
	if (inumber<-5) return 0;
	if (inumber==-1)
	{
		(*iinstance).Head3D.x=ix;
		(*iinstance).Head3D.y=iy;
		return 1;
	}
	if (inumber==-2)
	{
		(*iinstance).Tail3D.x=ix;
		(*iinstance).Tail3D.y=iy;
		return 1;
	}
	if (inumber==-3)
	{
		(*iinstance).Center3D.x=ix;
		(*iinstance).Center3D.y=iy;
		return 1;
	}
	if (inumber==-4)
	{
		(*iinstance).MajorAxisEnd3D.x=ix;
		(*iinstance).MajorAxisEnd3D.y=iy;
		return 1;
	}
	if (inumber==-5)
	{
		(*iinstance).MinorAxisEnd3D.x=ix;
		(*iinstance).MinorAxisEnd3D.y=iy;
		return 1;
	}
	float tl_x,tl_y;
	retrievepoints(iinstance,&tl_x,&tl_y,inumber);
	tl_x-=ix;
	tl_y-=iy;
	if (inumber==1)
	{
		(*iinstance).Head3D.x-=tl_x;
		(*iinstance).Head3D.y-=tl_y;
		return 1;
	}
	if (inumber==2)
	{
		(*iinstance).Tail3D.x-=tl_x;
		(*iinstance).Tail3D.y-=tl_y;
		return 1;
	}
	(*iinstance).Head3D.x-=tl_x;
	(*iinstance).Head3D.y-=tl_y;
	(*iinstance).Tail3D.x-=tl_x;
	(*iinstance).Tail3D.y-=tl_y;
	(*iinstance).Center3D.x-=tl_x;
	(*iinstance).Center3D.y-=tl_y;
	(*iinstance).MajorAxisEnd3D.x-=tl_x;
	(*iinstance).MajorAxisEnd3D.y-=tl_y;
	(*iinstance).MinorAxisEnd3D.x-=tl_x;
	(*iinstance).MinorAxisEnd3D.y-=tl_y;
	return 1;
}
#define LOCALMACRO_1(whatabout) case STRUCTURE_OBJECTTYPE_ ## whatabout: return retrieveprops_ ## whatabout(what);break;
int retrieveprops_basic(int what,int objecttype)
{
	switch (objecttype)
	{
		LOCALMACROES
	}
	return 0;
}
#undef LOCALMACRO_1
template <class thisinstance> inline _u32 clickfor_template(float posx,float posy,int objecttype,_u32 mode)
{
	_u32 found=0;
	int internalpointcount;
	multilist<thisinstance> * imultilist=retrievemultilist<thisinstance>();
	for (int ilv1=0;ilv1<(*imultilist).filllevel;ilv1++)
	{
		internalpointcount=retrieveprops_basic(1,objecttype);
		thisinstance * tlinstance=&((*imultilist).bufferlist[ilv1]);
		if ((*tlinstance).exist)
		{
			if (mode & 1) if (hit(tlinstance,posx,posy,0)>0)
			{
				selection_clickselection[ilv1]|=(1<<objecttype);
				found|=1<<objecttype;
			}
			if (mode & 2) for (int ilv3=1;ilv3<internalpointcount+1;ilv3++)
			{
				if (hit(tlinstance,posx,posy,ilv3)>0)
				{
					selection_clickselection[ilv1*internalpointcount+ilv3-1]|=(1<<(objecttype+STRUCTURE_OBJECTTYPE_ListSize));
					found|=1<<(objecttype+STRUCTURE_OBJECTTYPE_ListSize);
				}
			}
		}
	}
	return found;
}

#define LOCALMACRO_1(whatabout) case STRUCTURE_OBJECTTYPE_ ## whatabout: return retrievepoints((whatabout ## _instance*)iinstance,ix,iy,inumber);break;
int retrievepoints_basic(basic_instance * iinstance,float * ix,float * iy,int inumber,int objecttype)
{
	switch (objecttype)
	{
		LOCALMACROES
	}
	return 0;
}
#undef LOCALMACRO_1
#define LOCALMACRO_1(whatabout) case STRUCTURE_OBJECTTYPE_ ## whatabout: return placepoints((whatabout ## _instance*)iinstance,ix,iy,inumber);break;
int placepoints_basic(basic_instance * iinstance,float ix,float iy,int inumber,int objecttype)
{
	switch (objecttype)
	{
		LOCALMACROES
	}
	return 0;
}
#undef LOCALMACRO_1
#define LOCALMACRO_1(whatabout) case STRUCTURE_OBJECTTYPE_ ## whatabout: return clickfor_template<whatabout ## _instance>(ix,iy,objecttype,mode);break;
_u32 clickfor(float ix,float iy,int objecttype,float iclickradius=constants_clickradius,_u32 mode=1)
{
	glob_clickradius=iclickradius;
	switch (objecttype)
	{
		LOCALMACRO_1(n)
		LOCALMACRO_1(b)
		LOCALMACRO_1(graphic)
		LOCALMACRO_1(arrow)
	}
	return 0;
}
#undef LOCALMACRO_1

template <class whatabout> inline int retrievepoints_relay(whatabout * iinstance,float * ix,float * iy,int inumber)//template may become unnecessary
{
	return retrievepoints(iinstance,ix,iy,inumber);
}

n_instance * snapatom(float posx,float posy,_small * iatomnr=NULL)
{
	float best=2000000000;
	n_instance * bestatom=NULL;
	for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
	{
		n_instance * tlatom=&((*glob_n_multilist).bufferlist[ilv1]);
		if ((*tlatom).exist)
		{
			if (selection_clickselection[ilv1] & (1<<STRUCTURE_OBJECTTYPE_n))
			{
				float current=sqr(posx-(*tlatom).xyz.x)+sqr(posy-(*tlatom).xyz.y);
				if (current<best)
				{
					if (iatomnr!=NULL) *iatomnr=ilv1;
					best=current;
					bestatom=tlatom;
				}
			}
		}
	}
	return bestatom;
}
n_instance * snapatom_short(float iposx,float iposy,_small * iatomnr=NULL,int idistance=100)
{
	for (_small ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
	{
		n_instance * tlatom=&((*glob_n_multilist).bufferlist[ilv1]);
		if ((*tlatom).exist)
		{
			if ((sqr(iposx-(*tlatom).xyz.x)+sqr(iposy-(*tlatom).xyz.y))<idistance)
			{
				if (iatomnr!=NULL) *iatomnr=ilv1;
				return tlatom;
			}
		}
	}
	return NULL;
}

basic_instance * getclicked(int ino)
{
	_u32 compare=1<<ino;
	basicmultilist * tlmultilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ino].name);
	for (int ilv1=0;ilv1<(*tlmultilist).filllevel;ilv1++)
	{
		if (selection_clickselection[ilv1] & compare)
		{
			basic_instance * tlinstance=(basic_instance*)(((char*)(*tlmultilist).pointer)+STRUCTURE_OBJECTTYPE_List[ino].size*ilv1);
			if ((*tlinstance).exist)
			{
				return tlinstance;
			}
		}
	}
	return NULL;
}

basic_instance * edit_locatebyid(_u32 ino,int iid,int * number=NULL)
{
	WORKIFIX_REGISTERED_TRADEMARK_workthrough_variables
	icompare=1<<ino;

	basicmultilist * tlmultilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ino].name);
	if (tlmultilist==NULL) return NULL;
	ibufferpos=(char*)((*tlmultilist).pointer);
	isize=STRUCTURE_OBJECTTYPE_List[ino].size;
	CDXMLREAD_functype tldummy;
	ioffset=(*tlmultilist).getproperties("id",&tldummy);
	if (ioffset<0) return NULL;
	
	for (int ilv2=0;ilv2<(*tlmultilist).filllevel;ilv2++)
	{
		if ((*((basic_instance*)(ibufferpos+isize*ilv2))).exist)
		{
			_u32 tlid=((*((_u32*)(ibufferpos+isize*ilv2+ioffset))));
			if (tlid==iid)
			{
				if (number!=NULL)
				{
					*number=ilv2;
				}
				return ((((basic_instance*)(ibufferpos+isize*ilv2))));
			}
		}
	}
	return NULL;
}

n_instance * summonatom(int * inr=NULL)
{
	if ((*glob_n_multilist).filllevel<bufferlistsize)
	{
		int tl_nr=-1;
		n_instance * tlinstance;
		if (janitor_getmaxid(STRUCTURE_OBJECTTYPE_n)==-1) return NULL;
		tl_nr=(*glob_n_multilist).filllevel;
		tlinstance=new(&((*glob_n_multilist).bufferlist[tl_nr])) n_instance;
		(*tlinstance).Element=constants_Element_implicitcarbon;
		(*tlinstance).color=control_drawproperties.color;
		(*tlinstance).charge=0;
		(*tlinstance).protons=4;
		selection_currentselection[tl_nr]&=(~(1<<STRUCTURE_OBJECTTYPE_n));
		atom_actual_node[tl_nr].bondcount=0;
		((*glob_n_multilist).filllevel)++;
		(*tlinstance).id=(*glob_n_multilist).maxid;
		(*glob_n_multilist).maxid++;
		if (inr!=NULL)
		{
			*inr=tl_nr;
		}
		return tlinstance;
		
	}
	return NULL;
}
arrow_instance * summonarrow(int * inr=NULL)
{
	if ((*glob_arrow_multilist).filllevel<bufferlistsize)
	{
		int tl_nr=-1;
		arrow_instance * tlinstance;
		tl_nr=(*glob_arrow_multilist).filllevel;
		tlinstance=new(&((*glob_arrow_multilist).bufferlist[tl_nr])) arrow_instance;
		selection_currentselection[tl_nr]&=(~(1<<STRUCTURE_OBJECTTYPE_arrow));
		(*tlinstance).color=control_drawproperties.color;
		((*glob_arrow_multilist).filllevel)++;
		if (inr!=NULL)
		{
			*inr=tl_nr;
		}
		return tlinstance;
	}
	return NULL;
}
b_instance * summonbond(int i_id_begin,int i_id_end,int i_nr_begin,int i_nr_end)
{
	if ((*glob_b_multilist).filllevel<bufferlistsize)
	{
		b_instance * tlinstance;
		tlinstance=new(&((*glob_b_multilist).bufferlist[(*glob_b_multilist).filllevel])) b_instance;
		selection_currentselection[(*glob_b_multilist).filllevel]&=(~(1<<STRUCTURE_OBJECTTYPE_b));
		(*tlinstance).B=i_id_begin;
		(*tlinstance).E=i_id_end;
		(*tlinstance).Display=control_drawproperties.bond_Display1;
		(*tlinstance).Order=0x10;
		(*tlinstance).color=control_drawproperties.color;
		atom_actual_node[i_nr_begin]+=((*glob_b_multilist).filllevel);
		atom_actual_node[i_nr_end]+=((*glob_b_multilist).filllevel);
		((*glob_b_multilist).filllevel)++;
		return tlinstance;
	}
	return NULL;
}
float edit_get_atom_best_free_side(int atomnr)
{
//	atom_actual_node//Should I really rely on them? better leave them as they were. But then it wont work for NEW atoms...
	float angle,angle2;
	float best=0;int bestnr=-1;float bestvalue=0;
	for (int ilv0=0;ilv0<atom_actual_node[atomnr].bondcount;ilv0++)
	{
		for (int ilv1=ilv0;ilv1<atom_actual_node[atomnr].bondcount;ilv1++)
		{
			int atomnr2=getother(atomnr,atom_actual_node[atomnr].bonds[ilv1]);
			int atomnr2b=getother(atomnr,atom_actual_node[atomnr].bonds[ilv0]);
			for (int ilv2=0;ilv2<2;ilv2++)
			{
				angle=fmod(0.5*(getangle((*glob_n_multilist).bufferlist[atomnr2].xyz.x-(*glob_n_multilist).bufferlist[atomnr].xyz.x,(*glob_n_multilist).bufferlist[atomnr2].xyz.y-(*glob_n_multilist).bufferlist[atomnr].xyz.y)+getangle((*glob_n_multilist).bufferlist[atomnr2b].xyz.x-(*glob_n_multilist).bufferlist[atomnr].xyz.x,(*glob_n_multilist).bufferlist[atomnr2b].xyz.y-(*glob_n_multilist).bufferlist[atomnr].xyz.y))+constants_angular_distance*(ilv2?1:-1)+4*Pi,2*Pi);
				float worst=2*Pi;int worstnr=-1;
				for (int ilv3=0;ilv3<atom_actual_node[atomnr].bondcount;ilv3++)
				{
					int atomnr3=getother(atomnr,atom_actual_node[atomnr].bonds[ilv3]);
					float angle2=fmod(getangle((*glob_n_multilist).bufferlist[atomnr3].xyz.x-(*glob_n_multilist).bufferlist[atomnr].xyz.x,(*glob_n_multilist).bufferlist[atomnr3].xyz.y-(*glob_n_multilist).bufferlist[atomnr].xyz.y)-angle+4*Pi,2*Pi);
					if (angle2>Pi)
					{
						angle2-=2*Pi;
					}
					angle2=fabs(angle2);
					if (angle2<worst)
					{
						worst=angle2;
						worstnr=ilv3;
					}
				}
				if (worst>best)
				{
					bestnr=worstnr;
					bestvalue=angle;
					best=worst;
				}
			}
		}
	}
	if (bestnr!=-1)
	{
		return bestvalue;
	}
	return (Pi-constants_angular_distance)/2;
}
int edit_errichten(int startatom)
{
	int atomnr2=-1;
	n_instance * tlatom=&((*glob_n_multilist).bufferlist[startatom]);
	if (tlatom==0) {return -1;}
	n_instance * tlatom2;
	b_instance * tlbond;
	if (atom_actual_node[startatom].bondcount==constants_maxbonds) return -1;
	float tlangle;
	tlangle=edit_get_atom_best_free_side(startatom);
	float posx=(*tlatom).xyz.x+constants_bondlength*cos(tlangle);
	float posy=(*tlatom).xyz.y+constants_bondlength*sin(tlangle);
	if (tlatom2=snapatom_short(posx,posy,&atomnr2,10))
	{
	}
	else
	{
		tlatom2=summonatom(&atomnr2);
		if (tlatom2)
		{
			(*tlatom2).xyz.x=posx;
			(*tlatom2).xyz.y=posy;
			(*tlatom2).xyz.z=(*tlatom).xyz.z;
			(*tlatom2).Z=(*tlatom).Z;
		}
	}
	if ((tlatom) && (tlatom2))
	{
		tlbond=summonbond((*tlatom).id,(*tlatom2).id,startatom,atomnr2);
		if (tlbond)
		{
			(*tlbond).Z=(*tlatom).Z;
		}
	}
	return atomnr2;
}
catalogized_command_funcdef(SPROUT2)
{
	int counter=0;
	if (selection_currentselection_found & (1<<STRUCTURE_OBJECTTYPE_n))
	{
		for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
		{
			if (selection_currentselection[ilv1] & (1<<STRUCTURE_OBJECTTYPE_n))
			{
				if ((*glob_n_multilist).bufferlist[ilv1].exist)
				{ 
					counter++;
					edit_errichten(ilv1);
				}
			}
		}
		return counter;
	}
	int tlatom;
	gethot(STRUCTURE_OBJECTTYPE_n,&tlatom);
	if (tlatom!=-1)
	{
		int sproutcount=atoi(value);
		for (int ilv1=0;ilv1<sproutcount;ilv1++)
		{
			edit_errichten(ilv1);
		}
		return 1;
	}
	return 0;
}
catalogized_command_iterated_funcdef(SPROUT)
{
	int found=0;
	for (int ilv1=0;ilv1<atoi(value);ilv1++)
	{
		if ((control_hot[STRUCTURE_OBJECTTYPE_n]=edit_errichten(iindex))>=0) {getatoms();found++;}
	}
	return found;
}
int edit_setelement(int i_Elementnr,n_instance * iinstance,int iindex)
{
	if ((*(n_instance*)iinstance).Element!=i_Elementnr)
	{
		if ((element[i_Elementnr].maxbonds!=element[(*(n_instance*)iinstance).Element].maxbonds) || (element[i_Elementnr].hasVE!=element[(*(n_instance*)iinstance).Element].hasVE))
		{
			(*(n_instance*)iinstance).protons=element[i_Elementnr].maxbonds;
		}
		(*(n_instance*)iinstance).Element=i_Elementnr;
	}
	else
	{
		(*(n_instance*)iinstance).protons-=1;
		if ((*(n_instance*)iinstance).protons-getbondsum(iindex)<0)
		{
			(*(n_instance*)iinstance).protons=element[i_Elementnr].maxbonds;
		}
	}
	if ((*(n_instance*)iinstance).Element==constants_Element_implicitcarbon)
	{
		(*(n_instance*)iinstance).protons=4-abs((*(n_instance*)iinstance).charge);
	}
	if (element[i_Elementnr].maxbonds!=-1)
	{
		if ((*(n_instance*)iinstance).protons>8-(element[i_Elementnr].hasVE-(*(n_instance*)iinstance).charge))
		{
			(*(n_instance*)iinstance).protons=8-(element[i_Elementnr].hasVE-(*(n_instance*)iinstance).charge);
		}
		if ((*(n_instance*)iinstance).protons>element[i_Elementnr].hasVE-(*(n_instance*)iinstance).charge)
		{
			(*(n_instance*)iinstance).protons=element[i_Elementnr].hasVE-(*(n_instance*)iinstance).charge;
		}
	}
	return 1;
}
catalogized_command_iterated_funcdef(LABELTEXT)
{
	for (int ilv1=0;ilv1<sizeof(element)/sizeof(element_);ilv1++)
	{
		if (strcmp(value,element[ilv1].name)==0)
		{
			return edit_setelement(ilv1,(n_instance*)iinstance,iindex);
		}
	}
	return 0;
}
catalogized_command_iterated_funcdef(LASTLABELTEXT)
{
	LABELTEXT(parameter,element[control_drawproperties.Element].name,imultilist,iinstance,iindex);
}
catalogized_command_iterated_funcdef(SETITEMVARIABLE)
{
	for (int ilv1=0;ilv1<(*iinstance)._->properties_count;ilv1++)
	{
		if (strcmp((*iinstance)._->properties[ilv1].name,parameter)==0)
		{
			*(((char*)iinstance)+((*iinstance)._->properties[ilv1].ref))=atoi(value);//TODO: other things than chars!
			return 1;
		}
	}
	return 0;
}
catalogized_command_funcdef(FILE_NEW)
{
	for (int ilv1=1;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
	{
		basicmultilist * tlmultilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
		if (tlmultilist)
		{
			(*tlmultilist).filllevel=0;
			//TODO: undo: buffer lists!
		}
	}
	return 1;
}
extern float control_coorsx;
extern float control_coorsy;
void checkupinconsistencies();
catalogized_command_funcdef(CHUP)
{
	checkupinconsistencies();
	return 1;
}
catalogized_command_funcdef(BLOT)
{
	int issueshiftstart();
	selection_clearselection(selection_clickselection);
	clickfor(control_coorsx,control_coorsy,STRUCTURE_OBJECTTYPE_n,1000,1);
	n_instance * iinstance=(n_instance*)getclicked(STRUCTURE_OBJECTTYPE_n);
	printf("|%i,%lli,%lli\n",(*glob_n_multilist).maxid,(*glob_n_multilist).filllevel,(*glob_b_multilist).filllevel);
	if (iinstance)
	{
		printf("%f,%f,%i\n",(*iinstance).xyz.x,(*iinstance).xyz.y,(*iinstance).id);
	}
	return 1;
}
void edit_scoopcolors(basic_instance * master)
{
	int ilv1,ilv2,ilv3;
	_u32 icolor;
	unsigned char r,g,b;
	CDXMLREAD_functype idummy;
	for (int ilv1=0;ilv1<8;ilv1++)
	{
		ADD_TO_MULTILISTREFERENCE(master,color);
	}
	for (ilv1=0;ilv1<8;ilv1++)//sortta explanation for da weird colortable order, roflz
	{
		r=ilv1%2;
		g=(ilv1>>1)%2;
		b=(ilv1>>2)%2;
		if (b) {r^=1;g^=r;r^=g;}else{r^=g;}
		(*glob_CAMBRIDGE_color_multilist).bufferlist[(ilv1+1)%8].b=b;
		(*glob_CAMBRIDGE_color_multilist).bufferlist[(ilv1+1)%8].g=g;
		(*glob_CAMBRIDGE_color_multilist).bufferlist[(ilv1+1)%8].r=r;
	}
	basicmultilist * tl_multilist;
	for (ilv1=1;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
	{
		tl_multilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
		if (tl_multilist!=NULL)
		{
			int icompare=(1<<ilv1);
			int tl_size=(*tl_multilist).itemsize;
			int ifilllevel=(*tl_multilist).filllevel;//separately, so it doesn't grow while executing the loop
			int ioffset=(*tl_multilist).getproperties("color",&idummy);
			if (ioffset!=-1)
			{
				for (ilv2=0;ilv2<ifilllevel;ilv2++)
				{
					if ((*tl_multilist)[ilv2].exist)
					{
						icolor=*(_u32*)(((char*)(&((*tl_multilist)[ilv2])))+ioffset);
						r=(icolor&0xFF0000)>>16;
						g=(icolor&0xFF00)>>8;
						b=(icolor&0xFF);
						for (ilv3=0;ilv3<(*glob_CAMBRIDGE_color_multilist).filllevel;ilv3++)
						{
							if ((((unsigned char)((*((CAMBRIDGE_color_instance*)&((*glob_CAMBRIDGE_color_multilist)[ilv3]))).r*255))==r) && 
							(((unsigned char)((*((CAMBRIDGE_color_instance*)&((*glob_CAMBRIDGE_color_multilist)[ilv3]))).g*255))==g) && 
							(((unsigned char)((*((CAMBRIDGE_color_instance*)&((*glob_CAMBRIDGE_color_multilist)[ilv3]))).b*255))==b))
							{
								goto idone;
							}
						}
						ADD_TO_MULTILISTREFERENCE(master,color);
						(*tl_CAMBRIDGE_color_instance).r=r/255.0;
						(*tl_CAMBRIDGE_color_instance).g=g/255.0;
						(*tl_CAMBRIDGE_color_instance).b=b/255.0;
						printf("r:%i,g:%i,b:%i\n",r,g,b);
						printf("r:%f,g:%f,b:%f\n",(*tl_CAMBRIDGE_color_instance).r,(*tl_CAMBRIDGE_color_instance).g,(*tl_CAMBRIDGE_color_instance).b);
						idone:;
					}
				}
			}
		}
	}
}
catalogized_command_funcdef(SAVE_TYPE)
{
	FILE * ifile=fopen(parameter,"w");
	if (ifile==NULL) return 0;
	fprintf(ifile,"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
	fprintf(ifile,"<!DOCTYPE CDXML SYSTEM \"http://www.cambridgesoft.com/xml/cdxml.dtd\" >\n");
	for (int ilv1=0;ilv1<multilist_count;ilv1++)
	{
		if (multilistlist[ilv1].usage==1)
		{
			(*(multilistlist[ilv1].instance)).filllevel=0;
		}
	}
	CAMBRIDGE_colortable_instance i_CAMBRIDGE_colortable_instance=CAMBRIDGE_colortable_instance();
	(*glob_CAMBRIDGE_colortable_multilist).ADD(&i_CAMBRIDGE_colortable_instance);
	edit_scoopcolors(&i_CAMBRIDGE_colortable_instance);
	fprintf(ifile,"%s","<CDXML\n"
" CreationProgram=\"ChemDraw 12.0.2.1076\"\n"
" Name=\"whocares.cdxml\"\n"
" BoundingBox=\"5.5 155.04 724.05 736.75\"\n"
" WindowPosition=\"-1073741824 1073741824\"\n"
" WindowSize=\"0 0\"\n"
" FractionalWidths=\"yes\"\n"
" InterpretChemically=\"yes\"\n"
" ShowAtomQuery=\"yes\"\n"
" ShowAtomStereo=\"no\"\n"
" ShowAtomEnhancedStereo=\"yes\"\n"
" ShowAtomNumber=\"no\"\n"
" ShowBondQuery=\"yes\"\n"
" ShowBondRxn=\"yes\"\n"
" ShowBondStereo=\"no\"\n"
" ShowTerminalCarbonLabels=\"no\"\n"
" ShowNonTerminalCarbonLabels=\"no\"\n"
" HideImplicitHydrogens=\"no\"\n"
" LabelFont=\"3\"\n"
" LabelSize=\"18\"\n"
" LabelFace=\"96\"\n"
" CaptionFont=\"3\"\n"
" CaptionSize=\"18\"\n"
" HashSpacing=\"2.7\"\n"
" MarginWidth=\"2\"\n"
" LineWidth=\"1\"\n"
" BoldWidth=\"4\"\n"
" BondLength=\"30\"\n"
" BondSpacing=\"12\"\n"
" ChainAngle=\"120\"\n"
" LabelJustification=\"Auto\"\n"
" CaptionJustification=\"Left\"\n"
" AminoAcidTermini=\"NH2COOH\"\n"
" ShowSequenceTermini=\"yes\"\n"
" ShowSequenceBonds=\"yes\"\n"
" PrintMargins=\"10.71 12.93 10.71 12.93\"\n"
" MacPrintInfo=\"00030000025802580000000019641380FFCEFFE7199613D30367052803FC00020000025802580000000019641380000100640064000000010001010100000001270F000100010000000000000000000000000002001901900000000000600000000000000000000100000000000000000000000000000000\"\n"
" color=\"0\"\n"
" bgcolor=\"1\"\n"
"><colortable>\n");
for (int ilv1=0;ilv1<(*glob_CAMBRIDGE_color_multilist).filllevel;ilv1++)
{
	fprintf(ifile," <color r=\"%f\" g=\"%f\" b=\"%f\"/>\n",(*glob_CAMBRIDGE_color_multilist).bufferlist[ilv1].r,(*glob_CAMBRIDGE_color_multilist).bufferlist[ilv1].g,(*glob_CAMBRIDGE_color_multilist).bufferlist[ilv1].b);
}

fprintf(ifile,"%s","</colortable><fonttable>\n"
"<font id=\"1\" charset=\"windows-1258\" name=\"Arial\"/>\n"
"<font id=\"2\" charset=\"Unknown\" name=\"Symbol\"/>\n"
"</fonttable>");//TODO: font id's(!)
	//TODO: all subobjects of page must get filllevel=0 before add!
	CAMBRIDGE_page_instance i_CAMBRIDGE_page_instance=CAMBRIDGE_page_instance();
	i_CAMBRIDGE_page_instance.id=0;AUTOSTRUCT_EXISTS_SET_NAME(&i_CAMBRIDGE_page_instance,id);
	i_CAMBRIDGE_page_instance.BoundingBox.left=0;AUTOSTRUCT_EXISTS_SET_NAME(&i_CAMBRIDGE_page_instance,BoundingBox);
	i_CAMBRIDGE_page_instance.BoundingBox.top=0;
	i_CAMBRIDGE_page_instance.BoundingBox.right=28346.46;
	i_CAMBRIDGE_page_instance.BoundingBox.bottom=28346.46;
	i_CAMBRIDGE_page_instance.Width=28346.46;AUTOSTRUCT_EXISTS_SET_NAME(&i_CAMBRIDGE_page_instance,Width);
	i_CAMBRIDGE_page_instance.Height=28346.46;AUTOSTRUCT_EXISTS_SET_NAME(&i_CAMBRIDGE_page_instance,Height);
	i_CAMBRIDGE_page_instance.HeaderPosition=36;AUTOSTRUCT_EXISTS_SET_NAME(&i_CAMBRIDGE_page_instance,HeaderPosition);
	i_CAMBRIDGE_page_instance.FooterPosition=36;AUTOSTRUCT_EXISTS_SET_NAME(&i_CAMBRIDGE_page_instance,FooterPosition);
	i_CAMBRIDGE_page_instance.PageOverlap=0;AUTOSTRUCT_EXISTS_SET_NAME(&i_CAMBRIDGE_page_instance,PageOverlap);
	i_CAMBRIDGE_page_instance.PrintTrimMarks=1;AUTOSTRUCT_EXISTS_SET_NAME(&i_CAMBRIDGE_page_instance,PrintTrimMarks);
	i_CAMBRIDGE_page_instance.HeightPages=37;AUTOSTRUCT_EXISTS_SET_NAME(&i_CAMBRIDGE_page_instance,HeightPages);
	i_CAMBRIDGE_page_instance.WidthPages=48;AUTOSTRUCT_EXISTS_SET_NAME(&i_CAMBRIDGE_page_instance,WidthPages);
	i_CAMBRIDGE_page_instance.DrawingSpace=1;AUTOSTRUCT_EXISTS_SET_NAME(&i_CAMBRIDGE_page_instance,DrawingSpace);
	(*glob_CAMBRIDGE_page_multilist).ADD(&i_CAMBRIDGE_page_instance);
	CONVCAMBRIDGE_internaltomain(&i_CAMBRIDGE_page_instance);
	output_object(ifile,&i_CAMBRIDGE_page_instance);
	fprintf(ifile,"</CDXML>");
	fclose(ifile);
	return 1;
}
catalogized_command_funcdef(SAVEAS)
{
	LHENDRAW_filedlgmode=1;
	control_filemenu_mode=1;
	return 1;
}
catalogized_command_funcdef(LOAD_INTO_SEARCHBUF)
{
	printf("TODO***stub\n");
}
void edit_add_deltahydrogens()
{
	for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
	{
		float i_bond_sum=0;
		n_instance * tl_n_instance=(*glob_n_multilist).bufferlist+ilv1;
		if ((*tl_n_instance).exist)
		{
			for (int ilv2=0;ilv2<atom_actual_node[ilv1].bondcount;ilv2++)
			{
				i_bond_sum+=(*glob_b_multilist).bufferlist[atom_actual_node[ilv1].bonds[ilv2]].Order/16.0;
			}
		}
		if (fmod(i_bond_sum,1.0)>0.4)
		{
			i_bond_sum=trunc(i_bond_sum)+1;
		}
		(*tl_n_instance).protons+=i_bond_sum;
	}
}
catalogized_command_funcdef(LOAD_TYPE)
{
	FILE * infile;
	infile=fopen(parameter,"r");
	if (infile==NULL) return 0;
	currentinstance=new(CAMBRIDGEPREFIX(Total_Document_instance));
	input_fsm(infile);
	fclose(infile);
	CAMBRIDGECONV_maintointernal();
	svg_findaround();
	getatoms();
	edit_add_deltahydrogens();
	return 1;
}
catalogized_command_funcdef(LOADAS)
{
	LHENDRAW_filedlgmode=1;
	control_filemenu_mode=0;
	return 1;
}
catalogized_command_funcdef(SAVE)
{
	printf("TODO***stub\n");
	return 1;
}
catalogized_command_funcdef(UNDO)
{
	if (undo_undodirty) {storeundo(~0);}
	undo_trackundo();
	restoreundo(~0,0);
	return 1;
}
catalogized_command_funcdef(REDO)
{
	if (undo_trackredo(0)>0)
	{
		return restoreundo(~0,0);
	}
	return 0;
}
catalogized_command_funcdef(SELECTALL)
{
	selection_currentselection_found=0;
	_u32 icompare=0;
	int isize=0;
	for (int ilv1=1;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
	{
		icompare=1<<ilv1;
		basicmultilist * tlmultilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
		isize=STRUCTURE_OBJECTTYPE_List[ilv1].size;
		for (int ilv2=0;ilv2<(*tlmultilist).filllevel;ilv2++)
		{
			if (value[0]=='1')
			{
				char tl_exbool=(*(basic_instance*)(((char*)(*tlmultilist).pointer)+ilv2*isize)).exist;
				selection_currentselection[ilv2]=icompare*tl_exbool+(selection_currentselection[ilv2]&(~icompare));
				selection_currentselection_found|=icompare*tl_exbool;
			}
			else
			{
				selection_currentselection[ilv2]=0;
			}
		}
	}
	return 1;
}
catalogized_command_funcdef(RESETDRAWTOOL)
{
	control_drawproperties.bond_multiplicity=1;
	control_drawproperties.bond_Display1=0;
	return 1;
}
catalogized_command_funcdef(TEST_SMASH)
{
	char zero=0;
	srand(counter1);
	TELESCOPE_aggressobject(glob_t_multilist,rand()%(*glob_t_multilist).filllevel);
	TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_s);
	TELESCOPE_split(atoi(value),&zero,1);
	return 1;
}
extern int control_mousestate;
catalogized_command_funcdef(OPEN_PSE)
{
	control_mousestate=0x10;
	return 0;
}
catalogized_command_funcdef(HOME)
{
	SDL_zoomx=1;SDL_zoomy=1;SDL_scrollx=0;SDL_scrolly=0;
	return 1;
}
catalogized_command_funcdef(ONETOONE)
{
	SDL_scrollx+=(gfx_canvassizex/2)*((1/SDL_zoomx)-1);
	SDL_scrolly+=(gfx_canvassizey/2)*((1/SDL_zoomy)-1);
	SDL_zoomx=1;SDL_zoomy=1;
	return 1;
}
catalogized_command_funcdef(SEARCH)
{
	printf("TODO***stub\n");
	return 1;
}

char edit_scoop_atomstring[4];
int edit_scoop_numhydrogens;
int edit_scoop_charge;
edit_formatstruct edit_scoop_packedformats[6];
s_instance edit_scoop_formats[6];
_u8 edit_scoop_valids;//0: BIG 1: SMALL1 2: SMALL2 3: H 4: Nr 5: Charge
void processatomsymbol(int * fsm,char * pointer,s_instance * format)
{
	char ihv1;
	if ((*fsm)==6) return;
	if (pointer==NULL)
	{
		(*fsm)=6;
		return;
	}
	iback:
	if ((*pointer)==0) return;
	ihv1=(*pointer);
	if ((*fsm)==0)
	{
		if ((ihv1>='A') && (ihv1<='Z'))
		{
			edit_scoop_atomstring[0]=ihv1;
			edit_scoop_valids|=1<<0;
			edit_scoop_formats[0]=*format;
			(*fsm)=1;
			pointer++;
			goto iback;
		}
		if ((ihv1>='a') && (ihv1<='z'))//protons, neutrons or something
		{
			edit_scoop_atomstring[0]=ihv1;
			edit_scoop_valids|=1<<0;
			edit_scoop_formats[0]=*format;
			(*fsm)=5;
			pointer++;
			goto iback;
		}
		if ((ihv1=='.') || (ihv1==' ') || (ihv1==':'))
		{
			if (ihv1!=' ')
			{
				edit_scoop_atomstring[0]=ihv1;
			}
			edit_scoop_valids|=1<<0;
			edit_scoop_formats[0]=*format;
			(*fsm)=5;
			pointer++;
			goto iback;
		}
		(*fsm)=6;
		return;
	}
	if ((*fsm)==1)
	{
		if (ihv1=='H')
		{
			(*fsm)=3;
			edit_scoop_valids|=1<<3;
			edit_scoop_formats[3]=*format;
			edit_scoop_numhydrogens=1;
			pointer++;
			goto iback;
		}
		if ((ihv1>='a') && (ihv1<='z'))
		{
			edit_scoop_atomstring[2]=ihv1;
			edit_scoop_valids|=1<<1;
			edit_scoop_formats[1]=*format;
			(*fsm)=2;
			pointer++;
			goto iback;
		}
		(*fsm)=6;
		return;
	}
	if ((*fsm)==2)
	{
		if (ihv1=='H')//Some idea: would this be wise to do for fluorine?
		{
			(*fsm)=3;
			edit_scoop_valids|=1<<3;
			edit_scoop_formats[3]=*format;
			edit_scoop_numhydrogens=1;
			pointer++;
			goto iback;
		}
		if ((ihv1>='a') && (ihv1<='z'))
		{
			edit_scoop_atomstring[2]=ihv1;//Overwrites
			edit_scoop_valids|=1<<2;
			edit_scoop_formats[2]=*format;
			(*fsm)=2;
			pointer++;
			goto iback;
		}
		(*fsm)=6;
		return;
	}
	if ((*fsm)==3)
	{
		if ((ihv1>='0') && (ihv1<='9'))
		{
			edit_scoop_numhydrogens=ihv1-'0';
			edit_scoop_valids|=1<<4;
			edit_scoop_formats[4]=*format;
			(*fsm)=4;
			pointer++;
			goto iback;
		}
		(*fsm)=6;
		return;
	}
	if ((*fsm)==4)
	{
		if ((ihv1>='0') && (ihv1<='9'))
		{
			edit_scoop_numhydrogens=(edit_scoop_numhydrogens*10)+ihv1-'0';
			edit_scoop_valids|=1<<4;
			edit_scoop_formats[4]=*format;
			(*fsm)=4;
			pointer++;
			goto iback;
		}
		(*fsm)=6;
		return;
	}
	if ((*fsm)==5)
	{
		(*fsm)=6;
		return;
	}
	return;
}
int edit_interpretaselementwithimplicithydrogens(multilist<n_instance> * imultilist,int inumber)
{
	edit_scoop_valids=0;
	(*(_u32*)edit_scoop_atomstring)=0;
	edit_scoop_numhydrogens=0;
	edit_scoop_charge=0;
	char * ipointer;
	int i_backval=0;
	int fsm=0;//0: nothing 1: One big letter 2: One big Letter, and also small letters 3: Hydrogens-H 4: Number reached 5: other symbol, done 6: Invalid-cant interpret
	i_backval=TELESCOPE_aggressobject(imultilist,inumber);
	if (i_backval)
	{
		i_backval=TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_s);
		if (i_backval) ipointer=(char*)TELESCOPE_getproperty_contents();
	}
	else
	{
		edit_scoop_numhydrogens=4;
		(*imultilist).bufferlist[inumber].Element=constants_Element_implicitcarbon;
		goto yes_its_an_element;
	}
	iback:
	if (i_backval)
	{
		processatomsymbol(&fsm,ipointer,(s_instance*)TELESCOPE_getproperty());
		if (((*(s_instance*)TELESCOPE_getproperty()).face & 0x60)!=0x60)
		{
			fsm=6;
		}
		i_backval=TELESCOPE_searchthroughobject_next(TELESCOPE_ELEMENTTYPE_s);
		if (i_backval)
		{
			ipointer=(char*)TELESCOPE_getproperty_contents();
			goto iback;
		}
	}
	if (fsm==6)
	{
		return 0;
	}
	for (int ilv1=0;ilv1<sizeof(element)/sizeof(element_);ilv1++)
	{
		if (strcmp(edit_scoop_atomstring,element[ilv1].name)==0)
		{
			(*imultilist).bufferlist[inumber].Element=ilv1;
			goto yes_its_an_element;
		}
	}
	return 0;
	yes_its_an_element:;

	iback2://Then, we delete any s objects of the object
	TELESCOPE_aggressobject(imultilist,inumber);
	i_backval=TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_s);
	if (i_backval)
	{
		TELESCOPE_clear_item();
		goto iback2;
	}
	TELESCOPE_aggressobject(imultilist,inumber);
	i_backval=TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_s_f);
	if (i_backval)
	{
		TELESCOPE_clear_item();
	}
	TELESCOPE_aggressobject(imultilist,inumber);
	_u8 cursor=1;
	_u8 fill=0;
	for (int ilv1=0;ilv1<6;ilv1++)
	{
		if (cursor & edit_scoop_valids)
		{
			edit_scoop_packedformats[fill].color=edit_scoop_formats[ilv1].color;
			edit_scoop_packedformats[fill].face=edit_scoop_formats[ilv1].face;
		}
		cursor<<1;
	}
	TELESCOPE_add(TELESCOPE_ELEMENTTYPE_s_f,(char*)&edit_scoop_packedformats,fill*sizeof(edit_formatstruct));
	(*(s_f_instance*)TELESCOPE_getproperty()).valids=edit_scoop_valids;
	(*(s_f_instance*)TELESCOPE_getproperty()).length=sizeof(s_f_instance)+fill*sizeof(edit_formatstruct);
	(*(s_f_instance*)TELESCOPE_getproperty()).type=TELESCOPE_ELEMENTTYPE_s_f;
	(*imultilist).bufferlist[inumber].protons=edit_scoop_numhydrogens;
	(*imultilist).bufferlist[inumber].color=edit_scoop_formats[0].color;
	return 1;
}
int menu_itembyname(const char * name,int * menu=NULL,int * index=NULL);
catalogized_command_funcdef(FILEDLG_DEVICE_SEL)
{
	DIR * DD=opendir(parameter);
	struct dirent * dirpy;
	if (DD)
	{
		control_filememory.count=0;
		for (int ilv1=0;ilv1<255;ilv1++)
		{
			dirpy=readdir(DD);
			if (dirpy==NULL) goto readfinished;
			strncpy(control_filememory_buffer[ilv1],dirpy->d_name,255);control_filememory_buffer[ilv1][255]=0;
			strncpy(control_currentdirectory,parameter,255);control_currentdirectory[255]=0;
			control_filememory_attribs[ilv1]=dirpy->d_type;
			control_filememory.count++;
			menu_selectedmenuelement=menu_itembyname("FILEDLG_FILE_SEL");
		}
		readfinished:;
		closedir(DD);
		control_filememory.scroll=0;
		control_filememory.number=0;
		return 1;
	}
	return -30;
}
catalogized_command_funcdef(FILEDLG_FILE_SEL)
{
	int formerlength=strlen(control_currentdirectory);
	sprintf(control_currentdirectory+strlen(control_currentdirectory),"%c%s",constants_Directoryslash,parameter);//TODO: limit
	DIR * DD=opendir(control_currentdirectory);
	struct dirent * dirpy;
	if (DD)
	{
		control_filememory.count=0;
		for (int ilv1=0;ilv1<255;ilv1++)
		{
			dirpy=readdir(DD);
			if (dirpy==NULL) goto readfinished;
			strncpy(control_filememory_buffer[ilv1],dirpy->d_name,255);control_filememory_buffer[ilv1][255]=0;
			control_filememory_attribs[ilv1]=dirpy->d_type;
			control_filememory.count++;
		}
		readfinished:;
		control_filememory.scroll=0;
		control_filememory.number=0;
		closedir(DD);
	}
	else
	{
		control_currentdirectory[formerlength]=0;
		strncpy(control_filenamehead,parameter,255);control_filenamehead[255]=0;
		menu_selectedmenuelement=menu_itembyname("FILEDLG_FILE_HEAD");
	}
	return 1;
}
catalogized_command_funcdef(FILEDLG_FILE_PATH)
{
	DIR * DD=opendir(control_currentdirectory);
	struct dirent * dirpy;
	if (DD)
	{
		control_filememory.count=0;
		for (int ilv1=0;ilv1<255;ilv1++)
		{
			dirpy=readdir(DD);
			if (dirpy==NULL) goto readfinished;
			strncpy(control_filememory_buffer[ilv1],dirpy->d_name,255);control_filememory_buffer[ilv1][255]=0;
			control_filememory_attribs[ilv1]=dirpy->d_type;
			control_filememory.count++;
		}
		readfinished:;
		control_filememory.scroll=0;
		control_filememory.number=0;
		menu_selectedmenuelement=menu_itembyname("FILEDLG_FILE_HEAD");
		closedir(DD);
		return 1;
	}
	return -30;
}
catalogized_command_funcdef(FILEDLG_FILE_HEAD)
{
	menu_selectedmenuelement=0;
	return 1;
}
char control_totalfilename[stringlength+stringlength+2];
catalogized_command_funcdef(FILEDLG_FILE_SAVE)
{
	DIR * DD=opendir(control_currentdirectory);
	char retval=-30;
	if (DD)
	{
		sprintf(control_totalfilename,"%s/%s",control_currentdirectory,control_filenamehead);
		retval=SAVE_TYPE(control_totalfilename,".cdxml");
		if (retval>=1)
		{
			retval=1;
			LHENDRAW_filedlgmode=0;
		}
		closedir(DD);
	}
	return retval;
}
catalogized_command_funcdef(FILEDLG_FILE_LOAD)
{
	DIR * DD=opendir(control_currentdirectory);
	char retval=-30;
	if (DD)
	{
		sprintf(control_totalfilename,"%s/%s",control_currentdirectory,control_filenamehead);
		retval=LOAD_TYPE(control_totalfilename,".cdxml");
		if (retval>=1)
		{
			LHENDRAW_filedlgmode=0;
		}
		closedir(DD);
	}
	return retval;
}
catalogized_command_funcdef(SET_ALL_ITEMS)//TODO: works for _i32 only, right now
{
	intl suboffset=0;
	CDXMLREAD_functype dummy;
	for (int ilv1=1;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
	{
		basicmultilist * tl_multilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
		suboffset=(*tl_multilist).getproperties(parameter,&dummy);
		_u32  icompare=(1<<ilv1);
		int tl_size=(*tl_multilist).itemsize;
		int ifilllevel=(*tl_multilist).filllevel;//separately, so it doesn't grow while executing the loop
		for (int ilv2=0;ilv2<ifilllevel;ilv2++)
		{
			if (selection_currentselection[ilv2] & icompare)
			{
				if ((*tl_multilist)[ilv2].exist)
				{
					*((_i32*)(((char*)(&((*tl_multilist)[ilv2])))+suboffset))=atoi(value);
				}
			}
		}
	}
}
catalogized_command_funcdef(OPTIONS)
{
	return 0;
}
