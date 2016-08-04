//defines the main datastructures needed to read in xml files, but they are also used for cdx files.
//defines also all structures for automatically generated source

struct TELESCOPE_buffer
{
	char * buffer;
	intl count;
};
struct edit_formatstruct
{
	_u32 color;
	_u8 face;
}__attribute__((packed));
struct bienum
{
	char name[40];
	intl number;
};
#define REGISTER_content the_template.possible_contents.add
void CDXMLREAD_basic(char * input,void * output);
typedef int __attribute__((sysv_abi))(*CDXMLREAD_functype)(char * input,void * output);
typedef int __attribute__((sysv_abi))(*CDXMLREAD_TOBUFFER_functype)(TELESCOPE_buffer ibuffer,char * input,void * output);
//This is vomittingly expensive. Why cant I make the contents[] "virtual static" ? :G
typedef int __attribute__((sysv_abi))(*catalogized_command_functype)(const char * parameter,const char * value);
struct basicmultilist;struct basic_basic_instance;struct basic_instance;struct basic_instance_nested;
typedef int __attribute__((sysv_abi))(*catalogized_command_iterated_functype)(const char * parameter,const char * value,basicmultilist * imultilist,basic_instance * iinstance,int iindex);
//This has some speciality. It is being iterated only over the object which it is intended for. But if it is for generic, it gets iterated over all of them
typedef int __attribute__((sysv_abi))(*AUTOSTRUCT_GETSET_FUNCTYPE)(char state,int input);//1: set 2: get 4: toggle\n
#define catalogized_command_funcdef(MACROPARAM) int __attribute__((sysv_abi))MACROPARAM(const char * parameter,const char * value)
#define catalogized_command_iterated_funcdef(MACROPARAM) int __attribute__((sysv_abi))MACROPARAM(const char * parameter,const char * value,basicmultilist * imultilist,basic_instance * iinstance,int iindex)

//The name superconstellation arises because naming an airplane superconstellation sounds like rubbish in german.
//Better use it for a datastructure whose members have little in common, because that is what superconstellation implies
struct superconstellation//Information on a single content or property of an object
{
	char name[30];
	int ref;//relative position inside of the object, in bytes
	//The following delegates apply to properties only
	int size;//size of the variable in RAM, in bytes
	CDXMLREAD_functype delegate;
	CDXMLREAD_functype writedelegate;
	CDXMLREAD_functype binreaddelegate;
	CDXMLREAD_functype binwritedelegate;
};
struct AUTOSTRUCT_cstyle_vtable
{
	superconstellation * properties;
	int properties_count;
	superconstellation * contents;
	int contents_count;
	const char * FullName;//Points into a static object-specific string.
	const char * Name;//Points into FullName.

};
#define AUTOSTRUCT_GET_ROUTINE(AUTOSTRUCT_MACRONAME,COUNT_MACROPARAM) static superconstellation AUTOSTRUCT_MACRONAME[]; \
        virtual	int get ## AUTOSTRUCT_MACRONAME(const char * name) \
	{ \
		for (int ilv1=0;ilv1<COUNT_MACROPARAM;ilv1++) \
		{ \
			if (strcmp(name,AUTOSTRUCT_MACRONAME[ilv1].name)==0) \
			{ \
				return AUTOSTRUCT_MACRONAME[ilv1].ref; \
			} \
		} \
		return -1; \
	}
#define AUTOSTRUCT_PROPERTY_ROUTINE(COUNT_MACROPARAM) static superconstellation properties[]; \
	virtual int getproperties(const char * name, CDXMLREAD_functype * delegateoutput,int * posoutput=NULL) \
	{ \
		for (int ilv1=0;ilv1<COUNT_MACROPARAM;ilv1++) \
		{ \
			if (strcmp(name,properties[ilv1].name)==0) \
			{ \
				if (posoutput!=NULL){*posoutput=ilv1;}\
				*(delegateoutput)=properties[ilv1].delegate; \
				return properties[ilv1].ref; \
			} \
		} \
		return -1; \
	}
#define AUTOSTRUCT_cstyle_PROPERTY_ROUTINE(MACROPARAM,MACROPARAM_COUNT)\
        int getproperties(const char * name,CDXMLREAD_functype * delegateoutput)\
	{\
		for (int ilv1=0;ilv1<MACROPARAM_COUNT;ilv1++)\
		{\
			if (strcmp(name,MACROPARAM::properties[ilv1].name)==0)\
			{\
				*(delegateoutput)=MACROPARAM::properties[ilv1].delegate;\
				return MACROPARAM::properties[ilv1].ref;\
			}\
		}\
		return -1;\
	}
