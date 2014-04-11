//This unit does most processing of the internal data
int control_hotatom=-1;
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
n_instance * gethotatom(int * nr=NULL)
{
	if ((control_hotatom>=0) && (control_hotatom<(*glob_n_multilist).filllevel))
	{
		if ((*glob_n_multilist).bufferlist[control_hotatom].exist)
		{
			if (nr!=NULL)
			{
				(*nr)=control_hotatom;
			}
		}
	}
	if (nr!=NULL)
	{
		*(nr)=-1;
	}
	return NULL;
}
int hit(n_instance * iinstance,float ix,float iy)
{
	if ((sqr(ix-(*iinstance).xyz.x)+sqr(iy-(*iinstance).xyz.y))<glob_clickradius)
	{
		return 1;
	}
	return 0;
}
int hit(b_instance * iinstance,float ix,float iy)
{
	n_instance *iinstance1=0;
	n_instance *iinstance2=0;
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
template <class thisinstance> inline int clickfor_template(float posx,float posy,int objecttype)
{
	char found=0;
	multilist<thisinstance> * imultilist=retrievemultilist<thisinstance>();
	for (int ilv1=0;ilv1<(*imultilist).filllevel;ilv1++)
	{
		thisinstance * tlinstance=&((*imultilist).bufferlist[ilv1]);
		if ((*tlinstance).exist)
		{
			if (hit(tlinstance,posx,posy)>0)
			{
				selection_clickselection[ilv1]|=(1<<objecttype);
				found=1;
			}
		}
	}
	return found;
}
inline int retrievepoints(n_instance * iinstance,float * ix,float * iy,int inumber)
{
	if (inumber>0) return 0;
	(*ix)=(*iinstance).xyz.x;
	(*iy)=(*iinstance).xyz.y;
	return 1;
}
inline int retrievepoints(b_instance * iinstance,float * ix,float * iy,int inumber)
{
	if (inumber>0) return 0;
	n_instance *iinstance1=0;
	n_instance *iinstance2=0;
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

#define LOCALMACRO_1(whatabout) case STRUCTURE_OBJECTTYPE_ ## whatabout: return retrievepoints((whatabout ## _instance*)iinstance,ix,iy,inumber);break;
int retrievepoints_basic(basic_instance * iinstance,float * ix,float * iy,int inumber,int objecttype)
{
	switch (objecttype)
	{
		LOCALMACRO_1(n)
		LOCALMACRO_1(b)
	}
	return 0;
}
#undef LOCALMACRO_1
#define LOCALMACRO_1(whatabout) case STRUCTURE_OBJECTTYPE_ ## whatabout: return clickfor_template<whatabout ## _instance>(ix,iy,objecttype);break;
int clickfor(float ix,float iy,int objecttype,float iclickradius=constants_clickradius)
{
	glob_clickradius=iclickradius;
	switch (objecttype)
	{
		LOCALMACRO_1(n)
		LOCALMACRO_1(b)
	}
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
			else
			{
				return NULL;
			}
		}
	}
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
b_instance * summonbond(int i_id_begin,int i_id_end)
{
	if ((*glob_b_multilist).filllevel<bufferlistsize)
	{
		b_instance * tlinstance;
		tlinstance=new(&((*glob_b_multilist).bufferlist[(*glob_b_multilist).filllevel])) b_instance;
		selection_currentselection[(*glob_b_multilist).filllevel]&=(~(1<<STRUCTURE_OBJECTTYPE_b));
		(*tlinstance).B=i_id_begin;
		(*tlinstance).E=i_id_end;
		atom_actual_node[i_id_begin]+=((*glob_b_multilist).filllevel);
		atom_actual_node[i_id_end]+=((*glob_b_multilist).filllevel);
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
	int atomnr=-1;
	n_instance * tlatom=&((*glob_n_multilist).bufferlist[startatom]);
	n_instance * tlatom2;
	b_instance * tlbond;
	if (atom_actual_node[startatom].bondcount==constants_maxbonds) return -1;
	float tlangle;
	tlangle=edit_get_atom_best_free_side(startatom);
	float posx=(*tlatom).xyz.x+constants_bondlength*cos(tlangle);
	float posy=(*tlatom).xyz.y+constants_bondlength*sin(tlangle);
	if (tlatom2=snapatom_short(posx,posy,&atomnr,10))
	{
	}
	else
	{
		tlatom2=summonatom(&atomnr);
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
		tlbond=summonbond((*tlatom).id,(*tlatom2).id);
		if (tlbond)
		{
			(*tlbond).color=0;
			(*tlbond).Z=(*tlatom).Z;
			(*tlbond).Order=1;
		}
	}
	return atomnr;
}
catalogized_command_funcdef(SPROUT)
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
	gethotatom(&tlatom);
	if (tlatom!=-1)
	{
		int sproutcount=atoi(variable);
		for (int ilv1=0;ilv1<sproutcount;ilv1++)
		{
			edit_errichten(ilv1);
		}
		return 1;
	}
	return 0;
}
