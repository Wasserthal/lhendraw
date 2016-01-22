//This unit does most processing of the internal data
#define FORCEEXTENSION \
{\
	__label__ FORCEEXTENSION_finished;\
	if (value==NULL) goto FORCEEXTENSION_YES;\
	if (strcmp(value,"")==0)\
	{\
		FORCEEXTENSION_YES:;\
		value=parameter+strlen(parameter);\
		while ((*value)!='.')\
		{\
			if (value==parameter)\
			{\
				value=constants_cdxstring;\
				goto FORCEEXTENSION_finished;\
			}\
			value--;\
		}\
	}\
	FORCEEXTENSION_finished:;\
}
#define LOCALMACROES\
		LOCALMACRO_1(n)\
		LOCALMACRO_1(b)\
		LOCALMACRO_1(graphic)\
		LOCALMACRO_1(arrow)\
		LOCALMACRO_1(t)\
		LOCALMACRO_1(curve)
extern char control_filename[stringlength*2+2];
extern int control_saveuponexit;
struct control_drawproperties_
{
	_i32 bond_Order;
	_i32 bond_Display;
	_i32 bond_Display2;
	_i32 curve_Closed;
	_i32 FillType;//0: line 1: solid
	_i32 LineType;
	_i32 attribute_tool;
	int color;
	_i32 Element;
	_i32 ring_element_count;
	_i32 ring_unsaturation;
	_i32 GraphicType;
	int ARROW_subtool;//0:draw 1: Skip 2: Situp
	int CHARGE_subtool;//0:Substitute charges 1: del 2: draw plainly
	int SELECTION_subtool;//0:Rectangular 1: round
	int CURVE_subtool;//0: putpoints 1: handles 2: pencil
	_i32 face;
	_i32 arrow_ArrowheadType;//0x10001 means retrosynthetic
	_i32 arrow_ArrowheadHead;
	_i32 arrow_ArrowheadTail;
	_i32 arrow_ArrowShaftSpacing;
	_i32 BracketType;
};
struct control_searchproperties_
{
	int foundbehavior;//0: return filename 1: start new thread, open file 2: start new thread open file and mark 3: start new thread, open file and mark anything 4: iterate, but return the number of overlapping matches 5: iterate, but return the number of non-overlapping matches 6: like 4, but yield no filename 7: like 5, but yield no filename 0x11-0x17: like 1-7, but end after one file!
	int invert;//1 inverts search!
	int resolvelabels;//0: exact match things like R, ~ or * 1: interpret their MEANING in the search pattern.
	_i32 expand_haystack_atoms;
	_i32 expand_template_atoms;
};
struct control_displayproperties_
{
	int outofarea;//0: off 1: on 2: reserved for: cannot scroll there 3: dragons
};
structenum * searchreflectedstruct(const char * input);
void applytransform_single(float matrix[3][3],cdx_Point3D * input,cdx_Point3D * output,cdx_Point3D * pivot);
_small edit_current5bondcarbon=0;
control_drawproperties_ control_drawproperties={16,0,0,0,0,0,4,0,constants_Element_implicitcarbon,6,1,0,0,0,0,1,0,1,2,1,1,4};
control_drawproperties_ control_drawproperties_init={16,0,0,0,0,0,4,0,constants_Element_implicitcarbon,6,1,0,0,0,0,1,0,1,1,1,1,4};
control_searchproperties_ control_searchproperties={0,0,1,1,0};
control_displayproperties_ control_displayproperties={1};
int control_hotatom=-1;
//Copies a set of atoms and bonds from one buffer to another. Can take atoms from ANY other buffer
char * undo_retrievebuffer(intl start,intl list);
undo_singlebuffer * undo_retrievehandle(intl start,intl list);
int getbondsum(intl inumber)
{
	float i_bond_sum=0;
	for (int ilv2=0;ilv2<atom_actual_node[inumber].bondcount;ilv2++)
	{
		i_bond_sum+=(*glob_b_multilist)[atom_actual_node[inumber].bonds[ilv2]].Order/16.0;
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
	if (ino==0)
	{
		intl tl_sum=0;
		for (int ilv1=1;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
		{
			intl tl_isum=0;
			tl_isum=janitor_getmaxid(ilv1);
			if (tl_isum>=0) tl_sum+=tl_isum;
		}
		return tl_sum;
	}
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
				iid=tlid;
			}
		}
	}
	(*tlmultilist).maxid=iid;
	return iid;
}
int edit_getnewZ()
{
	if (janitor_maxZ>0) {janitor_maxZ++;printf("ret:%i\n",janitor_maxZ);return janitor_maxZ;}
	janitor_maxZ=0;
	_u32 icompare;
	CDXMLREAD_functype dummy;
	int isize;
	int ioffset;
	char * ibufferpos;
	char isuccessful=0;
	for (int ilv1=1;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
	{
		icompare=1<<ilv1;
		isize= STRUCTURE_OBJECTTYPE_List[ilv1].size;
		basicmultilist * tlmultilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
		if (tlmultilist==NULL) goto hasnoZ;
		ioffset=(*tlmultilist).getproperties("xyz",&dummy);
		if (ioffset<0) goto hasnoZ;
		ibufferpos=(char*)((*tlmultilist).pointer);
		for (int ilv2=0;ilv2<(*tlmultilist).filllevel;ilv2++)
		{
			if ((*((basic_instance*)(ibufferpos+isize*ilv2))).exist)
			{
				if ((*((_i32*)(ibufferpos+isize*ilv2+ioffset)))>janitor_maxZ)
				{
					janitor_maxZ=(*((_i32*)(ibufferpos+isize*ilv2+ioffset)));
				}
			}
		}
		hasnoZ:;
	}
	janitor_maxZ+=1;
	printf("ret:%i\n",janitor_maxZ);
	return janitor_maxZ;
}
void janitor_biasids(int i_bias)
{
	janitor_id_list[0]=i_bias;
	for (int ilv1=1;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
	{
		janitor_id_list[ilv1]=janitor_id_list[ilv1-1]+max(0,janitor_getmaxid(ilv1));
	}
	janitor_id_list[STRUCTURE_OBJECTTYPE_ListSize]=janitor_id_list[STRUCTURE_OBJECTTYPE_ListSize-1];
	for (int ilv1=1;ilv1<TELESCOPE_ELEMENTTYPE_ListSize;ilv1++)
	{
		janitor_id_list[STRUCTURE_OBJECTTYPE_ListSize+ilv1]=janitor_id_list[STRUCTURE_OBJECTTYPE_ListSize+ilv1-1]+max(0,janitor_getmaxid(ilv1));
	}
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
int edit_getBoundingBoxMode(graphic_instance * i_graphic_instance)
//Retval: 0: Bounding Box only 1: Standard 2: top, left are at Center3D.x Center3D.y
{
	switch ((*i_graphic_instance).GraphicType)
	{
		case 1:
		return 0;
		case 6:
		{
			switch ((*i_graphic_instance).BracketType)
			{
				case 0:
				case 1:
				case 2:
				return 1;
				case 3:
				case 4:
				case 5:
				return 0;
			}
			break;
		}
		case 0:
		case 3:
		case 7:
		{
			return 1;
			break;
		}
		break;
		case 2:
		{
			//TODO: arc not supported
			return 1;
		}
		break;
		case 5:
		case 4:
		{
			return 2;
		}
		break;
	}
	return 1;
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
void getatoms()//makes some preprocessing
{
	float theside,thelevel;
	float tl_angle;
	for (int ilv1=0;ilv1<glob_n_multilist->filllevel;ilv1++)
	{
		atom_actual_node[ilv1].bondcount=0;
	}
	for (int ilv1=0;ilv1<glob_t_multilist->filllevel;ilv1++)
	{
		text_actual_node[ilv1].owner=-1;
	}
	for (int ilv1=0;ilv1<(*glob_b_multilist).filllevel;ilv1++)//defines processable bonds
	{
		if ((*glob_b_multilist)[ilv1].exist)
		{
			bond_actual_node[ilv1].cotanleft[0]=Pi/4;
			bond_actual_node[ilv1].cotanleft[1]=Pi/4;
			bond_actual_node[ilv1].cotanright[0]=Pi/4;
			bond_actual_node[ilv1].cotanright[1]=Pi/4;
			bond_actual_node[ilv1].xcotanleft[0]=Pi/2;
			bond_actual_node[ilv1].xcotanleft[1]=Pi/2;
			bond_actual_node[ilv1].xcotanright[0]=Pi/2;
			bond_actual_node[ilv1].xcotanright[1]=Pi/2;
			bond_actual_node[ilv1].leftdefined=0;
			bond_actual_node[ilv1].rightdefined=0;
			char diagnose_foundstart,diagnose_foundend;
			diagnose_foundstart=0;diagnose_foundend=0;
			for (int ilv2=0;ilv2<(*glob_n_multilist).filllevel;ilv2++)
			{
				if (((*glob_n_multilist))[ilv2].exist)
				{
					if (((*glob_n_multilist))[ilv2].id==((*glob_b_multilist))[ilv1].E)
					{
						bond_actual_node[ilv1].end=ilv2;
						atom_actual_node[ilv2]+=ilv1;
						diagnose_foundstart=1;
					}
					if (((*glob_n_multilist))[ilv2].id==((*glob_b_multilist))[ilv1].B)
					{
						bond_actual_node[ilv1].start=ilv2;
						atom_actual_node[ilv2]+=ilv1;
						diagnose_foundend=1;
					}
				}
			}
			if ((diagnose_foundstart ==0) | (diagnose_foundend==0))
			{
				error("Error: No start/end found on bond!");
			}
		}
	}
	for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)//defines processable atoms
	{
		n_instance * tlatominstance=&((*glob_n_multilist)[ilv1]);
		if (tlatominstance->exist)
		{
			theside=0;thelevel=0;
			for (int ilv2=0;ilv2<atom_actual_node[ilv1].bondcount;ilv2++)
			{
				_small partner=getother(ilv1,(atom_actual_node[ilv1]).bonds[ilv2]);
				
				tl_angle=getangle((*glob_n_multilist)[partner].xyz.x-(*tlatominstance).xyz.x,(*glob_n_multilist)[partner].xyz.y-(*tlatominstance).xyz.y);//TODO:not good at real-time!!!
				theside+=cos(tl_angle);
				thelevel+=sin(tl_angle);
				angle_between[ilv2][ilv2]=tl_angle;
				number_between[ilv2]=(atom_actual_node[ilv1]).bonds[ilv2];
			}
			atom_actual_node[ilv1].labelside=(theside>0.2);
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
					(*i_bond_actual_node).leftdefined|=1<<((*i_bond_actual_node).start==ilv1);
				}
				if (tlrightnr!=-1)
				{
					(*i_bond_actual_node).rightdefined|=1<<((*i_bond_actual_node).start==ilv1);
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
		}
	}
	for (int ilv1=0;ilv1<(*glob_b_multilist).filllevel;ilv1++)//refines undefined double bonds
	{
		b_instance * currentbondinstance=&((*glob_b_multilist)[ilv1]);
		int i_side_orvariable;
		int i_side_orvariable2;
		i_side_orvariable=0;
		i_side_orvariable2=0;
		if (((*currentbondinstance).DoublePosition & 0x100)==0)
		{
			for (int ilv2=0;ilv2<atom_actual_node[bond_actual_node[ilv1].start].bondcount;ilv2++)//or operation
			{
				i_side_orvariable|=getleftof(&((*glob_n_multilist)[bond_actual_node[ilv1].start].xyz),&((*glob_n_multilist)[bond_actual_node[ilv1].end].xyz),&((*glob_n_multilist)[getother((bond_actual_node[ilv1].start),atom_actual_node[bond_actual_node[ilv1].start].bonds[ilv2])].xyz));
			}
			if (i_side_orvariable==3)
			{
				i_side_orvariable=0;
			}
			for (int ilv2=0;ilv2<atom_actual_node[bond_actual_node[ilv1].end].bondcount;ilv2++)//or operation
			{
				i_side_orvariable2|=getleftof(&((*glob_n_multilist)[bond_actual_node[ilv1].start].xyz),&((*glob_n_multilist)[bond_actual_node[ilv1].end].xyz),&((*glob_n_multilist)[getother((bond_actual_node[ilv1].end),atom_actual_node[bond_actual_node[ilv1].end].bonds[ilv2])].xyz));
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
int get_bond_between_actual_node(int inatom1, int inatom2)
{
	int imax=(*glob_b_multilist).filllevel;
	for (int ilv1=0;ilv1<atom_actual_node[inatom1].bondcount;ilv1++)
	{
		for (int ilv2=0;ilv2<atom_actual_node[inatom2].bondcount;ilv2++)
		{
			if (atom_actual_node[inatom1].bonds[ilv1]==atom_actual_node[inatom2].bonds[ilv2])
			{
				return atom_actual_node[inatom1].bonds[ilv1];
			}
		}
	}
	return -3;
}
int gethot()
{
	if ((control_hotatom>=0) && (control_hotatom<(*glob_n_multilist).filllevel))
	{
		if ((*glob_n_multilist).bufferlist()[control_hotatom].exist)
		{
			return control_hotatom;
		}
	}
	control_hotatom=-1;
	return -1;
}
int edit_endatom_count=0;
#define edit_endatom_max 2
int edit_endatom[edit_endatom_max];
int edit_endatom_quality[edit_endatom_max];
double edit_endatom_vector[edit_endatom_max][3];
int edit_judgepoint(int index,double *ivector)
{
	int i_u_count=0;
	int i_s_count=0;
	double i_selectedvector[3]={0,0,0};
	double i_unselectedvector[3]={0,0,0};
	int i_partner=0;
	n_instance * i_n_instance=glob_n_multilist->bufferlist()+index;
	if (i_n_instance->exist)
	{
		if ((selection_currentselection[index] & (1<<STRUCTURE_OBJECTTYPE_n))>0)
		{
			for (int ilv2=0;ilv2<atom_actual_node[index].bondcount;ilv2++)
			{
				i_partner=getother(index,atom_actual_node[index].bonds[ilv2]);
				if (selection_currentselection[atom_actual_node[index].bonds[ilv2]] & (1<<STRUCTURE_OBJECTTYPE_b))
				{
					i_selectedvector[0]+=glob_n_multilist->bufferlist()[i_partner].xyz.x-glob_n_multilist->bufferlist()[index].xyz.x;
					i_selectedvector[1]+=glob_n_multilist->bufferlist()[i_partner].xyz.y-glob_n_multilist->bufferlist()[index].xyz.y;
					i_selectedvector[3]+=glob_n_multilist->bufferlist()[i_partner].xyz.z-glob_n_multilist->bufferlist()[index].xyz.z;
					i_s_count++;
				}
				else
				{
					i_unselectedvector[0]+=glob_n_multilist->bufferlist()[i_partner].xyz.x-glob_n_multilist->bufferlist()[index].xyz.x;
					i_unselectedvector[1]+=glob_n_multilist->bufferlist()[i_partner].xyz.y-glob_n_multilist->bufferlist()[index].xyz.y;
					i_unselectedvector[3]+=glob_n_multilist->bufferlist()[i_partner].xyz.z-glob_n_multilist->bufferlist()[index].xyz.z;
					i_u_count++;
				}
			}
		}
	}
	double * sourcevector;
	int sourcecount=1;
	if ((i_s_count<i_u_count) && (i_s_count>0))
	{
		sourcevector=i_selectedvector;
		sourcecount=i_s_count;
	}
	else
	{
		if (i_u_count>0)
		{
			sourcevector=i_unselectedvector;
			sourcecount=i_u_count;
		}
		else
		{
			sourcevector=i_unselectedvector;//==&{0,0,0}
			sourcecount=1;//to prevent divide by zero;
		}
	}
	ivector[0]=sourcevector[0]/sourcecount;
	ivector[1]=sourcevector[1]/sourcecount;
	ivector[2]=sourcevector[2]/sourcecount;
	if (i_u_count>0) i_s_count=0;
	return min(i_u_count,0xF)*0x10+min(i_s_count,0xF);
}
void edit_judgeselection(int backindex)//determines what type of pivot the current selection has
{
	edit_endatom_count=0;
	for (int ilv1=1;ilv1<glob_n_multilist->filllevel;ilv1++)
	{
		n_instance * tl_n_instance=glob_n_multilist->bufferlist()+ilv1;
		if (tl_n_instance->exist)
		{
			if ((selection_currentselection[ilv1] & (1<<STRUCTURE_OBJECTTYPE_n))>0)
			{
				for (int ilv2=0;ilv2<atom_actual_node[ilv1].bondcount;ilv2++)
				{
					if (selection_currentselection[atom_actual_node[ilv1].bonds[ilv2]] & (1<<STRUCTURE_OBJECTTYPE_b))
					{
					}
					else
					{
						double tl_vector[3]={0,0,0};
						int tl_backval=edit_judgepoint(ilv1,tl_vector);
						int tl_checked_unit;
						for (tl_checked_unit=0;tl_checked_unit<edit_endatom_count;tl_checked_unit++)
						{
							if (tl_backval<edit_endatom_quality[tl_checked_unit])
							{
								instance_add:;
								for (int ilv3=min(edit_endatom_count,edit_endatom_max-1);ilv3>tl_checked_unit;ilv3--)
								{
									edit_endatom[ilv3]=edit_endatom[ilv3-1];
									edit_endatom_quality[ilv3]=edit_endatom_quality[ilv3-1];
									edit_endatom_vector[ilv3][0]=edit_endatom_vector[ilv3-1][0];
									edit_endatom_vector[ilv3][1]=edit_endatom_vector[ilv3-1][1];
									edit_endatom_vector[ilv3][2]=edit_endatom_vector[ilv3-1][2];
								}
								edit_endatom[tl_checked_unit]=ilv1;
								edit_endatom_quality[tl_checked_unit]=tl_backval;
								edit_endatom_vector[tl_checked_unit][0]=tl_vector[0];
								edit_endatom_vector[tl_checked_unit][1]=tl_vector[1];
								edit_endatom_vector[tl_checked_unit][2]=tl_vector[2];
								edit_endatom_count=tl_checked_unit+1;//cut them off
								goto instance_done;
							}
						}
						if (edit_endatom_count<edit_endatom_max)
						{
							tl_checked_unit=edit_endatom_count;
							goto instance_add;
						}
						goto instance_done;
					}
				}
			}
		}
		instance_done:;
	}
}
int edit_checkclickpixels()
{
	for (int ilv1=1;ilv1<4;ilv1++)
	{
		if (canvas[ilv1]!=0xFFFFFFFF) goto iclear;
		if (canvas[(ilv1)*5]!=0xFFFFFFFF) goto iclear;
		if (canvas[ilv1+20]!=0xFFFFFFFF) goto iclear;
		if (canvas[(ilv1)*5+4]!=0xFFFFFFFF) goto iclear;
		for (int ilv2=1;ilv2<4;ilv2++)
		{
			if (canvas[ilv2*5+ilv1]!=0xFFFFFFFF) goto iclear;
		}
	}
	return 0;
	iclear:
	for (int ilv1=0;ilv1<5;ilv1++)
	{
		for (int ilv2=0;ilv2<5;ilv2++)
		{
			canvas[ilv1*5+ilv2]=0xFFFFFFFF;
		}
	}
	return 1;
}
void edit_clickpixels(int posx,int posy)
{
	gfx_bufferset_ target;
	gfx_store_bufferset(&gfx_old_bufferset);
	target.screensizex=5;
	target.screensizey=5;
	target.canvassizex=5;
	target.canvassizey=5;
	target.canvasminx=0;
	target.canvasminy=0;
	target.canvasmaxx=5;
	target.canvasmaxy=5;
	target.screen=screen;
	target.canvas=target.screen;
	target.scrollx=SDL_scrollx+(posx-2)/SDL_zoomx;
	target.scrolly=SDL_scrolly+(posy-2)/SDL_zoomy;
	target.zoomx=SDL_zoomx;
	target.zoomy=SDL_zoomy;
	target.depth=4;
	gfx_restore_bufferset(&target);
	screenclear(0xFFFFFFFF);
	gfx_output(1);
	gfx_restore_bufferset(&gfx_old_bufferset);
}
inline int retrieveprops_n(int what)
{
	if (what==1)
	{
		return -1;
	}
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
inline int retrieveprops_t(int what)
{
	return 0;
}
inline int retrieveprops_curve(int what)
{
	if (what==1)
	{
		return -1;
	}
	if (what==-1)
	{
		return -1;
	}
	return 0;
}
inline int removepoints(n_instance * iinstance,int inumber,basicmultilist * imultilist=NULL)
{
	if (inumber>0)
	{
		int tl_backval=0;
		int ilv1=0;
		if (imultilist==NULL) imultilist=glob_n_multilist;
		tl_backval=TELESCOPE_aggressobject(imultilist,iinstance-((multilist<n_instance>*)imultilist)->bufferlist());
		tl_backval=TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_Symbol);
		iback:;
		if (tl_backval>0)
		{
			ilv1++;
			Symbol_instance * tl_Symbol_instance=(Symbol_instance*)TELESCOPE_getproperty();
			if (ilv1==inumber)
			{
				switch (tl_Symbol_instance->SymbolType)
				{
					case 2:
					case 4:
					case 8:
					(*iinstance).charge--;
					break;
					case 3:
					case 5:
					case 9:
					(*iinstance).charge--;
					break;
				}
				int backval=TELESCOPE_clear_item();
				//TODO: reCALCULATE charge!
				return (backval>0);
			}
			tl_backval=TELESCOPE_searchthroughobject_next(TELESCOPE_ELEMENTTYPE_Symbol);
			goto iback;
		}
	}
	return 0;
}
inline int removepoints(b_instance * iinstance,int inumber,basicmultilist * imultilist=NULL)
{
	return 0;
}
inline int removepoints(graphic_instance * iinstance,int inumber,basicmultilist * imultilist=NULL)
{
	return 0;
}
inline int removepoints(arrow_instance * iinstance,int inumber,basicmultilist * imultilist=NULL)
{
	return 0;
}
inline int removepoints(t_instance * iinstance,int inumber,basicmultilist * imultilist=NULL)
{
	return 0;
}
inline int removepoints(curve_instance * iinstance,int inumber,basicmultilist * imultilist=NULL)
{
	if (inumber>0)
	{
		inumber--;
		if (inumber<iinstance->CurvePoints.count)
		{
			for (int ilv1=inumber;ilv1<iinstance->CurvePoints.count-1;ilv1++)
			{
				iinstance->CurvePoints.a[ilv1]=iinstance->CurvePoints.a[ilv1+1];
			}
			iinstance->CurvePoints.count--;
		}
	}
	return 0;
}
//ANY retrievepoints on an object with a negative prop count must keep it's telescope-element set to read the corresponding item
inline int retrievepoints(n_instance * iinstance,float * ix,float * iy,float * iz,int inumber,basicmultilist * imultilist=NULL)
{
	if (inumber>0)
	{
		int tl_backval=0;
		int ilv1=0;
		if (imultilist==NULL) imultilist=glob_n_multilist;
		tl_backval=TELESCOPE_aggressobject(imultilist,iinstance-((multilist<n_instance>*)imultilist)->bufferlist());
		tl_backval=TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_Symbol);
		iback:;
		if (tl_backval>0)
		{
			ilv1++;
			Symbol_instance * tl_Symbol_instance=(Symbol_instance*)TELESCOPE_getproperty();
			if (ilv1==inumber)
			{
				(*ix)=(*iinstance).xyz.x+(*tl_Symbol_instance).dxyz.x;
				(*iy)=(*iinstance).xyz.y+(*tl_Symbol_instance).dxyz.y;
				(*iz)=(*iinstance).xyz.z+(*tl_Symbol_instance).dxyz.z;
				return 1;
			}
			tl_backval=TELESCOPE_searchthroughobject_next(TELESCOPE_ELEMENTTYPE_Symbol);
			goto iback;
		}
		return 0;
	}
	if (inumber<0) return 0;
	(*ix)=(*iinstance).xyz.x;
	(*iy)=(*iinstance).xyz.y;
	(*iz)=(*iinstance).xyz.z;
	return 1;
}
inline int retrievepoints(b_instance * iinstance,float * ix,float * iy,float * iz,int inumber,basicmultilist * imultilist=NULL)
{
	if (inumber>0) return 0;
	if (inumber<0) return 0;
	n_instance *iinstance1=NULL;
	n_instance *iinstance2=NULL;
	if (imultilist!=NULL) return 0;//Not handled!(TODO?)
	for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
	{
		n_instance * tlinstance=&((*glob_n_multilist)[ilv1]);
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
	(*iz)=((*iinstance1).xyz.z+(*iinstance2).xyz.z)*0.5;
	return 1;
}
inline int retrievepoints(graphic_instance * iinstance,float * ix,float * iy,float * iz,int inumber,basicmultilist * imultilist=NULL)
{
	if (inumber>2) return 0;
	if (inumber<-5) return 0;
	if (inumber==-3)
	{
		*ix=(*iinstance).Center3D.x;
		*iy=(*iinstance).Center3D.y;
		*iz=(*iinstance).Center3D.z;
		return 1;
	}
	if (inumber==-4)
	{
		*ix=(*iinstance).MajorAxisEnd3D.x;
		*iy=(*iinstance).MajorAxisEnd3D.y;
		*iz=(*iinstance).MajorAxisEnd3D.z;
		return 1;
	}
	if (inumber==-5)
	{
		*ix=(*iinstance).MinorAxisEnd3D.x;
		*iy=(*iinstance).MinorAxisEnd3D.y;
		*iz=(*iinstance).MinorAxisEnd3D.z;
		return 1;
	}
	if ((inumber==1) || (inumber==-1))
	{
		(*ix)=(*iinstance).BoundingBox.left;
		(*iy)=(*iinstance).BoundingBox.top;
		(*iz)=0;
		return 1;
	}
	if ((inumber==2) || (inumber==-2))
	{
		(*ix)=(*iinstance).BoundingBox.right;
		(*iy)=(*iinstance).BoundingBox.bottom;
		(*iz)=0;
		return 1;
	}
	(*ix)=((*iinstance).BoundingBox.right+(*iinstance).BoundingBox.left)/2;
	(*iy)=((*iinstance).BoundingBox.bottom+(*iinstance).BoundingBox.top)/2;
	(*iz)=0;
	return 1;
}
inline int retrievepoints(arrow_instance * iinstance,float * ix,float * iy,float * iz,int inumber,basicmultilist * imultilist=NULL)
{
	if (inumber>2) return 0;
	if (inumber<-5) return 0;
	if (inumber==-1)
	{
		*ix=(*iinstance).Head3D.x;
		*iy=(*iinstance).Head3D.y;
		*iz=(*iinstance).Head3D.z;
		return 1;
	}
	if (inumber==-2)
	{
		*ix=(*iinstance).Tail3D.x;
		*iy=(*iinstance).Tail3D.y;
		*iz=(*iinstance).Tail3D.z;
		return 1;
	}
	if (inumber==-3)
	{
		*ix=(*iinstance).Center3D.x;
		*iy=(*iinstance).Center3D.y;
		*iz=(*iinstance).Center3D.z;
		return 1;
	}
	if (inumber==-4)
	{
		*ix=(*iinstance).MajorAxisEnd3D.x;
		*iy=(*iinstance).MajorAxisEnd3D.y;
		*iz=(*iinstance).MajorAxisEnd3D.z;
		return 1;
	}
	if (inumber==-5)
	{
		*ix=(*iinstance).MinorAxisEnd3D.x;
		*iy=(*iinstance).MinorAxisEnd3D.y;
		*iz=(*iinstance).MinorAxisEnd3D.z;
		return 1;
	}
	if (inumber==1)
	{
		*ix=(*iinstance).Head3D.x;
		*iy=(*iinstance).Head3D.y;
		*iz=(*iinstance).Head3D.z;
		return 1;
	}
	if (inumber==2)
	{
		*ix=(*iinstance).Tail3D.x;
		*iy=(*iinstance).Tail3D.y;
		*iz=(*iinstance).Tail3D.z;
		return 1;
	}
	(*ix)=((*iinstance).Head3D.x+(*iinstance).Tail3D.x)/2;
	(*iy)=((*iinstance).Head3D.y+(*iinstance).Tail3D.y)/2;
	(*iz)=((*iinstance).Head3D.z+(*iinstance).Tail3D.z)/2;
	return 1;
}
inline int retrievepoints(t_instance * iinstance,float * ix,float * iy,float * iz,int inumber,basicmultilist * imultilist=NULL)
{
	if (inumber!=0) return 0;
	(*ix)=iinstance->xyz.x;
	(*iy)=iinstance->xyz.y;
	(*iz)=iinstance->xyz.z;
	return 1;
}
inline int retrievepoints(curve_instance * iinstance,float * ix,float * iy,float * iz,int inumber,basicmultilist * imultilist=NULL)
{
			(*iz)=0;//TODO:3D remove this
	if (inumber==0)
	{
		float sumx,sumy,sumz;
		sumx=0;
		sumy=0;
		sumz=0;
		for (int ilv1=0;ilv1<iinstance->CurvePoints.count;ilv1++)
		{
			sumx+=iinstance->CurvePoints.a[ilv1].x;
			sumy+=iinstance->CurvePoints.a[ilv1].y;
			//TODO:3D sumz+=iinstance->CurvePoints.a[ilv1].z;
		}
		if (iinstance->CurvePoints.count>=1)
		{
			(*ix)=sumx/(iinstance->CurvePoints.count);
			(*iy)=sumy/(iinstance->CurvePoints.count);
			//TODO: 3D (*iz)=sumz/iinstance->CurvePoints.count;
			return 1;
		}
	}
	if (inumber<0) inumber=-inumber;
	if (inumber>0)
	{
		if (inumber<=iinstance->CurvePoints.count)
		{
			(*ix)=iinstance->CurvePoints.a[inumber-1].x;
			(*iy)=iinstance->CurvePoints.a[inumber-1].y;
//			(*iz)=iinstance->CurvePoints.a[inumber].z;//TODO
			return 1;
		}
	}
	return 0;
}
int hit(n_instance * iinstance,float ix,float iy,int ino)
{
	float idistance;
	float tl_x,tl_y,tl_z;
	if (retrievepoints(iinstance,&tl_x,&tl_y,&tl_z,ino)>0)
	{
		idistance=(sqr(ix-tl_x)+sqr(iy-tl_y));
		if (idistance<glob_clickradius)
		{
			if (idistance<glob_subpoint_clickradius) return 1;
			if ((selection_clickabilitymatrix.types2[2] & (1<<STRUCTURE_OBJECTTYPE_b))==0) return 1;
			int inr=(((_iXX)iinstance)-((_iXX)(glob_n_multilist->bufferlist())))/sizeof(n_instance);
			int tl_bondcount=atom_actual_node[inr].bondcount;
			for (int ilv1=0;ilv1<tl_bondcount;ilv1++)
			{
				n_instance * iinstance2=(glob_n_multilist->bufferlist())+getother(inr,atom_actual_node[inr].bonds[ilv1]);
				float main_angle=getangle((*iinstance2).xyz.x-(*iinstance).xyz.x,(*iinstance2).xyz.y-(*iinstance).xyz.y);
				float tl_angle;
				tl_angle=getangle(ix-(*iinstance).xyz.x,iy-(*iinstance).xyz.y);
				tl_angle=fmod(tl_angle-main_angle+4*Pi,2*Pi);
				if ((tl_angle<Pi/4) || (tl_angle>7*Pi/4))
				{
					return 0;
				}
			}
			return 1;
		}
		else
		{
			return 0;
		}
	}
	return -1;
}
int hit(b_instance * iinstance,float ix,float iy,int ino)
{
	float idistance;
	n_instance *iinstance1=NULL;
	n_instance *iinstance2=NULL;
	if (ino!=0) return -1;
	for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
	{
		n_instance * tlinstance=&((*glob_n_multilist)[ilv1]);
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
	idistance=sqr(ix-((*iinstance1).xyz.x+(*iinstance2).xyz.x)*0.5)+sqr(iy-((*iinstance1).xyz.y+(*iinstance2).xyz.y)*0.5);
	if (idistance<glob_clickradius)
	{
		if (sqr(ix-(*iinstance1).xyz.x)+sqr(iy-(*iinstance1).xyz.y)<glob_subpoint_clickradius) return 0;
		if (sqr(ix-(*iinstance2).xyz.x)+sqr(iy-(*iinstance2).xyz.y)<glob_subpoint_clickradius) return 0;
		float main_angle=getangle((*iinstance2).xyz.x-(*iinstance1).xyz.x,(*iinstance2).xyz.y-(*iinstance1).xyz.y);
		float tl_angle;
		tl_angle=getangle(ix-(*iinstance1).xyz.x,iy-(*iinstance1).xyz.y);
		tl_angle=fmod(tl_angle-main_angle+4*Pi,2*Pi);
		if ((tl_angle>Pi/4) && (tl_angle<7*Pi/4))
		{
			return 0;
		}
		tl_angle=getangle((*iinstance2).xyz.x-ix,(*iinstance2).xyz.y-iy);
		tl_angle=fmod(tl_angle-main_angle+4*Pi,2*Pi);
		if ((tl_angle>Pi/4) && (tl_angle<7*Pi/4))
		{
			return 0;
		}
		return 1;
	}
	return 0;
}
int hit(graphic_instance * iinstance,float ix,float iy,int ino)
{
	float tl_x,tl_y,tl_z;
	if (retrievepoints(iinstance,&tl_x,&tl_y,&tl_z,ino)>0)
	{
		if ((sqr(ix-tl_x)+sqr(iy-tl_y))<glob_clickradius)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	return -1;
}
int hit(arrow_instance * iinstance,float ix,float iy,int ino)
{
	float tl_x,tl_y,tl_z;
	if (retrievepoints(iinstance,&tl_x,&tl_y,&tl_z,ino)>0)
	{
		if ((sqr(ix-tl_x)+sqr(iy-tl_y))<glob_clickradius)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	return -1;
}
int hit(t_instance * iinstance,float ix,float iy, int ino)
{
	if (ino!=0) return -1;
	if ((ix>=(*iinstance).BoundingBox.left) && (iy>=(*iinstance).BoundingBox.top)
	&& (ix<(*iinstance).BoundingBox.right) && (iy<(*iinstance).BoundingBox.bottom))
	{
		return 1;
	}
	return 0;
}
int hit(curve_instance * iinstance,float ix,float iy, int ino)
{
	float tl_x,tl_y,tl_z;
	if (retrievepoints(iinstance,&tl_x,&tl_y,&tl_z,ino)>0)
	{
		if ((sqr(ix-tl_x)+sqr(iy-tl_y))<glob_clickradius)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	return -1;
}
inline int placepoints(n_instance * iinstance,float ix,float iy,float iz,int inumber,basicmultilist * imultilist=NULL)
{
	int tl_backval;
	int ilv1=0;
	if (inumber>0)
	{
		if (imultilist==NULL) imultilist=glob_n_multilist;
		tl_backval=TELESCOPE_aggressobject(imultilist,iinstance-((multilist<n_instance>*)imultilist)->bufferlist());
		tl_backval=TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_Symbol);
		iback:;
		if (tl_backval>0)
		{
			ilv1++;
			Symbol_instance * tl_Symbol_instance=(Symbol_instance*)TELESCOPE_getproperty();
			if (ilv1==inumber)
			{
				(*tl_Symbol_instance).dxyz.x=ix-(*iinstance).xyz.x;
				(*tl_Symbol_instance).dxyz.y=iy-(*iinstance).xyz.y;
				(*tl_Symbol_instance).dxyz.z=iz-(*iinstance).xyz.z;
				return 1;
			}
			tl_backval=TELESCOPE_searchthroughobject_next(TELESCOPE_ELEMENTTYPE_Symbol);
			goto iback;
		}
		return 0;
	}
	if (inumber<0) return 0;
	(*iinstance).xyz.x=ix;
	(*iinstance).xyz.y=iy;
	(*iinstance).xyz.z=iz;
	return 1;
}
inline int placepoints(b_instance * iinstance,float ix,float iy,float iz,int inumber,basicmultilist * imultilist=NULL)
{
	if (inumber>0) return 0;
	if (inumber<0) return 0;
	float tl_x,tl_y,tl_z;
	if (imultilist!=NULL) return 0;//Not handled!(TODO?)
	if (retrievepoints(iinstance,&tl_x,&tl_y,&tl_z,inumber)<=0) return 0;
	tl_x-=ix;
	tl_y-=iy;
	n_instance *iinstance1=NULL;
	n_instance *iinstance2=NULL;
	for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
	{
		n_instance * tlinstance=&((*glob_n_multilist)[ilv1]);
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
	(*iinstance1).xyz.z-=tl_z;
	(*iinstance2).xyz.x-=tl_x;
	(*iinstance2).xyz.y-=tl_y;
	(*iinstance2).xyz.z-=tl_z;
	return 1;
}
void scalepoint(cdx_Point3D * inpoint,float pivotx,float pivoty,float startx,float starty,float endx,float endy)
{
	if (fabs(startx-pivotx)>0.1)
	{
		float dist=startx-pivotx;
		inpoint->x=pivotx+((inpoint->x-pivotx)/(startx-pivotx))*(endx-pivotx);
	}
	if (fabs(starty-pivoty)>0.1)
	{
		float dist=starty-pivoty;
		inpoint->y=pivoty+((inpoint->y-pivoty)/(starty-pivoty))*(endy-pivoty);
	}
	return;
}
inline int placepoints(graphic_instance * iinstance,float ix,float iy,float iz,int inumber,basicmultilist * imultilist=NULL)
{
	if (inumber>2) return 0;
	if (inumber<-5) return 0;
	if (inumber==-3)
	{
		(*iinstance).Center3D.x=ix;
		(*iinstance).Center3D.y=iy;
		(*iinstance).Center3D.z=iz;
		return 1;
	}
	if (inumber==-4)
	{
		(*iinstance).MajorAxisEnd3D.x=ix;
		(*iinstance).MajorAxisEnd3D.y=iy;
		(*iinstance).MajorAxisEnd3D.z=iz;
		return 1;
	}
	if (inumber==-5)
	{
		(*iinstance).MinorAxisEnd3D.x=ix;
		(*iinstance).MinorAxisEnd3D.y=iy;
		(*iinstance).MinorAxisEnd3D.z=iz;
		return 1;
	}
	if ((inumber==1) || (inumber==-1))
	{
		if (inumber>0)
		{
			scalepoint(&((*iinstance).Center3D),(*iinstance).BoundingBox.right,(*iinstance).BoundingBox.bottom,(*iinstance).BoundingBox.left,(*iinstance).BoundingBox.top,ix,iy);
			scalepoint(&((*iinstance).MinorAxisEnd3D),(*iinstance).BoundingBox.right,(*iinstance).BoundingBox.bottom,(*iinstance).BoundingBox.left,(*iinstance).BoundingBox.top,ix,iy);
			scalepoint(&((*iinstance).MajorAxisEnd3D),(*iinstance).BoundingBox.right,(*iinstance).BoundingBox.bottom,(*iinstance).BoundingBox.left,(*iinstance).BoundingBox.top,ix,iy);
		}
		(*iinstance).BoundingBox.left=ix;
		(*iinstance).BoundingBox.top=iy;
		return 1;
	}
	if ((inumber==2) || (inumber==-2))
	{
		if (inumber>0)
		{
			scalepoint(&((*iinstance).Center3D),(*iinstance).BoundingBox.left,(*iinstance).BoundingBox.top,(*iinstance).BoundingBox.right,(*iinstance).BoundingBox.bottom,ix,iy);
			scalepoint(&((*iinstance).MinorAxisEnd3D),(*iinstance).BoundingBox.left,(*iinstance).BoundingBox.top,(*iinstance).BoundingBox.right,(*iinstance).BoundingBox.bottom,ix,iy);
			scalepoint(&((*iinstance).MajorAxisEnd3D),(*iinstance).BoundingBox.left,(*iinstance).BoundingBox.top,(*iinstance).BoundingBox.right,(*iinstance).BoundingBox.bottom,ix,iy);
		}
		(*iinstance).BoundingBox.right=ix;
		(*iinstance).BoundingBox.bottom=iy;
		return 1;
	}
	float tl_x,tl_y,tl_z;
	if (retrievepoints(iinstance,&tl_x,&tl_y,&tl_z,inumber)<=0) return 0;
	tl_x-=ix;
	tl_y-=iy;
	(*iinstance).BoundingBox.left-=tl_x;
	(*iinstance).BoundingBox.top-=tl_y;
	(*iinstance).BoundingBox.right-=tl_x;
	(*iinstance).BoundingBox.bottom-=tl_y;
	(*iinstance).Center3D.x-=tl_x;
	(*iinstance).Center3D.y-=tl_y;
	(*iinstance).MajorAxisEnd3D.x-=tl_x;
	(*iinstance).MajorAxisEnd3D.y-=tl_y;
	(*iinstance).MinorAxisEnd3D.x-=tl_x;
	(*iinstance).MinorAxisEnd3D.y-=tl_y;
	return 1;
}
inline int placepoints(arrow_instance * iinstance,float ix,float iy,float iz,int inumber,basicmultilist * imultilist=NULL)
{
	if (inumber>2) return 0;
	if (inumber<-5) return 0;
	if (inumber==-1)
	{
		(*iinstance).Head3D.x=ix;
		(*iinstance).Head3D.y=iy;
		(*iinstance).Head3D.z=iz;
		return 1;
	}
	if (inumber==-2)
	{
		(*iinstance).Tail3D.x=ix;
		(*iinstance).Tail3D.y=iy;
		(*iinstance).Tail3D.z=iz;
		return 1;
	}
	if (inumber==-3)
	{
		(*iinstance).Center3D.x=ix;
		(*iinstance).Center3D.y=iy;
		(*iinstance).Center3D.z=iz;
		return 1;
	}
	if (inumber==-4)
	{
		(*iinstance).MajorAxisEnd3D.x=ix;
		(*iinstance).MajorAxisEnd3D.y=iy;
		(*iinstance).MajorAxisEnd3D.z=iz;
		return 1;
	}
	if (inumber==-5)
	{
		(*iinstance).MinorAxisEnd3D.x=ix;
		(*iinstance).MinorAxisEnd3D.y=iy;
		(*iinstance).MinorAxisEnd3D.z=iz;
		return 1;
	}
	float tl_x,tl_y,tl_z;
	if (retrievepoints(iinstance,&tl_x,&tl_y,&tl_z,inumber)<=0) return 0;
	tl_x-=ix;
	tl_y-=iy;
	tl_z-=iz;
	if (inumber==1)
	{
		float newangle=getangle(ix-(*iinstance).Tail3D.x,iy-(*iinstance).Tail3D.y);
		float newdist=sqrt(fsqr(ix-(*iinstance).Tail3D.x)+fsqr(iy-(*iinstance).Tail3D.y));
		float oldangle=getangle((*iinstance).Head3D.x-(*iinstance).Tail3D.x,(*iinstance).Head3D.y-(*iinstance).Tail3D.y);
		float olddist=sqrt(fsqr((*iinstance).Head3D.x-(*iinstance).Tail3D.x)+fsqr((*iinstance).Head3D.y-(*iinstance).Tail3D.y));
		float tl_matrix[3][3];
		if (olddist<0.0001) olddist=0.0001;
		tl_matrix[0][0]=(newdist/olddist)*cos(newangle-oldangle);
		tl_matrix[0][1]=(newdist/olddist)*sin(newangle-oldangle);
		tl_matrix[0][2]=0;
		tl_matrix[1][0]=-(newdist/olddist)*sin(newangle-oldangle);
		tl_matrix[1][1]=(newdist/olddist)*cos(newangle-oldangle);
		tl_matrix[1][2]=0;
		tl_matrix[2][0]=0;
		tl_matrix[2][1]=0;
		tl_matrix[2][2]=1;
		cdx_Point3D tl_old=iinstance->Head3D;
		cdx_Point3D tl_new;
		cdx_Point3D tl_piv=iinstance->Tail3D;
		for (int ilv1=-1;ilv1>=-5;ilv1--)
		{
			retrievepoints(iinstance,&(tl_old.x),&(tl_old.y),&(tl_old.z),ilv1);
			applytransform_single(tl_matrix,&tl_old,&tl_new,&tl_piv);
			placepoints(iinstance,tl_new.x,tl_new.y,tl_new.z,ilv1);
		}
		return 1;
	}
	if (inumber==2)
	{
		float newangle=getangle(ix-(*iinstance).Head3D.x,iy-(*iinstance).Head3D.y);
		float newdist=sqrt(fsqr(ix-(*iinstance).Head3D.x)+fsqr(iy-(*iinstance).Head3D.y));
		float oldangle=getangle((*iinstance).Tail3D.x-(*iinstance).Head3D.x,(*iinstance).Tail3D.y-(*iinstance).Head3D.y);
		float olddist=sqrt(fsqr((*iinstance).Tail3D.x-(*iinstance).Head3D.x)+fsqr((*iinstance).Tail3D.y-(*iinstance).Head3D.y));
		float tl_matrix[3][3];
		if (olddist<0.0001) olddist=0.0001;
		tl_matrix[0][0]=(newdist/olddist)*cos(newangle-oldangle);
		tl_matrix[0][1]=(newdist/olddist)*sin(newangle-oldangle);
		tl_matrix[0][2]=0;
		tl_matrix[1][0]=-(newdist/olddist)*sin(newangle-oldangle);
		tl_matrix[1][1]=(newdist/olddist)*cos(newangle-oldangle);
		tl_matrix[1][2]=0;
		tl_matrix[2][0]=0;
		tl_matrix[2][1]=0;
		tl_matrix[2][2]=1;
		cdx_Point3D tl_old=iinstance->Tail3D;
		cdx_Point3D tl_new;
		cdx_Point3D tl_piv=iinstance->Head3D;
		for (int ilv1=-1;ilv1>=-5;ilv1--)
		{
			retrievepoints(iinstance,&(tl_old.x),&(tl_old.y),&(tl_old.z),ilv1);
			applytransform_single(tl_matrix,&tl_old,&tl_new,&tl_piv);
			placepoints(iinstance,tl_new.x,tl_new.y,tl_new.z,ilv1);
		}
		return 1;
	}
	(*iinstance).Head3D.x-=tl_x;
	(*iinstance).Head3D.y-=tl_y;
	(*iinstance).Head3D.z-=tl_z;
	(*iinstance).Tail3D.x-=tl_x;
	(*iinstance).Tail3D.y-=tl_y;
	(*iinstance).Tail3D.z-=tl_z;
	(*iinstance).Center3D.x-=tl_x;
	(*iinstance).Center3D.y-=tl_y;
	(*iinstance).Center3D.z-=tl_z;
	(*iinstance).MajorAxisEnd3D.x-=tl_x;
	(*iinstance).MajorAxisEnd3D.y-=tl_y;
	(*iinstance).MajorAxisEnd3D.z-=tl_z;
	(*iinstance).MinorAxisEnd3D.x-=tl_x;
	(*iinstance).MinorAxisEnd3D.y-=tl_y;
	(*iinstance).MinorAxisEnd3D.z-=tl_z;
	return 1;
}
inline int placepoints(t_instance * iinstance,float ix,float iy,float iz,int inumber,basicmultilist * imultilist=NULL)
{
	float tl_x,tl_y,tl_z;
	if (retrievepoints(iinstance,&tl_x,&tl_y,&tl_z,inumber)>0)
	{
		tl_x-=ix;
		tl_y-=iy;
		tl_z-=iz;
		(*iinstance).xyz.x-=tl_x;
		(*iinstance).xyz.y-=tl_y;
		(*iinstance).xyz.z-=tl_z;
		(*iinstance).BoundingBox.left-=tl_x;
		(*iinstance).BoundingBox.right-=tl_x;
		(*iinstance).BoundingBox.top-=tl_y;
		(*iinstance).BoundingBox.bottom-=tl_y;
		return 1;
	}
	return 0;
}
inline int placepoints(curve_instance * iinstance,float ix,float iy,float iz,int inumber,basicmultilist * imultilist=NULL)
{
	char all=0;
	if (inumber<0) inumber=-inumber;
	if (inumber==0)
	{
		float tl_x,tl_y,tl_z;
		retrievepoints(iinstance,&tl_x,&tl_y,&tl_z,0);
		tl_x=ix-tl_x;
		tl_y=iy-tl_y;
		tl_z=iz-tl_z;
		for (int ilv1=0;ilv1<iinstance->CurvePoints.count;ilv1++)
		{
			iinstance->CurvePoints.a[ilv1].x+=tl_x;
			iinstance->CurvePoints.a[ilv1].y+=tl_y;
		}
		return 1;
	}
	if (inumber>0)
	{
		if (inumber<=iinstance->CurvePoints.count)
		{
			iinstance->CurvePoints.a[inumber-1].x=ix;
			iinstance->CurvePoints.a[inumber-1].y=iy;
			return 1;
		}
		if (inumber==iinstance->CurvePoints.count)
		{
			iinstance->CurvePoints.a[0].x=ix;
			iinstance->CurvePoints.a[0].y=iy;
			return 1;
		}
	}
	return 0;
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
#define LOCALMACRO_1(whatabout) case STRUCTURE_OBJECTTYPE_ ## whatabout: return hit((whatabout ## _instance*)iinstance,ix,iy,inumber);break;
int edit_hit(basic_instance * iinstance,float ix,float iy,int inumber,int objecttype)
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
	int ilv3;
	int follower=0;
	int internalpointcount;
	multilist<thisinstance> * imultilist=retrievemultilist<thisinstance>();
	internalpointcount=retrieveprops_basic(1,objecttype);
	for (int ilv1=0;ilv1<(*imultilist).filllevel;ilv1++)
	{
		thisinstance * tlinstance=&((*imultilist)[ilv1]);
		if ((*tlinstance).exist)
		{
			if (mode & 1) if (hit(tlinstance,posx,posy,0)>0)
			{
				selection_clickselection[ilv1]|=(1<<objecttype);
				found|=1<<objecttype;
			}
			if (mode & 2)
			{
				int tl_backval;
				if (internalpointcount>0)
				{
					follower=ilv1*internalpointcount;
				}
	 			ilv3=1;
				iback:;
				if (follower<(LHENDRAW_buffersize/sizeof(selection_datatype)))
				{
					tl_backval=hit(tlinstance,posx,posy,ilv3);
					if (tl_backval>=0)
					{
						if (tl_backval>0)
						{
							selection_clickselection[follower]|=(1<<(objecttype+STRUCTURE_OBJECTTYPE_ListSize));
							found|=1<<(objecttype+STRUCTURE_OBJECTTYPE_ListSize);
						}
						ilv3++;
						follower++;
						goto iback;
					}
				}
			}
		}
	}
	return found;
}

#define LOCALMACRO_1(whatabout) case STRUCTURE_OBJECTTYPE_ ## whatabout: return removepoints((whatabout ## _instance*)iinstance,inumber,imultilist);break;
int removepoints_basic(basic_instance * iinstance,int inumber,int objecttype,basicmultilist * imultilist=NULL)
{
	switch (objecttype)
	{
		LOCALMACROES
	}
	return 0;
}
#undef LOCALMACRO_1
#define LOCALMACRO_1(whatabout) case STRUCTURE_OBJECTTYPE_ ## whatabout: return retrievepoints((whatabout ## _instance*)iinstance,ix,iy,iz,inumber,imultilist);break;
int retrievepoints_basic(basic_instance * iinstance,float * ix,float * iy,float * iz,int inumber,int objecttype,basicmultilist * imultilist=NULL)
{
	float tl_iz=0;
	if (iz==NULL) iz=&tl_iz;
	switch (objecttype)
	{
		LOCALMACROES
	}
	return 0;
}
#undef LOCALMACRO_1
#define LOCALMACRO_1(whatabout) case STRUCTURE_OBJECTTYPE_ ## whatabout: return placepoints((whatabout ## _instance*)iinstance,ix,iy,iz,inumber,imultilist);break;
int placepoints_basic(basic_instance * iinstance,float ix,float iy,float iz,int inumber,int objecttype,basicmultilist * imultilist=NULL)
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
		LOCALMACROES
	}
	return 0;
}
#undef LOCALMACRO_1

char edit_atommatch(n_instance * iinstance1,n_instance * iinstance2)
{
	if ((fsqr((*iinstance1).xyz.x-(*iinstance2).xyz.x)+fsqr((*iinstance1).xyz.y-(*iinstance2).xyz.y))<9)
	{
		return 1;
	}
	return 0;
}
void edit_joinatom(n_instance * iinstance1,n_instance * iinstance2,int index2)
{
	int id=(*iinstance1).id;
	int oldid=(*iinstance2).id;
	for (int ilv1=0;ilv1<glob_b_multilist->filllevel;ilv1++)
	{
		b_instance * tl_b_instance=(*glob_b_multilist).bufferlist()+ilv1;
		if (tl_b_instance->exist)
		{
			if (tl_b_instance->B==oldid) tl_b_instance->B=id;
			if (tl_b_instance->E==oldid) tl_b_instance->E=id;
		}
	}
	TELESCOPE_aggressobject(glob_n_multilist,index2);
	TELESCOPE_clear();
	iinstance2->exist=0;
}
n_instance * snapatom(float posx,float posy,_small * iatomnr=NULL)
{
	float best=2000000000;
	n_instance * bestatom=NULL;
	for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
	{
		n_instance * tlatom=&((*glob_n_multilist)[ilv1]);
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
		n_instance * tlatom=&((*glob_n_multilist)[ilv1]);
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

basic_instance * getclicked(int imap,float clckx,float clcky,int * backtype=NULL,int * backindex=NULL,basic_instance ** backsub=NULL,float * backvalue=NULL,int * backsubnr=NULL)
{
	basic_instance * bestinstance=NULL;
	float bestvalue=0x2000000000;
	float thisvalue;
	int isize;
	for (int ilv0=1;ilv0<STRUCTURE_OBJECTTYPE_ListSize;ilv0++)
	{
		int necessary=((imap & (1<<ilv0))>0)+(((imap & (1<<(ilv0+STRUCTURE_OBJECTTYPE_ListSize)))>0)*2);
		if (necessary>0)
		{
			_u32 compare=1<<ilv0;
			basicmultilist * tlmultilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv0].name);
			isize=(*tlmultilist).itemsize;
			int internalpointcount=retrieveprops_basic(1,ilv0);
			int follower3=0;
			for (int ilv1=0;ilv1<(*tlmultilist).filllevel;ilv1++)
			{
				basic_instance * tlinstance=(basic_instance*)(((char*)(*tlmultilist).pointer)+STRUCTURE_OBJECTTYPE_List[ilv0].size*ilv1);
				if ((*tlinstance).exist)
				{
					float ix,iy,iz;
					if ((selection_clickselection[ilv1] & compare) && (necessary & 1))
					{
						retrievepoints_basic(tlinstance,&ix,&iy,&iz,0,ilv0);
						thisvalue=fsqr(ix-clckx)+fsqr(iy-clcky);
						if (thisvalue<bestvalue)
						{
							bestinstance=tlinstance;
							if (backtype!=NULL)
							{
								*backtype=ilv0;
							}
							if (backindex!=NULL)
							{
								*backindex=ilv1;
							}
							if (backsub!=NULL)
							{
								(*backsub)=NULL;
							}
							if (backsubnr!=NULL)
							{
								(*backsubnr)=0;
							}
							bestvalue=thisvalue;
						}
					}
					if (internalpointcount>=0)
					{
						for (int ilv2=0;ilv2<internalpointcount;ilv2++)
						{
							if ((selection_clickselection[ilv1*internalpointcount+ilv2] & (compare<<STRUCTURE_OBJECTTYPE_ListSize)) && (necessary & 2))
							{
								retrievepoints_basic(tlinstance,&ix,&iy,&iz,ilv2+1,ilv0);
								thisvalue=fsqr(ix-clckx)+fsqr(iy-clcky);
								if (thisvalue<bestvalue)
								{
									bestinstance=tlinstance;
									if (backtype!=NULL)
									{
										*backtype=ilv0+STRUCTURE_OBJECTTYPE_ListSize;
									}
									if (backindex!=NULL)
									{
										*backindex=ilv1*internalpointcount+ilv2;
									}
									if (backvalue!=NULL)
									{
										*backvalue=thisvalue;
									}
									if (backsubnr!=NULL)
									{
										(*backsubnr)=ilv2+1;
									}
									bestvalue=thisvalue;
								}
							}
						}
					}
					else
					{
						TELESCOPE_aggressobject(tlmultilist,ilv1);
						int tl_backval;
						int ilv2=0;
						tl_backval=retrievepoints_basic(tlinstance,&ix,&iy,&iz,ilv2+1,ilv0);
						while (tl_backval)
						{
							if (follower3<(LHENDRAW_buffersize/sizeof(selection_datatype)))
							{
								if (selection_clickselection[follower3] & (compare<<STRUCTURE_OBJECTTYPE_ListSize))
								{
									thisvalue=fsqr(ix-clckx)+fsqr(iy-clcky);
									if (thisvalue<bestvalue)
									{
										bestinstance=(basic_instance*)(tlmultilist->pointer+isize*ilv1);
										if (backtype!=NULL)
										{
											*backtype=ilv0+STRUCTURE_OBJECTTYPE_ListSize;
										}
										if (backindex!=NULL)
										{
											*backindex=follower3;
										}
										if (backsub!=NULL)
										{
											*backsub=(basic_instance*)TELESCOPE_getproperty();//Note that TELESCOPE_tempval gets set by retrievepoints_basic!
										}
										if (backsubnr!=NULL)
										{
											(*backsubnr)=ilv2+1;
										}
										if (backvalue!=NULL)
										{
											*backvalue=thisvalue;
										}
										bestvalue=thisvalue;
									}
								}
								follower3++;
								ilv2++;
								tl_backval=retrievepoints_basic(tlinstance,&ix,&iy,&iz,ilv2+1,ilv0);
							}
							else
							{
								//TODO: evaluate buffer shortage
							}
						}
					}
				}
			}
		}
	}
	return bestinstance;
}


n_instance * edit_summonatom(_i32 * inr=NULL)
{
	if ((*glob_n_multilist).filllevel<(*glob_n_multilist).getmaxitems())
	{
		int tl_nr=-1;
		n_instance * tlinstance;
		if (janitor_getmaxid(STRUCTURE_OBJECTTYPE_n)<0) return NULL;
		tl_nr=(*glob_n_multilist).filllevel;
		tlinstance=new(&((*glob_n_multilist)[tl_nr])) n_instance;
		(*tlinstance).Element=constants_Element_implicitcarbon;
		(*tlinstance).color=control_drawproperties.color;
		(*tlinstance).charge=0;
		(*tlinstance).protons=4;
		(*tlinstance).Z=edit_getnewZ();
		selection_currentselection[tl_nr]&=(~(1<<STRUCTURE_OBJECTTYPE_n));
		atom_actual_node[tl_nr].bondcount=0;
		((*glob_n_multilist).filllevel)++;
		(*tlinstance).id=(*glob_n_multilist).maxid+1;
		(*glob_n_multilist).maxid++;
		if (inr!=NULL)
		{
			*inr=tl_nr;
		}
		return tlinstance;
	}
	return NULL;
}
graphic_instance * edit_summongraphic(_i32 * inr=NULL)
{
	if ((*glob_graphic_multilist).filllevel<(*glob_graphic_multilist).getmaxitems())
	{
		int tl_nr=-1;
		graphic_instance * tlinstance;
		if (janitor_getmaxid(STRUCTURE_OBJECTTYPE_graphic)<0) return NULL;
		tl_nr=(*glob_graphic_multilist).filllevel;
		tlinstance=new(&((*glob_graphic_multilist)[tl_nr])) graphic_instance;
		(*tlinstance).color=control_drawproperties.color;
		(*tlinstance).Z=edit_getnewZ();
		selection_currentselection[tl_nr]&=(~(1<<STRUCTURE_OBJECTTYPE_graphic));
		((*glob_graphic_multilist).filllevel)++;
		(*tlinstance).FillType=control_drawproperties.FillType;
		(*tlinstance).id=(*glob_graphic_multilist).maxid+1;
		(*tlinstance).LineType=control_drawproperties.LineType;
		(*tlinstance).BracketType=control_drawproperties.BracketType;
		(*glob_graphic_multilist).maxid++;
		if (inr!=NULL)
		{
			*inr=tl_nr;
		}
		return tlinstance;
	}
	return NULL;
}
tlcplate_instance * edit_summontlcplate(int * nr=NULL)
{
	if ((*glob_tlcplate_multilist).filllevel<(*glob_tlcplate_multilist).getmaxitems())
	{
		int tl_nr=-1;
		tlcplate_instance * tlinstance;
		tl_nr=(*glob_tlcplate_multilist).filllevel;
		tlinstance=new(&((*glob_tlcplate_multilist)[tl_nr])) tlcplate_instance;
		(*tlinstance).color=control_drawproperties.color;
		(*tlinstance).Z=edit_getnewZ();
		(*tlinstance).LineType=control_drawproperties.LineType;
		((*glob_tlcplate_multilist).filllevel)++;
		cdx_tlcspot itlcspot;
		tlclane_instance tl_tlclane_instance;
		TELESCOPE_aggressobject(glob_tlcplate_multilist,tl_nr);
		tl_tlclane_instance.length=sizeof(tlclane_instance)+sizeof(cdx_tlcspot)*3;
		tl_tlclane_instance.type=TELESCOPE_ELEMENTTYPE_tlclane;
		itlcspot.Rf=0.3;
		itlcspot.color=0x00FF00;
		itlcspot.Curve_Type=1;
		TELESCOPE_add(TELESCOPE_ELEMENTTYPE_tlclane,(char*)&itlcspot,sizeof(cdx_tlcspot)*3);
		TELESCOPE_measure(TELESCOPE_ELEMENTTYPE_tlclane,& glob_contentbuffer[STRUCTURE_OBJECTTYPE_tlcplate]);
		cdx_tlcspot * spots=(cdx_tlcspot*)TELESCOPE_getproperty_contents();
		itlcspot.Rf=0.2;
		itlcspot.color=0x00FF00;
		spots[1]=itlcspot;
		itlcspot.Rf=0.1;
		itlcspot.color=0x00FF00;
		spots[2]=itlcspot;
		*((tlclane_instance*)TELESCOPE_getproperty())=tl_tlclane_instance;
		TELESCOPE_aggressobject(glob_tlcplate_multilist,tl_nr);
		tl_tlclane_instance.length=sizeof(tlclane_instance)+sizeof(cdx_tlcspot)*2;
		tl_tlclane_instance.type=TELESCOPE_ELEMENTTYPE_tlclane;
		itlcspot.Rf=0.2;
		itlcspot.color=0x00FF00;
		TELESCOPE_add(TELESCOPE_ELEMENTTYPE_tlclane,(char*)&itlcspot,sizeof(cdx_tlcspot)*2);
		TELESCOPE_measure(TELESCOPE_ELEMENTTYPE_tlclane,& glob_contentbuffer[STRUCTURE_OBJECTTYPE_tlcplate]);
		spots=(cdx_tlcspot*)TELESCOPE_getproperty_contents();
		itlcspot.Rf=0.1;
		itlcspot.color=0x00FF00;
		spots[1]=itlcspot;
		*((tlclane_instance*)TELESCOPE_getproperty())=tl_tlclane_instance;
		if (nr!=NULL)
		{
			*nr=tl_nr;
		}
		return tlinstance;
	}
	return NULL;
}
catalogized_command_funcdef(EQUILIBRIUM_ARROWS)
{
	int to_value=atoi(value);
	if (control_drawproperties.arrow_ArrowheadType!=to_value)
	{
		control_drawproperties.arrow_ArrowheadType=to_value;
		if (to_value==4)
		{
			control_drawproperties.arrow_ArrowheadHead=3;
			control_drawproperties.arrow_ArrowheadTail=3;
			control_drawproperties.arrow_ArrowShaftSpacing=4;
			if (strcmp(parameter,"1")==0)
			{
				SETITEMVARIABLES("ArrowheadType","Solid");
				SETITEMVARIABLES("ArrowheadHead","HalfLeft");
				SETITEMVARIABLES("ArrowheadTail","HalfLeft");
				SETITEMVARIABLES("ArrowShaftSpacing","4");
			}
			return 1;
		}
	}
	else
	{
		control_drawproperties.arrow_ArrowheadType=1;
		to_value=1;
	}
	control_drawproperties.arrow_ArrowheadHead=2;
	control_drawproperties.arrow_ArrowheadTail=1;
	control_drawproperties.arrow_ArrowShaftSpacing=0;
	if (strcmp(parameter,"1")==0)
	{
		SETITEMVARIABLES("ArrowheadType",lookup_bienum(CDXML_ArrowheadType,CDXML_ArrowheadType_max,to_value));
		SETITEMVARIABLES("ArrowheadHead",lookup_bienum(CDXML_ArrowheadHead,CDXML_ArrowheadHead_max,control_drawproperties.arrow_ArrowheadHead));
		SETITEMVARIABLES("ArrowheadTail",lookup_bienum(CDXML_ArrowheadTail,CDXML_ArrowheadTail_max,control_drawproperties.arrow_ArrowheadTail));
		SETITEMVARIABLES("ArrowShaftSpacing","0");
	}
	return 1;
}
arrow_instance * edit_summonarrow(int * inr=NULL)
{
	if ((*glob_arrow_multilist).filllevel<(*glob_arrow_multilist).getmaxitems())
	{
		int tl_nr=-1;
		arrow_instance * tlinstance;
		tl_nr=(*glob_arrow_multilist).filllevel;
		tlinstance=new(&((*glob_arrow_multilist)[tl_nr])) arrow_instance;
		selection_currentselection[tl_nr]&=(~(1<<STRUCTURE_OBJECTTYPE_arrow));
		(*tlinstance).color=control_drawproperties.color;
		(*tlinstance).ArrowheadType=control_drawproperties.arrow_ArrowheadType;
		(*tlinstance).ArrowheadHead=control_drawproperties.arrow_ArrowheadHead;
		(*tlinstance).ArrowheadTail=control_drawproperties.arrow_ArrowheadTail;
		(*tlinstance).LineType=control_drawproperties.LineType;
		(*tlinstance).FillType=control_drawproperties.FillType;
		if ((control_drawproperties.arrow_ArrowheadType>=2) && (control_drawproperties.arrow_ArrowheadType<=3))
		{
			(*tlinstance).ArrowShaftSpacing=4;
		}
		if (control_drawproperties.arrow_ArrowheadType==4)
		{
			(*tlinstance).ArrowheadType=1;
			(*tlinstance).ArrowShaftSpacing=4;
		}
		((*glob_arrow_multilist).filllevel)++;
		if (inr!=NULL)
		{
			*inr=tl_nr;
		}
		return tlinstance;
	}
	return NULL;
}
b_instance * edit_summonbond(int i_id_begin,int i_id_end,int i_nr_begin,int i_nr_end)
{
	if ((*glob_b_multilist).filllevel<(*glob_b_multilist).getmaxitems())
	{
		b_instance * tlinstance;
		tlinstance=new(&((*glob_b_multilist)[(*glob_b_multilist).filllevel])) b_instance;
		selection_currentselection[(*glob_b_multilist).filllevel]&=(~(1<<STRUCTURE_OBJECTTYPE_b));
		(*tlinstance).B=i_id_begin;
		(*tlinstance).E=i_id_end;
		(*tlinstance).Display=control_drawproperties.bond_Display;
		(*tlinstance).DoublePosition=0;
		(*tlinstance).Display2=control_drawproperties.bond_Display2;
		(*tlinstance).Order=0x10;
		(*tlinstance).color=control_drawproperties.color;
		(*tlinstance).Z=edit_getnewZ();
		atom_actual_node[i_nr_begin]+=((*glob_b_multilist).filllevel);
		atom_actual_node[i_nr_end]+=((*glob_b_multilist).filllevel);
		bond_actual_node[(*glob_b_multilist).filllevel].start=i_nr_begin;
		bond_actual_node[(*glob_b_multilist).filllevel].end=i_nr_end;
		((*glob_b_multilist).filllevel)++;
		(*tlinstance).id=(*glob_b_multilist).maxid+1;
		(*glob_b_multilist).maxid++;
		return tlinstance;
	}
	return NULL;
}
t_instance * edit_summontext(int * inr=NULL)
{
	if ((*glob_t_multilist).filllevel<(*glob_t_multilist).getmaxitems())
	{
		int tl_nr=-1;
		t_instance * tlinstance;
		tl_nr=(*glob_t_multilist).filllevel;
		tlinstance=new(&((*glob_t_multilist)[tl_nr])) t_instance;
		(*tlinstance).color=control_drawproperties.color;
		selection_currentselection[tl_nr]&=(~(1<<STRUCTURE_OBJECTTYPE_t));
		(*tlinstance).Z=edit_getnewZ();
		((*glob_t_multilist).filllevel)++;
		if (inr!=NULL)
		{
			*inr=tl_nr;
		}
		return tlinstance;
	}
	return NULL;
}
curve_instance * edit_summoncurve(int * inr=NULL)
{
	if ((*glob_curve_multilist).filllevel<(*glob_curve_multilist).getmaxitems())
	{
		int tl_nr=-1;
		curve_instance * tlinstance;
		tl_nr=(*glob_curve_multilist).filllevel;
		tlinstance=new(&((*glob_curve_multilist)[tl_nr])) curve_instance;
		(*tlinstance).color=control_drawproperties.color;
		(*tlinstance).CurvePoints.count=0;
		(*tlinstance).Closed=control_drawproperties.curve_Closed;
		(*tlinstance).ArrowheadHead=control_drawproperties.arrow_ArrowheadHead;
		(*tlinstance).ArrowheadTail=control_drawproperties.arrow_ArrowheadTail;
		(*tlinstance).ArrowheadType=control_drawproperties.arrow_ArrowheadType;
		if ((control_drawproperties.arrow_ArrowheadType>=2) && (control_drawproperties.arrow_ArrowheadType<=3))
		{
			(*tlinstance).ArrowShaftSpacing=4;
		}
		(*tlinstance).FillType=control_drawproperties.FillType;
		(*tlinstance).LineType=control_drawproperties.LineType;
		(*tlinstance).Z=edit_getnewZ();
		selection_currentselection[tl_nr]&=(~(1<<STRUCTURE_OBJECTTYPE_curve));
		((*glob_curve_multilist).filllevel)++;
		if (inr!=NULL)
		{
			*inr=tl_nr;
		}
		return tlinstance;
	}
	return NULL;
}
int atom_addsymbol(int inr,int itype)
{
	switch (itype)
	{
		case 2: case 4: case 8:
		if ((*glob_n_multilist)[inr].charge<0)
		{
			TELESCOPE_aggressobject(glob_n_multilist,inr);
			(*glob_n_multilist)[inr].charge=0;
			TELESCOPE_clear();
			return 1;
		}
		(*glob_n_multilist)[inr].charge+=1;
		break;
		case 3: case 5: case 9:
		if ((*glob_n_multilist)[inr].charge>0)
		{
			TELESCOPE_aggressobject(glob_n_multilist,inr);
			(*glob_n_multilist)[inr].charge=0;
			TELESCOPE_clear();
			return 1;
		}
		(*glob_n_multilist)[inr].charge-=1;
		break;
	}
	if (((itype>=2) && (itype<=5)) || ((itype>=8) && (itype<=9)))
	{
		int tl_Element=(*glob_n_multilist)[inr].Element;
		if ((*glob_n_multilist)[inr].charge>0)
		{
			if (element[tl_Element].hasVE>4) (*glob_n_multilist)[inr].protons++; else (*glob_n_multilist)[inr].protons--;
		}
		if ((*glob_n_multilist)[inr].charge<0)
		{
			if (element[tl_Element].hasVE==3) (*glob_n_multilist)[inr].protons++; else (*glob_n_multilist)[inr].protons--;
		}
	}
	TELESCOPE_aggressobject(glob_n_multilist,inr);
	Symbol_instance tl_Symbol_instance;
	tl_Symbol_instance.length=sizeof(Symbol_instance);
	tl_Symbol_instance.type=TELESCOPE_ELEMENTTYPE_Symbol;
	tl_Symbol_instance.dxyz.x=9.19;
	tl_Symbol_instance.dxyz.y=-9.19;
	tl_Symbol_instance.dxyz.z=0;
	tl_Symbol_instance.SymbolType=itype;
	int wert=TELESCOPE_add(TELESCOPE_ELEMENTTYPE_Symbol,NULL,0);
	if (wert>0)
	{
		*((Symbol_instance*)TELESCOPE_getproperty())=tl_Symbol_instance;
	}
	return wert;
}
float edit_get_atom_best_free_side(int atomnr)
{
//	atom_actual_node//Should I really rely on them? better leave them as they were. But then it wont work for NEW atoms...
	float angle,angle2;
	float best=0;int bestnr=-1;float bestvalue=0;
	if (atom_actual_node[atomnr].bondcount>2)
	{
		int atomnr2=getother(atomnr,atom_actual_node[atomnr].bonds[0]);
		angle=fmod(getangle((*glob_n_multilist)[atomnr2].xyz.x-(*glob_n_multilist)[atomnr].xyz.x,(*glob_n_multilist)[atomnr2].xyz.y-(*glob_n_multilist)[atomnr].xyz.y)+5*Pi,2*Pi);
		if (atom_actual_node[atomnr].bondcount>=6)
		{

		}
		return angle;
	}
	if (atom_actual_node[atomnr].bondcount==2)
	{
		int atomnr2=getother(atomnr,atom_actual_node[atomnr].bonds[atom_actual_node[atomnr].bondcount-2]);
		int atomnr2b=getother(atomnr,atom_actual_node[atomnr].bonds[atom_actual_node[atomnr].bondcount-1]);
		angle=fmod((2*getangle((*glob_n_multilist)[atomnr2].xyz.x-(*glob_n_multilist)[atomnr].xyz.x,(*glob_n_multilist)[atomnr2].xyz.y-(*glob_n_multilist)[atomnr].xyz.y))-getangle((*glob_n_multilist)[atomnr2b].xyz.x-(*glob_n_multilist)[atomnr].xyz.x,(*glob_n_multilist)[atomnr2b].xyz.y-(*glob_n_multilist)[atomnr].xyz.y)+4*Pi,2*Pi);
		return angle;
	}
	if (atom_actual_node[atomnr].bondcount==1)
	{
		int atomnr2=getother(atomnr,atom_actual_node[atomnr].bonds[0]);
		if (((control_drawproperties.bond_Order>16) && (glob_b_multilist->bufferlist()[atom_actual_node[atomnr].bonds[0]].Order>24))||(control_drawproperties.bond_Order>32)||(glob_b_multilist->bufferlist()[atom_actual_node[atomnr].bonds[0]].Order>40))
		{
			return fmod(getangle((*glob_n_multilist)[atomnr2].xyz.x-(*glob_n_multilist)[atomnr].xyz.x,(*glob_n_multilist)[atomnr2].xyz.y-(*glob_n_multilist)[atomnr].xyz.y)+5*Pi,2*Pi);
		}
		if (atom_actual_node[atomnr2].bondcount<=1) goto skipadverse;
		{
			int atomnr3=getother(atomnr2,atom_actual_node[atomnr2].bonds[0]);
			if (atomnr3==atomnr)
			{
				atomnr3=getother(atomnr2,atom_actual_node[atomnr2].bonds[1]);
			}
			if (fmod(getangle((*glob_n_multilist)[atomnr3].xyz.x-(*glob_n_multilist)[atomnr2].xyz.x,(*glob_n_multilist)[atomnr3].xyz.y-(*glob_n_multilist)[atomnr2].xyz.y)-
			getangle((*glob_n_multilist)[atomnr].xyz.x-(*glob_n_multilist)[atomnr2].xyz.x,(*glob_n_multilist)[atomnr].xyz.y-(*glob_n_multilist)[atomnr2].xyz.y)+4*Pi,2*Pi)>Pi)
			return fmod(getangle((*glob_n_multilist)[atomnr2].xyz.x-(*glob_n_multilist)[atomnr].xyz.x,(*glob_n_multilist)[atomnr2].xyz.y-(*glob_n_multilist)[atomnr].xyz.y)-constants_angular_distance+6*Pi,2*Pi);
		}
		skipadverse:;
		return fmod(getangle((*glob_n_multilist)[atomnr2].xyz.x-(*glob_n_multilist)[atomnr].xyz.x,(*glob_n_multilist)[atomnr2].xyz.y-(*glob_n_multilist)[atomnr].xyz.y)+constants_angular_distance+4*Pi,2*Pi);
	}
	return (Pi-constants_angular_distance)/2;
}
int edit_errichten(int startatom)
{
	int atomnr2=-1;
	n_instance * tlatom=&((*glob_n_multilist)[startatom]);
	if (tlatom==0) {return -1;}
	n_instance * tlatom2;
	b_instance * tlbond;
	if (atom_actual_node[startatom].bondcount==constants_maxbonds) return -1;
	float tlangle;
	tlangle=edit_get_atom_best_free_side(startatom);
	float posx=(*tlatom).xyz.x+constants_bondlength*cos(tlangle);
	float posy=(*tlatom).xyz.y+constants_bondlength*sin(tlangle);
	tlatom2=snapatom_short(posx,posy,&atomnr2,10);
	if (tlatom2)
	{
	}
	else
	{
		tlatom2=edit_summonatom(&atomnr2);
		if (tlatom2)
		{
			control_hotatom=atomnr2;
			(*tlatom2).xyz.x=posx;
			(*tlatom2).xyz.y=posy;
			(*tlatom2).xyz.z=(*tlatom).xyz.z;
			(*tlatom2).Z=(*tlatom).Z+1;
		}
	}
	if ((tlatom) && (tlatom2))
	{
		tlbond=edit_summonbond((*tlatom).id,(*tlatom2).id,startatom,atomnr2);
		if (tlbond)
		{
			(*tlbond).Z=(*tlatom).Z+2;
			(*tlbond).Order=control_drawproperties.bond_Order;
		}
	}
	return atomnr2;
}
catalogized_command_funcdef(SPROUT2)
{
	int counter=0;
	int sproutcount=atoi(value);
	if (selection_currentselection_found & (1<<STRUCTURE_OBJECTTYPE_n))
	{
		for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
		{
			if (selection_currentselection[ilv1] & (1<<STRUCTURE_OBJECTTYPE_n))
			{
				if ((*glob_n_multilist)[ilv1].exist)
				{ 
					counter++;
					for (int ilv1=0;ilv1<sproutcount;ilv1++)
					{
						edit_errichten(ilv1);
					}
				}
			}
		}
		return counter;
	}
	if (gethot()!=-1)
	{
		for (int ilv1=0;ilv1<sproutcount;ilv1++)
		{
			edit_errichten(control_hotatom);
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
		if ((control_hotatom=edit_errichten(iindex))>=0) {getatoms();found++;}
	}
	return found;
}
int edit_setelement(int i_Elementnr,n_instance * iinstance,int iindex)
{
	iback:;
	TELESCOPE_aggressobject(glob_n_multilist,iindex);
	int tl_backval=TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_s);
	if (tl_backval>0)
	{
		TELESCOPE_clear_item();
		goto iback;
	}
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
	iback:;
	TELESCOPE_aggressobject(glob_n_multilist,iindex);
	int tl_backval=TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_s);
	if (tl_backval>0)
	{
		TELESCOPE_clear_item();
		goto iback;
	}
	TELESCOPE_aggressobject(glob_n_multilist,iindex);
	s_instance tl_s_instance;
	tl_s_instance.length=sizeof(s_instance);
	tl_s_instance.type=TELESCOPE_ELEMENTTYPE_s;
	tl_s_instance.face=0x60+(control_drawproperties.face & 0xF);
	tl_s_instance.font=1;
	tl_s_instance.size=12;
	tl_s_instance.type=TELESCOPE_ELEMENTTYPE_s;
	tl_s_instance.length=sizeof(s_instance)+strlen(value)+1;//trailing 0 //TODO: which string
	TELESCOPE_add(TELESCOPE_ELEMENTTYPE_s,value,tl_s_instance.length-sizeof(s_instance));//TODO: string
	*((s_instance*)TELESCOPE_getproperty())=tl_s_instance;
	(*(n_instance*)iinstance).Element=-1;
	return 1;
}
catalogized_command_iterated_funcdef(LASTLABELTEXT)
{
	LABELTEXT(parameter,element[control_drawproperties.Element].name,imultilist,iinstance,iindex);
	return 1;
}
catalogized_command_iterated_funcdef(SETITEMVARIABLE)
{
	for (int ilv1=0;ilv1<(*iinstance)._->properties_count;ilv1++)
	{
		if (strcmp((*iinstance)._->properties[ilv1].name,parameter)==0)
		{
			printf("Does%08llX,\n",*(long long*)(((char*)iinstance)+((*iinstance)._->properties[ilv1].ref)));
			if (((catalogized_command_iterated_functype)((*iinstance)._->properties[ilv1].delegate))((char*)value,(((char*)iinstance)+((*iinstance)._->properties[ilv1].ref)),imultilist,iinstance,iindex)>=0)
			{
				printf("it work%s\n",value);
				return 1;
			}
		}
	}
	return 0;
}
catalogized_command_funcdef(SETITEMVARIABLES)
{
	char imemory[100];
	char * imemoryp=imemory;
	int isize;
	int ioffset;
	int iparametersize;
	char * tl_pointer;
	for (int ilv1=1;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
	{
		_i32 icompare=1<<ilv1;
		basicmultilist * tl_multilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
		for (int ilv1=0;ilv1<(*tl_multilist)._->properties_count;ilv1++)
		{
			if (strcmp((*tl_multilist)._->properties[ilv1].name,parameter)==0)
			{
				iparametersize=(*tl_multilist)._->properties[ilv1].size;
				ioffset=(*tl_multilist)._->properties[ilv1].ref;
				if (iparametersize>100)
				{
					imemoryp=(char*)malloc(iparametersize);
				}
				else
				{
					imemoryp=imemory;
				}
				(*tl_multilist)._->properties[ilv1].delegate((char*)value,imemoryp);
				goto istart;
			}
		}
		goto iskip;
		istart:
		isize=(*tl_multilist).itemsize;
		for (int ilv2=0;ilv2<(*tl_multilist).filllevel;ilv2++)
		{
			basic_instance * tl_basic_instance=(basic_instance*)((*tl_multilist).pointer+(isize*ilv2));
			if ((*tl_basic_instance).exist)
			{
				if (selection_currentselection[ilv2] & (1<<ilv1))
				{
					tl_pointer=((char*)tl_basic_instance)+ioffset;
					for (int ilv3=0;ilv3<iparametersize;ilv3++)
					{
						(*(tl_pointer++))=imemoryp[ilv3];
					}
				}
			}
		}
		if (iparametersize>100)
		{
			free(imemoryp);
		}
		iskip:;
	}
	return 1;
}
catalogized_command_funcdef(RESETBONDSTYLE)
{
	control_drawproperties.bond_Order=16;
	control_drawproperties.bond_Display=0;
	control_drawproperties.bond_Display2=0;
	if (strcmp(value,"0")==0)
	{
		SETITEMVARIABLES("Order","1");
	}
	else
	{
		SETITEMVARIABLES("Order",value);
	}
	SETITEMVARIABLES("Display","0");
	SETITEMVARIABLES("Display2","0");
	return 1;
}
catalogized_command_iterated_funcdef(SWAPDOUBLEPOSITION)
{
	_i32 * idoubleposition=&((*(b_instance*)iinstance).DoublePosition);
	if (((*(b_instance*)iinstance).Order<24) || ((*(b_instance*)iinstance).Order>=48))
	{
		(*(b_instance*)iinstance).Order=32;
		return 1;
	}
	if ((*idoubleposition)<0x100)
	{
		*idoubleposition=0x100;
	}
	else
	{
		*idoubleposition=((((*idoubleposition)-0x100)+1)%3)+0x100;
	}
	return 1;
}
catalogized_command_funcdef(FILE_NEW)
{
	for (int ilv1=1;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
	{
		basicmultilist * tlmultilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
		if (tlmultilist)
		{
			(*tlmultilist).filllevel=0;
			glob_contentbuffer[ilv1].count=0;
			//TODO: undo: buffer lists!
		}
	}
	for (int ilv1=0;ilv1<sizeof(janitor_id_list)/sizeof(int);ilv1++)
	{
		janitor_id_list[ilv1]=0;
	}
	return 1;
}
extern float control_coorsx;
extern float control_coorsy;
void checkupinconsistencies();
catalogized_command_iterated_funcdef(CHARGE)
{
	if (imultilist==glob_n_multilist)
	{
		atom_addsymbol(iindex,(value[0]=='+')?4:5);
	}
	return 1;
}
catalogized_command_funcdef(CHUP)
{
	checkupinconsistencies();
	return 1;
}
void	issueshiftstart();
catalogized_command_funcdef(BLOT)
{
	issueshiftstart();
	selection_clearselection(selection_clickselection);
	clickfor(control_coorsx,control_coorsy,STRUCTURE_OBJECTTYPE_n,1000,1);
	n_instance * iinstance=(n_instance*)getclicked((1<<STRUCTURE_OBJECTTYPE_n),control_coorsx,control_coorsy);
	printf("|%i,%lli,%lli\n",(*glob_n_multilist).maxid,(*glob_n_multilist).filllevel,(*glob_b_multilist).filllevel);
	if (iinstance)
	{
		printf("%f,%f,%i\n",(*iinstance).xyz.x,(*iinstance).xyz.y,(*iinstance).id);
	}
	return 1;
}
catalogized_command_funcdef(BONDDATA)
{
	for (int ilv2=0;ilv2<glob_b_multilist->filllevel;ilv2++)
	{
		if (glob_b_multilist->bufferlist()[ilv2].exist)
		{
			if (selection_currentselection[ilv2] & (1<<STRUCTURE_OBJECTTYPE_b))
			{
				printf("%i;%i\n",glob_b_multilist->bufferlist()[ilv2].B,glob_b_multilist->bufferlist()[ilv2].E);
			}
		}
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
		(*glob_CAMBRIDGE_color_multilist)[(ilv1+1)%8].b=b;
		(*glob_CAMBRIDGE_color_multilist)[(ilv1+1)%8].g=g;
		(*glob_CAMBRIDGE_color_multilist)[(ilv1+1)%8].r=r;
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
						idone:;
					}
				}
			}
		}
	}
}
_u32 crc_table[]={
0x00000000,0x77073096,0xee0e612c,0x990951ba,0x076dc419,0x706af48f,0xe963a535,0x9e6495a3,0x0edb8832,0x79dcb8a4,0xe0d5e91e,0x97d2d988,0x09b64c2b,0x7eb17cbd,0xe7b82d07,0x90bf1d91,0x1db71064,0x6ab020f2,0xf3b97148,0x84be41de,0x1adad47d,0x6ddde4eb,0xf4d4b551,0x83d385c7,0x136c9856,0x646ba8c0,0xfd62f97a,0x8a65c9ec,0x14015c4f,0x63066cd9,0xfa0f3d63,0x8d080df5,0x3b6e20c8,0x4c69105e,0xd56041e4,0xa2677172,0x3c03e4d1,0x4b04d447,0xd20d85fd,0xa50ab56b,0x35b5a8fa,0x42b2986c,0xdbbbc9d6,0xacbcf940,0x32d86ce3,0x45df5c75,0xdcd60dcf,0xabd13d59,0x26d930ac,0x51de003a,0xc8d75180,0xbfd06116,0x21b4f4b5,0x56b3c423,0xcfba9599,0xb8bda50f,0x2802b89e,0x5f058808,0xc60cd9b2,0xb10be924,0x2f6f7c87,0x58684c11,0xc1611dab,0xb6662d3d,0x76dc4190,0x01db7106,0x98d220bc,0xefd5102a,0x71b18589,0x06b6b51f,0x9fbfe4a5,0xe8b8d433,0x7807c9a2,0x0f00f934,0x9609a88e,0xe10e9818,0x7f6a0dbb,0x086d3d2d,0x91646c97,0xe6635c01,0x6b6b51f4,0x1c6c6162,0x856530d8,0xf262004e,0x6c0695ed,0x1b01a57b,0x8208f4c1,0xf50fc457,0x65b0d9c6,0x12b7e950,0x8bbeb8ea,0xfcb9887c,0x62dd1ddf,0x15da2d49,0x8cd37cf3,0xfbd44c65,0x4db26158,0x3ab551ce,0xa3bc0074,0xd4bb30e2,0x4adfa541,0x3dd895d7,0xa4d1c46d,0xd3d6f4fb,0x4369e96a,0x346ed9fc,0xad678846,0xda60b8d0,0x44042d73,0x33031de5,0xaa0a4c5f,0xdd0d7cc9,0x5005713c,0x270241aa,0xbe0b1010,0xc90c2086,0x5768b525,0x206f85b3,0xb966d409,0xce61e49f,0x5edef90e,0x29d9c998,0xb0d09822,0xc7d7a8b4,0x59b33d17,0x2eb40d81,0xb7bd5c3b,0xc0ba6cad,0xedb88320,0x9abfb3b6,0x03b6e20c,0x74b1d29a,0xead54739,0x9dd277af,0x04db2615,0x73dc1683,0xe3630b12,0x94643b84,0x0d6d6a3e,0x7a6a5aa8,0xe40ecf0b,0x9309ff9d,0x0a00ae27,0x7d079eb1,0xf00f9344,0x8708a3d2,0x1e01f268,0x6906c2fe,0xf762575d,0x806567cb,0x196c3671,0x6e6b06e7,0xfed41b76,0x89d32be0,0x10da7a5a,0x67dd4acc,0xf9b9df6f,0x8ebeeff9,0x17b7be43,0x60b08ed5,0xd6d6a3e8,0xa1d1937e,0x38d8c2c4,0x4fdff252,0xd1bb67f1,0xa6bc5767,0x3fb506dd,0x48b2364b,0xd80d2bda,0xaf0a1b4c,0x36034af6,0x41047a60,0xdf60efc3,0xa867df55,0x316e8eef,0x4669be79,0xcb61b38c,0xbc66831a,0x256fd2a0,0x5268e236,0xcc0c7795,0xbb0b4703,0x220216b9,0x5505262f,0xc5ba3bbe,0xb2bd0b28,0x2bb45a92,0x5cb36a04,0xc2d7ffa7,0xb5d0cf31,0x2cd99e8b,0x5bdeae1d,0x9b64c2b0,0xec63f226,0x756aa39c,0x026d930a,0x9c0906a9,0xeb0e363f,0x72076785,0x05005713,0x95bf4a82,0xe2b87a14,0x7bb12bae,0x0cb61b38,0x92d28e9b,0xe5d5be0d,0x7cdcefb7,0x0bdbdf21,0x86d3d2d4,0xf1d4e242,0x68ddb3f8,0x1fda836e,0x81be16cd,0xf6b9265b,0x6fb077e1,0x18b74777,0x88085ae6,0xff0f6a70,0x66063bca,0x11010b5c,0x8f659eff,0xf862ae69,0x616bffd3,0x166ccf45,0xa00ae278,0xd70dd2ee,0x4e048354,0x3903b3c2,0xa7672661,0xd06016f7,0x4969474d,0x3e6e77db,0xaed16a4a,0xd9d65adc,0x40df0b66,0x37d83bf0,0xa9bcae53,0xdebb9ec5,0x47b2cf7f,0x30b5ffe9,0xbdbdf21c,0xcabac28a,0x53b39330,0x24b4a3a6,0xbad03605,0xcdd70693,0x54de5729,0x23d967bf,0xb3667a2e,0xc4614ab8,0x5d681b02,0x2a6f2b94,0xb40bbe37,0xc30c8ea1,0x5a05df1b,0x2d02ef8d,
};
void edit_crc_step(_u32 & crc,_u8 *buf,int len)
{
	int n;
	for (n = 0; n < len; n++)
	{
		crc = crc_table[(crc ^ buf[n]) & 0xff] ^ (crc >> 8);
	}
}
#define ENDIANIZE4 \
	iihv1=((iihv1 & 0xFF)<<24) | ((iihv1 & 0xFF00)<<8) | ((iihv1 & 0xFF0000)>>8) | ((iihv1 & 0xFF000000)>>24);
#define ENDIANIZE2 \
	iihv1=((iihv1 & 0xFF)<<8) | ((iihv1 & 0xFF00)>>8);
void process_adler(_u16 & adler1,_u16 & adler2,_u8 input)
{
	adler1=(adler1+input)%65521;
	adler2=(adler2+adler1)%65521;
}
int save_png(gfx_bufferset_ & target,FILE * ifile,_i32 width,_i32 height)
{
	int checksumstart_CRC32B,checksumstart_ADLER32;
	fprintf(ifile,"\x89PNG\x0d\x0a\x1a\x0a");
	_i32 iihv1;
	iihv1=13;
	ENDIANIZE4
	fwrite(&iihv1,4,1,ifile);
	fprintf(ifile,"IHDR");
	iihv1=width;
	ENDIANIZE4
	fwrite(&iihv1,4,1,ifile);
	iihv1=height;
	ENDIANIZE4
	fwrite(&iihv1,4,1,ifile);
	iihv1=8;
	fwrite(&iihv1,1,1,ifile);
	iihv1=6;
	fwrite(&iihv1,1,1,ifile);
	iihv1=0;
	fwrite(&iihv1,1,1,ifile);
	iihv1=0;
	fwrite(&iihv1,1,1,ifile);
	iihv1=0;
	fwrite(&iihv1,1,1,ifile);
	fseek(ifile,12,SEEK_SET);
	{
		__label__ iback;
		iihv1=0xFFFFFFFF;
		iback:;
		_u8 ihv1;
		int backval=fread(&ihv1,1,1,ifile);
		if (backval==1)
		{
			edit_crc_step(*(_u32*)&iihv1,&ihv1,1);
			goto iback;
		}
		fseek(ifile,ftell(ifile),SEEK_SET);
		iihv1=iihv1^(0xFFFFFFFF);
		ENDIANIZE4
		fwrite(&iihv1,4,1,ifile);
	}
	iihv1=((4*width)+6)*height+6;
	ENDIANIZE4
	fwrite(&iihv1,4,1,ifile);
	checksumstart_CRC32B=ftell(ifile);
	fprintf(ifile,"IDAT");
	iihv1=0x78;//ZLIB Header
	fwrite(&iihv1,1,1,ifile);
	iihv1=0x01;//ZLIB Header
	fwrite(&iihv1,1,1,ifile);
	checksumstart_ADLER32=ftell(ifile);
	_u16 adler1=1;
	_u16 adler2=0;
	for (int ilv1=0;ilv1<height;ilv1++)
	{
		iihv1=(ilv1==(height-1))?0x01:0x00;
		fwrite(&iihv1,1,1,ifile);
		iihv1=width*4+1;
		fwrite(&iihv1,2,1,ifile);
		iihv1=~(width*4+1);
		fwrite(&iihv1,2,1,ifile);
		_u8 ihv1;
		ihv1=0;
		process_adler(adler1,adler2,ihv1);
		fwrite(&ihv1,1,1,ifile);
		for (int ilv2=0;ilv2<width*4;ilv2++)
		{
			int diff=0;
			if ((ilv2%4)==0)diff=2;
			if ((ilv2%4)==2)diff=-2;
			ihv1=*(((_u8*)(target.screen+ilv1*width))+ilv2+diff);
			process_adler(adler1,adler2,ihv1);
			fwrite(&ihv1,1,1,ifile);
		}
	}
	iihv1=adler2*65536+adler1;
	ENDIANIZE4
	fwrite(&iihv1,4,1,ifile);
	fseek(ifile,checksumstart_CRC32B,SEEK_SET);
	{
		__label__ iback;
		iihv1=0xFFFFFFFF;
		iback:;
		_u8 ihv1;
		int backval=fread(&ihv1,1,1,ifile);
		if (backval==1)
		{
			edit_crc_step(*(_u32*)&iihv1,&ihv1,1);
			goto iback;
		}
		fseek(ifile,ftell(ifile),SEEK_SET);
		iihv1=iihv1^(0xFFFFFFFF);
		ENDIANIZE4
		fwrite(&iihv1,4,1,ifile);
	}
	fwrite("\x00\x00\x00\x00IEND\xae\x42\x60\x82",12,1,ifile);
	return 1;
}
void save_bmp(gfx_bufferset_ & target,FILE * ifile,_i32 width,_i32 height)
{
	_u32 iihv1;
	int xtraheadersize=0;
	if (control_export.bmp_compression==3)
	{
		xtraheadersize=16;
	}
	fprintf(ifile,"BM");
	iihv1=14+40+xtraheadersize+(4*width*height);
	fwrite(&iihv1,4,1,ifile);
	iihv1=0;
	fwrite(&iihv1,4,1,ifile);
	iihv1=14+40+xtraheadersize;
	fwrite(&iihv1,4,1,ifile);
	//INFOSTRUCT
	iihv1=40+xtraheadersize;
	fwrite(&iihv1,4,1,ifile);
	iihv1=width;
	fwrite(&iihv1,4,1,ifile);
	iihv1=-height;
	fwrite(&iihv1,4,1,ifile);
	iihv1=1;
	fwrite(&iihv1,2,1,ifile);
	iihv1=32;
	fwrite(&iihv1,2,1,ifile);
	iihv1=control_export.bmp_compression;
	fwrite(&iihv1,4,1,ifile);
	iihv1=4*width*height;
	fwrite(&iihv1,4,1,ifile);
	iihv1=4000;
	fwrite(&iihv1,4,1,ifile);
	iihv1=4000;
	fwrite(&iihv1,4,1,ifile);
	iihv1=0;
	fwrite(&iihv1,4,1,ifile);
	iihv1=0;
	fwrite(&iihv1,4,1,ifile);
	if (xtraheadersize>=12)
	{
		iihv1=0xFF0000;
		fwrite(&iihv1,4,1,ifile);
		iihv1=0xFF00;
		fwrite(&iihv1,4,1,ifile);
		iihv1=0xFF;
		fwrite(&iihv1,4,1,ifile);
	}
	if (xtraheadersize>=16)
	{
		iihv1=0xFF000000;
		fwrite(&iihv1,4,1,ifile);
	}
	fwrite(target.screen,4*width*height,1,ifile);
}
catalogized_command_funcdef(PRINT_HIRES)
{
	control_export.hires=1;
	return 1;
}
int save_image(FILE * ifile,const char * value)
{
	float left,top,fwidth,fheight;
	checkupinconsistencies();
	getcaptions(&fwidth,&fheight,&left,&top);
	fwidth-=left;
	fheight-=top;
	if ((fwidth<0) || (fheight<0))
	{
		left=0;top=0;fwidth=0;fheight=0;
	}
	left-=8;
	top-=8;
	fwidth+=16;
	fheight+=16;
	int width=fwidth;
	int height=fheight;
	gfx_bufferset_ target;
	gfx_store_bufferset(&gfx_old_bufferset);
	target.screensizex=width;
	target.screensizey=height;
	target.canvassizex=width;
	target.canvassizey=height;
	target.canvasminx=0;
	target.canvasminy=0;
	target.canvasmaxx=width;
	target.canvasmaxy=height;
	target.screen=(_u32*)malloc(4*width*height);//intended TEMPORARY malloc
	target.canvas=target.screen;
	target.scrollx=left;
	target.scrolly=top;
	target.zoomx=1;
	target.zoomy=1;
	target.depth=4;
	gfx_restore_bufferset(&target);
	screenclear(0xFFFFFFFF);
	gfx_output();
	for (int ilv1=0;ilv1<width*height;ilv1++)
	{
		*(((_u8*)(screen+ilv1))+3)=0xFF-*(((_u8*)(screen+ilv1))+3);
	}
	if (control_export.hires)
	{
		HQ_inmemory=(_RGB*)target.screen;
		HQ_outmemory=(_RGB*)malloc(16*width*height);//indended TEMPORARY malloc
		HQ_insizex=width;
		HQ_insizey=height;
		HQ_mainloop();
		free(target.screen);
		target.screen=(_u32*)HQ_outmemory;
		width*=2;
		height*=2;
		target.screensizex=width;
		target.screensizey=height;
		target.canvas=target.screen;
		target.canvassizex=width;
		target.canvassizey=height;
	}
	if (strcmp(value,".png")==0)
	{
		save_png(target,ifile,width,height);
	}
	else
	{
		save_bmp(target,ifile,width,height);
	}
	free(target.screen);
	fclose(ifile);
	gfx_restore_bufferset(&gfx_old_bufferset);
	return 1;
}
void ps_controlprocedure(bool irestriction,char hatches);
int save_postscript(FILE * ifile,const char * value)
{
	outfile=ifile;
	checkupinconsistencies();
	svg_findaround();
	getatoms();
	fprintf(ifile,"%s %f %f\n","%!PS-Adobe-3.0\x0A%%Pages: 1\x0A%%BoundingBox: 0 0 ",SVG_width-SVG_ileft,SVG_height-SVG_itop);
	SVG_currentshiftx=-SVG_ileft;
	SVG_currentshifty=-SVG_itop;
	SVG_currentfringex=((unsigned int)-1)>>1;
	SVG_currentfringey=((unsigned int)-1)>>1;
	initZlist();
	ps_controlprocedure(0,1);
	ps_controlprocedure(0,0);
	fprintf(ifile,"showpage\n");
	fclose(ifile);
	return 1;
}
int svg_main(FILE * ifile);
extern int control_aggresstextcursor(const char * cursorstring);
extern int control_textedit_selectmode;
extern int control_mousestate;
extern _uXX control_textedit_cursor;
extern char arbitrarycursorstring[4];
catalogized_command_funcdef(COPY)
{
	int control_textedit_cursor2;
	if (LHENDRAW_clipboardmode)
	{
		#ifndef NOCLIPBOARD
		service_clipboard();
		#endif
		if (LHENDRAW_clipboardmode)
		{
			free(LHENDRAW_clipboardbuffer);
		}
	}
	if (control_mousestate==0x40)
	{
		if (control_textedit_selectmode)
		{
			if (control_aggresstextcursor(NULL))
			{
				control_textedit_cursor+=(_uXX)TELESCOPE_getproperty_contents();
				control_textedit_cursor+=3;
				int tl_length;
				int tl_memsize=256;
				LHENDRAW_clipboardbuffer=(char*)malloc(tl_memsize);
				int elapsed=0;
				int endfound=0;
				tl_back:;
				tl_length=strlen((char*)control_textedit_cursor);
				control_textedit_cursor2=(_uXX)strstr((char*)control_textedit_cursor,arbitrarycursorstring);
				if (control_textedit_cursor2==(_uXX)NULL)
				{
					arbitrarycursorstring[2]^=1;//try the other one, too...
					control_textedit_cursor2=(_uXX)strstr((char*)control_textedit_cursor,arbitrarycursorstring);
				}
				if (control_textedit_cursor2!=(_uXX)NULL)
				{
					endfound=1;
					tl_length=control_textedit_cursor2-control_textedit_cursor;
				}
				if (tl_length>=tl_memsize-elapsed-1)
				{
					LHENDRAW_clipboardbuffer=(char*)realloc(LHENDRAW_clipboardbuffer,tl_memsize*2);
					tl_memsize*=2;
				}
				memcpy(LHENDRAW_clipboardbuffer+elapsed,(char*)control_textedit_cursor,tl_length);
				elapsed+=tl_length;
				if (!endfound)
				{
					if (TELESCOPE_searchthroughobject_next(TELESCOPE_ELEMENTTYPE_s))
					{
						 control_textedit_cursor=(_uXX)TELESCOPE_getproperty_contents();
						 goto tl_back;
					}
				}
				control_textedit_cursor-=(_uXX)TELESCOPE_getproperty_contents();
				LHENDRAW_clipboardbuffer_count=elapsed;
			}
		}
	}
	else
	{
		char control_totalfilename[stringlength+1];
		sprintf(control_totalfilename,"mkdir -p %s/.clipboard",getenv("HOME"));
		system(control_totalfilename);
		edit_fileoperationrefersonlytopartofdocument=1;
		edit_file_always_overwrite=1;
		system("clipboard -n 2>/dev/null");//new
		sprintf(control_totalfilename,"%s/.clipboard/clipboard.cdx",getenv("HOME"));
		printf("%s",control_totalfilename);
		SAVE_TYPE(control_totalfilename,".cdx");
		sprintf(control_totalfilename,"%s/.clipboard/clipboard.cdxml",getenv("HOME"));
		printf("%s",control_totalfilename);
		SAVE_TYPE(control_totalfilename,".cdxml");
		system("clipboard -w 2>/dev/null");//write
		edit_fileoperationrefersonlytopartofdocument=0;
		edit_file_always_overwrite=0;
		LHENDRAW_clipboardmode=0;
		return 0;
	}
	#ifndef NOCLIPBOARD
	XSetSelectionOwner(gfx_Display,clipboard_sseln,gfx_Window,CurrentTime);
	LHENDRAW_clipboardmode=1;
	#endif
	return 1;
}
extern int control_aggresstextcursor(const char *);
extern int control_squashselection();
int edit_load_lastfilllevel[STRUCTURE_OBJECTTYPE_ListSize];
void edit_import_corrections()
{
	for (int ilv1=1;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
	{
		int i_multilistnumber;
		_u32 icompare=1<<ilv1;
		for (int ilv2=0;ilv2<edit_load_lastfilllevel[ilv1];ilv2++)
		{
			selection_currentselection[ilv2]&=~icompare;
		}
		int tl_currentfilllevel=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name)->filllevel;
		for (int ilv2=edit_load_lastfilllevel[ilv1];ilv2<tl_currentfilllevel;ilv2++)
		{
			selection_currentselection[ilv2]|=icompare;
		}
	}
}
catalogized_command_funcdef(PASTE)
{
	if (LHENDRAW_clipboardmode==1) goto loaded;
	LHENDRAW_clipboardbuffer=NULL;
	#ifndef NOCLIPBOARD
	{
		Atom sel_type = None;
		XEvent evt;
		unsigned int context = clipboard_XCLIB_XCOUT_NONE;
//		LHENDRAW_clipboardbuffer=(char *) XFetchBuffer(gfx_Display, (int *) &LHENDRAW_clipboardbuffer_count, 0);
		while (1)
		{
			//TODO: what if clipboard is empty? Will XPending(gfx_Display) help?
			if (context != clipboard_XCLIB_XCOUT_NONE) /*if (XPending(gfx_Display))*/ XNextEvent(gfx_Display, &evt);
			xcout(gfx_Display,gfx_Window,evt,clipboard_sseln,clipboard_target,&sel_type,(unsigned char**)&LHENDRAW_clipboardbuffer,&LHENDRAW_clipboardbuffer_count,&context);
			if (context == clipboard_XCLIB_XCOUT_NONE) break;
		}
	}
	#endif
	loaded:;
	if (LHENDRAW_clipboardbuffer==NULL) return 1;
	if (control_mousestate==0x40)
	{
		if (control_aggresstextcursor("\uE000"))
		{
			//TODO: check for text mode
			for (int ilv1=0;ilv1<LHENDRAW_clipboardbuffer_count;ilv1++)
			{
				if (LHENDRAW_clipboardbuffer[ilv1]==0)
				{
					LHENDRAW_clipboardbuffer_count=ilv1;
				}
			}
			//Not needed in this case. is it needed at all?
			LHENDRAW_clipboardbuffer=(char*)realloc(LHENDRAW_clipboardbuffer,LHENDRAW_clipboardbuffer_count+1);
			LHENDRAW_clipboardbuffer[LHENDRAW_clipboardbuffer_count]=0;
			//TODO: overflow check
			if (control_textedit_selectmode==1)
			{
				control_squashselection();
			}
			TELESCOPE_insertintoproperties_offset(LHENDRAW_clipboardbuffer,LHENDRAW_clipboardbuffer_count,control_textedit_cursor);
			cantinsert:;
		}
	}
	else
	{
		char control_totalfilename[stringlength+1];
		system("clipboard -r .cdx 2>/dev/null");//asks clipboard for cdx file
		sprintf(control_totalfilename,"%s/.clipboard/clipboard.cdx",getenv("HOME"));
		printf("%s",control_totalfilename);
		edit_fileoperationrefersonlytopartofdocument=1;
		LOAD_TYPE(control_totalfilename,".cdx");
		system("clipboard -q 2>/dev/null");//closes clipboard
		edit_import_corrections();
		edit_fileoperationrefersonlytopartofdocument=0;
	}
	if (LHENDRAW_clipboardmode==1) return 1;
	free(LHENDRAW_clipboardbuffer);
	LHENDRAW_clipboardmode=0;
	return 1;
}
extern graphic_instance * edit_summongraphic(_i32 * inr);
void CONVCAMBRIDGE_splitbracketpairs()
{
	memory_spacecheck();
	for (int ilv1=0;ilv1<glob_graphic_multilist->filllevel;ilv1++)
	{
		graphic_instance * tl_graphic_instance=glob_graphic_multilist->bufferlist()+ilv1;
		if (tl_graphic_instance->exist)
		{
			if (tl_graphic_instance->GraphicType==6)
			{
				if ((tl_graphic_instance->BracketType>=0) && (tl_graphic_instance->BracketType<=2))
				{
					tl_graphic_instance->BracketType=(tl_graphic_instance->BracketType==0)?5:((tl_graphic_instance->BracketType==1)?3:4);//sorry, simple subtraction of 3 wasn't possible.
					cdx_Rectangle tl_BBX;
					_i32 tl_backno=0;
					graphic_instance * tl_next_graphic_instance=edit_summongraphic(&tl_backno);
					if (tl_next_graphic_instance)
					{
						tl_BBX.left=tl_graphic_instance->Center3D.x+tl_graphic_instance->MajorAxisEnd3D.x-tl_graphic_instance->MinorAxisEnd3D.x;
						tl_BBX.right=-tl_graphic_instance->Center3D.x+tl_graphic_instance->MajorAxisEnd3D.x+tl_graphic_instance->MinorAxisEnd3D.x;
						tl_BBX.top=tl_graphic_instance->Center3D.y-tl_graphic_instance->MinorAxisEnd3D.y+tl_graphic_instance->MajorAxisEnd3D.y;
						tl_BBX.bottom=-tl_graphic_instance->Center3D.y+tl_graphic_instance->MinorAxisEnd3D.y+tl_graphic_instance->MajorAxisEnd3D.y;
						memcpy(tl_next_graphic_instance,tl_graphic_instance,sizeof(graphic_instance));
						tl_next_graphic_instance->BoundingBox=tl_BBX;
						selection_currentselection[tl_backno]=(selection_currentselection[tl_backno]&(~(1<<STRUCTURE_OBJECTTYPE_graphic)))|(selection_currentselection[ilv1] & (1<<STRUCTURE_OBJECTTYPE_graphic));
						tl_BBX.left=tl_next_graphic_instance->Center3D.x-tl_next_graphic_instance->MajorAxisEnd3D.x+tl_next_graphic_instance->MinorAxisEnd3D.x;
						tl_BBX.right=tl_next_graphic_instance->Center3D.x*3-tl_next_graphic_instance->MajorAxisEnd3D.x-tl_next_graphic_instance->MinorAxisEnd3D.x;
						tl_BBX.bottom=tl_next_graphic_instance->Center3D.y*3-tl_next_graphic_instance->MajorAxisEnd3D.y-tl_next_graphic_instance->MinorAxisEnd3D.y;
						tl_BBX.top=tl_next_graphic_instance->Center3D.y-tl_next_graphic_instance->MajorAxisEnd3D.y+tl_next_graphic_instance->MinorAxisEnd3D.y;
						tl_graphic_instance->BoundingBox=tl_BBX;
					}
				}
			}
		}
	}
}
catalogized_command_funcdef(SAVE_TYPE)
{
	struct stat tl_buffer;
	#ifndef NOPOSIX
	if ((stat(parameter,&tl_buffer)==0) && (edit_file_always_overwrite==0))
	{
		if (userwarning("File exists! Overwrite?\n")==0)return 0;
	}
	#endif
	FILE * ifile=fopen(parameter,"w+");
	if (ifile==NULL) return 0;
	FORCEEXTENSION
	if ((strcmp(value,".bmp")==0) || (strcmp(value,".png")==0))
	{
		return save_image(ifile,value);
	}
	if ((strcmp(value,".ps")==0) || (strcmp(value,".eps")==0))
	{
		return save_postscript(ifile,value);
	}
	if (strcmp(value,".svg")==0)
	{
		return svg_main(ifile);
	}
	if (edit_fileoperationrefersonlytopartofdocument==0)
	{
		strncpy(control_filename,parameter,stringlength*2);
		control_setfilename(parameter);
	}
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
	if (strcmp(value,".cdxml")==0)
	{
	fprintf(ifile,"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
	fprintf(ifile,"<!DOCTYPE CDXML SYSTEM \"http://www.cambridgesoft.com/xml/cdxml.dtd\" >\n");
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
	fprintf(ifile," <color r=\"%f\" g=\"%f\" b=\"%f\"/>\n",(*glob_CAMBRIDGE_color_multilist)[ilv1].r,(*glob_CAMBRIDGE_color_multilist)[ilv1].g,(*glob_CAMBRIDGE_color_multilist)[ilv1].b);
}

fprintf(ifile,"%s","</colortable><fonttable>\n"
"<font id=\"1\" charset=\"windows-1258\" name=\"Arial\"/>\n"
"<font id=\"2\" charset=\"Unknown\" name=\"Symbol\"/>\n"
"<font id=\"3\" charset=\"iso-10646\" name=\"Arial\"/>\n"
"</fonttable>");
	}
	if (strcmp(value,".cdx")==0)
	{
		fprintf(ifile,"VjCD0100\4\3\2\1");
		_u16 iu16=0;
		for (int ilv1=0;ilv1<5;ilv1++)fwrite(&iu16,2,1,ifile);
		printf("SAVING CDX\n");
	}
	janitor_biasids(4);
	//TODO: all subobjects of page must get filllevel=0 before add!
	glob_CAMBRIDGE_CDXML_multilist->filllevel=0;
	CAMBRIDGE_CDXML_instance i_CAMBRIDGE_CDXML_instance=CAMBRIDGE_CDXML_instance();
	(*glob_CAMBRIDGE_CDXML_multilist).ADD(&i_CAMBRIDGE_CDXML_instance);
	glob_CAMBRIDGE_page_multilist->filllevel=0;
	ADD_TO_MULTILISTREFERENCE(&i_CAMBRIDGE_CDXML_instance,page);
	tl_CAMBRIDGE_page_instance->id=0;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_page_instance,id);
	tl_CAMBRIDGE_page_instance->BoundingBox.left=0;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_page_instance,BoundingBox);
	tl_CAMBRIDGE_page_instance->BoundingBox.top=0;
	tl_CAMBRIDGE_page_instance->BoundingBox.right=28346.46;
	tl_CAMBRIDGE_page_instance->BoundingBox.bottom=28346.46;
	tl_CAMBRIDGE_page_instance->Width=28346.46;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_page_instance,Width);
	tl_CAMBRIDGE_page_instance->Height=28346.46;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_page_instance,Height);
	tl_CAMBRIDGE_page_instance->HeaderPosition=36;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_page_instance,HeaderPosition);
	tl_CAMBRIDGE_page_instance->FooterPosition=36;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_page_instance,FooterPosition);
	tl_CAMBRIDGE_page_instance->PageOverlap=0;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_page_instance,PageOverlap);
	tl_CAMBRIDGE_page_instance->PrintTrimMarks=1;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_page_instance,PrintTrimMarks);
	tl_CAMBRIDGE_page_instance->HeightPages=37;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_page_instance,HeightPages);
	tl_CAMBRIDGE_page_instance->WidthPages=48;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_page_instance,WidthPages);
	tl_CAMBRIDGE_page_instance->DrawingSpace=1;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_page_instance,DrawingSpace);
	CONVCAMBRIDGE_splitbracketpairs();
	CONVCAMBRIDGE_internaltomain(tl_CAMBRIDGE_page_instance);
	if (strcmp(value,".cdxml")==0)
	{
		output_object(ifile,tl_CAMBRIDGE_page_instance);
		fprintf(ifile,"</CDXML>");
	}
	if (strcmp(value,".cdx")==0)
	{
		AUTOSTRUCT_EXISTS_SET_NAME((&i_CAMBRIDGE_CDXML_instance),colortable);
		AUTOSTRUCT_EXISTS_SET_NAME((&i_CAMBRIDGE_CDXML_instance),fonttable);
		output_object_bin(ifile,&i_CAMBRIDGE_CDXML_instance);
		_u16 iu16=0;
		fwrite(&iu16,2,1,ifile);
	}
	fclose(ifile);
	return 1;
}
catalogized_command_funcdef(SAVEAS)
{
	LHENDRAW_filedlgmode=1;
	control_filemenu_mode=1;
	menu_selectedmenuelement=0;
	return 1;
}
catalogized_command_funcdef(EXPORTAS)
{
	LHENDRAW_filedlgmode=1;
	control_filemenu_mode=2;
	menu_selectedmenuelement=0;
	return 1;
}
catalogized_command_funcdef(IMPORTAS)
{
	LHENDRAW_filedlgmode=1;
	control_filemenu_mode=3;
	menu_selectedmenuelement=0;
	return 1;
}
catalogized_command_funcdef(SEARCHAS)
{
	LHENDRAW_filedlgmode=1;
	control_filemenu_mode=4;
	menu_selectedmenuelement=0;
	return 1;
}
int edit_bondsum(int nr,int dir)
{
	float i_bond_sum=0;
	n_instance * tl_n_instance=(*glob_n_multilist).bufferlist()+nr;
	if ((*tl_n_instance).exist)
	{
		for (int ilv2=0;ilv2<atom_actual_node[nr].bondcount;ilv2++)
		{
			i_bond_sum+=(*glob_b_multilist)[atom_actual_node[nr].bonds[ilv2]].Order/16.0;
		}
	}
	if (fmod(i_bond_sum,1.0)>0.4)
	{
		i_bond_sum=trunc(i_bond_sum)+1;
	}
	switch (dir)
	{
		case 0: return i_bond_sum;
		case 1: (*tl_n_instance).protons+=i_bond_sum;if ((*tl_n_instance).Element==constants_Element_implicitcarbon) {(*tl_n_instance).protons=4-abs((*tl_n_instance).charge);} return 1;
		case 2: (*tl_n_instance).protons-=i_bond_sum;return 1;
	}
	return 0;
}
void edit_add_deltahydrogens(_small ilastfilllevel)
{
	for (int ilv1=ilastfilllevel;ilv1<(*glob_n_multilist).filllevel;ilv1++)
	{
		edit_bondsum(ilv1,1);
	}
}
catalogized_command_funcdef(LOAD_TYPE)
{
	FILE * infile;
	_small tl_n_lastfilllevel;
	for (int ilv1=1;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
	{
		edit_load_lastfilllevel[ilv1]=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name)->filllevel;
	}
	infile=fopen(parameter,"r");
	if (infile==NULL) return 0;
	control_saveuponexit=0;
	strncpy(control_filename,parameter,511);control_filename[511]=0;
	FORCEEXTENSION
	LHENDRAW_loadmemoryoverflow=0;
	filestructure_text_buffer.count=0;
	filestructure_curve_buffer.count=0;
	for (int ilv1=0;ilv1<multilist_count;ilv1++)
	{
		if (multilistlist[ilv1].usage==1)
		{
			multilistlist[ilv1].instance->reset();
		}
	}
	currentinstance=new(CAMBRIDGEPREFIX(Total_Document_instance));//TODO mem: leaks
	if (edit_fileoperationrefersonlytopartofdocument==0)
	{
		strncpy(control_filename,parameter,stringlength*2);
		control_setfilename(parameter);
	}
	if (strcmp(value,".cdxml")==0)
	{
		input_fsm(infile);
	}
	else if (strcmp(value,".cdx")==0)
	{
		input_bin(infile);
	}
	else
	{
		fclose(infile);
		return 0;
	}
	fclose(infile);
	tl_n_lastfilllevel=(*glob_n_multilist).filllevel;
	CAMBRIDGECONV_maintointernal();
	svg_findaround();
	getatoms();
	edit_add_deltahydrogens(tl_n_lastfilllevel);
	for (int ilv1=0;ilv1<multilist_count;ilv1++)
	{
		if (multilistlist[ilv1].instance->filllevel>=multilistlist[ilv1].instance->getmaxitems()) error2(2,"Multilist overflow! %s",multilistlist[ilv1].name);
	}
	return 1;
}
catalogized_command_funcdef(LOADAS)
{
	LHENDRAW_filedlgmode=1;
	control_filemenu_mode=0;
	menu_selectedmenuelement=0;
	return 1;
}
catalogized_command_funcdef(SAVE)
{
	char istring[3*stringlength+3];
	sprintf(istring,"\n\n  OVERWRITE file named\n  %s,%s?",control_currentdirectory,control_filenamehead);
	if (userwarning(istring))
	{
		SAVE_TYPE(control_filename,"");
	}
	return 1;
}
catalogized_command_funcdef(WARNING_OK)
{
	LHENDRAW_userdecision=1;
	LHENDRAW_warndlgmode=0;
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
	control_drawproperties.bond_Order=16;
	control_drawproperties.bond_Display=0;
	return 1;
}
catalogized_command_funcdef(TEST_SMASH)
{
	char zero=0;
	TELESCOPE_aggressobject(glob_t_multilist,rand()%(*glob_t_multilist).filllevel);
	TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_s);
	TELESCOPE_split(atoi(value),&zero,1);
	return 1;
}
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
catalogized_command_funcdef(ZOOM)
{
	SDL_scrollx+=(gfx_canvassizex/2)/SDL_zoomx;
	SDL_scrolly+=(gfx_canvassizey/2)/SDL_zoomy;
	if (parameter[0]=='+')
	{
		SDL_zoomx*=atof(value);
		SDL_zoomy*=atof(value);
	}
	else
	{
		SDL_zoomx/=atof(value);
		SDL_zoomy/=atof(value);
	}
	SDL_scrollx-=(gfx_canvassizex/2)/SDL_zoomx;
	SDL_scrolly-=(gfx_canvassizey/2)/SDL_zoomy;
	return 1;
}
catalogized_command_funcdef(ONETOONE)
{
	SDL_scrollx+=(gfx_canvassizex/2)*((1/SDL_zoomx)-1);
	SDL_scrolly+=(gfx_canvassizey/2)*((1/SDL_zoomy)-1);
	SDL_zoomx=1;SDL_zoomy=1;
	return 1;
}
catalogized_command_funcdef(TOGGLETEXTATTRIBUTE)
{
	printf("TODO***stub\n");
	return 1;
}

