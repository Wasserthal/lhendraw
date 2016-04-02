/*
Janitor: Controls and tidy-up the internal data structure.
Why not order by depth order?
This would make rearrangements necessary, which is time-consuming
A depth-order list will be built at realtime, one for each program cycle (consisting of 1. Graphics and 2. Change).
Object Buffers:
gap design
All objects are appended on the end of their list.
Deleted objects will not be replaced, thus creating gaps consisting of objects with exist-flag=0
The property lists, which are of variable size, are indirected to by the objects by their position in the buffer in bytes, and indirect to the object by its INDEX in the buffer.
Indirection between objects is done by their id.
The
the sortlist doesnt get more items than any other lists altogether.
As long the objects are only appended on the end, we can do a list sort which would go with n.(no exclamation mark here, please!)
*/
/*struct multi_objref
{
	int listnr;//number of multilist, -1 means empty
	int nr;
	int Z;//Or better let that out? either link ratio or actual order will be relevant!
}multi_objref;*/
int minusoneint=-1;
typedef struct multi_objref_
{
	int listnr;//number of multilist, -1 means empty
	int nr;//-1 means empty
}multi_objref_;
multi_objref_ * objectZorderlist=NULL;
int objectZorderlist_count;
multi_objref_ * sortarr2[2]={NULL,NULL};
typedef struct multi_Z_geometry_
{
	char * offset;
	int elementsize;
}multi_Z_geometry_;
multi_Z_geometry_ multi_Z_geometry[multilistlistlength];
int janitor_id_list[STRUCTURE_OBJECTTYPE_ListSize+TELESCOPE_ELEMENTTYPE_ListSize];

/*
how is the depth order list created for the first time?
The objects are dumbly listed. Then we do a merge sort.

*/
//After each merge sort, the Items are renumbered.
//After a list sort, the Items are only renumbered if some were deleted, or inserted in the middle.
//Problem: Z-Order is changed of Objects that are technically unchanged.
//Consequence: Z-Order must not be inside of the objects, but in the order list.
//No back effects!
//If objects should be inserted at a determined POSITION, The ordering list is ordered FIRST,
//Howto retrieve partner numbers on a freshly inserted/deleted object?
//Then anything is <i>ad felis</i>, sortlist gotta be rebuild after each change, we omit storing it...
//Storing the Z order in the Objects, gotta rebuild each time anyway.
//We dont need to sort if anything fits, but renumbering might be still necessary.

char somewhereinZlist(int listnr, int nr)
{
	for (int ilv1=0;ilv1<objectZorderlist_count;ilv1++)
	{
		if (objectZorderlist[ilv1].listnr==-1)
		{
			objectZorderlist[ilv1].listnr=listnr;
			objectZorderlist[ilv1].nr=nr;
			return 1;
		}
	}
	return 0;
}
char insertinZlist(int tlthisZ, int listnr, int nr)
{
	if ((tlthisZ<0) || (tlthisZ>=objectZorderlist_count))
	{
		return 0;
	}
	if (objectZorderlist[tlthisZ].listnr==-1)
	{
		objectZorderlist[tlthisZ].listnr=listnr;
		objectZorderlist[tlthisZ].nr=nr;
		return 1;
	}
	return 0;
}
char deletefromZlist(int listnr, int nr)
{
	for (int ilv1=0;ilv1<objectZorderlist_count;ilv1++)
	{
		if (objectZorderlist[ilv1].listnr==listnr)
		{
			if (objectZorderlist[ilv1].nr==nr)
			{
				objectZorderlist[ilv1].nr=-1;
				objectZorderlist[ilv1].listnr=-1;
				return 1;
			}
		}
	}
	return -1;
}
//function instead of macro - good for debugging
/*unsigned int * janitor_getZ(multi_objref_ & MACROPARAM)
{
	unsigned int * backval;
	multi_objref_ * pointer;
	pointer=&(MACROPARAM);
	if ((*pointer).listnr!=-1)
	{
		multi_Z_geometry_ * pointer2;
		pointer2=multi_Z_geometry+(*pointer).listnr;
		backval=(unsigned int *)((int*)((*pointer2).offset+((*pointer2).elementsize)*(*pointer).nr));
	}
	else
	{
		backval=(unsigned int*)&minusoneint;
	}
	return backval;
}*/
#define janitor_getZ(MACROPARAM) \
({\
	unsigned int * backval;\
	multi_objref_ * pointer;\
	pointer=&(MACROPARAM);\
	if ((*pointer).listnr!=-1)\
	{\
		multi_Z_geometry_ * pointer2;\
		pointer2=multi_Z_geometry+(*pointer).listnr;\
		backval=(unsigned int *)((int*)((*pointer2).offset+((*pointer2).elementsize)*(*pointer).nr));\
	}\
	else\
	{\
		backval=(unsigned int*)&minusoneint;\
	}\
	backval;\
})
void mergesortrecursion(int single,int max,char ibool)
{
	int usedup1,usedup2;int remainingright;
	for (int ilv1=0;ilv1<max;ilv1+=2*single)
	{
		usedup1=0;usedup2=0;
		int done=0;
		remainingright=max-ilv1-single;
		if (remainingright<0)
		{
			while (usedup1<single+remainingright)
			{
				sortarr2[ibool^1][ilv1+done]=sortarr2[ibool][ilv1+usedup1];
				usedup1++;
				done++;
			}
			goto ifertig;
		}
		if (remainingright>single) remainingright=single;
		iback:
		if (usedup1>=single)
		{
			goto finishright;
		}
		if (usedup2>=remainingright)
		{
			goto finishleft;
		}
		if (*janitor_getZ(sortarr2[ibool][ilv1+usedup1])<*janitor_getZ(sortarr2[ibool][ilv1+single+usedup2]))
		{
			sortarr2[ibool ^ 1][ilv1+done]=sortarr2[ibool][ilv1+usedup1];
			usedup1++;
		}
		else
		{
			sortarr2[ibool ^ 1][ilv1+done]=sortarr2[ibool][ilv1+single+usedup2];
			usedup2++;
		}
		done++;
		goto iback;
		finishleft:
		while (usedup1<single)
		{
			sortarr2[ibool^1][ilv1+done]=sortarr2[ibool][ilv1+usedup1];
			usedup1++;
			done++;
		}
		goto ifertig;
		finishright:
		while (usedup2<remainingright)
		{
			sortarr2[ibool^1][ilv1+done]=sortarr2[ibool][ilv1+single+usedup2];
			usedup2++;
			done++;
		}
		ifertig:
		;
	}
}

