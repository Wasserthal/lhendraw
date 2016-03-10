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
struct basicmultilist;struct basic_instance;
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
	char * FullName;//Points into a static object-specific string.
	char * Name;//Points into FullName.

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
#define AUTOSTRUCT_EXISTS_SET_NAME(AUTOSTRUCT_VARIABLE,AUTOSTRUCT_ELEMENT_NAME) ((*((*AUTOSTRUCT_VARIABLE).getINTERNALPropertyexistflags()))|=(1<<AUTOSTRUCT_Numberofproperty(# AUTOSTRUCT_ELEMENT_NAME,(*AUTOSTRUCT_VARIABLE).properties,(*AUTOSTRUCT_VARIABLE).INTERNALPropertycount)))
#define AUTOSTRUCT_EXISTS_CLEAR_NAME(AUTOSTRUCT_VARIABLE,AUTOSTRUCT_ELEMENT_NAME) ((*((*AUTOSTRUCT_VARIABLE).getINTERNALPropertyexistflags()))&=~(1<<AUTOSTRUCT_Numberofproperty(# AUTOSTRUCT_ELEMENT_NAME,(*AUTOSTRUCT_VARIABLE).properties,(*AUTOSTRUCT_VARIABLE).INTERNALPropertycount)))

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
		//TODO: move buffer reset here, especially when dealing with more buffers
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
};
class basicmultilistreference
{
	public:
	basicmultilist * instances;
	intl start_in_it;
	intl count_in_it;
	intl mynumber;
	virtual void * addnew(){return 0;};
	basic_instance * operator [](int ino)
	{
		return (basic_instance*)(((char*)((*instances).pointer))+(ino+start_in_it)*((*instances).itemsize));
	}
	basicmultilistreference(){};
	~basicmultilistreference(){};
};
struct basic_instance
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
	AUTOSTRUCT_cstyle_vtable * _;
	basic_instance * master;
	virtual int hit(float ix,float iy){return 0;};
/*	int & operator [] (O_int which)///TODO: enum NEEDED for each datatype
	{
		return *(int*)(((char*)this)+(*(((int*)_)+which)));
	}*/
	char exist;
	basic_instance(){master=NULL;exist=1;};//TODO put exist into basic_instance_propertybuffer
	~basic_instance(){};
};
#define dependantlistsize 8192
template <class whatabout> class multilist : public basicmultilist
{
	public:
	inline whatabout * bufferlist(){return (whatabout*)pointer;}
	//TODO: split off the non-contentbuffer "CAMBRIDGE" items-multilist as new data type, carrying the dependants, so this large variable is not stored in the undo steps. Turn it into an own buffer later!
	multilistreference<whatabout> dependants[8192];//needed to point to contained objects from another list, while still being able to move items in this mulitilist (this in the sense of this-pointer)
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
		memory_alloc(&pointer,2);
		itemsize=sizeof(whatabout);
		_=&((*(whatabout*)0).INTERNAL_cstyle_vtable);
		return;
	}
	int ADD(whatabout * iwhatabout)//without dependants, use item buffer instead
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
	void REMOVE(intl index)//without dependants, use item buffer instead
	{
		bufferlist()[index].exist=0;
	}
	void * insert(whatabout input,intl position,intl mynumber)//position must be given to allow later refills
	{
		//TODO: check what's going on here!
		if (filllevel>=getmaxitems()-1)
		{
			memory_overflow_hook();
		}
		for (int ilv1=filllevel;ilv1>position;ilv1--)
		{
			memcpy(bufferlist()+ilv1,bufferlist()+ilv1-1,sizeof(whatabout));//Thats why oop sucks!
			for (int ilv2=0;ilv2<sizeof(whatabout::contents)/sizeof(superconstellation);ilv2++)
			{
				basicmultilistreference * tl_multilistreference=*(basicmultilistreference**)(((char*)(&(bufferlist()[ilv1])))+whatabout::contents[ilv2].ref);
				for (int ilv3=(*tl_multilistreference).start_in_it;ilv3<(*tl_multilistreference).start_in_it+(*tl_multilistreference).count_in_it;ilv3++)
				{
					basic_instance * tl_basic_instance=(basic_instance*)(((char*)((*((*tl_multilistreference).instances)).pointer))+(*((*tl_multilistreference).instances)).itemsize*ilv3);
					(*tl_basic_instance).master=(basic_instance*)(&(bufferlist()[ilv1]));
				}
			}
		}
		for (int ilv1=mynumber+1;ilv1<ourcount;ilv1++)
		{
			dependants[ilv1].start_in_it++;
		}
		bufferlist()[position]=input;
		(*(intl*)&(bufferlist()[position]))=*(intl*)&input;//hack for bug in gcc: vtable index not copied to array //not TODO checked if it has to be intl
		filllevel++;
		return &(bufferlist()[position]);
	}
	multilistreference<whatabout> * getme(multilistreference<whatabout> * input)
	{
		(*input).start_in_it=filllevel;
		(*input).count_in_it=0;
		if (ourcount<dependantlistsize)
		{
			memcpy(dependants+ourcount,input,sizeof(multilistreference<whatabout>));
			dependants[ourcount].mynumber=ourcount;
			ourcount++;
		}
		else
		{//TODO: Prevent memory overflow, better: 
			error("Memory overflow on multilistreference_creation\n");
		}
		return &(dependants[ourcount-1]);
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
	multilistlist[multilist_count].instance=new(multilist<whatabout>);//TODO mem: leaks
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

/*struct basic_instance_multilistreference:basic_instance
{
	public:
	basic_instance * master;
	basic_instance_multilistreference(){master=NULL;};
	~basic_instance_multilistreference(){};
};*/

//TODO: call that following contentbuffer, not propertybuffer! or better, call THIS basic_instance, the old one basic_basic_instance, and all instances of basic_instance in xmlparse.hxx to basic_instance_multilistreference. or, however, to basic_basic_instance in the case the call has nothing to do with masters. Good that multilists do not care about the listed datastructure's referencing type.
struct basic_instance_propertybuffer:basic_instance
{
	intl pos_in_buffer;//set to the place where it WOULD be if empty
};

//This is a hack:
//1. Objects dependent on variables initalized out of code area
//3. Offsetof with inherited objects.
//4. Initialization of static list members in order to obtain self-reflecting code.
AUTOSTRUCT_cstyle_vtable gummydummy_instance_vtable={NULL,0,NULL,0,(char*)"CAMBRIDGE_gummydummy",(char*)"gummydummy"};
struct gummydummy_instance_: basic_instance
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
