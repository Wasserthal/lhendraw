char *glob_n_undo_buffer;
char *glob_n_undo_contentbuffer;
char *glob_arrow_undo_buffer;
char *glob_arrow_undo_contentbuffer;
char *glob_b_undo_buffer;
char *glob_b_undo_contentbuffer;
char *glob_graphic_undo_buffer;
char *glob_graphic_undo_contentbuffer;
char *glob_text_undo_buffer;
char *glob_text_undo_contentbuffer;
TELESCOPE_buffer filestructure_text_buffer;
TELESCOPE_buffer filestructure_curve_buffer;
TELESCOPE_buffer internalstructure_text_buffer;
TELESCOPE_buffer internalstructure_b_buffer;
TELESCOPE_buffer internalstructure_n_buffer;
TELESCOPE_buffer internalstructure_arrow_buffer;
TELESCOPE_buffer internalstructure_text_undobuffer;
TELESCOPE_buffer internalstructure_b_undobuffer;
TELESCOPE_buffer internalstructure_n_undobuffer;
TELESCOPE_buffer internalstructure_arrow_undobuffer;
#define selection_maxbuttons 43
#define constants_bondlength 30
#define constants_angular_distance 2.094395102
#define constants_clickradius 170
#define constants_Element_loneradical 0
#define constants_Element_lonepair 1
#define constants_Element_implicitcarbon 9
float glob_clickradius=170;
#define constants_maxbonds 10
_u32 resources_bitmap_buttons[selection_maxbuttons][32][32];
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
TELESCOPE_tempvar_ TELESCOPE_tempvar;
void TELESCOPE_measure(int tag)
{
	TELESCOPE * start;
	TELESCOPE_element * tl_telescope_element;
	int total=0;
	int share=0;
	printf("start\n");
	iback:
	start=(TELESCOPE*)(((char*)((internalstructure_text_buffer).buffer))+total);
	printf("le:%iow:%i\n",start->length,start->owner);
/*	for (int ilv1=0;ilv1<20;ilv1++)
	{
		printf("%02hhX ",*(((char*)((internalstructure_text_buffer).buffer))+ilv1));
	}
	printf("\n");*/
	share=0;
	iback2:
	tl_telescope_element=(TELESCOPE_element*)(((char*)((internalstructure_text_buffer).buffer))+total+sizeof(TELESCOPE)+share);
	printf("  le:%ity:%i:%s\n",tl_telescope_element->length,tl_telescope_element->type,((char*)((internalstructure_text_buffer).buffer))+total+sizeof(TELESCOPE)+share+TELESCOPE_ELEMENTTYPE_List[tag].size);
	if (tl_telescope_element->length==0)
	{
		goto idontgoback;
	}
	share+=tl_telescope_element->length;
	if (share<start->length-sizeof(TELESCOPE))
	{
		goto iback2;
	}
	idontgoback:
	total+=start->length;
	if (total<internalstructure_text_buffer.count)
	{
		goto iback;
	}
	return;
}
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
	if (input==glob_t_multilist)
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
	}
	return 0;
}
int enumerate_contents()
{
}
char * next_content()
{
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
int TELESCOPE_stretch_buffer(basicmultilist * imultilist,int ideltaplus,int itype)
{
	int ideltaplus2;char enlengthen=0;char enlengthen2=0;
	TELESCOPE_buffer * ibuffer=TELESCOPE_tempvar.buffer;
	ideltaplus2=ideltaplus;
	if (TELESCOPE_verify_objectpresent()) goto wehadit;
	ideltaplus2+=sizeof(TELESCOPE);
	enlengthen=1;
	wehadit:
	if (!(TELESCOPE_tempvar.inside_TELESCOPE_element))
	{
		ideltaplus2+=TELESCOPE_ELEMENTTYPE_List[itype].size;
		enlengthen2=1;
	}
	if ((*ibuffer).count+ideltaplus2>(*ibuffer).max)
	{
		return -1;
	}
	for (int ilv1=(*ibuffer).count+ideltaplus2-1;ilv1>TELESCOPE_tempvar.pos+ideltaplus2+TELESCOPE_tempvar.subpos2;ilv1--)//TODO: FASTER! buffercopy_reverse((*ibuffer).buffer+(*ibuffer).count,ipos,(*ibuffer).buffer+(*ibuffer).count+ideltaplus2,ipos+ideltaplus2);
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
void TELESCOPE_aggressobject(basicmultilist * imultilist,int objectpos)
{
	TELESCOPE_tempvar.multilist=imultilist;
	TELESCOPE_tempvar.inside_TELESCOPE=0;
	TELESCOPE_tempvar.inside_TELESCOPE_element=0;
	TELESCOPE_tempvar.objectsize=(*imultilist).itemsize;
	getbufferfromstructure(imultilist,&TELESCOPE_tempvar.buffer);
	TELESCOPE_tempvar.objectpos=objectpos;
	TELESCOPE_tempvar.pos=(*((basic_instance_propertybuffer*)(((char*)((*imultilist).pointer))+(TELESCOPE_tempvar.objectsize*TELESCOPE_tempvar.objectpos)))).pos_in_buffer;
}
int TELESCOPE_searchthroughobject(int tag)
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
			if (tag==(*iTELESCOPE_element).type) {TELESCOPE_tempvar.inside_TELESCOPE_element=1;TELESCOPE_tempvar.subpos2=TELESCOPE_tempvar.subpos2=0;return 1;};
			TELESCOPE_tempvar.subpos+=(*iTELESCOPE_element).length;
			TELESCOPE_tempvar.subpos2=TELESCOPE_tempvar.subpos;
		}
		return 0;
	}
	TELESCOPE_tempvar.subpos=0;
	TELESCOPE_tempvar.subpos2=0;
	return 0;
}
int TELESCOPE_searchthroughobject_next(int tag)
{
	int ilength;
	int oldsubpos=TELESCOPE_tempvar.subpos;
	if (TELESCOPE_tempvar.inside_TELESCOPE)
	{
		if (TELESCOPE_tempvar.inside_TELESCOPE_element)
		{
			ilength=(*((TELESCOPE*)((*TELESCOPE_tempvar.buffer).buffer+TELESCOPE_tempvar.pos))).length-sizeof(TELESCOPE);
			TELESCOPE_element * iTELESCOPE_element=(TELESCOPE_element*)((*(TELESCOPE_tempvar.buffer)).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos);
			TELESCOPE_tempvar.subpos+=(*iTELESCOPE_element).length;
			TELESCOPE_tempvar.subpos2=TELESCOPE_tempvar.subpos;
			while (TELESCOPE_tempvar.subpos<ilength)
			{
				TELESCOPE_element * iTELESCOPE_element=(TELESCOPE_element*)((*(TELESCOPE_tempvar.buffer)).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos);
				if (tag==(*iTELESCOPE_element).type) {TELESCOPE_tempvar.inside_TELESCOPE_element=1;TELESCOPE_tempvar.subpos2==TELESCOPE_tempvar.subpos;return 1;};
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
void TELESCOPE_rushtoend()
{
	TELESCOPE_element * iTELESCOPE_element=(TELESCOPE_element*)((*(TELESCOPE_tempvar.buffer)).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos);
	TELESCOPE_tempvar.subpos2=TELESCOPE_tempvar.subpos+(*iTELESCOPE_element).length;
}
#define TELESCOPE_item ((TELESCOPE_element*)((*(TELESCOPE_tempvar.buffer)).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos))
#define TELESCOPE_item_1(PARAM) ((PARAM ## _instance*)((*(TELESCOPE_tempvar.buffer)).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos))
int TELESCOPE_insertintoproperties(int tag,char * iinput,int ilength)//TODO: memory overflow handling
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
int TELESCOPE_add(int tag,char * iinput,int ilength)//Like insertintoproperties, but unconditionally creates a NEW TELESCOPE_element
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
int TELESCOPE_split(int ipos,char * iadditive_input,int ilength)
{
	int ilv1;
	TELESCOPE_element * tl_Element=((TELESCOPE_element*)((*TELESCOPE_tempvar.buffer).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos));
	int itype=(*tl_Element).type;
	TELESCOPE_element * tl_SecondElement=(TELESCOPE_element*)(((char*)tl_Element)+TELESCOPE_ELEMENTTYPE_List[itype].size+ipos+ilength);
	TELESCOPE_rushtoend();
	int ideltaplus2=ilength+TELESCOPE_ELEMENTTYPE_List[itype].size;
	int isecondlength=(*tl_Element).length-ipos-TELESCOPE_ELEMENTTYPE_List[itype].size;
	if ((*(TELESCOPE_tempvar.buffer)).count+ideltaplus2>=(*(TELESCOPE_tempvar.buffer)).max) return -1;
	for (ilv1=(*(TELESCOPE_tempvar.buffer)).count+ideltaplus2-1;ilv1>=TELESCOPE_tempvar.subpos+TELESCOPE_tempvar.pos+TELESCOPE_ELEMENTTYPE_List[itype].size+(*tl_Element).length-isecondlength;ilv1--)//stretches buffer and copies end
	{
		(*(TELESCOPE_tempvar.buffer)).buffer[ilv1]=(*(TELESCOPE_tempvar.buffer)).buffer[ilv1-ideltaplus2];
	}
	for (;ilv1>=TELESCOPE_tempvar.subpos+TELESCOPE_tempvar.pos+ipos+ilength+TELESCOPE_ELEMENTTYPE_List[itype].size;ilv1--)//copies the structure
	{
		(*(TELESCOPE_tempvar.buffer)).buffer[ilv1]=(*(TELESCOPE_tempvar.buffer)).buffer[ilv1-ideltaplus2-ipos];
	}
	(*tl_SecondElement).length=isecondlength+TELESCOPE_ELEMENTTYPE_List[itype].size;
	char * ilv1b=(*TELESCOPE_tempvar.buffer).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos2;
	for (int ilv1=0;ilv1<ilength;ilv1++)//Copies additive input
	{
		(*ilv1b)=iadditive_input[ilv1];
	}
	for (int ilv1=TELESCOPE_tempvar.objectpos+1;ilv1<(*TELESCOPE_tempvar.multilist).filllevel;ilv1++)
	{
		(*((basic_instance_propertybuffer*)(((char*)((*TELESCOPE_tempvar.multilist).pointer))+(TELESCOPE_tempvar.objectsize*ilv1)))).pos_in_buffer+=ideltaplus2;
	}
	(*(TELESCOPE_tempvar.buffer)).count+=ideltaplus2;
	(*tl_Element).length=ipos+ilength+TELESCOPE_ELEMENTTYPE_List[itype].size;
	(*(TELESCOPE*)((*TELESCOPE_tempvar.buffer).buffer+TELESCOPE_tempvar.pos)).length+=ideltaplus2;
	return 0;
}
int TELESCOPE_clear()
{
	if (TELESCOPE_verify_objectpresent())
	{
		(*((TELESCOPE*)(((*(TELESCOPE_tempvar.buffer)).buffer)+TELESCOPE_tempvar.pos))).owner=-1;
	}
}
void * TELESCOPE_getproperty()
{
	return (void*)((*TELESCOPE_tempvar.buffer).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos);
}
void * TELESCOPE_getproperty_contents()
{
	return (void*)((*TELESCOPE_tempvar.buffer).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos+TELESCOPE_ELEMENTTYPE_List[(*(TELESCOPE_element*)((*(TELESCOPE_tempvar.buffer)).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos)).type].size);
}
int initmemory()
{
	filestructure_text_buffer.buffer=(char*)malloc(1000000);
	filestructure_text_buffer.max=1000000;
	filestructure_text_buffer.count=0;
	filestructure_curve_buffer.buffer=(char*)malloc(1000000);
	filestructure_curve_buffer.max=1000000;
	filestructure_curve_buffer.count=0;
	internalstructure_text_buffer.buffer=(char*)malloc(1000000);
	internalstructure_text_buffer.max=1000000;
	internalstructure_text_buffer.count=0;
	internalstructure_n_buffer.buffer=(char*)malloc(1000000);
	internalstructure_n_buffer.max=1000000;
	internalstructure_n_buffer.count=0;
	internalstructure_arrow_buffer.buffer=(char*)malloc(1000000);
	internalstructure_arrow_buffer.max=1000000;
	internalstructure_arrow_buffer.count=0;
	internalstructure_b_buffer.buffer=(char*)malloc(1000000);
	internalstructure_b_buffer.max=1000000;
	internalstructure_b_buffer.count=0;
	glob_n_undo_buffer=(char*)malloc(sizeof(multilist<n_instance>)+sizeof(n_instance)*bufferlistsize);
	glob_arrow_undo_buffer=(char*)malloc(sizeof(multilist<arrow_instance>)+sizeof(arrow_instance)*bufferlistsize);
	glob_b_undo_buffer=(char*)malloc(sizeof(multilist<b_instance>)+sizeof(b_instance)*bufferlistsize);
	glob_graphic_undo_buffer=(char*)malloc(sizeof(multilist<n_instance>)+sizeof(n_instance)*bufferlistsize);
	glob_text_undo_contentbuffer=(char*)malloc(1000000);
	glob_b_undo_contentbuffer=(char*)malloc(1000000);
	glob_n_undo_contentbuffer=(char*)malloc(1000000);
	glob_arrow_undo_contentbuffer=(char*)malloc(1000000);
}
