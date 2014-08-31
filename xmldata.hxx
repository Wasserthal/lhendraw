//defines the main datastructures needed to read in xml files, but they are also used for cdx files.
//defines also all structures for automatically generated source

struct TELESCOPE_buffer
{
	char * buffer;
	intl max;
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
	CDXMLREAD_functype delegate;
	CDXMLREAD_functype writedelegate;
	CDXMLREAD_functype binreaddelegate;
//	CDXMLREAD_functype binwritedelegate;
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

char tagnamestring[stringlength+1];
char parameterstring[bufferlength+1];
char paramvaluestring[bufferlength+1];
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
	_u32 maxid;
	void * pointer;
	basicmultilist(){pointer=NULL;itemsize=0;};
	inline basic_instance & operator[](int ino)
	{
		return *((basic_instance*)(((char*)pointer)+(ino*itemsize)));
	}
	virtual int getproperties(const char * name,CDXMLREAD_functype * delegateoutput)
	{
		fprintf(stderr,"Programming error! You asked for the properties of a basic multilist!");
		exit(1);
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
	virtual void * addnew(){};
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
	virtual int hit(float ix,float iy){};
/*	int & operator [] (O_int which)///TODO: enum NEEDED for each datatype
	{
		return *(int*)(((char*)this)+(*(((int*)_)+which)));
	}*/
	char exist;
	basic_instance(){master=NULL;exist=1;};//TODO put exist into basic_instance_propertybuffer
	~basic_instance(){};
};
template <class whatabout> class multilist : public basicmultilist
{
	public:
	whatabout * bufferlist;
	multilistreference<whatabout> * dependants[bufferlistsize];
	multilist()
	{
		filllevel=0;
		ourcount=0;
		maxid=0;
		bufferlist=(whatabout*)malloc(sizeof(whatabout)*bufferlistsize);
		itemsize=sizeof(whatabout);
		pointer=bufferlist;
		return;
	}
	void * ADD(whatabout * iwhatabout)//without dependants, use item buffer instead
	{
		bufferlist[filllevel]=*iwhatabout;
		bufferlist[filllevel]._vptr=(*iwhatabout)._vptr;
		filllevel++;
	}
	void * REMOVE(intl index)//without dependants, use item buffer instead
	{
		bufferlist[index].exist=0;
	}
	void * insert(whatabout input,intl position,intl mynumber)//position must be given to allow later refills
	{
		for (int ilv1=filllevel;ilv1>position;ilv1--)
		{
			for (int ilv2=0;ilv2<sizeof(bufferlist[0]);ilv2++)//Thats why oop sucks!
			{
				*(((char*)(&(bufferlist[ilv1])))+ilv2)=*(((char*)(&(bufferlist[ilv1-1])))+ilv2);
			}
			for (int ilv2=0;ilv2<sizeof(whatabout::contents)/sizeof(superconstellation);ilv2++)
			{
				basicmultilistreference * tl_multilistreference=*(basicmultilistreference**)(((char*)(&(bufferlist[ilv1])))+whatabout::contents[ilv2].ref);
				for (int ilv3=(*tl_multilistreference).start_in_it;ilv3<(*tl_multilistreference).start_in_it+(*tl_multilistreference).count_in_it;ilv3++)
				{
					basic_instance * tl_basic_instance=(basic_instance*)(((char*)((*((*tl_multilistreference).instances)).pointer))+(*((*tl_multilistreference).instances)).itemsize*ilv3);
					(*tl_basic_instance).master=(basic_instance*)(&(bufferlist[ilv1]));
				}
			}
		}
		for (int ilv1=mynumber+1;ilv1<ourcount;ilv1++)
		{
			(*(dependants[ilv1])).start_in_it++;
		}
		bufferlist[position]=input;
		(*(intl*)&(bufferlist[position]))=*(intl*)&input;//hack for bug in gcc: vtable index not copied to array //not TODO checked if it has to be intl
		filllevel++;
		return &(bufferlist[position]);
	}
	intl getme(multilistreference<whatabout> * input)
	{
		(*input).start_in_it=filllevel;
		(*input).count_in_it=0;
		dependants[ourcount]=input;
		return ourcount++;
	}
	virtual int getproperties(const char * name,CDXMLREAD_functype * delegateoutput)
	{
		(*((whatabout*)0)).whatabout::getproperties(name,delegateoutput);
	}
	~multilist()
	{
		free(bufferlist);
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
	return 0;
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
	return 0;
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
	multilistlist[multilist_count].instance=new(multilist<whatabout>);
	strcpy(multilistlist[multilist_count].name,thetypesname);
	(*((multilistlist[multilist_count].instance))).index=multilist_count;
	return (multilist<whatabout> *) multilistlist[multilist_count++].instance;
}

template <class whatabout> class multilistreference : public basicmultilistreference
{
	public:
	void * add(whatabout input)
	{
		(*((multilist<whatabout>*)instances)).insert(input,start_in_it+count_in_it,mynumber);
		count_in_it++;
	}
	void * addnew()
	{
		whatabout tempwert=whatabout();
		return (*((multilist<whatabout>*)instances)).insert(tempwert,start_in_it+(count_in_it++),mynumber);
	}
	multilistreference()
	{
		instances=(basicmultilist*)registermultilist<whatabout>(whatabout::INTERNALgetname());
		/*multilistreference(backvalue);*/
		mynumber=(*((multilist<whatabout>*)instances)).getme(this);
	};
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
struct gummydummy_instance: basic_instance
{
	const char * getName(){static char name[]="gummydummy"; return(char*)&name;}
	int getcontents(char * name){return -1;}
	int getproperties(const char * name,CDXMLREAD_functype * delegateoutput,int * posoutput){return -1;}
};
struct undo_singlebuffer
{
	char * buffer;//When==NULL, this buffer is empty, apply the buffer of the parent
	char * contentbuffer;//NULL if no buffer.
	char imultilist[sizeof(multilist<basic_instance>)];
	TELESCOPE_buffer bufferhead;
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
			tl_CAMBRIDGE_ ## WHAT ## _instance=(CAMBRIDGE_ ## WHAT ## _instance*)nextinstance_list->addnew();\
		}\
		(*tl_CAMBRIDGE_ ## WHAT ## _instance).master=WHOM;\
	}
