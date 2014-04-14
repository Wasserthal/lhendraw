char *glob_n_undo_buffer;
char *glob_n_undo_contentbuffer;
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
TELESCOPE_buffer internalstructure_text_undobuffer;
TELESCOPE_buffer internalstructure_b_undobuffer;
TELESCOPE_buffer internalstructure_n_undobuffer;
#define selection_maxbuttons 25
#define constants_bondlength 30
#define constants_angular_distance 2.094395102
#define constants_clickradius 170
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
	if (TELESCOPE_tempvar.pos<(*(TELESCOPE_tempvar.buffer)).max)
	{
		return ((*((TELESCOPE*)(((*(TELESCOPE_tempvar.buffer)).buffer)+TELESCOPE_tempvar.pos))).owner==TELESCOPE_tempvar.objectpos);
	}
	else
	{
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
	for (int ilv1=(*ibuffer).count+ideltaplus2;ilv1>TELESCOPE_tempvar.pos+ideltaplus2+TELESCOPE_tempvar.subpos2;ilv1--)//TODO: FASTER! buffercopy_reverse((*ibuffer).buffer+(*ibuffer).count,ipos,(*ibuffer).buffer+(*ibuffer).count+ideltaplus2,ipos+ideltaplus2);
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
			TELESCOPE_tempvar.subpos2=TELESCOPE_tempvar.subpos+TELESCOPE_ELEMENTTYPE_List[itype].size;
		}
		(*((TELESCOPE*)((*ibuffer).buffer+TELESCOPE_tempvar.pos))).length+=ideltaplus2;
	}
	//shifts the buffer pointers of the other objects.
	for (int ilv1=TELESCOPE_tempvar.objectpos+1;ilv1<(*imultilist).filllevel;ilv1++)
	{
		(*((basic_instance_propertybuffer*)(((char*)((*imultilist).pointer))+(TELESCOPE_tempvar.objectsize*TELESCOPE_tempvar.objectpos)))).pos_in_buffer+=ideltaplus2;
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
		ilength=(*((TELESCOPE*)((*TELESCOPE_tempvar.buffer).buffer+TELESCOPE_tempvar.pos))).length;
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
			ilength=(*((TELESCOPE*)((*TELESCOPE_tempvar.buffer).buffer+TELESCOPE_tempvar.pos))).length;
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
int TELESCOPE_insertintoproperties(basicmultilist * imultilist,int objectpos,int tag,char * iinput,int ilength)//TODO: memory overflow handling
{
	TELESCOPE_aggressobject(imultilist,objectpos);//TODO: make dependent of external call
	if (TELESCOPE_searchthroughobject(tag))
	{
		while (TELESCOPE_searchthroughobject_next)
		{
		}
		TELESCOPE_rushtoend();
	}
	if (TELESCOPE_stretch_buffer(imultilist,ilength,tag)==-1) {return -1;}
	char * ilv1b=(*TELESCOPE_tempvar.buffer).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos2;
	for (int ilv1=0;ilv1<ilength;ilv1++,ilv1b++)//TODO: faster...
	{
		(*ilv1b)=(*(iinput+ilv1));
	}
	return 1;
}
int TELESCOPE_add(int tag,char * iinput,int ilength)//Like insertintoproperties, but unconditionally creates a NEW TELESCOPE_element
{
	TELESCOPE_tempvar.inside_TELESCOPE=TELESCOPE_verify_objectpresent();//TODO: such calls in aggressobject and nowhere else
	if (TELESCOPE_tempvar.inside_TELESCOPE)
	{
		TELESCOPE_tempvar.subpos=(*((TELESCOPE_head*)((*TELESCOPE_tempvar.buffer).buffer+TELESCOPE_tempvar.pos))).length;
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
	internalstructure_b_buffer.buffer=(char*)malloc(1000000);
	internalstructure_b_buffer.max=1000000;
	internalstructure_b_buffer.count=0;
	glob_n_undo_buffer=(char*)malloc(sizeof(multilist<n_instance>)+sizeof(n_instance)*bufferlistsize);
	glob_b_undo_buffer=(char*)malloc(sizeof(multilist<b_instance>)+sizeof(b_instance)*bufferlistsize);
	glob_graphic_undo_buffer=(char*)malloc(sizeof(multilist<n_instance>)+sizeof(n_instance)*bufferlistsize);
	glob_text_undo_contentbuffer=(char*)malloc(1000000);
	glob_b_undo_contentbuffer=(char*)malloc(1000000);
	glob_n_undo_contentbuffer=(char*)malloc(1000000);
}
