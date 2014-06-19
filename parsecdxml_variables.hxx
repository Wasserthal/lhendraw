//defines and processes internal variables
//This is the home of the bufferlist programming
//The bufferlists are arrays of TELESCOPE called elements, one for each object in the corresponding multilist, IN THE SAME ORDER.
//There is a reference to the list position of each multilist object in each TELESCOPE,
//and there is a reference to the bufferstart-relative position of its TELESCOPE.
//Some objects have no TELESCOPE. In this case, the pointer points to the TELESCOPE of the next object that has one, or at the end of the buffer
//because that is the position where a TELESCOPE should be inserted for that object.
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
#define selection_maxbuttons 46
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
TELESCOPE_tempvar_ TELESCOPE_debugvar;
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
int TELESCOPE_aggressobject(basicmultilist * imultilist,int objectpos)
{
	TELESCOPE_tempvar.multilist=imultilist;
	TELESCOPE_tempvar.inside_TELESCOPE=0;
	TELESCOPE_tempvar.inside_TELESCOPE_element=0;
	TELESCOPE_tempvar.objectsize=(*imultilist).itemsize;
	getbufferfromstructure(imultilist,&TELESCOPE_tempvar.buffer);
	TELESCOPE_tempvar.objectpos=objectpos;
	TELESCOPE_tempvar.pos=(*((basic_instance_propertybuffer*)(((char*)((*imultilist).pointer))+(TELESCOPE_tempvar.objectsize*TELESCOPE_tempvar.objectpos)))).pos_in_buffer;
	return TELESCOPE_verify_objectpresent();
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
		if ((TELESCOPE_debugvar.pos+ilength)>(*(TELESCOPE_debugvar.buffer)).max)
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
				if (tag==(*iTELESCOPE_element).type) {TELESCOPE_tempvar.inside_TELESCOPE_element=1;TELESCOPE_tempvar.subpos2=TELESCOPE_tempvar.subpos;return 1;};
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
	printf("+%i;%lli---%s---\n",ideltaplus2,TELESCOPE_ELEMENTTYPE_List[itype].size,((char*)tl_Element)+TELESCOPE_ELEMENTTYPE_List[itype].size);
	return 0;
}
int telescope_merge(int ipos,int ilength)
{
	char * ilv1;
	TELESCOPE_element * tl_Element=((TELESCOPE_element*)((*TELESCOPE_tempvar.buffer).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos));
	int itype=(*tl_Element).type;
	TELESCOPE_element * tl_SecondElement=(TELESCOPE_element*)(((char*)tl_Element)+(*tl_Element).length);
	int ideltaminus2=ilength+TELESCOPE_ELEMENTTYPE_List[itype].size;
	int isecondlength=(*tl_SecondElement).length;
	for (ilv1=((char*)tl_Element)-ilength;ilv1<(*TELESCOPE_tempvar.buffer).buffer-ideltaminus2;ilv1++)//compresses buffer and concatenates end
	{
		*ilv1=*(ilv1+ideltaminus2);
	}
	(*tl_SecondElement).length=isecondlength+TELESCOPE_ELEMENTTYPE_List[itype].size;
	char * ilv1b=(*TELESCOPE_tempvar.buffer).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos+TELESCOPE_ELEMENTTYPE_List[itype].size+ipos;
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

int TELESCOPE_clear()//clears all elements of that object
{
	if (TELESCOPE_verify_objectpresent())
	{
		(*((TELESCOPE*)(((*(TELESCOPE_tempvar.buffer)).buffer)+TELESCOPE_tempvar.pos))).owner=-1;
	}
}
int TELESCOPE_clear_item()
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
}
void * TELESCOPE_getproperty()//returns the pointer to the current content, and it should be named TELESCOPE_getcontent
{
	return (void*)((*TELESCOPE_tempvar.buffer).buffer+TELESCOPE_tempvar.pos+TELESCOPE_tempvar.subpos);
}
void * TELESCOPE_getproperty_contents()//Like before, but returns the pointer to the end of the contained object, giving the buffer it holds.
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
