//defines and processes internal variables
//This is the home of the bufferlist programming
//The bufferlists are arrays of TELESCOPE called elements, one for each object in the corresponding multilist, IN THE SAME ORDER.
//There is a reference to the list position of each multilist object in each TELESCOPE,
//and there is a reference to the bufferstart-relative position of its TELESCOPE.
//Some objects have no TELESCOPE. In this case, the pointer points to the TELESCOPE of the next object that has one, or at the end of the buffer
//because that is the position where a TELESCOPE should be inserted for that object.
TELESCOPE_buffer filestructure_text_buffer;
TELESCOPE_buffer filestructure_curve_buffer;
char LHENDRAW_filedlgmode=0;
char LHENDRAW_warndlgmode=0;
int LHENDRAW_userdecision=0;
char LHENDRAW_leave=0;
const char * LHENDRAW_warning=NULL;
int warndlg_loop(const char*warning);
_i32 janitor_maxZ=0;
_small LHENDRAW_maxbuttons=74;
typedef _u32 selection_datatype;
#define constants_bondlength 30
#define constants_angular_distance 2.094395102
#define constants_clickradius 225
#define constants_Element_loneradical 0
#define constants_Element_lonepair 1
#define constants_Element_implicitcarbon 9
float glob_clickradius=225;
float glob_subpoint_clickradius=50;
#define constants_maxbonds 10
#define constants_undostep_max 40
#ifndef CROFTOIDAL
#define constants_Directoryslash '/'
#else
#define constants_Directoryslash '\\'
#endif
#ifdef SDL2
#define SDLK_LSUPER SDLK_LGUI
#define SDLK_RSUPER SDLK_RGUI
#endif
char constants_cdxstring[]=".cdx";
typedef _u32 resources_button_[32][32];
resources_button_ * resources_bitmap_buttons;
extern TELESCOPE_buffer glob_contentbuffer[];
struct TELESCOPE_tempvar_
{
	int pos;//object inside buffer
	int objectsize;
	int subpos;//TELESCOPE_element inside object
	int subpos2;//write cursor inside object
	int objectpos;
	TELESCOPE_buffer * buffer;
	char inside_TELESCOPE;
	char inside_TELESCOPE_element;
	basicmultilist * multilist;
};
int control_menutextedithorziscroll=0;
int control_menutexteditcursor=0;
struct control_export_
{
	int bmp_compression=0;//0: normal;3:ARGB,32bpp only
}control_export;
TELESCOPE_tempvar_ TELESCOPE_tempvar;
TELESCOPE_tempvar_ TELESCOPE_debugvar;
char control_filememory_buffer[255][256];
char control_devicememory_buffer[255][64];
_u32 control_filememory_attribs[255];
int menu_selectedmenuelement;
char control_filemenu_mode=0;
int undo_undodirty=0;//set when undo was stored just to become immediately modified in the same command, like the storeundo at issueclick
structenum control_devicememory
{
	{"control_devicememory"},
	0,255,&control_devicememory_buffer,1,sizeof(control_devicememory_buffer[0]),-1,0,
};
structenum control_filememory
{
	{"control_filememory"},
	0,255,&control_filememory_buffer,1,sizeof(control_filememory_buffer[0]),-1,0,
};
char control_currentdirectory[1024];
char control_filenamehead[256];
void TELESCOPE_measure(int tag,TELESCOPE_buffer * ibuffer)
{
	TELESCOPE * start;
	TELESCOPE_element * tl_telescope_element;
	int total=0;
	int share=0;
	printf("count:%i\n",(int)ibuffer->count);
	iback:
	start=(TELESCOPE*)(((char*)((*ibuffer).buffer))+total);
	printf("%ile:%iow:%i\n",total,start->length,start->owner);
/*	for (int ilv1=0;ilv1<20;ilv1++)
	{
		printf("%02hhX ",*(((char*)((*ibuffer).buffer))+ilv1));
	}
	printf("\n");*/
	share=0;
	iback2:
	tl_telescope_element=(TELESCOPE_element*)(((char*)((*ibuffer).buffer))+total+sizeof(TELESCOPE)+share);
	printf("  %i:le:%ity:%i:%s\n",share,tl_telescope_element->length,tl_telescope_element->type,((char*)((*ibuffer).buffer))+total+sizeof(TELESCOPE)+share+TELESCOPE_ELEMENTTYPE_List[tag].size);
	if (tl_telescope_element->length==0)
	{
		goto idontgoback;
	}
	share+=tl_telescope_element->length;
	if (tl_telescope_element->length==0) {printf("Zero-Length Element at %i:\n",share);return;}
	if (share<start->length-sizeof(TELESCOPE))
	{
		goto iback2;
	}
	idontgoback:
	total+=start->length;
	if (start->length==0) {printf("Zero-Length Object at %i:\n",total);return;}
	if (total<(*ibuffer).count)
	{
		goto iback;
	}
	return;
}
int undosteps_count=0;
int currentundostep=-1;
TELESCOPE_buffer * undo_retrievecontentbuffer(intl start,intl list);
int getbufferfromstructure(basicmultilist * input,TELESCOPE_buffer * * bufferptr)
{
	if (input==CAMBRIDGEPREFIX2(glob,s_multilist))
	{
		(*bufferptr)=&filestructure_text_buffer;
		return 1;
	}
	if (input==CAMBRIDGEPREFIX2(glob,curve_multilist))
	{
		(*bufferptr)=&filestructure_curve_buffer;
		return 1;
	}
/*	if (input==glob_t_multilist)
	{
		(*bufferptr)=&internalstructure_text_buffer;
		return 1;
	}
	if (input==glob_n_multilist)
	{
		(*bufferptr)=&internalstructure_n_buffer;
		return 1;
	}
	if (input==glob_arrow_multilist)
	{
		(*bufferptr)=&internalstructure_arrow_buffer;
		return 1;
	}*/
	char * name=NULL;
	for (int ilv1=0;ilv1<multilist_count;ilv1++)
	{
		if (multilistlist[ilv1].instance==input)
		{
			name=multilistlist[ilv1].name;
		}
	}
	for (int ilv1=1;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
	{
		if (strcmp(name,STRUCTURE_OBJECTTYPE_List[ilv1].name)==0)
		{
			(*bufferptr)=glob_contentbuffer+ilv1;
			return 1;
		}
	}
	return 0;
}
char TELESCOPE_verify_objectpresent()
{
	if (TELESCOPE_tempvar.pos<(*(TELESCOPE_tempvar.buffer)).count)
	{
		return ((*((TELESCOPE*)(((*(TELESCOPE_tempvar.buffer)).buffer)+TELESCOPE_tempvar.pos))).owner==TELESCOPE_tempvar.objectpos);
	}
	else
	{
		TELESCOPE_tempvar.inside_TELESCOPE_element=0;
		return 0;
	}
}
int TELESCOPE_verify_objectcorrect()//-1: error 0: end of list 1: object 2: empty object
{
	if (TELESCOPE_debugvar.pos<(*(TELESCOPE_debugvar.buffer)).count)
	{
		TELESCOPE_debugvar.objectpos=(*((TELESCOPE*)(((*(TELESCOPE_debugvar.buffer)).buffer)+TELESCOPE_debugvar.pos))).owner;
		if (TELESCOPE_debugvar.objectpos==-1)
		{
			return 2;
		}
		if ((TELESCOPE_debugvar.objectpos<0) || (TELESCOPE_debugvar.objectpos>(*(TELESCOPE_debugvar.multilist)).filllevel))
		{
			fprintf(stderr,"TELESCOPE invalid object number%i of %lli\n",TELESCOPE_debugvar.objectpos,(*(TELESCOPE_debugvar.multilist)).filllevel);exit(1);
		}
		basic_instance_propertybuffer * i_basic_instance=((basic_instance_propertybuffer*)(((char*)((*(TELESCOPE_debugvar.multilist)).pointer))+(TELESCOPE_debugvar.objectsize*TELESCOPE_debugvar.objectpos)));
		if ((*i_basic_instance).exist==0)
		{
			fprintf(stderr,"TELESCOPE owned by dead object\n");exit(1);
		}
		if ((*i_basic_instance).pos_in_buffer!=TELESCOPE_debugvar.pos)
		{
			fprintf(stderr,"TELESCOPE and object went asynchronous:%i,%i\n",(int)(*i_basic_instance).pos_in_buffer,(int)TELESCOPE_debugvar.pos);exit(1);
		}
	}
	else
	{
		if (TELESCOPE_debugvar.pos>(*(TELESCOPE_debugvar.buffer)).count)
		{
			fprintf(stderr,"TELESCOPE_buffer_overflow at beginning of object");exit(1);
		}
		return 0;
	}
	return 1;
}
int TELESCOPE_stretch_buffer(basicmultilist * imultilist,int ideltaplus,int itype)
{
	int ideltaplus2;char enlengthen=0;char enlengthen2=0;
	TELESCOPE_buffer * ibuffer=TELESCOPE_tempvar.buffer;
	ideltaplus2=ideltaplus;
	if (TELESCOPE_verify_objectpresent()) {goto wehadit;}
	ideltaplus2+=sizeof(TELESCOPE);
	enlengthen=1;
	wehadit:
	if (!(TELESCOPE_tempvar.inside_TELESCOPE_element))
	{
		ideltaplus2+=TELESCOPE_ELEMENTTYPE_List[itype].size;
		enlengthen2=1;
	}
	if ((*ibuffer).count+ideltaplus2>LHENDRAW_buffersize)
	{
		return -1;
	}
	for (int ilv1=(*ibuffer).count+ideltaplus2-1;ilv1>=TELESCOPE_tempvar.pos+ideltaplus2+TELESCOPE_tempvar.subpos2;ilv1--)//TODO: FASTER! buffercopy_reverse(
	{
		(*ibuffer).buffer[ilv1]=(*ibuffer).buffer[ilv1-ideltaplus2];
	}
	//TODO: Prellblöcke!
	if (enlengthen)
	{
		TELESCOPE_tempvar.inside_TELESCOPE=1;
		(*((TELESCOPE*)((*ibuffer).buffer+TELESCOPE_tempvar.pos))).owner=TELESCOPE_tempvar.objectpos;
		(*((TELESCOPE*)((*ibuffer).buffer+TELESCOPE_tempvar.pos))).length=ideltaplus2;
		TELESCOPE_tempvar.subpos=sizeof(TELESCOPE);
		TELESCOPE_tempvar.inside_TELESCOPE_element=1;
		(*((TELESCOPE_element*)((*ibuffer).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos))).type=itype;
		(*((TELESCOPE_element*)((*ibuffer).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos))).length=ideltaplus+TELESCOPE_ELEMENTTYPE_List[itype].size;
		TELESCOPE_tempvar.subpos2=TELESCOPE_tempvar.subpos+TELESCOPE_ELEMENTTYPE_List[itype].size;
	}
	else
	{
		if (enlengthen2)
		{
			TELESCOPE_tempvar.inside_TELESCOPE_element=1;
			(*((TELESCOPE_element*)((*ibuffer).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos))).type=itype;
			(*((TELESCOPE_element*)((*ibuffer).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos))).length=ideltaplus+TELESCOPE_ELEMENTTYPE_List[itype].size;
			(*((TELESCOPE*)((*ibuffer).buffer+TELESCOPE_tempvar.pos))).length+=ideltaplus+TELESCOPE_ELEMENTTYPE_List[itype].size;
			TELESCOPE_tempvar.subpos2=TELESCOPE_tempvar.subpos+TELESCOPE_ELEMENTTYPE_List[itype].size;
		}
		else
		{
			(*((TELESCOPE_element*)((*ibuffer).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos))).length+=ideltaplus;
			(*((TELESCOPE*)((*ibuffer).buffer+TELESCOPE_tempvar.pos))).length+=ideltaplus2;
		}
	}
	//shifts the buffer pointers of the other objects.
	for (int ilv1=TELESCOPE_tempvar.objectpos+1;ilv1<(*imultilist).filllevel;ilv1++)
	{
		(*((basic_instance_propertybuffer*)(((char*)((*imultilist).pointer))+(TELESCOPE_tempvar.objectsize*ilv1)))).pos_in_buffer+=ideltaplus2;
	}
	(*ibuffer).count+=ideltaplus2;
	return 1;
}
int TELESCOPE_aggressobject(basicmultilist * imultilist,int objectpos)
{
	TELESCOPE_tempvar.multilist=imultilist;
	TELESCOPE_tempvar.inside_TELESCOPE=0;
	TELESCOPE_tempvar.inside_TELESCOPE_element=0;
	TELESCOPE_tempvar.objectsize=(*imultilist).itemsize;
	getbufferfromstructure(imultilist,&TELESCOPE_tempvar.buffer);
	TELESCOPE_tempvar.objectpos=objectpos;
	TELESCOPE_tempvar.subpos=0;
	TELESCOPE_tempvar.subpos2=0;
	TELESCOPE_tempvar.pos=(*((basic_instance_propertybuffer*)(((char*)((*imultilist).pointer))+(TELESCOPE_tempvar.objectsize*TELESCOPE_tempvar.objectpos)))).pos_in_buffer;
	return TELESCOPE_verify_objectpresent();
}
int TELESCOPE_searchthroughobject_multi(_u32 tag)
{
	int ilength;
	if (TELESCOPE_verify_objectpresent())
	{
		TELESCOPE_tempvar.inside_TELESCOPE=1;
		ilength=(*((TELESCOPE*)((*TELESCOPE_tempvar.buffer).buffer+TELESCOPE_tempvar.pos))).length-sizeof(TELESCOPE);
		TELESCOPE_tempvar.subpos=sizeof(TELESCOPE);
		while (TELESCOPE_tempvar.subpos<ilength)
		{
			TELESCOPE_element * iTELESCOPE_element=(TELESCOPE_element*)((*(TELESCOPE_tempvar.buffer)).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos);
			if (tag & (1<<(*iTELESCOPE_element).type)) {TELESCOPE_tempvar.inside_TELESCOPE_element=1;TELESCOPE_tempvar.subpos2=TELESCOPE_tempvar.subpos2=0;return 1;};
			TELESCOPE_tempvar.subpos+=(*iTELESCOPE_element).length;
			TELESCOPE_tempvar.subpos2=TELESCOPE_tempvar.subpos;
		}
		return 0;
	}
	TELESCOPE_tempvar.subpos=0;
	TELESCOPE_tempvar.subpos2=0;
	return 0;
}
int TELESCOPE_buffercheck(basicmultilist * imultilist)
{
	int ilength;
	int backval;
	TELESCOPE_debugvar.multilist=imultilist;
	TELESCOPE_debugvar.inside_TELESCOPE=0;
	TELESCOPE_debugvar.inside_TELESCOPE_element=0;
	TELESCOPE_debugvar.objectsize=(*imultilist).itemsize;
	getbufferfromstructure(imultilist,&TELESCOPE_debugvar.buffer);
	TELESCOPE_debugvar.pos=0;
	iback:
	TELESCOPE_debugvar.subpos=0;
	TELESCOPE_debugvar.subpos2=0;
	backval=TELESCOPE_verify_objectcorrect();
	if (backval<0) return backval;
	if (backval)
	{
		TELESCOPE_debugvar.inside_TELESCOPE=1;
		ilength=(*((TELESCOPE*)((*TELESCOPE_debugvar.buffer).buffer+TELESCOPE_debugvar.pos))).length;
		TELESCOPE_debugvar.subpos=sizeof(TELESCOPE);
		if (ilength<=sizeof(TELESCOPE))
		{
			fprintf(stderr,"TELESCOPE stupid length%i\n",ilength);exit(1);
		}
		if ((TELESCOPE_debugvar.pos+ilength)>LHENDRAW_buffersize)
		{
			fprintf(stderr,"TELESCOPE buffer seems overflown%i\n",ilength);exit(1);
			return -21;
		}
	}
	if (backval & 1)
	{
		TELESCOPE_element * iTELESCOPE_element;
		while (TELESCOPE_debugvar.subpos<ilength)
		{
			iTELESCOPE_element=(TELESCOPE_element*)((*(TELESCOPE_debugvar.buffer)).buffer+TELESCOPE_debugvar.pos+TELESCOPE_debugvar.subpos);
			if (((*iTELESCOPE_element).type<0) || ((*iTELESCOPE_element).type>=TELESCOPE_ELEMENTTYPE_ListSize))
			{
				fprintf(stderr,"TELESCOPE bad element type%i\n",(*iTELESCOPE_element).type);exit(1);
			}
			fprintf(stderr,"%s",((*(TELESCOPE_debugvar.buffer)).buffer+TELESCOPE_debugvar.pos+TELESCOPE_debugvar.subpos)+TELESCOPE_ELEMENTTYPE_List[(*iTELESCOPE_element).type].size);
			fprintf(stderr," %i",(int)((*iTELESCOPE_element).length-TELESCOPE_ELEMENTTYPE_List[(*iTELESCOPE_element).type].size));
			TELESCOPE_debugvar.subpos+=(*iTELESCOPE_element).length;
			TELESCOPE_debugvar.subpos2=TELESCOPE_debugvar.subpos;
		}
		if (TELESCOPE_debugvar.subpos>ilength)
		{
			fprintf(stderr,"TELESCOPE object contents overflow%i,%i>%i\n",(*iTELESCOPE_element).length,TELESCOPE_debugvar.subpos,ilength);exit(1);
		}
	}
	TELESCOPE_debugvar.subpos=0;
	TELESCOPE_debugvar.subpos2=0;
	if (backval)
	{
		TELESCOPE_debugvar.pos+=ilength;
		goto iback;
	}
	return 0;
}
int TELESCOPE_searchthroughobject_next_multi(_u32 tag)
{
	int ilength;
	int oldsubpos=TELESCOPE_tempvar.subpos;
	if (TELESCOPE_tempvar.inside_TELESCOPE)
	{
		if (TELESCOPE_tempvar.inside_TELESCOPE_element)
		{
			ilength=(*((TELESCOPE*)((*TELESCOPE_tempvar.buffer).buffer+TELESCOPE_tempvar.pos))).length;
			if (TELESCOPE_tempvar.subpos>=ilength) {return 0;}
			TELESCOPE_element * iTELESCOPE_element=(TELESCOPE_element*)((*(TELESCOPE_tempvar.buffer)).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos);
			TELESCOPE_tempvar.subpos+=(*iTELESCOPE_element).length;
			TELESCOPE_tempvar.subpos2=TELESCOPE_tempvar.subpos;
			while (TELESCOPE_tempvar.subpos<ilength)
			{
				TELESCOPE_element * iTELESCOPE_element=(TELESCOPE_element*)((*(TELESCOPE_tempvar.buffer)).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos);
				if (tag & (1<<(*iTELESCOPE_element).type)) {TELESCOPE_tempvar.inside_TELESCOPE_element=1;TELESCOPE_tempvar.subpos2=TELESCOPE_tempvar.subpos;return 1;};
				TELESCOPE_tempvar.subpos+=(*iTELESCOPE_element).length;
				TELESCOPE_tempvar.subpos2=TELESCOPE_tempvar.subpos;
			}
			TELESCOPE_tempvar.subpos=oldsubpos;
			TELESCOPE_tempvar.subpos2=TELESCOPE_tempvar.subpos;
			return 0;
		}
	}
	TELESCOPE_tempvar.subpos=0;
	TELESCOPE_tempvar.subpos2=0;
	return 0;
}
int TELESCOPE_searchthroughobject(int tag)
{
	return TELESCOPE_searchthroughobject_multi(1<<tag);
}
int TELESCOPE_searchthroughobject_next(int tag)
{
	return TELESCOPE_searchthroughobject_next_multi(1<<tag);
}
void TELESCOPE_rushtoend()
{
	TELESCOPE_element * iTELESCOPE_element=(TELESCOPE_element*)((*(TELESCOPE_tempvar.buffer)).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos);
	TELESCOPE_tempvar.subpos2=TELESCOPE_tempvar.subpos+(*iTELESCOPE_element).length;
}
#define TELESCOPE_item ((TELESCOPE_element*)((*(TELESCOPE_tempvar.buffer)).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos))
#define TELESCOPE_item_1(PARAM) ((PARAM ## _instance*)((*(TELESCOPE_tempvar.buffer)).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos))
int TELESCOPE_insertintoproperties(int tag,const char * iinput,int ilength)//TODO: memory overflow handling
{
	if (TELESCOPE_searchthroughobject(tag))
	{
		while (TELESCOPE_searchthroughobject_next(tag))
		{
		}
		TELESCOPE_rushtoend();
	}
	if (TELESCOPE_stretch_buffer(TELESCOPE_tempvar.multilist,ilength,tag)==-1) {return -1;}
	char * ilv1b=(*TELESCOPE_tempvar.buffer).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos2/*TODO: what if an object HAD to be inserted?*/;
	//TODO: what if an object HAD to be inserted???
	for (int ilv1=0;ilv1<ilength;ilv1++,ilv1b++)//TODO: faster...
	{
		(*ilv1b)=(*(iinput+ilv1));
	}
	return 1;
}
int TELESCOPE_insertintoproperties_offset(const char * iinput,int ilength,int ioffset)//TODO: memory overflow handling
{
	TELESCOPE_element * tl_element=((TELESCOPE_element*)((*(TELESCOPE_tempvar.buffer)).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos));
	int tl_elementlength=tl_element->length-TELESCOPE_ELEMENTTYPE_List[tl_element->type].size;
	TELESCOPE_rushtoend();
	if (TELESCOPE_stretch_buffer(TELESCOPE_tempvar.multilist,ilength,tl_element->type)==-1) {return -1;}
	if (abs(ioffset)>tl_elementlength) return -1;
	if (ioffset>=0)
	{
		ioffset-=tl_elementlength;
	}
	char * ilv1b=(*TELESCOPE_tempvar.buffer).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos2+ilength-1;
	for (int ilv1=0;ilv1>ioffset;ilv1--,ilv1b--)
	{
		(*ilv1b)=(*(ilv1b-ilength));
	}
	ilv1b=(*TELESCOPE_tempvar.buffer).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos2+ioffset;
	for (int ilv1=0;ilv1<ilength;ilv1++,ilv1b++)//TODO: faster...
	{
		(*ilv1b)=(*(iinput+ilv1));
	}
	return 1;
}
int TELESCOPE_add(int tag,const char * iinput,int ilength)//Like insertintoproperties, but unconditionally creates a NEW TELESCOPE_element
{
	//This NEW TELESCOPE_element, or derived structure, must be obtained with TELESCOPE_getproperty and written later.
	TELESCOPE_tempvar.inside_TELESCOPE=TELESCOPE_verify_objectpresent();//TODO: such calls in aggressobject and nowhere else
	if (TELESCOPE_tempvar.inside_TELESCOPE)
	{
		TELESCOPE_tempvar.subpos=(*((TELESCOPE*)((*TELESCOPE_tempvar.buffer).buffer+TELESCOPE_tempvar.pos))).length;
		TELESCOPE_tempvar.subpos2=TELESCOPE_tempvar.subpos;
		TELESCOPE_tempvar.inside_TELESCOPE_element=0;
	}
	if (TELESCOPE_stretch_buffer(TELESCOPE_tempvar.multilist,ilength,tag)==-1) {return -1;}
	char * ilv1b=(*TELESCOPE_tempvar.buffer).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos2;
	for (int ilv1=0;ilv1<ilength;ilv1++,ilv1b++)//TODO: faster...
	{
		(*ilv1b)=(*(iinput+ilv1));
	}
	return 1;
}
int TELESCOPE_split(int ipos,const char * iadditive_input,int ilength)
{
	//should point to the second Element afterwards
	int ilv1;
	TELESCOPE_element * tl_Element=((TELESCOPE_element*)((*TELESCOPE_tempvar.buffer).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos));
	int itype=(*tl_Element).type;
	TELESCOPE_element * tl_SecondElement=(TELESCOPE_element*)(((char*)tl_Element)+TELESCOPE_ELEMENTTYPE_List[itype].size+ipos+ilength);
	TELESCOPE_rushtoend();
	int ideltaplus2=ilength+TELESCOPE_ELEMENTTYPE_List[itype].size;
	int isecondlength=(*tl_Element).length-ipos-TELESCOPE_ELEMENTTYPE_List[itype].size;
	if ((*(TELESCOPE_tempvar.buffer)).count+ideltaplus2>=LHENDRAW_buffersize) return -1;
	for (ilv1=(*(TELESCOPE_tempvar.buffer)).count+ideltaplus2-1;ilv1>=TELESCOPE_tempvar.subpos+TELESCOPE_tempvar.pos+TELESCOPE_ELEMENTTYPE_List[itype].size+(*tl_Element).length-isecondlength;ilv1--)//stretches buffer and copies end
	{
		(*(TELESCOPE_tempvar.buffer)).buffer[ilv1]=(*(TELESCOPE_tempvar.buffer)).buffer[ilv1-ideltaplus2];
	}
	for (;ilv1>=TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos+ipos+ilength+TELESCOPE_ELEMENTTYPE_List[itype].size;ilv1--)//copies the structure
	{
		(*(TELESCOPE_tempvar.buffer)).buffer[ilv1]=(*(TELESCOPE_tempvar.buffer)).buffer[ilv1-ideltaplus2-ipos];
	}
	(*tl_SecondElement).length=isecondlength+TELESCOPE_ELEMENTTYPE_List[itype].size;
	char * ilv1b=(*TELESCOPE_tempvar.buffer).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos+TELESCOPE_ELEMENTTYPE_List[itype].size+ipos;
	for (int ilv1=0;ilv1<ilength;ilv1++,ilv1b++)//Copies additive input
	{
		(*ilv1b)=iadditive_input[ilv1];
	}
	for (int ilv1=TELESCOPE_tempvar.objectpos+1;ilv1<(*TELESCOPE_tempvar.multilist).filllevel;ilv1++)
	{
		(*((basic_instance_propertybuffer*)(((char*)((*TELESCOPE_tempvar.multilist).pointer))+(TELESCOPE_tempvar.objectsize*ilv1)))).pos_in_buffer+=ideltaplus2;
	}
	(*(TELESCOPE_tempvar.buffer)).count+=ideltaplus2;
	(*tl_Element).length=ipos+ilength+TELESCOPE_ELEMENTTYPE_List[itype].size;
	if ((*tl_Element).length==0)
	{
		fprintf(stderr,"\\1");
	}
	if ((*tl_SecondElement).length==0)
	{
		fprintf(stderr,"\\2");
	}
	TELESCOPE_tempvar.subpos+=(*tl_Element).length;
	TELESCOPE_tempvar.subpos2=TELESCOPE_tempvar.subpos+(*tl_SecondElement).length;
	if ((*((TELESCOPE_element*)(((char*)tl_Element)+(*tl_Element).length))).length==0)
	{
		fprintf(stderr,"\\3");
	}
	(*(TELESCOPE*)((*TELESCOPE_tempvar.buffer).buffer+TELESCOPE_tempvar.pos)).length+=ideltaplus2;
	return 0;
}
int TELESCOPE_merge(int ipos,int ilength)
{
	char * ilv1;
	TELESCOPE_element * tl_Element=((TELESCOPE_element*)((*TELESCOPE_tempvar.buffer).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos));
	int itype=(*tl_Element).type;
	TELESCOPE_element * tl_SecondElement=(TELESCOPE_element*)(((char*)tl_Element)+(*tl_Element).length);
	int ideltaminus2=ilength+TELESCOPE_ELEMENTTYPE_List[itype].size;
	int isecondlength=(*tl_SecondElement).length;
	for (ilv1=((char*)tl_Element)+TELESCOPE_ELEMENTTYPE_List[itype].size+ipos;ilv1<(*TELESCOPE_tempvar.buffer).buffer+(*TELESCOPE_tempvar.buffer).count-ideltaminus2;ilv1++)//compresses buffer and concatenates end
	{
		*ilv1=*(ilv1+ideltaminus2);
	}
	(*tl_SecondElement).length=isecondlength+TELESCOPE_ELEMENTTYPE_List[itype].size;
	for (int ilv1=TELESCOPE_tempvar.objectpos+1;ilv1<(*TELESCOPE_tempvar.multilist).filllevel;ilv1++)
	{
		(*((basic_instance_propertybuffer*)(((char*)((*TELESCOPE_tempvar.multilist).pointer))+(TELESCOPE_tempvar.objectsize*ilv1)))).pos_in_buffer-=ideltaminus2;
	}
	(*(TELESCOPE_tempvar.buffer)).count-=ideltaminus2;
	(*tl_Element).length+=isecondlength-ilength;
	TELESCOPE_tempvar.subpos2=TELESCOPE_tempvar.subpos+(*tl_Element).length;
	(*(TELESCOPE*)((*TELESCOPE_tempvar.buffer).buffer+TELESCOPE_tempvar.pos)).length-=ideltaminus2;
	return 0;
}
int TELESCOPE_shrink(int ipos,int ilength)
{
	char * ilv1;
	TELESCOPE_element * tl_Element=((TELESCOPE_element*)((*TELESCOPE_tempvar.buffer).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos));
	int itype=(*tl_Element).type;
	int tl_length=(*tl_Element).length-TELESCOPE_ELEMENTTYPE_List[itype].size;
	if (abs(ipos)>tl_length) return -1;
	if (ipos<0) ipos+=tl_length;//TODO: convert to offset-type for better understandability
	if (ilength+ipos>tl_length) return -1;
	int ideltaminus2=ilength;
	for (ilv1=((char*)tl_Element)+TELESCOPE_ELEMENTTYPE_List[itype].size+ipos;ilv1<(*TELESCOPE_tempvar.buffer).buffer+(*TELESCOPE_tempvar.buffer).count-ideltaminus2;ilv1++)//compresses buffer
	{
		*ilv1=*(ilv1+ideltaminus2);
	}
	for (int ilv1=TELESCOPE_tempvar.objectpos+1;ilv1<(*TELESCOPE_tempvar.multilist).filllevel;ilv1++)
	{
		(*((basic_instance_propertybuffer*)(((char*)((*TELESCOPE_tempvar.multilist).pointer))+(TELESCOPE_tempvar.objectsize*ilv1)))).pos_in_buffer-=ideltaminus2;
	}
	(*(TELESCOPE_tempvar.buffer)).count-=ideltaminus2;
	(*tl_Element).length-=ilength;
	TELESCOPE_tempvar.subpos2=TELESCOPE_tempvar.subpos+(*tl_Element).length;
	(*(TELESCOPE*)((*TELESCOPE_tempvar.buffer).buffer+TELESCOPE_tempvar.pos)).length-=ideltaminus2;
	return 0;
}

int TELESCOPE_clear()//clears all elements of that object
{
	if (TELESCOPE_verify_objectpresent())
	{
		(*((TELESCOPE*)(((*(TELESCOPE_tempvar.buffer)).buffer)+TELESCOPE_tempvar.pos))).owner=-1;
	}
	return 1;
}
int TELESCOPE_clear_item()//clears one item. leaves the cursor system at its position, so the next item is automatically aimed at.
{
	int ilength;
	ilength=(*((TELESCOPE_element*)(((*(TELESCOPE_tempvar.buffer)).buffer)+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos))).length;
	for (int ilv1=TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos;ilv1<(*(TELESCOPE_tempvar.buffer)).count-ilength;ilv1++)
	{
		(*(TELESCOPE_tempvar.buffer)).buffer[ilv1]=(*(TELESCOPE_tempvar.buffer)).buffer[ilv1+ilength];
	}
	(*((TELESCOPE*)(((*(TELESCOPE_tempvar.buffer)).buffer)+TELESCOPE_tempvar.pos))).length-=ilength;
	for (int ilv1=TELESCOPE_tempvar.objectpos+1;ilv1<(*(TELESCOPE_tempvar.multilist)).filllevel;ilv1++)
	{
		(*((basic_instance_propertybuffer*)(((char*)((*(TELESCOPE_tempvar.multilist)).pointer))+(TELESCOPE_tempvar.objectsize*ilv1)))).pos_in_buffer-=ilength;
	}
	(*(TELESCOPE_tempvar.buffer)).count-=ilength;
	return 1;
}
void * TELESCOPE_getproperty()//returns the pointer to the current content, and it should be named TELESCOPE_getcontent
{
	return (void*)((*TELESCOPE_tempvar.buffer).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos);
}
void * TELESCOPE_getproperty_contents()//Like before, but returns the pointer to the end of the contained object, giving the buffer it holds.
{
	return (void*)((*TELESCOPE_tempvar.buffer).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos+TELESCOPE_ELEMENTTYPE_List[(*(TELESCOPE_element*)((*(TELESCOPE_tempvar.buffer)).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos)).type].size);
}
int TELESCOPE_getproperty_contentlength()//Like before, but returns the pointer to the end of the contained object, giving the buffer it holds.
{
	return ((TELESCOPE_element*)((*TELESCOPE_tempvar.buffer).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos))->length-TELESCOPE_ELEMENTTYPE_List[(*(TELESCOPE_element*)((*(TELESCOPE_tempvar.buffer)).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos)).type].size;
}
int initmemory()
{
	memory_alloc(&(filestructure_text_buffer.buffer),1);
	filestructure_text_buffer.count=0;
	memory_alloc(&(filestructure_curve_buffer.buffer),1);
	filestructure_curve_buffer.count=0;
	if ((filestructure_text_buffer.buffer!=NULL) && (filestructure_curve_buffer.buffer!=NULL)) return 1;
	return -1;
}