superconstellation * getsuperconstellation_p(AUTOSTRUCT_cstyle_vtable * ivtable,const char * name,int * posoutput)
{
	for (int ilv1=0;ilv1<ivtable->properties_count;ilv1++)
	{
		if (strcmp(name,ivtable->properties[ilv1].name)==0)
		{
			if (posoutput!=NULL){*posoutput=ilv1;}
			return ivtable->properties+ilv1;
		}
	}
	return NULL;
}
int AUTOSTRUCT_Numberofproperty(const char * name,superconstellation * iinput,int imax)
{
	for (int ilv1=0;ilv1<imax;ilv1++)
	{
		if (strcmp(name,iinput[ilv1].name)==0)
		{
			return ilv1;
		}
	}
	return -1;
}
#define AUTOSTRUCT_EXISTS(AUTOSTRUCT_CLASS,AUTOSTRUCT_VARIABLE,AUTOSTRUCT_ELEMENT) ((AUTOSTRUCT_VARIABLE.INTERNALPropertyexistflags>>AUTOSTRUCT_Numberofproperty(#AUTOSTRUCT_ELEMENT,(AUTOSTRUCT_VARIABLE.properties),AUTOSTRUCT_VARIABLE.INTERNALPropertycount))&1)
#define AUTOSTRUCT_EXISTS_SET(AUTOSTRUCT_VARIABLE,AUTOSTRUCT_ELEMENT_NR) ((*(AUTOSTRUCT_VARIABLE.getINTERNALPropertyexistflags()))|=(1<<AUTOSTRUCT_ELEMENT_NR))
#define AUTOSTRUCT_EXISTS_SET_NAME(AUTOSTRUCT_VARIABLE,AUTOSTRUCT_ELEMENT_NAME) ((*((*AUTOSTRUCT_VARIABLE).getINTERNALPropertyexistflags()))|=(1<<AUTOSTRUCT_Numberofproperty(# AUTOSTRUCT_ELEMENT_NAME,(*AUTOSTRUCT_VARIABLE).properties,(*AUTOSTRUCT_VARIABLE).getINTERNALPropertycount())))
#define AUTOSTRUCT_EXISTS_CLEAR_NAME(AUTOSTRUCT_VARIABLE,AUTOSTRUCT_ELEMENT_NAME) ((*((*AUTOSTRUCT_VARIABLE).getINTERNALPropertyexistflags()))&=~(1<<AUTOSTRUCT_Numberofproperty(# AUTOSTRUCT_ELEMENT_NAME,(*AUTOSTRUCT_VARIABLE).properties,(*AUTOSTRUCT_VARIABLE).getINTERNALPropertycount())))

char LHENDRAW_loadmemoryoverflow=0;
char * tagnamestring;
char * parameterstring;
char * paramvaluestring;
intl tagnamestring_length;
intl parameterstring_length;
intl paramvaluestring_length;

struct multilistlist_
{
	char name[stringlength+1];
	basicmultilist * instance;
	int usage;//0: internal="", 1:CAMBRIDGESOFT="CAMBRIDGE_" 2:hotkeys_xml="CONFIGBRIDGE_"
};
extern multilistlist_ multilistlist[];
intl multilist_count;
template <class whatabout> class multilistreference;

