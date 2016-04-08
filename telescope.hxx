//This is the home of the bufferlist programming
//The bufferlists are arrays of TELESCOPE called elements, one for each object in the corresponding multilist, IN THE SAME ORDER.
//There is a reference to the list position of each multilist object in each TELESCOPE,
//and there is a reference to the bufferstart-relative position of its TELESCOPE.
//Some objects have no TELESCOPE. In this case, the pointer points to the TELESCOPE of the next object that has one, or at the end of the buffer
//because that is the position where a TELESCOPE should be inserted for that object.

//All lengths are counted from the beginning of the element.
//the data of a TELESCOPE_element starts after the actual strucure.
//That data part is of total generic type.
struct superconstellation_directory
{
	int start,end;
	char name[stringlength];
};
extern superconstellation AUTOSTRUCTURE_propertylist[];
struct TELESCOPE_element
{
	int length;//starting from beginning of this structure
	int type;//see telescopeelementtype
	int getcontents(char * name)//TODO: remove this function. I also have to change the content list to a property list in linemode=1
	{
		return -1;
	};
	int getproperties(const char * name,CDXMLREAD_functype * delegateoutput,int * posoutput=NULL)
	{
		return -1;
	};
	const char * getName(){return 0;}
};
struct TELESCOPE//one for each object
{
	_i32 length; //starting from beginning of this structure
	_u32 owner;//number of the owner. ordinal numbers can be reset upon resorting, so 32 bit is enough. However, I dont WANT to resort, since I need unique ID's anyway.-1 means dummy.
};
TELESCOPE_buffer filestructure_text_buffer;
TELESCOPE_buffer filestructure_curve_buffer;
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
TELESCOPE_tempvar_ TELESCOPE_tempvar;
TELESCOPE_tempvar_ TELESCOPE_debugvar;
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
	if (tl_telescope_element->length<=0) {printf("Zero-Length Element at %i:\n",share);exit(1);return;}
	if (share<start->length-sizeof(TELESCOPE))
	{
		goto iback2;
	}
	idontgoback:
	total+=start->length;
	if (start->length<=0) {printf("Zero-Length Object at %i:\n",total);exit(1);return;}
	if (total<(*ibuffer).count)
	{
		goto iback;
	}
	return;
}
char TELESCOPE_verify_objectpresent()
{
	if (TELESCOPE_tempvar.buffer==NULL) goto inull;
	if (TELESCOPE_tempvar.pos<(*(TELESCOPE_tempvar.buffer)).count)
	{
		return ((*((TELESCOPE*)(((*(TELESCOPE_tempvar.buffer)).buffer)+TELESCOPE_tempvar.pos))).owner==TELESCOPE_tempvar.objectpos);
	}
	else
	{
		inull:;
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
			error("TELESCOPE invalid object number%i of %lli",TELESCOPE_debugvar.objectpos,(*(TELESCOPE_debugvar.multilist)).filllevel);
		}
		basic_instance_propertybuffer * i_basic_instance=((basic_instance_propertybuffer*)(((char*)((*(TELESCOPE_debugvar.multilist)).pointer))+(TELESCOPE_debugvar.objectsize*TELESCOPE_debugvar.objectpos)));
		if ((*i_basic_instance).exist==0)
		{
			error("TELESCOPE owned by dead object");
		}
		if ((*i_basic_instance).pos_in_buffer!=TELESCOPE_debugvar.pos)
		{
			error("TELESCOPE and object went asynchronous:%i,%i",(int)(*i_basic_instance).pos_in_buffer,(int)TELESCOPE_debugvar.pos);
		}
	}
	else
	{
		if (TELESCOPE_debugvar.pos>(*(TELESCOPE_debugvar.buffer)).count)
		{
			error("TELESCOPE_buffer_overflow at beginning of object");
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
void TELESCOPE_reindex(int newnumber)
{
	(*((TELESCOPE*)(((*(TELESCOPE_tempvar.buffer)).buffer)+TELESCOPE_tempvar.pos))).owner=newnumber;
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
			error("TELESCOPE stupid length%i",ilength);
		}
		if ((TELESCOPE_debugvar.pos+ilength)>LHENDRAW_buffersize)
		{
			error("TELESCOPE buffer seems overflown%i",ilength);
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
				error("TELESCOPE bad element type%i",(*iTELESCOPE_element).type);
			}
			print("%s",((*(TELESCOPE_debugvar.buffer)).buffer+TELESCOPE_debugvar.pos+TELESCOPE_debugvar.subpos)+TELESCOPE_ELEMENTTYPE_List[(*iTELESCOPE_element).type].size);
			print(" %i",(int)((*iTELESCOPE_element).length-TELESCOPE_ELEMENTTYPE_List[(*iTELESCOPE_element).type].size));
			TELESCOPE_debugvar.subpos+=(*iTELESCOPE_element).length;
			TELESCOPE_debugvar.subpos2=TELESCOPE_debugvar.subpos;
		}
		if (TELESCOPE_debugvar.subpos>ilength)
		{
			error("TELESCOPE object contents overflow%i,%i>%i",(*iTELESCOPE_element).length,TELESCOPE_debugvar.subpos,ilength);
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
	if (iinput!=NULL)
	{
		for (int ilv1=0;ilv1<ilength;ilv1++,ilv1b++)//TODO: faster...
		{
			(*ilv1b)=(*(iinput+ilv1));
		}
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
		print("\\1");
	}
	if ((*tl_SecondElement).length==0)
	{
		print("\\2");
	}
	TELESCOPE_tempvar.subpos+=(*tl_Element).length;
	TELESCOPE_tempvar.subpos2=TELESCOPE_tempvar.subpos+(*tl_SecondElement).length;
	if ((*((TELESCOPE_element*)(((char*)tl_Element)+(*tl_Element).length))).length==0)
	{
		print("\\3");
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
