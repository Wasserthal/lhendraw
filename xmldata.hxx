
#define REGISTER_content the_template.possible_contents.add
void CDXMLREAD_basic(char * input,void * output);
typedef int __attribute__((sysv_abi))(*CDXMLREAD_functype)(char * input,void * output);
//This is vomittingly expensive. Why cant I make the contents[] "virtual static" ? :G
#define AUTOSTRUCT_GET_ROUTINE(AUTOSTRUCT_MACRONAME,COUNT_MACROPARAM) static superconstellation AUTOSTRUCT_MACRONAME[]; \
        virtual	int get ## AUTOSTRUCT_MACRONAME(char * name) \
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
	virtual int getproperties(const char * name, CDXMLREAD_functype * delegateoutput) \
	{ \
		for (int ilv1=0;ilv1<COUNT_MACROPARAM;ilv1++) \
		{ \
			if (strcmp(name,properties[ilv1].name)==0) \
			{ \
				*(delegateoutput)=properties[ilv1].delegate; \
				return properties[ilv1].ref; \
			} \
		} \
		return -1; \
	}
struct stringstruct
{
	char a[stringlength+1];
	stringstruct(const char * in) {strcpy(a,in);}
	int getproperties(const char * name,CDXMLREAD_functype * delegateoutput)
	{
		fprintf(stderr,"Programming error! You asked for the properties of a stringstruct!");
	};
	stringstruct(){};
};

char tagnamestring[stringlength+1];
char parameterstring[bufferlength+1];
char paramvaluestring[bufferlength+1];
intl tagnamestring_length;
intl parameterstring_length;
intl paramvaluestring_length;

/*stringstruct operator = (char* input)
{
	stringstruct wert;
	strncpy(wert.a,input);
	return wert;
};*/
class basic_xml_element_set;
struct multilistlist_
{
	char names[multilistlistlength][stringlength+1];
	void * instances[multilistlistlength];
	
};
multilistlist_ multilistlist;
intl multilist_count = 0;
template <class whatabout> class multilistreference;

class basicmultilist
{
	public:
	intl filllevel;
	intl ourcount;
	int itemsize;
	intl index;
	void * pointer;
	basicmultilist(){pointer=NULL;itemsize=0;};
	virtual int getproperties(const char * name,CDXMLREAD_functype * delegateoutput)
	{
		fprintf(stderr,"Programming error! You asked for the properties of a basic multilist!");
		exit(1);
	}
	~basicmultilist(){};
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
		bufferlist=(whatabout*)malloc(sizeof(whatabout)*bufferlistsize);
		itemsize=sizeof(whatabout);
		pointer=bufferlist;
		return;
	}
	void * insert(whatabout input,intl position,intl mynumber)
	{
		for (int ilv1=filllevel;ilv1>position;ilv1--)
		{
			bufferlist[ilv1]=bufferlist[ilv1-1];
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
multilist<stringstruct> stringlist=multilist<stringstruct>();
basic_xml_element_set* xml_set_register[bufferlistsize];
intl xml_set_register_count=0;

template <class whatabout> multilist<whatabout> * retrievemultilist()
{
	for (int ilv1=0;ilv1<multilist_count;ilv1++)
	{
		if (strcmp(typeid(whatabout).name(),multilistlist.names[ilv1])==0)
		{
			return (multilist<whatabout> *) multilistlist.instances[ilv1];
		}
	}
	return 0;
}

basicmultilist * findmultilist(const char * thetypesname)
{
	for (int ilv1=0;ilv1<multilist_count;ilv1++)
	{
		if (strcmp(thetypesname,multilistlist.names[ilv1])==0)
		{
			return (basicmultilist*)multilistlist.instances[ilv1];
		}
	}
	return 0;
}

template <class whatabout> multilist<whatabout> * registermultilist(const char * thetypesname)
{
	for (int ilv1=0;ilv1<multilist_count;ilv1++)
	{
		if (strcmp(thetypesname,multilistlist.names[ilv1])==0)
		{
			return (multilist<whatabout> *) multilistlist.instances[ilv1];
		}
	}
	multilistlist.instances[multilist_count]=new(multilist<whatabout>);
	strcpy(multilistlist.names[multilist_count],thetypesname);
	(*((basicmultilist*)(multilistlist.instances[multilist_count]))).index=multilist_count;
	return (multilist<whatabout> *) multilistlist.instances[multilist_count++];
}

class basicmultilistreference
{
	public:
	basicmultilist * instances;
	intl start_in_it;
	intl count_in_it;
	intl mynumber;
	virtual void * addnew(){};
	basicmultilistreference(){};
	~basicmultilistreference(){};
};

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
		if (typeid(whatabout)==typeid(stringstruct))
		{
			instances=(basicmultilist*)&stringlist;
		}
		else
		{
			instances=(basicmultilist*)registermultilist<whatabout>(typeid(whatabout).name());
			/*multilistreference(backvalue);*/
			mynumber=(*((multilist<whatabout>*)instances)).getme(this);
		}
	};
	void multilistreferenx(multilist<whatabout> * input)
	{
		instances=(basicmultilist*)input;
		mynumber=(*((multilist<whatabout>*)instances)).getme(this);
	};
	~multilistreference(){};
};