class basicmultilist
{
	public:
	intl filllevel;
	intl ourcount;
	int itemsize;
	intl index;
	intl numberinlist;
	_u32 maxid;
	char * pointer;
	AUTOSTRUCT_cstyle_vtable * _;
	basicmultilist(){pointer=NULL;itemsize=0;};
	virtual intl getmaxitems() {return 0;}
	void reset()
	{
		filllevel=0;
		ourcount=0;
		maxid=0;
	}
	inline basic_instance & operator[](int ino)
	{
		return *((basic_instance*)(((char*)pointer)+(ino*itemsize)));
	}
	virtual int getproperties(const char * name,CDXMLREAD_functype * delegateoutput)
	{
		error("Programming error! You asked for the properties of a basic multilist!");
	}
	~basicmultilist(){};
	void * dependants;//needed to point to contained objects from another list, while still being able to move items in this mulitilist (this in the sense of this-pointer)
};
class basicmultilistreference
{
	public:
	basicmultilist * instances;
	intl start_in_it;
	intl count_in_it;
	intl mynumber;
	virtual void * addnew(){return 0;};
	basic_instance_nested * operator [](int ino)
	{
		return (basic_instance_nested*)(((char*)((*instances).pointer))+(ino+start_in_it)*((*instances).itemsize));
	}
	basicmultilistreference(){};
	~basicmultilistreference(){};
};
struct basic_basic_instance
{
	AUTOSTRUCT_cstyle_vtable * _;
/*	int & operator [] (O_int which)///TODO: enum NEEDED for each datatype
	{
		return *(int*)(((char*)this)+(*(((int*)_)+which)));
	}*/
	basic_basic_instance(){};
	~basic_basic_instance(){};
};
struct basic_instance_nested : public basic_basic_instance
{
	public:
	virtual int getcontents(const char * name)
	{
		return -1;
	};
	virtual int getproperties(const char * name,CDXMLREAD_functype * delegateoutput,int * posoutput=NULL)
	{
		return -1;
	};
	virtual const char * getName(){return 0;}
	virtual const char * getFullName(){return 0;}
	virtual _u32 * getINTERNALPropertyexistflags(){return NULL;}
	basic_instance_nested * master;
	basic_instance_nested(){master=NULL;};
	~basic_instance_nested(){};
};
int bkchem_discriminated=1;
template <class whatabout> class multilist : public basicmultilist
{
	public:
	inline whatabout * bufferlist(){return (whatabout*)pointer;}
	inline whatabout & operator[](int ino)
	{
		return *((whatabout*)(((char*)pointer)+(ino*itemsize)));
	}
	intl getmaxitems() {return LHENDRAW_buffersize/sizeof(whatabout);}
	multilist()
	{
		filllevel=0;
		ourcount=0;
		maxid=0;
		itemsize=sizeof(whatabout);
		_=&((*(whatabout*)0).INTERNAL_cstyle_vtable);
		if ((strncmp(_->FullName,"BKCHEM_",strlen("BKCHEM_"))==0) && (bkchem_discriminated))
		{
			pointer=NULL;dependants=NULL;return;
		}
		memory_alloc(&pointer,2);
		memory_alloc((char**)&(dependants),1);
		return;
	}
	int ADD(whatabout * iwhatabout)//without ((multilistreference<whatabout>*)dependants), use item buffer instead
	{
		if (filllevel<getmaxitems())
		{
			memcpy(pointer+filllevel*sizeof(whatabout),iwhatabout,sizeof(whatabout));
			filllevel++;
			return 1;
		}
		else
		{
			error("Programming error: overflow on internal multilist");
			return -1;
		}
	}
	void * insert(whatabout input,intl position,intl mynumber)//position must be given to allow later refills
	{
		//This is the sole reason for the multilistreference... To update the master entries if the master is shifted
		if (filllevel>=getmaxitems()-1)
		{
			memory_overflow_hook();
		}
		for (int ilv1=filllevel;ilv1>position;ilv1--)
		{
			memcpy(bufferlist()+ilv1,bufferlist()+ilv1-1,sizeof(whatabout));//copying the higher objects to a higher position
			for (int ilv2=0;ilv2<sizeof(whatabout::contents)/sizeof(superconstellation);ilv2++)//For each possible type of content of that object
			{
				basicmultilistreference * tl_multilistreference=*(basicmultilistreference**)(((char*)(&(bufferlist()[ilv1])))+whatabout::contents[ilv2].ref);
				for (int ilv3=(*tl_multilistreference).start_in_it;ilv3<(*tl_multilistreference).start_in_it+(*tl_multilistreference).count_in_it;ilv3++)
				{
					basic_instance_nested * tl_basic_instance=(basic_instance_nested*)(((char*)((*((*tl_multilistreference).instances)).pointer))+(*((*tl_multilistreference).instances)).itemsize*ilv3);
					(*tl_basic_instance).master=(basic_instance_nested*)(&(bufferlist()[ilv1]));//Refreshing master entries,because master is shifted now
				}
			}
		}
		for (int ilv1=mynumber+1;ilv1<ourcount;ilv1++)
		{
			((multilistreference<whatabout>*)dependants)[ilv1].start_in_it++;
		}
		memcpy(bufferlist()+position,&input,sizeof(whatabout));
		filllevel++;
		return &(bufferlist()[position]);
	}
	multilistreference<whatabout> * getme(multilistreference<whatabout> * input)
	{
		(*input).start_in_it=filllevel;
		(*input).count_in_it=0;
		if (ourcount<LHENDRAW_buffersize/sizeof(multilistreference<whatabout>))
		{
			memcpy(((multilistreference<whatabout>*)dependants)+ourcount,input,sizeof(multilistreference<whatabout>));
			((multilistreference<whatabout>*)dependants)[ourcount].mynumber=ourcount;
			ourcount++;
		}
		else
		{
			memory_overflow_hook();
		}
		return &(((multilistreference<whatabout>*)dependants)[ourcount-1]);
	}
	virtual int getproperties(const char * name,CDXMLREAD_functype * delegateoutput)
	{
		return (*((whatabout*)0)).whatabout::getproperties(name,delegateoutput);
	}
	~multilist()
	{
		//memory free removed because it is never destroyed during runtime
	};
};

template <class whatabout> multilist<whatabout> * retrievemultilist()
{
	for (int ilv1=0;ilv1<multilist_count;ilv1++)
	{
		if (strcmp(whatabout::INTERNALgetname(),multilistlist[ilv1].name)==0)
		{
			return (multilist<whatabout> *) multilistlist[ilv1].instance;
		}
	}
	return NULL;
}

