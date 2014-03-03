TELESCOPE_buffer filestructure_text_buffer;
TELESCOPE_buffer filestructure_curve_buffer;
TELESCOPE_buffer internalstructure_text_buffer;
struct TELESCOPE_tempvar_
{
	int pos;//object inside buffer
	int objectsize;
	int subpos;//TELESCOPE_element inside object
	int subpos2;//write cursor inside object
	int objectindex;
	int objectpos;
	TELESCOPE_buffer * buffer;
	char inside_TELESCOPE;
	char inside_TELESCOPE_element;
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
		return ((*((TELESCOPE*)(((*(TELESCOPE_tempvar.buffer)).buffer)+TELESCOPE_tempvar.pos))).owner==TELESCOPE_tempvar.objectindex);
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
		(*((TELESCOPE*)((*ibuffer).buffer+TELESCOPE_tempvar.pos))).owner=TELESCOPE_tempvar.objectindex;
		(*((TELESCOPE*)((*ibuffer).buffer+TELESCOPE_tempvar.pos))).length=ideltaplus2;
		TELESCOPE_tempvar.subpos+=sizeof(TELESCOPE);
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
void agressobject(basicmultilist * imultilist,int objectindex,int objectpos)
{
	TELESCOPE_tempvar.inside_TELESCOPE=0;
	TELESCOPE_tempvar.inside_TELESCOPE_element=0;
	TELESCOPE_tempvar.objectsize=(*imultilist).itemsize;
	getbufferfromstructure(imultilist,&TELESCOPE_tempvar.buffer);
	TELESCOPE_tempvar.objectpos=objectpos;
	TELESCOPE_tempvar.objectindex=objectindex;
	TELESCOPE_tempvar.pos=(*((basic_instance_propertybuffer*)(((char*)((*imultilist).pointer))+(TELESCOPE_tempvar.objectsize*TELESCOPE_tempvar.objectpos)))).pos_in_buffer;
}
int searchthroughobject(int tag)
{
	int ilength;
	if (TELESCOPE_verify_objectpresent())
	{
		TELESCOPE_tempvar.inside_TELESCOPE=1;
		ilength=(*((TELESCOPE*)((*TELESCOPE_tempvar.buffer).buffer+TELESCOPE_tempvar.pos))).length;
		TELESCOPE_tempvar.subpos=sizeof(TELESCOPE_head);
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
int searchthroughobject_next(int tag)
{
	int ilength;
	int oldsubpos=TELESCOPE_tempvar.subpos;
	if (TELESCOPE_tempvar.inside_TELESCOPE)
	{
		if (TELESCOPE_tempvar.inside_TELESCOPE_element)
		{
			ilength=(*((TELESCOPE*)((*TELESCOPE_tempvar.buffer).buffer+TELESCOPE_tempvar.pos))).length;
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
int TELESCOPE_insertintoproperties(basicmultilist * imultilist,int objectindex,int objectpos,int tag,char * iinput,int ilength)
{
	agressobject(imultilist,objectindex,objectpos);
	if (searchthroughobject(tag))
	{
		TELESCOPE_rushtoend();
	}
	TELESCOPE_stretch_buffer(imultilist,ilength,tag);
	char * ilv1b=(*TELESCOPE_tempvar.buffer).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos2;
	for (int ilv1=0;ilv1<ilength;ilv1++,ilv1b++)//TODO: faster...
	{
		(*ilv1b)=(*iinput);
	}
}
int initmemory()
{
	filestructure_text_buffer.buffer=(char*)malloc(1000000);
	filestructure_text_buffer.max=1000000;
	filestructure_text_buffer.count=0;
	filestructure_curve_buffer.buffer=(char*)malloc(1000000);
	filestructure_curve_buffer.max=1000000;
	filestructure_curve_buffer.count=0;
}