char edit_scoop_atomstring[4];
int edit_scoop_numhydrogens;
int edit_scoop_charge;
int edit_scoop_atomcount;
char edit_scoop_startedsmall;
edit_formatstruct edit_scoop_packedformats[6];
s_instance edit_scoop_formats[7];
_u8 edit_scoop_valids;//0: BIG 1: SMALL1 2: SMALL2 3: H 4: Nr 5: Charge 6: Main-atom count 
char edit_brackets_allowed;
char edit_grouplabels_allowed;
char edit_dashes_allowed;
char edit_hydrogens_allowed;
char edit_complexnicknames_allowed;
//TODO: continue at the point WHERE it was aborted.
//TODO: fully abort when abort was from fsm=0
int processatomsymbol(int * fsm,char * & pointer,s_instance * format)
{
	char ihv1;
	if (pointer==NULL)
	{
		(*fsm)=7;
		return 0;
	}
	iback:
	if ((*pointer)==0) return 0;
	ihv1=(*pointer);
	if (ihv1=='-')
	{
		if (edit_dashes_allowed)
		{
			pointer++;
			*fsm=7;
			return 1;
		}
	}
	if ((*fsm)==0)
	{
		if (((ihv1=='(') || (ihv1==')') || (ihv1=='[') || (ihv1==']')) && (edit_brackets_allowed))
		{
			edit_scoop_atomstring[0]=ihv1;
			edit_scoop_valids|=1<<0;
			edit_scoop_formats[0]=*format;
			edit_scoop_startedsmall=0;
			pointer++;
			(*fsm)=6;
			goto iback;
		}
		if ((ihv1>='A') && (ihv1<='Z'))
		{
			edit_scoop_atomstring[0]=ihv1;
			edit_scoop_valids|=1<<0;
			edit_scoop_formats[0]=*format;
			edit_scoop_startedsmall=0;
			(*fsm)=1;
			pointer++;
			goto iback;
		}
		if ((ihv1>='a') && (ihv1<='z'))//protons, neutrons or something
		{
			edit_scoop_atomstring[0]=ihv1;
			edit_scoop_valids|=1<<0;
			edit_scoop_formats[0]=*format;
			edit_scoop_startedsmall=1;
			(*fsm)=5;
			pointer++;
			goto iback;
		}
		if ((ihv1=='-') && (edit_dashes_allowed))
		{
			(*fsm)=7;
			pointer++;
		}
		if ((ihv1=='.') || (ihv1==' ') || (ihv1==':'))
		{
			if (ihv1!=' ')
			{
				edit_scoop_atomstring[0]=ihv1;
			}
			edit_scoop_startedsmall=0;
			edit_scoop_valids|=1<<0;
			edit_scoop_formats[0]=*format;
			(*fsm)=5;
			pointer++;
			goto iback;
		}
		(*fsm)=7;
		return 0;
	}
	if ((*fsm)==1)
	{
		if ((ihv1=='H') && (edit_hydrogens_allowed==1))
		{
			(*fsm)=4;
			edit_scoop_valids|=1<<3;
			edit_scoop_formats[3]=*format;
			edit_scoop_numhydrogens=1;
			pointer++;
			goto iback;
		}
		if ((ihv1>='0') && (ihv1<='9') && edit_dashes_allowed)
		{
			edit_scoop_atomcount=(edit_scoop_atomcount*10)+ihv1-'0';
			edit_scoop_valids|=1<<6;
			edit_scoop_formats[6]=*format;
			(*fsm)=6;
			pointer++;
			goto iback;
		}
		if ((ihv1>='A') && (ihv1<='Z') && (((edit_scoop_startedsmall) && (edit_grouplabels_allowed))||(edit_complexnicknames_allowed)))
		{
			goto valid_next_letter;
		}
		if ((ihv1>='a') && (ihv1<='z'))
		{
			valid_next_letter:;
			edit_scoop_atomstring[1]=ihv1;
			edit_scoop_valids|=1<<1;
			edit_scoop_formats[1]=*format;
			(*fsm)=2;
			pointer++;
			goto iback;
		}
		(*fsm)=7;
		return 1;
	}
	if ((*fsm)==2)
	{
		if ((ihv1=='H') && (edit_hydrogens_allowed==1))
		{
			(*fsm)=4;
			edit_scoop_valids|=1<<3;
			edit_scoop_formats[3]=*format;
			edit_scoop_numhydrogens=1;
			pointer++;
			goto iback;
		}
		if ((ihv1>='0') && (ihv1<='9') && edit_dashes_allowed)
		{
			edit_scoop_atomcount=(edit_scoop_atomcount*10)+ihv1-'0';
			edit_scoop_valids|=1<<6;
			edit_scoop_formats[6]=*format;
			(*fsm)=6;
			pointer++;
			goto iback;
		}
		if ((ihv1>='A') && (ihv1<='Z') && (edit_complexnicknames_allowed))
		{
			goto valid_next_letter2;
		}
		if ((ihv1>='a') && (ihv1<='z'))
		{
			valid_next_letter2:;
			edit_scoop_atomstring[2]=ihv1;//Overwrites
			edit_scoop_valids|=1<<2;
			edit_scoop_formats[2]=*format;
			(*fsm)=3;
			pointer++;
			goto iback;
		}
		(*fsm)=7;
		return 1;
	}
	if ((*fsm)==3)
	{
		if ((ihv1=='H') && (edit_hydrogens_allowed==1))
		{
			(*fsm)=4;
			edit_scoop_valids|=1<<3;
			edit_scoop_formats[3]=*format;
			edit_scoop_numhydrogens=1;
			pointer++;
			goto iback;
		}
		(*fsm)=7;
		return 1;
	}
	if ((*fsm)==4)
	{
		if ((ihv1>='0') && (ihv1<='9'))
		{
			edit_scoop_numhydrogens=ihv1-'0';
			edit_scoop_valids|=1<<4;
			edit_scoop_formats[4]=*format;
			(*fsm)=5;
			pointer++;
			goto iback;
		}
		(*fsm)=7;
		return 1;
	}
	if ((*fsm)==5)
	{
		if ((ihv1>='0') && (ihv1<='9'))
		{
			edit_scoop_numhydrogens=(edit_scoop_numhydrogens*10)+ihv1-'0';
			edit_scoop_valids|=1<<4;
			edit_scoop_formats[4]=*format;
			(*fsm)=5;
			pointer++;
			goto iback;
		}
		(*fsm)=7;
		return 1;
	}
	if ((*fsm)==6)
	{
		if ((ihv1>='0') && (ihv1<='9'))
		{
			edit_scoop_atomcount=(edit_scoop_atomcount*10)+ihv1-'0';
			edit_scoop_valids|=1<<6;
			edit_scoop_formats[6]=*format;
			(*fsm)=6;
			pointer++;
			goto iback;
		}
		(*fsm)=7;
		return 1;
	}
	return 1;
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
		(*imultilist)[inumber].Element=constants_Element_implicitcarbon;
		goto yes_its_an_element;
	}
	edit_brackets_allowed=0;
	edit_grouplabels_allowed=0;
	edit_dashes_allowed=0;
	edit_hydrogens_allowed=1;
	edit_complexnicknames_allowed=0;
	iback:
	if (i_backval)
	{
		processatomsymbol(&fsm,ipointer,(s_instance*)TELESCOPE_getproperty());
		if (((*(s_instance*)TELESCOPE_getproperty()).face & 0x60)!=0x60)
		{
			fsm=7;
		}
		i_backval=TELESCOPE_searchthroughobject_next(TELESCOPE_ELEMENTTYPE_s);
		if (i_backval)
		{
			ipointer=(char*)TELESCOPE_getproperty_contents();
			goto iback;
		}
	}
	if (fsm==7)
	{
		return 0;
	}
	for (int ilv1=0;ilv1<sizeof(element)/sizeof(element_);ilv1++)
	{
		if (strcmp(edit_scoop_atomstring,element[ilv1].name)==0)
		{
			(*imultilist)[inumber].Element=ilv1;
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
			fill++;
		}
		cursor=cursor<<1;
	}
	TELESCOPE_add(TELESCOPE_ELEMENTTYPE_s_f,(char*)&edit_scoop_packedformats,fill*sizeof(edit_formatstruct));
	(*(s_f_instance*)TELESCOPE_getproperty()).valids=edit_scoop_valids;
	(*(s_f_instance*)TELESCOPE_getproperty()).length=sizeof(s_f_instance)+fill*sizeof(edit_formatstruct);
	(*(s_f_instance*)TELESCOPE_getproperty()).type=TELESCOPE_ELEMENTTYPE_s_f;
	(*imultilist)[inumber].protons=edit_scoop_numhydrogens;
	(*imultilist)[inumber].color=edit_scoop_formats[0].color;
	return 1;
}
int edit_elementstack[100][element_max];
int edit_interpretasmoleculechainformula(multilist<n_instance> * imultilist,int inumber)
{
	int element_stack_counter=0;
	char * ipointer;
	int i_backval=0;
	int i_char_backval=0;
	int fsm=0;//0: nothing 1: One big letter 2: One big Letter, and also small letters 3: Hydrogens-H 4: Number reached 5: other symbol, done 6: Invalid-cant interpret
	edit_complexnicknames_allowed=0;
	for (int ilv1=0;ilv1<element_max;ilv1++)
	{
		edit_elementstack[element_stack_counter][ilv1]=0;
	}
	startagain:;
	i_backval=TELESCOPE_aggressobject(imultilist,inumber);
	if (i_backval)
	{
		i_backval=TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_s);
		if (i_backval) ipointer=(char*)TELESCOPE_getproperty_contents();
	}
	if (i_backval==0)
	{
		return 0;
	}
	edit_brackets_allowed=1;
	edit_grouplabels_allowed=1;
	edit_dashes_allowed=1;
	edit_hydrogens_allowed=0;
	int end_of_line=0;
	inextelement:;
	fsm=0;
	i_char_backval=0;
	edit_scoop_valids=0;
	(*(_u32*)edit_scoop_atomstring)=0;
	edit_scoop_numhydrogens=0;
	edit_scoop_charge=0;
	edit_scoop_atomcount=0;
	iback:
	i_char_backval=processatomsymbol(&fsm,ipointer,(s_instance*)TELESCOPE_getproperty());
	if (fsm>=7)
	{
		goto enoughtointerpret;
	}
	//TODO: evaluate subscript/superscript