basicmultilist * findmultilist(const char * thetypesname)
{
	for (int ilv1=0;ilv1<multilist_count;ilv1++)
	{
		if (strcmp(thetypesname,multilistlist[ilv1].name)==0)
		{
			return multilistlist[ilv1].instance;
		}
	}
	return NULL;
}

template <class whatabout> multilist<whatabout> * registermultilist(const char * thetypesname)
{
	for (int ilv1=0;ilv1<multilist_count;ilv1++)
	{
		if (strcmp(thetypesname,multilistlist[ilv1].name)==0)
		{
			return (multilist<whatabout> *) multilistlist[ilv1].instance;
		}
	}
	error("Unregistered multilist detected! Unintended for this version of lhendraw! Please remove this hint if you changed something\n");
	multilistlist[multilist_count].instance=new(multilist<whatabout>);//Code will never reach this point.
	strcpy(multilistlist[multilist_count].name,thetypesname);
	(*((multilistlist[multilist_count].instance))).index=multilist_count;
	return (multilist<whatabout> *) multilistlist[multilist_count++].instance;
}

template <class whatabout> class multilistreference : public basicmultilistreference
{
	public:
	void * addnew()
	{
		whatabout tempwert=whatabout();
		return (*((multilist<whatabout>*)instances)).insert(tempwert,start_in_it+(count_in_it++),mynumber);
	}
	multilistreference()
	{
	};
	void * operator new(size_t dummy)
	{
		multilistreference<whatabout> me;
		me.instances=(basicmultilist*)registermultilist<whatabout>(whatabout::INTERNALgetname());
		return (*((multilist<whatabout>*)(me.instances))).getme(&me);
	}
	void multilistreferenx(multilist<whatabout> * input)//not used???
	{
		instances=(basicmultilist*)input;
		mynumber=(*((multilist<whatabout>*)instances)).getme(this);
	};
	~multilistreference(){};
};


struct basic_instance:basic_basic_instance
{
	char exist;
	intl pos_in_buffer;//set to the place where it WOULD be if empty
        int getproperties(const char * name,CDXMLREAD_functype * delegateoutput)
	{
		for (int ilv1=0;ilv1<_->properties_count;ilv1++)
		{
			if (strcmp(name,_->properties[ilv1].name)==0)
			{
				*(delegateoutput)=_->properties[ilv1].delegate;
				return _->properties[ilv1].ref;
			}
		}
		return -1;
	}
	basic_instance(){exist=1;};
	~basic_instance(){};
};

//This is a hack:
//1. Objects dependent on variables initalized out of code area
//3. Offsetof with inherited objects.
//4. Initialization of static list members in order to obtain self-reflecting code.
AUTOSTRUCT_cstyle_vtable gummydummy_instance_vtable={NULL,0,NULL,0,(char*)"CAMBRIDGE_gummydummy",(char*)"gummydummy"};
struct gummydummy_instance_: basic_instance_nested
{
	public:
	const char * getName(){static char name[]="gummydummy"; return(char*)&name;}
	int getcontents(char * name){return -1;}
	int getproperties(const char * name,CDXMLREAD_functype * delegateoutput,int * posoutput){return -1;}
	gummydummy_instance_()
	{
		_=&gummydummy_instance_vtable;
	}
	~gummydummy_instance_()
	{
	}
} gummydummy_instance;
struct undo_singlebuffer
{
	char * buffer;//When==NULL, this buffer is empty, apply the buffer of the parent
	char * contentbuffer;//NULL if no buffer.
	char imultilist[sizeof(basicmultilist)];
	TELESCOPE_buffer bufferhead;//Only for ease. the pointers in here are by no means important and need only be updated when this element, intended for formatted output of a structure, is output'ed
};
#define chararray char *
//adds an object to its master
#define ADD_TO_MULTILISTREFERENCE(WHOM,WHAT)\
	CAMBRIDGE_ ## WHAT ## _instance * tl_CAMBRIDGE_ ## WHAT ## _instance;\
	{\
		int suboffset;\
		basicmultilistreference * nextinstance_list;\
		suboffset=((WHOM)->getcontents(# WHAT));\
		if (suboffset!=-1)\
		{\
			nextinstance_list=*(basicmultilistreference**)(((char*)WHOM)+suboffset);\
			tl_CAMBRIDGE_ ## WHAT ## _instance=(CAMBRIDGE_ ## WHAT ## _instance*)(nextinstance_list->addnew());\
			(*tl_CAMBRIDGE_ ## WHAT ## _instance).master=WHOM;\
		}\
		else\
		{\
			error("programming error! " #WHAT " is no Element of this " #WHOM ", at address%p!",WHOM);\
		}\
	}