struct xml_template_element
{
	public:
	stringstruct name;
	multilistreference<stringstruct> possible_contents;
	basic_xml_element_set * content;
};

struct superconstellation
{
	char name[30];
	int ref;
	CDXMLREAD_functype delegate;
};

struct basic_instance
{
	public:
	virtual int getcontents(char * name)
	{
		return -1;
	};
	virtual int getproperties(const char * name,CDXMLREAD_functype * delegateoutput)
	{
		return -1;
	};
	virtual char * getName(){return 0;}
	basic_instance * master;
	basic_instance(){master=NULL;};
	~basic_instance(){};
};

class basic_xml_element_set
{
	public:
	xml_template_element the_template;
	basic_instance * instances;
	char name[stringlength+1];
	multilistreference<stringstruct> possible_contents;
	basicmultilist * hismultilist;
	basic_xml_element_set() 
	{
		possible_contents=multilistreference<stringstruct>();
		hismultilist=NULL;
	};
	virtual int getproperties(const char * name,CDXMLREAD_functype * delegateoutput)
	{
		fprintf(stderr,"Programming error! You asked for the properties of a basic xml element set!");
		exit(1);
	}
	~basic_xml_element_set() {};
};



template <class whatabout> class xml_element_set:basic_xml_element_set
{
	public:
	multilistreference<whatabout> instances;
	xml_element_set() 
	{
		strcpy(name,typeid(whatabout).name());
		name[strlen(name)-strlen("_instance")]=0;
		xml_set_register[xml_set_register_count++]=this;
		hismultilist=findmultilist(typeid(whatabout).name());
	};
	xml_element_set(const char * first,...)
	{
		va_list inlist;
		const char * wert=first;
		va_start(inlist,first);
		back:
		REGISTER_content(wert);
		wert=va_arg(inlist,char *);
		if (wert!=0)
		{
			goto back;
		}
		va_end(inlist);
		strcpy(name,typeid(whatabout).name());
		name[strlen(name)-strlen("_instance")]=0;
		xml_set_register[xml_set_register_count++]=this;
	}
	virtual int getproperties(const char* name,CDXMLREAD_functype * delegateoutput)
	{
		(*((whatabout*)0)).whatabout::getproperties(name,delegateoutput);
//		(reinterpret_cast<whatabout>(*((whatabout*)1)).getproperties(name,delegateoutput);
//		return (static_cast<whatabout*>((void*)0))->getproperties(name,delegateoutput);
//		return ((*(static_cast<whatabout*>((void*)0))).getproperties(name,delegateoutput));
	}
	~xml_element_set(){};
};

//This is a hack:
//1. Objects dependent on variables initalized out of code area
//2. Object stringlist which all depend on during initialization initialized out of code area
//3. Offsetof with inherited objects.
//4. Initialization of static list members in order to obtain self-reflecting code.
struct gummydummy_instance: basic_instance
{
	char * getName(){static char name[]="gummydummy"; return(char*)&name;}
	int getcontents(char * name){return -1;}
	int getproperties(const char * name,CDXMLREAD_functype * delegateoutput){return -1;}
};
#define chararray char *