/*		if (((*(s_instance*)TELESCOPE_getproperty()).face & 0x60)!=0x60)
	{
		fsm=7;
	}*/
	i_backval=TELESCOPE_searchthroughobject_next(TELESCOPE_ELEMENTTYPE_s);
	if (i_backval)
	{
		ipointer=(char*)TELESCOPE_getproperty_contents();
		goto iback;
	}
	else
	{
		end_of_line=1;
	}
	enoughtointerpret:;
	if (edit_scoop_atomcount==0) edit_scoop_atomcount=1;
	for (int ilv1=0;ilv1<sizeof(element)/sizeof(element_);ilv1++)
	{
		if (constants_Element_implicitcarbon!=ilv1)
		{
			if (strcmp(edit_scoop_atomstring,element[ilv1].name)==0)
			{
				edit_elementstack[element_stack_counter][ilv1]+=edit_scoop_atomcount;
				goto yes_its_an_element;
			}
		}
	}
	//Not an element, but an 3-letter abbreviation
	for (int ilv1=0;ilv1<sizeof(element_abbreviation)/sizeof(element_abbreviation_);ilv1++)
	{
		if (strcmp(edit_scoop_atomstring,element_abbreviation[ilv1].name)==0)
		{
			for (int ilv2=0;element_abbreviation[ilv1].composition[ilv2]!=0;ilv2+=2)
			{
				for (int ilv3=0;ilv3<sizeof(element)/sizeof(element_);ilv3++)
				{
					if (strcmp(element[ilv3].name,(char*)&(element_abbreviation[ilv1].composition[ilv2+1]))==0)
					{
						edit_elementstack[element_stack_counter][ilv3]+=element_abbreviation[ilv1].composition[ilv2]*edit_scoop_atomcount;
					}
				}
			}
			goto yes_its_an_element;
		}
	}
	//Brackets:
	if ((edit_scoop_atomstring[0]=='(') || (edit_scoop_atomstring[0]==')') || (edit_scoop_atomstring[0]=='[') || (edit_scoop_atomstring[0]==']'))
	{
		if ((edit_scoop_atomstring[0]=='(') || (edit_scoop_atomstring[0]=='['))
		{
			if (element_stack_counter<99)
			{
				element_stack_counter++;
				for (int ilv1=0;ilv1<element_max;ilv1++)
				{
					edit_elementstack[element_stack_counter][ilv1]=0;
				}
			}
			goto yes_its_an_element;
		}
		if ((edit_scoop_atomstring[0]==')') || (edit_scoop_atomstring[0]==']'))
		{
			if (element_stack_counter>0)
			{
				for (int ilv1=0;ilv1<element_max;ilv1++)
				{
					edit_elementstack[element_stack_counter-1][ilv1]+=edit_elementstack[element_stack_counter][ilv1]*edit_scoop_atomcount;
				}
				element_stack_counter--;
				goto yes_its_an_element;
			}
		}
	}
	//Now, it may be a complex nickname like TMS 
	if ((edit_complexnicknames_allowed==0) && (strlen(edit_scoop_atomstring)!=0))
	{
		edit_complexnicknames_allowed=1;
		element_stack_counter=0;
		for (int ilv1=0;ilv1<element_max;ilv1++)
		{
			edit_elementstack[element_stack_counter][ilv1]=0;
		}
		goto startagain;
	}
	yes_its_an_element:;
	if (i_char_backval==0)
	{
		i_backval=TELESCOPE_searchthroughobject_next(TELESCOPE_ELEMENTTYPE_s);
		if (i_backval)
		{
			goto inextelement;
		}
		goto endabrechnung;
	}
	if (end_of_line==0) goto inextelement;
	endabrechnung:;
	if (element_stack_counter==0)
	{
		for (int ilv1=0;ilv1<element_max;ilv1++)
		{
			analysis_analysis[ilv1]+=edit_elementstack[0][ilv1];
		}
	}
	return 1;
}
#define L_SEPARATE \
{\
	if (ilv1!=0)\
	{\
		itelescope->connect&=~1;\
		TELESCOPE_split(ilv1,"",1);\
		((s_instance*)TELESCOPE_getproperty())->connect=1;\
		ilv1=0;\
		itelescope=(s_instance*)TELESCOPE_getproperty();\
		current_s_string=(char*)TELESCOPE_getproperty_contents();\
		ilength=min(strlen(current_s_string),TELESCOPE_getproperty_contentlength());\
	}\
	else\
	{\
		if (ilasttelescope!=NULL)\
		{\
			ilasttelescope->connect&=~1;\
		}\
	}\
}
char edit_resortstring(basicmultilist * imultilist,int iinstance) // resorts chemical text to be right-aligned
{
	//Explanation of the connect attribute:
	//0: means the next Element is not connected, and is set left to the current Element on right-aligned molecule labels
	//1: means the next Element is right of the current Element even on right-aligned molecule labels, e.g. numbers
	//0x4: means that this is a open-bracket. When right-aligned, the elements next to the corresponding 0x3 closing-bracket are drawn after it.
	//it is drawn as the corresponding closing-bracket, then.
	//0x3: a closing bracket with elements right to it. These Elements are not drawn in right-aligned molecule labels. It is then drawn as the corresponding opening-bracket.
	//0x2: another closing bracket without elements right to it. It is then drawn as the corresponding opening-bracket.
	int ilength;
	int fsm=0;//0=done 1: Capital letter 2: little letter 3: number 4: plus/minus 5: Bracket
	int ihv1;
	int backval;
	int startedsmall=0;
	TELESCOPE_aggressobject(imultilist,iinstance);
	s_instance *itelescope,*ilasttelescope;
	ilasttelescope=NULL;
	backval=TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_s);
	iback:;
	itelescope=(s_instance*)TELESCOPE_getproperty();
	if (backval<=0)
	{
		if (ilasttelescope!=NULL) ilasttelescope->connect&=~1;
		return 1;
	}
	char * current_s_string=(char*)TELESCOPE_getproperty_contents();
	ilength=min(strlen(current_s_string),TELESCOPE_getproperty_contentlength());
	itelescope->connect=1;
	for (int ilv1=0;ilv1<ilength;ilv1++)
	{
		ihv1=current_s_string[ilv1];
		if (ihv1==' ') {goto feeble_aftermath;}
		if (ihv1=='/') {goto feeble_aftermath;}
		if (ihv1=='\\') {goto feeble_aftermath;}
		if ((ihv1=='(') || (ihv1=='[') || (ihv1=='{'))
		{
			L_SEPARATE;
			itelescope->connect=4;
			itelescope->effface=itelescope->face&~0x60;
			fsm=0;
		}
		if ((ihv1==')') || (ihv1==']') || (ihv1=='}'))
		{
			L_SEPARATE;
			itelescope->connect=3;
			itelescope->effface=itelescope->face&~0x60;
			fsm=5;
		}
		if ((ihv1=='+') || (ihv1=='-'))
		{
			L_SEPARATE;
			itelescope->connect=1;
			itelescope->effface=itelescope->face&~0x60;
			fsm=0;
		}
		if ((ihv1>='0') && (ihv1<='9'))
		{
			if ((fsm==1) || (fsm==2) || (fsm==3))
			{
				((s_instance*)TELESCOPE_getproperty())->connect|=1;
				TELESCOPE_split(ilv1,"",1);
				ilv1=0;
				itelescope=(s_instance*)TELESCOPE_getproperty();
				current_s_string=(char*)TELESCOPE_getproperty_contents();
				ilength=min(strlen(current_s_string),TELESCOPE_getproperty_contentlength());
				itelescope->effface=(itelescope->face&~0x60)+0x20;
			}
			else
			{
				if (fsm==5)
				{
					if (ilv1!=0)
					{
						itelescope->connect|=1;
						TELESCOPE_split(ilv1,"",1);
						((s_instance*)TELESCOPE_getproperty())->connect=1;
						ilv1=0;
						itelescope=(s_instance*)TELESCOPE_getproperty();
						current_s_string=(char*)TELESCOPE_getproperty_contents();
						ilength=min(strlen(current_s_string),TELESCOPE_getproperty_contentlength());
						itelescope->effface=(itelescope->face&~0x60)+0x20;
					}
				}
				else
				{
					L_SEPARATE;
					itelescope->effface=itelescope->face&~0x60;
				}
			}
			fsm=3;
		}
		if ((ihv1>='a') && (ihv1<='z'))
		{
			startedsmall=0;
			if ((fsm!=1) && (fsm!=2))
			{
				L_SEPARATE;
				itelescope->effface=itelescope->face&~0x60;
				itelescope->connect=1;
				startedsmall=1;
				fsm=1;
			}
			else
			{
				fsm=2;
			}
		}
		if ((ihv1>='A') && (ihv1<='Z'))
		{
			if ((fsm==1) && (startedsmall==1))
			{
				itelescope->connect=1;
				startedsmall=0;
			}
			else
			{
				L_SEPARATE;
				itelescope->effface=itelescope->face&~0x60;
			}
			fsm=1;
		}
	}
	ilasttelescope=itelescope;
	backval=TELESCOPE_searchthroughobject_next(TELESCOPE_ELEMENTTYPE_s);
	goto iback;
	return 1;
	feeble_aftermath:
	TELESCOPE_aggressobject(imultilist,iinstance);
	backval=TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_s);
	while (backval>0)
	{
		((s_instance*)TELESCOPE_getproperty())->connect=1;
		backval=TELESCOPE_searchthroughobject_next(TELESCOPE_ELEMENTTYPE_s);
	}
	return 0;
}
int edit_textlength(basicmultilist * imultilist,int iindex,int deltax=0,int deltay=0,int * idistance=NULL)
{
	int lastlastbackcount,lastbackcount;
	lastbackcount=0;
	lastlastbackcount=0;
	int tl_backval;
	int itemsize=imultilist->itemsize;
	if (TELESCOPE_aggressobject(imultilist,iindex))
	{
		CDXMLREAD_functype dummy;
		basic_instance * tl_instance=(basic_instance*)(imultilist->pointer+iindex*itemsize);
		int pointoffset=(*imultilist).getproperties("xyz",&dummy);
		cdx_Point3D * xyz=(cdx_Point3D*)(((char*)tl_instance)+pointoffset);
		deltax-=(*xyz).x;
		deltay-=(*xyz).y;
		int tl_boxoffset=(*imultilist).getproperties("BoundingBox",&dummy);
		if (tl_instance->exist)
		{
			int maxwidth=0;
			int currentwidth=0;
			int height=0;
			int backcount=0;
			char * istring;
			int istring_length;
			tl_backval=TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_s);
			iback:;
			if (tl_backval==0) goto evaluate;
			istring=(char*)TELESCOPE_getproperty_contents();
			istring_length=strlen(istring);
			for (int ilv1=0;ilv1<istring_length;ilv1+=backcount)
			{
				if (istring[ilv1]==10)
				{
					backcount=1;
					currentwidth=0;
					height++;
				}
				else
				{
					fontpixinf_ * ifontpixinf=&fontpixinf[indexfromunicode(utf8resolve((unsigned char*)istring + ilv1,&backcount))];
					char imode=((s_instance*)TELESCOPE_getproperty())->face;
					currentwidth+=(*ifontpixinf).deltax+((imode & 1)==1)*1+((imode & 8)==8)*2;//larger
					if (idistance!=NULL)
					{
						if (deltay<height*16+8)
						{
							if ((deltax<currentwidth-3) || (deltay<(height*16)-8))
							{
								(*idistance)=ilv1;
								return 1;
							}
						}
					}
					if (currentwidth>maxwidth) maxwidth=currentwidth;
				}
			}
			tl_backval=TELESCOPE_searchthroughobject_next(TELESCOPE_ELEMENTTYPE_s);
			goto iback;
			evaluate:;
			if (tl_boxoffset!=-1)
			{
				cdx_Rectangle * iBBX=(cdx_Rectangle*)(((char*)tl_instance)+tl_boxoffset);
				(*iBBX).left=(*xyz).x-4;
				(*iBBX).top=(*xyz).y-6;
				(*iBBX).right=(*xyz).x+maxwidth+8;
				(*iBBX).bottom=(*xyz).y+(height)*16+8;
			}
			return 0;
		}
	}
	return 0;
}
int menu_itembyname(const char * name,int * menu=NULL,int * index=NULL);
void edit_filesort(char i_buffer[constants_maxlistentries][256],_u32 * i_attribs,structenum * i_filememory)
{
	int diffresult=0;
	for (int ilv1=0;ilv1<control_filememory.count-1;ilv1++)
	{
		for (int ilv2=ilv1+1;ilv2<(*i_filememory).count;ilv2++)
		{
			if (strncmp(i_buffer[ilv1],i_buffer[ilv2],255)>0)
			{
				for (int ilv3=0;ilv3<255;ilv3++)
				{
					char swap=i_buffer[ilv1][ilv3];
					i_buffer[ilv1][ilv3]=i_buffer[ilv2][ilv3];
					i_buffer[ilv2][ilv3]=swap;
				}
				_u32 swap=i_attribs[ilv1];
				i_attribs[ilv1]=i_attribs[ilv2];
				i_attribs[ilv2]=swap;
			}
		}
	}
	return;
}
catalogized_command_funcdef(FILEDLG_DEVICE_SEL)
{
	DIR * DD=opendir(parameter);
	struct dirent * dirpy;
	if (DD)
	{
		control_filememory.count=0;
		for (int ilv1=0;ilv1<constants_maxlistentries;ilv1++)
		{
			dirpy=readdir(DD);
			if (dirpy==NULL) goto readfinished;
			strncpy(control_filememory_buffer[ilv1],dirpy->d_name,255);control_filememory_buffer[ilv1][255]=0;
			strncpy(control_currentdirectory,parameter,255);control_currentdirectory[255]=0;
			control_filememory_attribs[ilv1]=dirpy->d_type;
			control_filememory.count++;
		}
		readfinished:;
		closedir(DD);
		control_filememory.scroll=0;
		control_filememory.number=0;
		edit_filesort(control_filememory_buffer,control_filememory_attribs,&control_filememory);
		menu_selectedmenuelement=menu_itembyname("FILEDLG_FILE_SEL");
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
		for (int ilv1=0;ilv1<constants_maxlistentries;ilv1++)
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
		edit_filesort(control_filememory_buffer,control_filememory_attribs,&control_filememory);
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
		for (int ilv1=0;ilv1<constants_maxlistentries;ilv1++)
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
		edit_filesort(control_filememory_buffer,control_filememory_attribs,&control_filememory);
		return 1;
	}
	return -30;
}
catalogized_command_funcdef(FILEDLG_FILE_HEAD)
{
	menu_selectedmenuelement=0;
	return 1;
}
catalogized_command_funcdef(FILEDLG_DEVICE_SEL_PORT)
{
	DIR * DD=opendir(parameter);
	struct dirent * dirpy;
	if (DD)
	{
		control_filememory_port.count=0;
		for (int ilv1=0;ilv1<constants_maxlistentries;ilv1++)
		{
			dirpy=readdir(DD);
			if (dirpy==NULL) goto readfinished;
			strncpy(control_filememory_port_buffer[ilv1],dirpy->d_name,255);control_filememory_port_buffer[ilv1][255]=0;
			strncpy(control_currentdirectory_port,parameter,255);control_currentdirectory_port[255]=0;
			control_filememory_port_attribs[ilv1]=dirpy->d_type;
			control_filememory_port.count++;
		}
		readfinished:;
		closedir(DD);
		control_filememory_port.scroll=0;
		control_filememory_port.number=0;
		edit_filesort(control_filememory_port_buffer,control_filememory_port_attribs,&control_filememory_port);
		menu_selectedmenuelement=menu_itembyname("FILEDLG_FILE_SEL_PORT");
		return 1;
	}
	return -30;
}
catalogized_command_funcdef(FILEDLG_FILE_SEL_PORT)
{
	int formerlength=strlen(control_currentdirectory_port);
	sprintf(control_currentdirectory_port+strlen(control_currentdirectory_port),"%c%s",constants_Directoryslash,parameter);//TODO: limit
	DIR * DD=opendir(control_currentdirectory_port);
	struct dirent * dirpy;
	if (DD)
	{
		control_filememory_port.count=0;
		for (int ilv1=0;ilv1<constants_maxlistentries;ilv1++)
		{
			dirpy=readdir(DD);
			if (dirpy==NULL) goto readfinished;
			strncpy(control_filememory_port_buffer[ilv1],dirpy->d_name,255);control_filememory_port_buffer[ilv1][255]=0;
			control_filememory_port_attribs[ilv1]=dirpy->d_type;
			control_filememory_port.count++;
		}
		readfinished:;
		control_filememory_port.scroll=0;
		control_filememory_port.number=0;
		closedir(DD);
		edit_filesort(control_filememory_port_buffer,control_filememory_port_attribs,&control_filememory_port);
	}
	else
	{
		control_currentdirectory_port[formerlength]=0;
		strncpy(control_filenamehead_port,parameter,255);control_filenamehead_port[255]=0;
		menu_selectedmenuelement=menu_itembyname("FILEDLG_FILE_HEAD_PORT");
	}
	return 1;
}
catalogized_command_funcdef(FILEDLG_FILE_PATH_PORT)
{
	DIR * DD=opendir(control_currentdirectory_port);
	struct dirent * dirpy;
	if (DD)
	{
		control_filememory_port.count=0;
		for (int ilv1=0;ilv1<constants_maxlistentries;ilv1++)
		{
			dirpy=readdir(DD);
			if (dirpy==NULL) goto readfinished;
			strncpy(control_filememory_port_buffer[ilv1],dirpy->d_name,255);control_filememory_port_buffer[ilv1][255]=0;
			control_filememory_port_attribs[ilv1]=dirpy->d_type;
			control_filememory_port.count++;
		}
		readfinished:;
		control_filememory_port.scroll=0;
		control_filememory_port.number=0;
		menu_selectedmenuelement=menu_itembyname("FILEDLG_FILE_HEAD_PORT");
		closedir(DD);
		edit_filesort(control_filememory_port_buffer,control_filememory_port_attribs,&control_filememory_port);
		return 1;
	}
	return -30;
}
catalogized_command_funcdef(FILEDLG_FILE_HEAD_PORT)
{
	menu_selectedmenuelement=0;
	return 1;
}
catalogized_command_funcdef(FILEDLG_DEVICE_SEL_SEARCH)
{
	DIR * DD=opendir(parameter);
	struct dirent * dirpy;
	if (DD)
	{
		control_filememory_search.count=0;
		for (int ilv1=0;ilv1<constants_maxlistentries;ilv1++)
		{
			dirpy=readdir(DD);
			if (dirpy==NULL) goto readfinished;
			strncpy(control_filememory_search_buffer[ilv1],dirpy->d_name,255);control_filememory_search_buffer[ilv1][255]=0;
			strncpy(control_currentdirectory_search,parameter,255);control_currentdirectory_search[255]=0;
			control_filememory_search_attribs[ilv1]=dirpy->d_type;
			control_filememory_search.count++;
		}
		readfinished:;
		closedir(DD);
		control_filememory_search.scroll=0;
		control_filememory_search.number=0;
		edit_filesort(control_filememory_search_buffer,control_filememory_search_attribs,&control_filememory_search);
		menu_selectedmenuelement=menu_itembyname("FILEDLG_FILE_SEL_SEARCH");
		return 1;
	}
	return -30;
}
catalogized_command_funcdef(FILEDLG_FILE_SEL_SEARCH)
{
	int formerlength=strlen(control_currentdirectory_search);
	sprintf(control_currentdirectory_search+strlen(control_currentdirectory_search),"%c%s",constants_Directoryslash,parameter);//TODO: limit
	DIR * DD=opendir(control_currentdirectory_search);
	struct dirent * dirpy;
	if (DD)
	{
		control_filememory_search.count=0;
		for (int ilv1=0;ilv1<constants_maxlistentries;ilv1++)
		{
			dirpy=readdir(DD);
			if (dirpy==NULL) goto readfinished;
			strncpy(control_filememory_search_buffer[ilv1],dirpy->d_name,255);control_filememory_search_buffer[ilv1][255]=0;
			control_filememory_search_attribs[ilv1]=dirpy->d_type;
			control_filememory_search.count++;
		}
		readfinished:;
		control_filememory_search.scroll=0;
		control_filememory_search.number=0;
		closedir(DD);
		edit_filesort(control_filememory_search_buffer,control_filememory_search_attribs,&control_filememory_search);
	}
	else
	{
		control_currentdirectory_search[formerlength]=0;
		strncpy(control_filenamehead_search,parameter,255);control_filenamehead_search[255]=0;
		menu_selectedmenuelement=menu_itembyname("FILEDLG_FILE_HEAD_SEARCH");
	}
	return 1;
}
catalogized_command_funcdef(FILEDLG_FILE_PATH_SEARCH)
{
	DIR * DD=opendir(control_currentdirectory_search);
	struct dirent * dirpy;
	if (DD)
	{
		control_filememory_search.count=0;
		for (int ilv1=0;ilv1<constants_maxlistentries;ilv1++)
		{
			dirpy=readdir(DD);
			if (dirpy==NULL) goto readfinished;
			strncpy(control_filememory_search_buffer[ilv1],dirpy->d_name,255);control_filememory_search_buffer[ilv1][255]=0;
			control_filememory_search_attribs[ilv1]=dirpy->d_type;
			control_filememory_search.count++;
		}
		readfinished:;
		control_filememory_search.scroll=0;
		control_filememory_search.number=0;
		menu_selectedmenuelement=menu_itembyname("FILEDLG_FILE_HEAD_SEARCH");
		closedir(DD);
		edit_filesort(control_filememory_search_buffer,control_filememory_search_attribs,&control_filememory_search);
		return 1;
	}
	return -30;
}
catalogized_command_funcdef(FILEDLG_FILE_HEAD_SEARCH)
{
	menu_selectedmenuelement=0;
	return 1;
}
char control_totalfilename[stringlength+stringlength+2];
catalogized_command_funcdef(FILEDLG_FILE_SAVE)
{
	if (strcmp(control_filenamehead,"")==0)
	{
		return -41;
	}
	DIR * DD=opendir(control_currentdirectory);
	char retval=-30;
	if (DD)
	{
		sprintf(control_totalfilename,"%s/%s",control_currentdirectory,control_filenamehead);
		retval=SAVE_TYPE(control_totalfilename,"");//TODO: insert selected type

		if (retval>=1)
		{
			retval=1;
			LHENDRAW_filedlgmode=0;
			strcpy(control_filename,control_totalfilename);
		}
		closedir(DD);
	}
	return retval;
}
catalogized_command_funcdef(FILEDLG_FILE_EXPORT)
{
	if (strcmp(control_filenamehead_port,"")==0)
	{
		return -41;
	}
	DIR * DD=opendir(control_currentdirectory_port);
	char retval=-30;
	if (DD)
	{
		sprintf(control_totalfilename,"%s/%s",control_currentdirectory_port,control_filenamehead_port);
		edit_fileoperationrefersonlytopartofdocument=1;
		retval=SAVE_TYPE(control_totalfilename,"");//TODO: insert selected type
		edit_fileoperationrefersonlytopartofdocument=0;

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
	if (strcmp(control_filenamehead,"")==0)
	{
		return -41;
	}
	control_saveuponexit=0;
	DIR * DD=opendir(control_currentdirectory);
	char retval=-30;
	if (DD)
	{
		sprintf(control_totalfilename,"%s/%s",control_currentdirectory,control_filenamehead);
		FILE_NEW("","");
		retval=LOAD_TYPE(control_totalfilename,"");//TODO: insert selected type
		if (retval>=1)
		{
			LHENDRAW_filedlgmode=0;
			strcpy(control_filename,control_totalfilename);
		}
		closedir(DD);
	}
	return retval;
}
catalogized_command_funcdef(FILEDLG_FILE_IMPORT)
{
	if (strcmp(control_filenamehead_port,"")==0)
	{
		return -41;
	}
	DIR * DD=opendir(control_currentdirectory_port);
	char retval=-30;
	if (DD)
	{
		sprintf(control_totalfilename,"%s/%s",control_currentdirectory_port,control_filenamehead_port);
		edit_fileoperationrefersonlytopartofdocument=1;
		retval=LOAD_TYPE(control_totalfilename,"");//TODO: insert selected type
		edit_import_corrections();
		edit_fileoperationrefersonlytopartofdocument=0;
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
	_u32 iwert=atoi(value);
	for (int ilv1=1;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
	{
		basicmultilist * tl_multilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
		suboffset=(*tl_multilist).getproperties(parameter,&dummy);
		if (suboffset<0)
		{
			continue;
		}
		_u32  icompare=(1<<ilv1);
		int tl_size=(*tl_multilist).itemsize;
		int ifilllevel=(*tl_multilist).filllevel;//separately, so it doesn't grow while executing the loop
		for (int ilv2=0;ilv2<ifilllevel;ilv2++)
		{
			if (selection_currentselection[ilv2] & icompare)
			{
				if ((*tl_multilist)[ilv2].exist)
				{
					*((_i32*)(((char*)(&((*tl_multilist)[ilv2])))+suboffset))=iwert;
				}
			}
		}
	}
	for (int ilv1=1;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
	{
		int follower3=0;
		basicmultilist * tl_multilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
		char * ibufferpos=(*tl_multilist).pointer;
		int tl_size=(*tl_multilist).itemsize;
		_u32  icompare=(1<<(ilv1+STRUCTURE_OBJECTTYPE_ListSize));
		for (int ilv2=0;ilv2<(*tl_multilist).filllevel;ilv2++)
		{
			if ((*((basic_instance*)(ibufferpos+tl_size*ilv2))).exist)
			{
				float tlpx,tlpy;
				int ilv3=1;
				while (retrievepoints_basic(((basic_instance*)(ibufferpos+tl_size*ilv2)),&tlpx,&tlpy,NULL,ilv3,ilv1)>0)
				{
					if (selection_currentselection[follower3] & icompare)
					{
						switch ((*((TELESCOPE_element*)TELESCOPE_getproperty())).type)
						{
							case TELESCOPE_ELEMENTTYPE_Symbol:
							((Symbol_instance*)TELESCOPE_getproperty())->color=iwert;
							break;
						}
					}
					ilv3++;
					follower3++;
					if (follower3>=(LHENDRAW_buffersize/sizeof(selection_datatype)))
					{
						goto ioverflow;
					}
				}
			}
		}
		ioverflow:;
	}
	for (int ilv1=1;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
	{
		basicmultilist * tl_multilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
		_u32  icompare=(1<<ilv1);
		int tl_size=(*tl_multilist).itemsize;
		for (int ilv2=0;ilv2<(*tl_multilist).filllevel;ilv2++)
		{
			if (selection_currentselection[ilv2] & icompare)
			{
				if ((*(basic_instance*)((*tl_multilist).pointer+(*tl_multilist).itemsize*ilv2)).exist)
				{
					TELESCOPE_aggressobject(tl_multilist,ilv2);
					int tl_backval=TELESCOPE_searchthroughobject_multi((1<<TELESCOPE_ELEMENTTYPE_s) | (1<<TELESCOPE_ELEMENTTYPE_s_f));
					while (tl_backval>0)
					{
						switch ((*((TELESCOPE_element*)TELESCOPE_getproperty())).type)
						{
							case TELESCOPE_ELEMENTTYPE_s:
							((s_instance*)TELESCOPE_getproperty())->color=iwert;
							break;
							case TELESCOPE_ELEMENTTYPE_s_f:
							int ivalids=((s_f_instance*)TELESCOPE_getproperty())->valids;
							int dist=0;
							int map=1;
							edit_formatstruct * tl_formatstruct=(edit_formatstruct*)TELESCOPE_getproperty_contents();
							for (int ilv1=0;ilv1<6;ilv1++)
							{
								if ((ivalids & map))
								{
									tl_formatstruct[dist].color=iwert;
									dist++;
								}
								map=map<<1;
							}
							ifertig:;
							break;
						}
						tl_backval=TELESCOPE_searchthroughobject_next_multi((1<<TELESCOPE_ELEMENTTYPE_s) | (1<<TELESCOPE_ELEMENTTYPE_s_f));
					}
				}
			}
		}
	}
	return 1;
}
catalogized_command_funcdef(OPTIONS)
{
	LHENDRAW_filedlgmode=2;
	menu_selectedmenuelement=0;
	return 0;
}
catalogized_command_funcdef(WARN_HYPERC)
{
	int ifound=0;
	getatoms();
	iback:;
	for (int ilv1=edit_current5bondcarbon;ilv1<glob_n_multilist->filllevel;ilv1++)
	{
		n_instance * tl_n_instance=glob_n_multilist->bufferlist()+ilv1;
		if (tl_n_instance->exist==0) continue;
		if ((tl_n_instance->Element!=constants_Element_implicitcarbon) && (tl_n_instance->Element!=constants_Element_implicitcarbon+1)) continue;
		int i_bond_sum;
		i_bond_sum=getbondsum(ilv1);
		if (i_bond_sum>=5)
		{
			SDL_scrollx=tl_n_instance->xyz.x-gfx_canvassizex/SDL_zoomx/2;
			SDL_scrolly=tl_n_instance->xyz.y-gfx_canvassizey/SDL_zoomy/2;
			ifound=1;
			edit_current5bondcarbon=ilv1+1;
			return 1;
		}
	}
	if (ifound==0){edit_current5bondcarbon=0;ifound=1;goto iback;}
	return 0;
}
catalogized_command_funcdef(CLEANUP)
{
	//TODO: make 3D!
	char changed;
	int count=0;
	_u32 icompare=1<<STRUCTURE_OBJECTTYPE_b;
	iback:;
	changed=0;
	count++;
	for (int ilv1=0;ilv1<glob_b_multilist->filllevel;ilv1++)
	{
		if (selection_currentselection[ilv1] & icompare)
		{
			b_instance * iinstance=(*glob_b_multilist).bufferlist()+ilv1;
			n_instance * iinstance1=glob_n_multilist->bufferlist()+bond_actual_node[ilv1].start;
			n_instance * iinstance2=glob_n_multilist->bufferlist()+bond_actual_node[ilv1].end;
			float length=fsqr(iinstance2->xyz.x-iinstance1->xyz.x)+fsqr(iinstance2->xyz.y-iinstance1->xyz.y);
			float idirection=getangle(iinstance2->xyz.x-iinstance1->xyz.x,iinstance2->xyz.y-iinstance1->xyz.y);
			if (length>=fsqr(constants_bondlength+0.02))
			{
				iinstance1->xyz.x+=cos(idirection)*0.01;
				iinstance2->xyz.x-=cos(idirection)*0.01;
				iinstance1->xyz.y+=sin(idirection)*0.01;
				iinstance2->xyz.y-=sin(idirection)*0.01;
				changed=1;
			}
			if (length<=fsqr(constants_bondlength-0.02))
			{
				iinstance1->xyz.x-=cos(idirection)*0.01;
				iinstance2->xyz.x+=cos(idirection)*0.01;
				iinstance1->xyz.y-=sin(idirection)*0.01;
				iinstance2->xyz.y+=sin(idirection)*0.01;
				changed=1;
			}
		}
	}
/*	icompare=1<<STRUCTURE_OBJECTTYPE_n;
	for (int ilv1=0;ilv1<glob_n_multilist->filllevel;ilv1++)
	{
		if (selection_currentselection[ilv1] & icompare)
		{
			n_instance * iinstance=(*glob_n_multilist).bufferlist()+ilv1;
			for (int ilv2=0;ilv2<atom_actual_node[ilv1].bondcount;ilv2++)
			{
//TODO: ignore bondorder-0 bonds				b_instance * i_b_instance1=glob_b_multilist->bufferlist()+atom_actual_node[ilv1].bonds[ilv2];
				n_instance * i_n_instance1=glob_n_multilist->bufferlist()+getother(ilv1,atom_actual_node[ilv1].bonds[ilv2]);
				for (int ilv3=ilv2+1;ilv3<atom_actual_node[ilv1].bondcount;ilv3++)
				{
//					b_instance * iinstance2=glob_b_multilist->bufferlist()+atom_actual_node[ilv1].bonds[ilv3];
					n_instance * i_n_instance2=glob_n_multilist->bufferlist()+getother(ilv1,atom_actual_node[ilv1].bonds[ilv3]);
					float angle1=getangle(i_n_instance1->xyz.x-iinstance->xyz.x,i_n_instance1->xyz.y-iinstance->xyz.y);
					float angle2=getangle(i_n_instance2->xyz.x-iinstance->xyz.x,i_n_instance2->xyz.y-iinstance->xyz.y);
					float iangle=fmod(angle2-angle1+4*Pi,Pi/12);
					float idirection1,idirection2;
					if (iangle>Pi/24) iangle-=Pi/12;
					if (fabs(iangle)>(Pi/3000))
					{
						float force=0.01;
						if ((fabs(iangle)>Pi/300))
						{
							force=0.11-0.1/(fabs(iangle)/(Pi/12));
						}
						force=0.01-0.0099*(fabs(iangle)/(Pi/12));
						if (iangle>0)
						{
							idirection1=angle1+Pi/2;
							idirection2=angle2-Pi/2;
						}
						else
						{
							idirection1=angle1-Pi/2;
							idirection2=angle2+Pi/2;
						}
						i_n_instance1->xyz.x+=cos(idirection1)*force;
						i_n_instance2->xyz.x+=cos(idirection2)*force;
						i_n_instance1->xyz.y+=sin(idirection1)*force;
						i_n_instance2->xyz.y+=sin(idirection2)*force;
						changed=1;
					}
				}
			}
		}
	}*/
	if ((changed) && (count<1000000)) goto iback;
	return 1;
}
catalogized_command_funcdef(REVERSEBEZIER)
{
	char wert=0;
	for (int ilv1=0;ilv1<(*glob_curve_multilist).filllevel;ilv1++)
	{
		if (selection_currentselection[ilv1] & (1<<STRUCTURE_OBJECTTYPE_curve))
		{
			curve_instance * tl_instance=(*glob_curve_multilist).bufferlist()+ilv1;
			if ((*tl_instance).exist)
			{
				int tl_excess=((*tl_instance).CurvePoints.count)%3;
				if (tl_excess>0)
				{
					for (int ilv2=tl_excess;ilv2<3;ilv2++)
					{
						(*tl_instance).CurvePoints.a[(*tl_instance).CurvePoints.count].x=(*tl_instance).CurvePoints.a[(*tl_instance).CurvePoints.count-1].x*2-(*tl_instance).CurvePoints.a[(*tl_instance).CurvePoints.count-2].x;
						(*tl_instance).CurvePoints.a[(*tl_instance).CurvePoints.count].y=(*tl_instance).CurvePoints.a[(*tl_instance).CurvePoints.count-1].y*2-(*tl_instance).CurvePoints.a[(*tl_instance).CurvePoints.count-2].y;
						//TODO: 3D(*tl_instance).CurvePoints.a[(*tl_instance).CurvePoints.count].z=(*tl_instance).CurvePoints.a[(*tl_instance).CurvePoints.count-1].z*2-(*tl_instance).CurvePoints.a[(*tl_instance).CurvePoints.count-2].z;
						(*tl_instance).CurvePoints.count++;
					}
				}
				for (int ilv1=0;ilv1<(*tl_instance).CurvePoints.count/2;ilv1++)
				{
					cdx_Point2D swap=(*tl_instance).CurvePoints.a[ilv1];
					(*tl_instance).CurvePoints.a[ilv1]=(*tl_instance).CurvePoints.a[(*tl_instance).CurvePoints.count-1-ilv1];
					(*tl_instance).CurvePoints.a[(*tl_instance).CurvePoints.count-1-ilv1]=swap;
					wert=1;
				}
			}
		}
	}
	return wert;
}
int edit_flexicopy(int undostep_no,multilist<n_instance> * n_target,multilist<b_instance> * b_target,selection_ iselection,intl * i_deltaback,float dx,float dy,char overwrite)
{
	n_instance * n_input=(n_instance*)undo_retrievebuffer(currentundostep,STRUCTURE_OBJECTTYPE_n);
	b_instance * b_input=(b_instance*)undo_retrievebuffer(currentundostep,STRUCTURE_OBJECTTYPE_b);
	multilist<n_instance> * i_n_multilist=(multilist<n_instance>*)&(undo_retrievehandle(undostep_no,STRUCTURE_OBJECTTYPE_n)->imultilist);
	multilist<b_instance> * i_b_multilist=(multilist<b_instance>*)&(undo_retrievehandle(undostep_no,STRUCTURE_OBJECTTYPE_b)->imultilist);
	int n_max=i_n_multilist->filllevel;
	int b_max=i_b_multilist->filllevel;
	_u32 icompare;
	intl min_id=(~((unsigned intl)0))>>1;
	intl max_id=0;
	icompare=1<<STRUCTURE_OBJECTTYPE_n;
	char * ibufferpos;
	char * ioldbufferpos;
	int ioffset;
	int internalpointcount;
	float tl_x,tl_y,tl_z;
	for (int ilv1=0;ilv1<n_max;ilv1++)
	{
		if (iselection[ilv1] & icompare)
		{
			if (max_id<n_input[ilv1].id) max_id=n_input[ilv1].id;
			if (min_id>n_input[ilv1].id) min_id=n_input[ilv1].id;
			ifertig:;
		}
	}
	//TODO: memory overflow management
	(*i_deltaback)=n_target->maxid-min_id+1;
	if (overwrite)
	{
		for (int ilv1=0;ilv1<n_max;ilv1++)
		{
			if (iselection[ilv1] & icompare)
			{
				memcpy((n_target->bufferlist())+(ilv1),n_input+ilv1,sizeof(n_instance));
				(n_target->bufferlist())[ilv1].xyz.x+=dx;
				(n_target->bufferlist())[ilv1].xyz.y+=dy;
			}
		}
		icompare=1<<STRUCTURE_OBJECTTYPE_b;
		for (int ilv1=0;ilv1<b_max;ilv1++)
		{
			if (iselection[ilv1] & icompare)
			{
				int istart=bond_actual_node[ilv1].start;
				int iend=bond_actual_node[ilv1].end;
				if (((iselection[istart] & (1<<STRUCTURE_OBJECTTYPE_n))==0) && ((iselection[iend] & (1<<STRUCTURE_OBJECTTYPE_n))==0))
				{
					(n_target->bufferlist())[istart].xyz.x=(*(n_input+istart)).xyz.x+dx;
					(n_target->bufferlist())[istart].xyz.y=(*(n_input+istart)).xyz.y+dy;
					(n_target->bufferlist())[iend].xyz.x=(*(n_input+iend)).xyz.x+dx;
					(n_target->bufferlist())[iend].xyz.y=(*(n_input+iend)).xyz.y+dy;
				}
			}
		}
	}
	else
	{
		for (int ilv1=0;ilv1<n_max;ilv1++)
		{
			if (iselection[ilv1] & icompare)
			{
/*				memcpy((n_target->bufferlist())+(n_target->filllevel),n_input+ilv1,sizeof(n_instance));
				(n_target->bufferlist())[n_target->filllevel].id+=(*i_deltaback);
				(n_target->bufferlist())[n_target->filllevel].xyz.x+=dx;
				(n_target->bufferlist())[n_target->filllevel].xyz.y+=dy;
				n_target->filllevel++;*/
			}
		}
		icompare=1<<STRUCTURE_OBJECTTYPE_b;
		for (int ilv1=0;ilv1<b_max;ilv1++)
		{
			if (iselection[ilv1] & icompare)
			{
				if ((iselection[bond_actual_node[ilv1].start] & (1<<STRUCTURE_OBJECTTYPE_n)) && (iselection[bond_actual_node[ilv1].end] & (1<<STRUCTURE_OBJECTTYPE_n)))
				{
					memcpy((b_target->bufferlist())+(b_target->filllevel),b_input+ilv1,sizeof(b_instance));
					(b_target->bufferlist())[b_target->filllevel].B+=(*i_deltaback);
					(b_target->bufferlist())[b_target->filllevel].E+=(*i_deltaback);
					b_target->filllevel++;
				}
				else
				{
					if (iselection[bond_actual_node[ilv1].start] & (1<<STRUCTURE_OBJECTTYPE_n))
					{
						memcpy((b_target->bufferlist())+(b_target->filllevel),b_input+ilv1,sizeof(b_instance));
						(b_target->bufferlist())[b_target->filllevel].B+=(*i_deltaback);
						b_target->filllevel++;
					}
					if (iselection[bond_actual_node[ilv1].end] & (1<<STRUCTURE_OBJECTTYPE_n))
					{
						memcpy((b_target->bufferlist())+(b_target->filllevel),b_input+ilv1,sizeof(b_instance));
						(b_target->bufferlist())[b_target->filllevel].E+=(*i_deltaback);
						b_target->filllevel++;
					}
				}

			}
		}
	}
	for (int ilv1=1;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
	{
		if (ilv1!=STRUCTURE_OBJECTTYPE_b)
		{
			int follower=0;
			icompare=1<<ilv1;
			int isize= STRUCTURE_OBJECTTYPE_List[ilv1].size;
			basicmultilist * tlmultilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
			basicmultilist * tloldmultilist=(basicmultilist*)&(undo_retrievehandle(undostep_no,ilv1)->imultilist);
			internalpointcount=retrieveprops_basic(1,ilv1);
			if (tlmultilist==NULL) goto i_fertig;
			CDXMLREAD_functype tldummy;
			ioffset=(*tlmultilist).getproperties("xyz",&tldummy);
			ibufferpos=(char*)((*tlmultilist).pointer);
			ioldbufferpos=undo_retrievehandle(undostep_no,ilv1)->buffer;
			cdx_Point2D * tlpoint2d;
			for (int ilv2=0;ilv2<(*tlmultilist).filllevel;ilv2++)
			{
				if ((*((basic_instance*)(ibufferpos+isize*ilv2))).exist)
				{
					if ((selection_currentselection[ilv2]) & icompare)
					{
						retrievepoints_basic(((basic_instance*)(ioldbufferpos+isize*ilv2)),&tl_x,&tl_y,&tl_z,0,ilv1);
						tl_x+=dx;
						tl_y+=dy;
						if (overwrite)
						{
							placepoints_basic(((basic_instance*)(ibufferpos+isize*ilv2)),tl_x,tl_y,tl_z,0,ilv1);
						}
						else
						{
							memcpy(((char*)(tlmultilist->pointer))+(tlmultilist->filllevel)*isize,ioldbufferpos+(isize*ilv2),isize);
							if (ilv1==STRUCTURE_OBJECTTYPE_n)
							{
								((n_instance*)(ibufferpos+isize*(tlmultilist->filllevel)))->id+=(*i_deltaback);
							}
							placepoints_basic(((basic_instance*)(ibufferpos+isize*(tlmultilist->filllevel))),tl_x,tl_y,tl_z,0,ilv1);
							((basic_instance_propertybuffer*)(ibufferpos+isize*(tlmultilist->filllevel)))->pos_in_buffer=(glob_contentbuffer+ilv1)->count;
							char * tl_pos_in_old_buffer=undo_retrievecontentbuffer(undostep_no,ilv1)->buffer+((basic_instance_propertybuffer*)(ioldbufferpos+isize*ilv2))->pos_in_buffer;
							if (*(_i32*)(tl_pos_in_old_buffer+4)==ilv2)
							{
								memcpy((glob_contentbuffer+ilv1)->buffer+(glob_contentbuffer+ilv1)->count,tl_pos_in_old_buffer,*(_i32*)tl_pos_in_old_buffer);
								*(((_i32*)((glob_contentbuffer+ilv1)->buffer+(glob_contentbuffer+ilv1)->count))+1)=tlmultilist->filllevel;
								(glob_contentbuffer+ilv1)->count+=*(_i32*)tl_pos_in_old_buffer;//TODO: limit
							}
							tlmultilist->filllevel++;
						}
					}
					else
					{
						if (internalpointcount>0)
						{
							follower=ilv2*internalpointcount;
						}
						if (follower<(LHENDRAW_buffersize/sizeof(selection_datatype)))
						{
							for (int ilv3=1;retrievepoints_basic((basic_instance*)(ioldbufferpos+isize*ilv2),&tl_x,&tl_y,&tl_z,ilv3,ilv1,tloldmultilist/*must be retrieved by undo_retrievehandle to obtain correct pointer*/)>0;ilv3++)
							{
								if (selection_currentselection[follower] & (1<<(STRUCTURE_OBJECTTYPE_ListSize+ilv1)))
								{
									placepoints_basic(((basic_instance*)(ibufferpos+isize*ilv2)),tl_x+dx,tl_y+dy,tl_z,ilv3,ilv1);
								}
								follower++;
							}
						}
					}
				}
			}
			i_fertig:;
		}
	}
	return 0;
}
int edit_readrepresentfrombuffer(char * input)
{
	ADD_TO_MULTILISTREFERENCE(currentinstance,represent);
	tl_CAMBRIDGE_represent_instance->object=*(_i32*)input;
	tl_CAMBRIDGE_represent_instance->attribute=*(_i16*)(input+4);
	return 0;
}
int edit_writerepresenttobuffer(char * input,void * output)
{
	basicmultilistreference * tl_basicmultilistreference=*((basicmultilistreference**)input);
	int tl_max=tl_basicmultilistreference->start_in_it+tl_basicmultilistreference->count_in_it;
	for (int ilv1=tl_basicmultilistreference->start_in_it;ilv1<tl_max;ilv1++)
	{
		_i16 length=0x06;
		fwrite(&length,2,1,(FILE*)output);
		CAMBRIDGE_represent_instance * tl_CAMBRIDGE_represent_instance=glob_CAMBRIDGE_represent_multilist->bufferlist()+ilv1;
		fwrite(&tl_CAMBRIDGE_represent_instance->object,4,1,(FILE*)output);
		fwrite(&tl_CAMBRIDGE_represent_instance->attribute,2,1,(FILE*)output);
		if (ilv1<tl_basicmultilistreference->start_in_it+tl_basicmultilistreference->count_in_it-1)
		{
			length=0xE;//Because here, multiple instances are written as multiple ones, so we need a new header
			fwrite(&length,2,1,(FILE*)output);
		}
	}
	return 0;
}
int edit_readcolortablefrombuffer(char * input)
{
	int count=*(_i16*)input;
	int ilv1=0;
	_u16 value=0;
	if (((glob_CAMBRIDGE_color_multilist)->filllevel+count)>glob_CAMBRIDGE_color_multilist->getmaxitems())
	{
		LHENDRAW_loadmemoryoverflow=1;
		return 0;
	}
	ilv1=2;
	multilist<CAMBRIDGE_color_instance> * tl_CAMBRIDGE_color_multilist=retrievemultilist<CAMBRIDGE_color_instance>();
	for (int ilv2=0;ilv2<count;ilv2++)
	{
		ADD_TO_MULTILISTREFERENCE(currentinstance,color);
		value=*(_i16*)(input+ilv1);
		ilv1+=2;
		tl_CAMBRIDGE_color_instance->r=value/65535.0;
		value=*(_i16*)(input+ilv1);
		ilv1+=2;
		tl_CAMBRIDGE_color_instance->g=value/65535.0;
		value=*(_i16*)(input+ilv1);
		ilv1+=2;
		tl_CAMBRIDGE_color_instance->b=value/65535.0;
	}
	return 0;
}
int edit_writecolortabletobuffer(char * input,void * output)
{
	static int length=glob_CAMBRIDGE_color_multilist->filllevel*6+2;
	fwrite(&length,2,1,(FILE*)output);
	length=glob_CAMBRIDGE_color_multilist->filllevel;
	fwrite(&length,2,1,(FILE*)output);
	for (int ilv1=0;ilv1<length;ilv1++)
	{
		_u16 dummy=65535.0*(*glob_CAMBRIDGE_color_multilist)[ilv1].r;
		fwrite(&dummy,2,1,(FILE*)output);
		dummy=65535.0*(*glob_CAMBRIDGE_color_multilist)[ilv1].g;
		fwrite(&dummy,2,1,(FILE*)output);
		dummy=65535.0*(*glob_CAMBRIDGE_color_multilist)[ilv1].b;
		fwrite(&dummy,2,1,(FILE*)output);
	}
	return 0;
}
int edit_readsfrombuffer(char * input)
{
	_i16 icount;
	int ilv1=0;
	cdx_Stylerun icurrentstylerun,icurrentstylerun2;
	icount=*(_i16*)input;
	intl istartpos=2;
	int currentpos=0;
	basic_instance * lastcurrentinstance=currentinstance;
	iback:;
	int best=-1;
	int bestval=2000000000;
	for (int ilv1=0;ilv1<icount;ilv1++)
	{
		memcpy(&icurrentstylerun,input+istartpos+sizeof(cdx_Stylerun)*ilv1,sizeof(cdx_Stylerun));
		if (icurrentstylerun.startpos==currentpos)
		{
			if (icurrentstylerun.startpos<bestval)
			{
				bestval=icurrentstylerun.startpos;
				best=ilv1;
			}
		}
	}
	if (best==-1)
	{
		icurrentstylerun.color=0;
		icurrentstylerun.size=12*20;
		icurrentstylerun.face=0;
		icurrentstylerun.font=1;
		icurrentstylerun.startpos=currentpos;
	}
	else
	{
		memcpy(&icurrentstylerun,input+istartpos+sizeof(cdx_Stylerun)*best,sizeof(cdx_Stylerun));
	}
	best=-1;
	bestval=2000000000;
	for (int ilv1=0;ilv1<icount;ilv1++)
	{
		memcpy(&icurrentstylerun2,input+istartpos+sizeof(cdx_Stylerun)*ilv1,sizeof(cdx_Stylerun));
		if (icurrentstylerun2.startpos>icurrentstylerun.startpos)
		{
			if (icurrentstylerun2.startpos<bestval)
			{
				bestval=icurrentstylerun2.startpos;
				best=ilv1;
			}
		}
	}
	if (best==-1)
	{
		icurrentstylerun2.startpos=paramvaluestring_length-2-sizeof(cdx_Stylerun)*icount;
	}
	else
	{
		memcpy(&icurrentstylerun2,input+istartpos+sizeof(cdx_Stylerun)*best,sizeof(cdx_Stylerun));
	}
	if (icurrentstylerun2.startpos-icurrentstylerun.startpos<=0)
	{
		return 0;
	}
	parameterstring_length=0;
	for (int ilv1=icurrentstylerun.startpos;ilv1<icurrentstylerun2.startpos;ilv1++)
	{
		_u8 ihv1=input[2+sizeof(cdx_Stylerun)*icount+ilv1];
		if ((ihv1 & 0x80)==0)
		{
			if (ihv1==0xD) ihv1=0xA;
			parameterstring[parameterstring_length++]=ihv1;
		}
		else
		{
			strcpy(parameterstring+parameterstring_length,list_win_1258[ihv1-0x80]);
			parameterstring_length+=strlen(list_win_1258[ihv1-0x80]);
			//TODO: other encodings
		}
	}
	parameterstring[parameterstring_length]=0;//TODO sizerestriction
	ADD_TO_MULTILISTREFERENCE(currentinstance,s);
	tl_CAMBRIDGE_s_instance->font=icurrentstylerun.font;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_s_instance,font);
	tl_CAMBRIDGE_s_instance->face=icurrentstylerun.face;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_s_instance,face);
	tl_CAMBRIDGE_s_instance->size=icurrentstylerun.size/20;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_s_instance,size);
	tl_CAMBRIDGE_s_instance->color=icurrentstylerun.color;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_s_instance,color);
	currentinstance=tl_CAMBRIDGE_s_instance;
	concludepctext();
	currentinstance=lastcurrentinstance;
	currentpos=icurrentstylerun2.startpos;
	goto iback;
	return 0;
}
int edit_printtranslate(FILE * output,const char * text,int * icursor)
{
	int i_starttype=0;
	int i_currenttype=0;
	int begincursor=(*icursor);
	char ihv1=0;
	iback:;
	if (text[(*icursor)]==0) {return 0;};
	if (text[(*icursor)] & 0x80)
	{
		for (int ilv1=0;ilv1<0x80;ilv1++)
		{
			if (strncmp(list_win_1258[ilv1],text+(*icursor),strlen(list_win_1258[ilv1]))==0)
			{
				if (i_starttype==0) {i_starttype=1;}else{if ((i_currenttype)!=1){return 1;}}
				i_currenttype=1;
				(*icursor)+=strlen(list_win_1258[ilv1]);
				ihv1=ilv1+0x80;
				fwrite(&ihv1,1,1,output);
				goto ifound;
			}
		}
		for (int ilv1=0;ilv1<list_greeklist_size;ilv1++)
		{
			if (strncmp(list_greeklist[ilv1].output,text+(*icursor),strlen(list_greeklist[ilv1].output))==0)
			{
				if (i_starttype==0) {i_starttype=2;}else{if ((i_currenttype)!=2){return 1;}}
				i_currenttype=2;
				(*icursor)+=strlen(list_greeklist[ilv1].output);
				fprintf(output,"%s",list_greeklist[ilv1].input);
				goto ifound;
			}
		}
		ihv1='?';
		fwrite(&ihv1,1,1,output);
		(*icursor)++;
		ifound:;
	}
	else
	{
		if (text[(*icursor)]==0xA) {fprintf(output,"\xD");goto wasalinebreak;}
		fwrite(text+(*icursor),1,1,output);
		(*icursor)++;
		wasalinebreak:;
	}
	goto iback;
	return 0;
}
void edit_printtest(char * text,int * i_elementcount,int * tl_starttype,int * length)
{
	int cursor=0;
	int i_currenttype=0;
	char ihv1=0;
	*tl_starttype=0;
	iback:;
	if (text[cursor]==0) {return;};
	if (text[cursor] & 0x80)
	{
		for (int ilv1=0;ilv1<0x80;ilv1++)
		{
			if (strncmp(list_win_1258[ilv1],text+cursor,strlen(list_win_1258[ilv1]))==0)
			{
				cursor+=strlen(list_win_1258[ilv1]);
				(*length)+=1;
				if ((*tl_starttype)==0) {(*tl_starttype)=1;}else{if ((i_currenttype)!=1){(*i_elementcount)+=1;}}
				i_currenttype=1;
				goto ifound;
			}
		}
		for (int ilv1=0;ilv1<list_greeklist_size;ilv1++)
		{
			if (strncmp(list_greeklist[ilv1].output,text+cursor,strlen(list_greeklist[ilv1].output))==0)
			{
				cursor+=strlen(list_greeklist[ilv1].output);
				(*length)+=strlen(list_greeklist[ilv1].input);
				if ((*tl_starttype)==0) {(*tl_starttype)=2;}else{if ((i_currenttype)!=2){(*i_elementcount)+=1;}}
				i_currenttype=2;
				goto ifound;
			}
		}
		ihv1='?';
		cursor++;
		length++;
		ifound:;
	}
	else
	{
		cursor++;
	}
	goto iback;
}
int edit_writestobuffer(char * input,void * output)
{
	int segcount=0;
	int textlength=0;
	static int length=0x00;
	basicmultilistreference * tl_basicmultilistreference=*((basicmultilistreference**)input);
	int tl_max=tl_basicmultilistreference->start_in_it+tl_basicmultilistreference->count_in_it;
	for (int ilv1=tl_basicmultilistreference->start_in_it;ilv1<tl_max;ilv1++)
	{
		int tl_count=1;
		int tl_starttype=0;
		CAMBRIDGE_s_instance * tl_CAMBRIDGE_s_instance=glob_CAMBRIDGE_s_multilist->bufferlist()+ilv1;
		edit_printtest(tl_CAMBRIDGE_s_instance->PCTEXT.a,&tl_count,&tl_starttype,&textlength);
		if (tl_starttype!=0)
		{
			tl_CAMBRIDGE_s_instance->font=tl_starttype;
		}
		segcount+=tl_count;
	}
	size_t fseekstart=ftell((FILE*)output);//writes EMPTY length
	size_t fseekcur;
	int icursor=0;
	fwrite(&length,2,1,(FILE*)output);
	length=tl_basicmultilistreference->count_in_it;//length in styleruns
	fwrite(&length,2,1,(FILE*)output);
	fseekcur=fseekstart+4+sizeof(cdx_Stylerun)*segcount;
	length=fseekcur-fseekstart-2;
	for (int ilv1=tl_basicmultilistreference->start_in_it;ilv1<tl_basicmultilistreference->start_in_it+tl_basicmultilistreference->count_in_it;ilv1++)
	{
		cdx_Stylerun tl_stylerun;
		int subcursor=0;
		iagain:;
		fseek((FILE*)output,4+(icursor)*sizeof(cdx_Stylerun)+fseekstart,SEEK_SET);
		icursor++;
		CAMBRIDGE_s_instance * tl_CAMBRIDGE_s_instance=glob_CAMBRIDGE_s_multilist->bufferlist()+ilv1;
		tl_stylerun.startpos=fseekcur-(4+(segcount)*sizeof(cdx_Stylerun)+fseekstart);
		tl_stylerun.color=tl_CAMBRIDGE_s_instance->color;
		tl_stylerun.font=tl_CAMBRIDGE_s_instance->font;
		tl_stylerun.face=tl_CAMBRIDGE_s_instance->face;
		tl_stylerun.size=tl_CAMBRIDGE_s_instance->size*20;
		fwrite(&tl_stylerun,sizeof(cdx_Stylerun),1,(FILE*)output);
		fseek((FILE*)output,fseekcur,SEEK_SET);
		if (edit_printtranslate((FILE*)output,tl_CAMBRIDGE_s_instance->PCTEXT.a,&subcursor)){goto iagain;}
		fseekcur=ftell((FILE*)output);
	}
	length=fseekcur-fseekstart-2;
	fseek((FILE*)output,fseekstart,SEEK_SET);
	fwrite(&length,2,1,(FILE*)output);
	fseek((FILE*)output,fseekcur,SEEK_SET);
	return 0;
}
int edit_writefonttabletobuffer(char * input,void * output)
{
	_i16 ilhv1=27;
	fwrite(&ilhv1,2,1,(FILE*)output);
	ilhv1=1;//WINDOWS
	fwrite(&ilhv1,2,1,(FILE*)output);
	ilhv1=2;//count
	fwrite(&ilhv1,2,1,(FILE*)output);
	ilhv1=1;//id1
	fwrite(&ilhv1,2,1,(FILE*)output);
	ilhv1=1258;
	fwrite(&ilhv1,2,1,(FILE*)output);
	ilhv1=5;
	fwrite(&ilhv1,2,1,(FILE*)output);
	fprintf((FILE*)output,"Arial");
	ilhv1=2;//id2
	fwrite(&ilhv1,2,1,(FILE*)output);
	ilhv1=0;
	fwrite(&ilhv1,2,1,(FILE*)output);
	ilhv1=6;
	fwrite(&ilhv1,2,1,(FILE*)output);
	fprintf((FILE*)output,"Symbol");
	return 0;
}
int edit_readfonttablefrombuffer(char * input)
{
	int ilength=0;
	int icounter=0;
	int ilv2;
	_i16 ilhv1;
	glob_CAMBRIDGE_font_multilist->filllevel=0;
	//skipping windowsness
	memcpy(&ilength,input+2,2);//ENDIAN
	icounter=4;
	if (((glob_CAMBRIDGE_font_multilist)->filllevel+ilength)>glob_CAMBRIDGE_font_multilist->getmaxitems())
	{
		LHENDRAW_loadmemoryoverflow=1;
		return 0;
	}
	for (int ilv1=0;ilv1<ilength;ilv1++)
	{
		ADD_TO_MULTILISTREFERENCE(currentinstance,font);
		memcpy(&ilhv1,input+icounter,2);//ENDIAN
		tl_CAMBRIDGE_font_instance->id=ilhv1;
		icounter+=2;
		memcpy(&ilhv1,input+icounter,2);//ENDIAN
		tl_CAMBRIDGE_font_instance->charset=ilhv1;
		icounter+=2;
		memcpy(&ilhv1,input+icounter,2);
		icounter+=2;
		if (ilhv1>stringlength) {icounter+=ilhv1;}
		else
		{
			for (ilv2=0;ilv2<ilhv1;)
			{
				memcpy((tl_CAMBRIDGE_font_instance->name.a+ilv2),input+icounter,1);
				icounter++;
				ilv2++;
				if (icounter>paramvaluestring_length) goto idone;
			}
			idone:;
			tl_CAMBRIDGE_font_instance->name.a[ilv2]=0;
		}
		if (icounter>paramvaluestring_length) return 0;
	}
	return 0;
}
//atom1 is the atom in the template, atom2 is the one that is supposed to "match" on the template, so the latter may have special properties the former hasn't
int edit_atom_fits(n_instance * atom1,n_instance * atom2)
{
	if (atom1->Element==atom2->Element)
	{
		goto chargecheck;
	}
	else
	{
		if (atom1->Element==constants_Element_implicitcarbon)
		{
			if (atom2->Element==constants_Element_implicitcarbon+1)
			{
				goto chargecheck;
			}
		}
	}
	return 0;
	chargecheck:;
	if ((atom1->charge>0) && (atom2->charge<atom1->charge)) return 0;
	if ((atom1->charge<0) && (atom2->charge>atom1->charge)) return 0;
	return 1;
}
int edit_bond_fits(b_instance * bond1,b_instance * bond2)
{
	if (bond1->Order>bond2->Order)
	{
		return 0;
	}
	return 1;
}
int edit_atoms_in_check=0;
int currentatom_in_check=0;
int * edit_searchbuffer;
int * edit_orderbuffer;
int * edit_branchbuffer;//stores for every orderbuffer-atom from what parent it had branched, and -1 for the root atom
void edit_moleculesearch_brell()
{
	for (int ilv1=0;ilv1<currentatom_in_check;ilv1++)
	{
		printf("°°%i°°",edit_branchbuffer[ilv1]);
	}
	printf("\n");
}
void edit_moleculesearch_oell()
{
	for (int ilv1=0;ilv1<currentatom_in_check;ilv1++)
	{
		printf("''%i''",edit_orderbuffer[ilv1]);
	}
	printf("\n");
}
void edit_moleculesearch_yell()
{
	for (int ilv1=0;ilv1<currentatom_in_check;ilv1++)
	{
		printf("`%c%i%c`",selection_clickselection[edit_searchbuffer[edit_orderbuffer[ilv1]]]?'(':'`',edit_searchbuffer[edit_orderbuffer[ilv1]],selection_clickselection[edit_searchbuffer[edit_orderbuffer[ilv1]]]?')':'`');
	}
	printf("\n");
	memory_yell(edit_searchbuffer);
	edit_moleculesearch_brell();
	edit_moleculesearch_oell();
}
int edit_getunclicked(int atom1,int * result,char selectionmode)
{
	char foundalready=0;
	if ((*result)==-1) foundalready=1;
	for (int ilv1=0;ilv1<atom_actual_node[atom1].bondcount;ilv1++)
	{
		int resultnr=getother(atom1,atom_actual_node[atom1].bonds[ilv1]);
		if (foundalready) goto ipossible;
		else
		{
			if (resultnr==*result) foundalready=1;
			goto iimpossible;
		}
		ipossible:;
		if (((selection_currentselection[resultnr] & (1<<STRUCTURE_OBJECTTYPE_n))!=0)==selectionmode)
		{
			if ((selection_clickselection[resultnr] & (1<<STRUCTURE_OBJECTTYPE_n))==0)
			{
				*result=resultnr;
				return 1;
			}
		}
		iimpossible:;
	}
	return 0;
}
//expands by 1 step
int edit_moleculesearch_findtemplate(int from)
{
	//setting (edit_orderbuffer[edit_atoms_in_check]) to -1 always, the used ones are clickselection-ed anyway, and one does not need to iterate through them.
	(edit_orderbuffer[edit_atoms_in_check])=-1;
	int retval=edit_getunclicked(edit_orderbuffer[from],&(edit_orderbuffer[edit_atoms_in_check]),1);
	if (retval<0) return 0;//error occurred. 0 means: abandon this
	if (retval==0)
	{
		if (from==edit_atoms_in_check-1)//Cheap way to tell whether this is the main loop
		{
			for (int ilv1=from-1;ilv1>=0;ilv1--)
			{
				if (edit_moleculesearch_findtemplate(ilv1)>0) return 1;
			}
			return 2;//means: finished
		}
		return 0;
	}
	selection_clickselection[edit_orderbuffer[edit_atoms_in_check]]|=(1<<STRUCTURE_OBJECTTYPE_n);
	edit_searchbuffer[edit_orderbuffer[edit_atoms_in_check]]=-1;
	edit_branchbuffer[edit_atoms_in_check]=from;
	edit_atoms_in_check++;
	return 1;
}
int edit_moleculesearch_findnext()
{
	int retval;
	edit_searchbuffer[edit_orderbuffer[currentatom_in_check]]=-1;
	iback:;
	retval=edit_getunclicked(edit_searchbuffer[edit_orderbuffer[edit_branchbuffer[currentatom_in_check]]],&(edit_searchbuffer[edit_orderbuffer[currentatom_in_check]]),0);
	if (retval<=0) return 0;//means: abandon this
	if (edit_atom_fits(glob_n_multilist->bufferlist()+edit_orderbuffer[currentatom_in_check],glob_n_multilist->bufferlist()+edit_searchbuffer[edit_orderbuffer[currentatom_in_check]])>0)
	{
		selection_clickselection[edit_searchbuffer[edit_orderbuffer[currentatom_in_check]]]|=(1<<STRUCTURE_OBJECTTYPE_n);
		currentatom_in_check++;
		return 1;
	}
	goto iback;
}
int edit_moleculesearch_findother()
{
	int retval;
	iback:;
	if (currentatom_in_check<=1) return 0;
	selection_clickselection[edit_searchbuffer[edit_orderbuffer[currentatom_in_check-1]]]&=~(1<<STRUCTURE_OBJECTTYPE_n);
	retval=edit_getunclicked(edit_searchbuffer[edit_orderbuffer[edit_branchbuffer[currentatom_in_check-1]]],&(edit_searchbuffer[edit_orderbuffer[currentatom_in_check-1]]),0);
	if (retval<=0) return 0;//means: abandon this
	if (edit_atom_fits(glob_n_multilist->bufferlist()+edit_orderbuffer[currentatom_in_check-1],glob_n_multilist->bufferlist()+edit_searchbuffer[edit_orderbuffer[currentatom_in_check-1]])>0)
	{
		selection_clickselection[edit_searchbuffer[edit_orderbuffer[currentatom_in_check-1]]]|=(1<<STRUCTURE_OBJECTTYPE_n);
		return 1;
	}
	goto iback;
}
int edit_moleculesearch_findlast()
{
	int retval;
	iback:;
	if (currentatom_in_check<=2) return 0;//means: abandon this
	selection_clickselection[edit_searchbuffer[edit_orderbuffer[currentatom_in_check-1]]]&=~(1<<STRUCTURE_OBJECTTYPE_n);
	currentatom_in_check--;
	selection_clickselection[edit_searchbuffer[edit_orderbuffer[currentatom_in_check-1]]]&=~(1<<STRUCTURE_OBJECTTYPE_n);
	retval=edit_getunclicked(edit_searchbuffer[edit_orderbuffer[edit_branchbuffer[currentatom_in_check-1]]],&(edit_searchbuffer[edit_orderbuffer[currentatom_in_check-1]]),0);
	if (retval>0)
	{
		selection_clickselection[edit_searchbuffer[edit_orderbuffer[currentatom_in_check-1]]]|=(1<<STRUCTURE_OBJECTTYPE_n);
		return 1;
	}
	goto iback;
}
int edit_moleculesearch()
{
	int ibackval=0;
	int retval;
	memory_alloc((char**)&edit_searchbuffer,5);
	memory_alloc((char**)&edit_orderbuffer,5);
	memory_alloc((char**)&edit_branchbuffer,5);
	selection_clearselection(selection_clickselection);
	//clickselected quaerand: currently taken in account in current match
	//clickselected quaeror: part of current match
	for (int ilv1=0;ilv1<glob_n_multilist->filllevel;ilv1++)
	{
		if (selection_currentselection[ilv1] & (1<<STRUCTURE_OBJECTTYPE_n))
		{
			n_instance * iinstance=glob_n_multilist->bufferlist()+ilv1;
			if (iinstance->exist)
			{
				selection_clickselection[ilv1]|=(1<<STRUCTURE_OBJECTTYPE_n);
				edit_orderbuffer[0]=ilv1;
				edit_branchbuffer[0]=-1;
				edit_atoms_in_check=1;
				goto igottemplateatom;
			}
		}
	}
	//Nothing was selected
	retval=0;
	goto done;
	igottemplateatom:;
	retval=0;
	do
	{
		retval=edit_moleculesearch_findtemplate(edit_atoms_in_check-1);
	} while ((retval>0) && (retval!=2));
	for (int ilv1=0;ilv1<glob_n_multilist->filllevel;ilv1++)//Loops all unselected atoms to start tree-matching there
	{
		if ((selection_currentselection[ilv1] & (1<<STRUCTURE_OBJECTTYPE_n))==0)
		{
			n_instance * iinstance=glob_n_multilist->bufferlist()+ilv1;
			if (iinstance->exist)
			{
				if (edit_atom_fits(glob_n_multilist->bufferlist()+edit_orderbuffer[0],iinstance)>0)
				{
					selection_clickselection[ilv1]|=(1<<STRUCTURE_OBJECTTYPE_n);
					edit_searchbuffer[edit_orderbuffer[0]]=ilv1;
					currentatom_in_check=1;
					edit_branchbuffer[0]=-1;
					iback:;
					ibackval=edit_moleculesearch_findnext();
					iskipback:;
					if (ibackval==1)
					{
						if (currentatom_in_check==edit_atoms_in_check)
						{
							int verificationmode=1;
							inscribe:;//run through this twice, once to check if bonds are fitting, second time to mark the fitting bonds
							for (int ilv2=0;ilv2<glob_n_multilist->filllevel;ilv2++)
							{
								if (selection_currentselection[ilv2] & (1<<STRUCTURE_OBJECTTYPE_n))
								{
									n_instance * iinstance2=glob_n_multilist->bufferlist()+ilv2;
									if (iinstance2->exist)
									{
										for (int ilv3=0;ilv3<atom_actual_node[ilv2].bondcount;ilv3++)
										{
											if (selection_currentselection[atom_actual_node[ilv2].bonds[ilv3]] & (1<<STRUCTURE_OBJECTTYPE_b))
											{
												int other=getother(ilv2,atom_actual_node[ilv2].bonds[ilv3]);
												if (selection_currentselection[other] & (1<<STRUCTURE_OBJECTTYPE_n))
												{
													int backval=get_bond_between_actual_node(edit_searchbuffer[ilv2],edit_searchbuffer[other]);
													if (backval>=0)
													{
														if (verificationmode)
														{
															if (edit_bond_fits(glob_b_multilist->bufferlist()+atom_actual_node[ilv2].bonds[ilv3],glob_b_multilist->bufferlist()+backval)<=0) goto failed;
														}
														else
														{
															selection_clickselection[backval]|=(1<<STRUCTURE_OBJECTTYPE_b);
														}
													}
													else
													{
														goto failed;
													}
												}
											}
										}
									}
								}
							}
							if (verificationmode)
							{
								verificationmode=0;
								goto inscribe;
							}
							selection_SUBTRACTselection(selection_clickselection,selection_currentselection);
							selection_copyselection(selection_currentselection,selection_clickselection);
							retval=1;
							goto done;
						}
						goto iback;
					}
					failed:;
					if (edit_moleculesearch_findother()>0)
					{
						ibackval=1;
						goto iskipback;
					}
					else
					{
						if (edit_moleculesearch_findlast()<=0)
						{
							goto badstart;
						}
						else
						{
							goto iback;
						}
					}
					badstart:;//remember to clear selection when jumping to this label with a bigger structure built up.
					selection_clickselection[ilv1]&=~(1<<STRUCTURE_OBJECTTYPE_n);
				}
			}
		}
	}
	retval=0;
	done:;
	memory_free(edit_branchbuffer);
	memory_free(edit_orderbuffer);
	memory_free(edit_searchbuffer);
	return retval;
}
catalogized_command_funcdef(SEARCH)
{
	checkupinconsistencies();
	return edit_moleculesearch();
}
catalogized_command_funcdef(SEARCHFILE)
{
	int retval;
	storeundo(~0);
	selection_clearselection(selection_currentselection);
	SELECTALL("","1");
	edit_fileoperationrefersonlytopartofdocument=1;
	retval=LOAD_TYPE(parameter,value);
	edit_fileoperationrefersonlytopartofdocument=0;
	if (retval>0)
	{
		retval=SEARCH("","");
	}
	restoreundo(~0,0);
	return (retval>0);
}
catalogized_command_funcdef(FILEDLG_FILE_SEARCH)
{
	char currentfilename_search[stringlength*2+2];
	DIR * DD=opendir(control_currentdirectory_search);
	printf("%s\n",control_currentdirectory_search);
	struct dirent * dirpy;
	int retval=-30;
	if (DD)
	{
		for (int ilv1=0;true;ilv1++)
		{
			dirpy=readdir(DD);
			if (dirpy==NULL) goto readfinished;
			strncpy(currentfilename_search,control_currentdirectory_search,stringlength);
			strncat(currentfilename_search,"/",1);
			strncat(currentfilename_search,dirpy->d_name,stringlength);
			currentfilename_search[stringlength*2+1]=0;
			FILE * testfile=fopen(currentfilename_search,"r");
			if (testfile)
			{
				fclose(testfile);
				if (SEARCHFILE(currentfilename_search,"")>0)
				{
					char orderstring[stringlength*3+3];
					if (strchr(currentfilename_search,'\'')==NULL)
					{
						sprintf(orderstring,"lhendraw '%s' &",currentfilename_search);
						system(orderstring);
					}
				}
			}
		}
		readfinished:;
		closedir(DD);
		return 1;
	}
	return retval;
}
extern int control_tool;
catalogized_command_funcdef(DUMPCLICKABMAT)
{
	FILE * clickabilitymatrixfile=fopen("./clickabilitymatrixlist.clickabilitymatrixtxt","a");
	fprintf(clickabilitymatrixfile,"control_clickabilitymatrixes[%i]=({clickabilitymatrix_ tl_clickabilitymatrix={%i,%i,%i,\n"
"{0x%X,0x%X,0x%X,0x%X,0x%X,0x%X,0x%X,0x%X}};tl_clickabilitymatrix;});\n",control_tool,selection_clickabilitymatrix.mode,selection_clickabilitymatrix.level0,selection_clickabilitymatrix.types1,selection_clickabilitymatrix.types2[0],selection_clickabilitymatrix.types2[1],selection_clickabilitymatrix.types2[2],selection_clickabilitymatrix.types2[3],selection_clickabilitymatrix.types2[4],selection_clickabilitymatrix.types2[5],selection_clickabilitymatrix.types2[6],selection_clickabilitymatrix.types2[7]);
	fclose(clickabilitymatrixfile);
	return 1;
}