void mergesort()
{
	int ilv1;
	for (ilv1=0;ilv1<objectZorderlist_count;ilv1++)
	{
		sortarr2[0][ilv1]=objectZorderlist[ilv1];
	}
	int t=1;int n=0;
	int subt;
	while (t<objectZorderlist_count)
	{
		t*=2;
		n++;
	}
	subt=1;
	for (ilv1=0;ilv1<n;ilv1++)
	{
		mergesortrecursion(subt,objectZorderlist_count, (ilv1 & 1));
		subt*=2;
	}
	for (int ilv2=0;ilv2<objectZorderlist_count;ilv2++)
	{
		objectZorderlist[ilv2]=sortarr2[(ilv1 & 1)][ilv2];
	}
}

void reenumerate()
{
	for (int ilv1=0;ilv1<objectZorderlist_count;ilv1++)
	{
		int * pointer=(int*)janitor_getZ(objectZorderlist[ilv1]);
		if (pointer!=&minusoneint)
		{
			*pointer=ilv1;
			janitor_maxZ=ilv1;
		}
	}
}

void initZlist()
{
	char iZorderbroken=0;
	objectZorderlist_count=0;
	for (int ilv1=1;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
	{
		objectZorderlist_count+=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name)->filllevel;
	}
	objectZorderlist_count*=2;
	realloced:;
	if (objectZorderlist_count*sizeof(multi_objref_)>=LHENDRAW_buffersize)
	{
		if (memory_realloc_x2()==0) error("memory not sufficient for Z-buffer!");
		goto realloced;
	}
	if (objectZorderlist==NULL)
	{
		memory_alloc((char**)&objectZorderlist,5);
		memory_alloc((char**)(sortarr2+0),5);
		memory_alloc((char**)(sortarr2+1),5);
	}
	for (int ilv1=0;ilv1<objectZorderlist_count;ilv1++)
	{
		objectZorderlist[ilv1].nr=-1;objectZorderlist[ilv1].listnr=-1;
	}
 	for (int ilv1=0;ilv1<multilist_count;ilv1++)
	{
		CDXMLREAD_functype thisfunc;
		int propertypos;
		if (multilistlist[ilv1].usage==0)
		{
			basicmultilist * thismultilist=multilistlist[ilv1].instance;
			propertypos=thismultilist->getproperties("Z",&thisfunc);
			if (propertypos!=-1)
			{
				multi_Z_geometry[ilv1].offset=(char*)(((*thismultilist)).pointer)+propertypos;
				multi_Z_geometry[ilv1].elementsize=(*thismultilist).itemsize;
				for (int ilv2=0;ilv2<(*thismultilist).filllevel;ilv2++)
				{
					if ((*thismultilist)[ilv2].exist)
					{
						multi_objref_ imulti_objref={ilv1,ilv2};
						int tlthisZ=*janitor_getZ(imulti_objref);
						if (!insertinZlist(tlthisZ, (*thismultilist).index,ilv2))//TODO: ... or here,find a patteern which always works, as calculating it from the numbering offset, somehow...
						{
							if (somewhereinZlist((*thismultilist).index,ilv2))
							{
								multi_objref_ _={ilv1,ilv2};
								objectZorderlist[objectZorderlist_count++]=_;
							print("Zorderbroken:%i of %i #%i\n",tlthisZ,ilv1,ilv2);
								iZorderbroken=1;
							}
							else
							{
								memory_overflow_hook();
							}
						}
					}
				}
			}
		}
	}
	if (iZorderbroken)
	{
		mergesort();
		reenumerate();
	}
}
void janitor_bufferresort()
{
	for (int ilv1=1;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
	{
		basicmultilist * imultilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
		int isize=STRUCTURE_OBJECTTYPE_List[ilv1].size;
		int tl_object=0;
		int tl_readpos=0;
		int tl_writepos=0;
		int length;
		TELESCOPE_buffer * ibuffer=glob_contentbuffer+ilv1;
		TELESCOPE * itelescope;
		while (tl_readpos<ibuffer->count)
		{
			itelescope=(TELESCOPE*)(ibuffer->buffer+tl_readpos);
			length=itelescope->length;
			tl_readpos+=length;
			if (itelescope->owner==-1)
			{
				for (int ilv3=tl_readpos;ilv3<ibuffer->count;ilv3++)
				{
					ibuffer->buffer[ilv3-length]=ibuffer->buffer[ilv3];
				}
				ibuffer->count-=length;
				//shifts the buffer pointers of the other objects.
				for (int ilv3=tl_object+1;ilv3<(*imultilist).filllevel;ilv3++)
				{
					basic_instance_propertybuffer * iinstance=(basic_instance_propertybuffer*)(((char*)((*imultilist).pointer))+(isize*ilv3));
					if ((*iinstance).pos_in_buffer>=tl_readpos)
					{
						(*iinstance).pos_in_buffer-=length;
					}
				}
			}
			else
			{
				tl_object=itelescope->owner;
				tl_writepos+=length;
			}
		}
	}
	return;
}
extern selection_datatype * selection_currentselection;
void janitor_memoryresort()
{
	for (int ilv1=1;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
	{
		basicmultilist * imultilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
		_uXX icompare=1<<ilv1;
		int isize=STRUCTURE_OBJECTTYPE_List[ilv1].size;
		int tl_difference=0;
		TELESCOPE_buffer * ibuffer=glob_contentbuffer+ilv1;
		for (int ilv2=0;ilv2+tl_difference<(*imultilist).filllevel;)
		{
			basic_instance_propertybuffer * iinstance=(basic_instance_propertybuffer*)(((char*)((*imultilist).pointer))+(isize*(ilv2+tl_difference)));
			if ((*iinstance).exist==0)
			{
				tl_difference+=1;
			}
			else
			{
				if (tl_difference!=0)
				{
					if (TELESCOPE_aggressobject(imultilist,ilv2+tl_difference))
					{
						TELESCOPE_reindex(ilv2);
					}
					memcpy((((char*)((*imultilist).pointer))+(isize*ilv2)),(((char*)((*imultilist).pointer))+(isize*(ilv2+tl_difference))),isize);
					_uXX selected=selection_currentselection[ilv2+tl_difference]&icompare;
					selection_currentselection[ilv2]&=~icompare;
					selection_currentselection[ilv2]|=selected;
				}
				ilv2++;
			}
		}
		(*imultilist).filllevel-=tl_difference;
	}
	return;
}
void janitor_tidynumbers()
{
	return;
}
extern void checkupinconsistencies();
catalogized_command_funcdef(REMEM)
{
	janitor_bufferresort();
	janitor_memoryresort();
	janitor_tidynumbers();
	checkupinconsistencies();
	return 1;
}
catalogized_command_funcdef(MEMX2)
{
	return memory_realloc_x2();
}
catalogized_command_funcdef(MEMD2)
{
	return memory_realloc_d2();
}
#define WORKIFIX_REGISTERED_TRADEMARK_workthrough_variables\
	_u32 icompare;\
	int isize;\
	char * ibufferpos;\
	int ioffset;
#define WORKIFIX_REGISTERED_TRADEMARK_workthrough(MACROPARAM_PROPERTY)\
				for (int ilv1=0;ilv1<sizeof(STRUCTURE_OBJECTTYPE_List)/sizeof(trienum);ilv1++)\
				{\
					icompare=1<<ilv1;\
					int isize= STRUCTURE_OBJECTTYPE_List[ilv1].size;\
					basicmultilist * tlmultilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);\
					if (tlmultilist==NULL) goto i_macrointernal_fertig;\
					CDXMLREAD_functype tldummy;\
					ioffset=(*tlmultilist).getproperties(# MACROPARAM_PROPERTY,&tldummy);\
					if (ioffset<0) goto i_macrointernal_fertig;\
					ibufferpos=(char*)((*tlmultilist).pointer);\
					cdx_Point2D * tlpoint2d;\
					for (int ilv2=0;ilv2<(*tlmultilist).filllevel;ilv2++)\
					{\
						if ((*((basic_instance*)(ibufferpos+isize*ilv2))).exist)\
						{
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
