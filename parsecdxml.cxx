//This is the main unit, contains alsomost C++ code, and inserts the other units in the middle
//Be aware that filestructure.hxx is dynamically generated from filestructure.draft by filestructure_maker (filestructuremaker.c)
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <typeinfo>
#include <string.h>
#include <stdarg.h>
#define intl long long
#ifndef stringlength
#define stringlength 512
#endif
#define bufferlength 256
#define bufferlistsize 4096
#define multilistlistlength 512
#define REGISTER_content the_template.possible_contents.add
void CDXMLREAD_basic(char * input,void * output);
typedef int __attribute__((sysv_abi))(*CDXMLREAD_functype)(char * input,void * output);
//This is vomittingly expensive. Why cant I make the contents[] "virtual static" ? :G
#define AUTOSTRUCT_GET_ROUTINE(AUTOSTRUCT_MACRONAME,COUNT_MACROPARAM) static superconstellation AUTOSTRUCT_MACRONAME[]; \
        virtual	int get ## AUTOSTRUCT_MACRONAME(char * name) \
	{ \
		for (int ilv1=0;ilv1<COUNT_MACROPARAM;ilv1++) \
		{ \
			printf(">>%s!.",AUTOSTRUCT_MACRONAME[ilv1].name); \
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
	basicmultilist(){};
	~basicmultilist(){};
};

char spaciatic(char input)
{
	if (input==' ')
	{
		return 1;
	}
	if (input==10)
	{
		return 1;
	}
	if (input==13)
	{
		return 1;
	}
	return 0;
}
template <class whatabout> class multilist : public basicmultilist
{
	public:
	whatabout * bufferlist;
	multilistreference<whatabout> * dependants[bufferlistsize];
	intl filllevel;
	intl ourcount;
	intl testbeef;
	multilist()
	{
		filllevel=0;
		ourcount=0;
		testbeef=0xdeadbeef;
		printf("deadbeef");
		bufferlist=(whatabout*)malloc(sizeof(whatabout)*bufferlistsize);
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
	basic_xml_element_set() 
	{
		possible_contents=multilistreference<stringstruct>();
	};
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
#include "cxxdata.h"
#include "enums.hxx"
#include "filestructure.hxx"
#include "createsvg.hxx"

char sentenumeric(char input)
{
	if ((input>='a') && (input<='z'))
	{
		return 1;
	}
	if ((input>='A') && (input<='Z'))
	{
		return 1;
	}
	if ((input>='0') && (input<='9'))
	{
		return 1;
	}
	if (input=='_')
	{
		return 1;
	}
	return 0;
}

basic_instance * currentinstance;

#ifdef DEBUG
intl debugcounter=0;
#endif

void entertag()
{
	intl suboffset;
	basicmultilistreference * nextinstance_list;
	basic_instance * nextinstance;
	tagnamestring[tagnamestring_length]=0;
	printf("%i",tagnamestring_length);
	printf(" %s ",tagnamestring,10);
	suboffset=(currentinstance->getcontents(tagnamestring));
	if (suboffset!=-1)
	{
		nextinstance_list=*(basicmultilistreference**)(((char*)currentinstance)+suboffset);
		nextinstance=(basic_instance*)nextinstance_list->addnew();//TODO error must be here.
	}
	else
	{
		printf("%s has no member named %s at %X!\n",currentinstance->getName(),tagnamestring,debugcounter);
		nextinstance=new(gummydummy_instance);
	}
	printf("next%llX,",nextinstance);
	printf(">>%s<<",nextinstance->getName());
	(*nextinstance).master=currentinstance;
	printf("enter %s\n",tagnamestring);
	currentinstance=nextinstance;
};

void grow_pctext(char ichar)
{
	CDXMLREAD_functype thisfunc;
	printf("as%c",ichar);
	int txtoffset=currentinstance->getproperties("PCTEXT",&thisfunc);
	if (txtoffset==-1)
	{
		return;
	}
	char * txtpos=((char*)currentinstance)+txtoffset;
	int ctroffset=currentinstance->getproperties("PCTEXTcounter",&thisfunc);
	int * ctrpos=(int*)(((char*)currentinstance)+ctroffset);
	if (ctroffset==-1)
	{
		return;	
	}
	printf("is%c",ichar);
	txtpos[(*ctrpos)++]=ichar;	
}

inline void concludepctext()
{
	grow_pctext(0);
}

void concludeparameterstring()
{
	parameterstring[parameterstring_length]=0;
	parameterstring_length=0;
};
void concludeparamvaluestring()
{
	paramvaluestring[paramvaluestring_length]=0;
	paramvaluestring_length=0;
};

void scoopparam()
{
	CDXMLREAD_functype thisfunc;
	if (strcmp(parameterstring,"PCTEXTcounter")==0)
	{
		printf("Hacking attempt detected (PCSTRING)!");exit(1);
	}
	int suboffset=(currentinstance->getproperties(parameterstring,&thisfunc));
	if (suboffset!=-1)
	{
		thisfunc(paramvaluestring,((char*)currentinstance)+suboffset);
		printf(" ;....; %s\n",paramvaluestring);
	}
	else
	{
		printf("%s has no parameter named %s\n",currentinstance->getName(),parameterstring);
		return;
	}
	
	return;
}

void exittag()
{
	basic_instance * lastinstance=currentinstance;
	currentinstance=(*currentinstance).master;
	if (strcmp(lastinstance->getName(),"gummydummy")==0)
	{
		delete(lastinstance);
	}
};

void input_fsm(FILE* infile)
{
	intl fsmint=0; //0: in_nothing. 1: bracket-opening 2: Tagreading 3: parameterstringreading 4: bracket-closing 5: Qmark-ignoring 7: waiting_for_tag_end 8: Addstring - Hyphenation 9: After equals symbol 10 paramvaluestringreading.
	char ichar='A';
	bool bexittag=0;
	tagnamestring_length=0;
	parameterstring_length=0;
	paramvaluestring_length=0;
	currentinstance=new(Total_Document_instance);
	iback:
	fread(&ichar,1,1,infile);
	#ifdef DEBUG
	debugcounter++;
	#endif
	switch (fsmint)
	{
		case 0:
			if (ichar=='<')
			{
				fsmint=1;
				break;
			}
		break;
		case 1:
			if (spaciatic(ichar))
			{
				break;
			}
			if (sentenumeric(ichar))
			{
				tagnamestring[0]=ichar;
				tagnamestring_length=1;
				fsmint=2;
				break;
			}
			if (ichar=='?')
			{
				fsmint=5;
				break;
			}
			if (ichar=='!')
			{
				fsmint=5;
				break;
			}
			if (ichar=='/')
			{
				bexittag=1;
				break;
			}
			if (ichar=='>')
			{
				if (bexittag==1)
				{
					printf("Error: only slash in brackets!");exit(1);
				}
				bexittag=0;
				fsmint=4;
			}
			printf("Error: invalid beginning of tag!");exit(1);
		break;
		case 2:
			if (sentenumeric(ichar))
			{
				tagnamestring[tagnamestring_length]=ichar;
				tagnamestring_length++;
				break;
			}
			else
			{
				if (bexittag==1)
				{
					if (ichar=='/')
					{
						printf("Error: The Tag to end was defined by another /");exit(1);
						break;
					}
					if (ichar=='>')
					{
						exittag();	
						bexittag=0;
						fsmint=4;
						break;
					}
					if (spaciatic(ichar))
					{
						break;
					}
				}
				else
				{
					fsmint=3;
					entertag();
					parameterstring_length=0;
					paramvaluestring_length=0;
					//omission of break is intended here
				}
				if (ichar=='>')
				{
					bexittag=0;
					fsmint=4;
					break;
				}
				break;
			}
			if (ichar=='/')
			{
				if (bexittag==1)
				{
					printf("error: exiting tag had also exit on end");exit(1);
				}
				exittag();
				bexittag=0;
				fsmint=7;
				break;
			}
		break;
		case 3:
			if (ichar=='=')
			{
				concludeparameterstring();
				paramvaluestring_length=0;
				fsmint=8;
				break;
			}
			if (ichar=='>')
			{
				bexittag=0;
				concludeparameterstring();
				fsmint=4;
				break;
			}
			if (ichar=='/')
			{
				exittag();
				bexittag=0;
				fsmint=7;
				break;
			}
			if (!spaciatic(ichar))
			{
				parameterstring[parameterstring_length++]=ichar;
			}
		break;
		case 8:
			if (ichar=='"')
			{
				if (paramvaluestring_length!=0)
				{
					printf("Error: Starting the Hypenation too late");exit(1);
				}
				paramvaluestring_length=0;
				fsmint=9;
				break;
			}
			else
			{
				paramvaluestring[paramvaluestring_length++]=ichar;
			}
		break;
		case 4:
			if (ichar=='<')
			{
				concludepctext();
				fsmint=1;
			}
			else
			{
				grow_pctext(ichar);
			}			
		break;
		case 5:
			if (ichar=='?')
			{
				fsmint=7;
				break;	
			}
			if (ichar=='>')
			{
				fsmint=4;
				if (bexittag)
				{
					exittag();
					bexittag=0;
				}
				break;	
			}
		break;
		case 7:
			if (spaciatic(ichar))
			{
				break;
			}
			if (ichar=='>')
			{
				fsmint=4;break;	
			}
			printf("Error: \">\" expected at end of tag");
			#ifdef DEBUG
				printf("%%-> %llX <-i\n",debugcounter);
			#endif
			for (int ilv1=0;ilv1<15;ilv1++)
			{
				fread(&ichar,1,1,infile);
				printf("%c",ichar);
			}
			exit(1);
		break;
		case 9:
			if (ichar=='"')
			{
				concludeparamvaluestring();
				scoopparam();
				fsmint=3;
				break;
			}
			paramvaluestring[paramvaluestring_length++]=ichar;
		break;
		default:
		printf("Error: Internal error!Invalid fsmint!!!!:%i",fsmint);exit(1);
	}
	if (!feof(infile))
	{
		goto iback;
	}
}


int main(int argc, char * * argv)
{
	setvbuf(stdout,NULL,_IONBF,0);
	/*xml_set_register_count=0;
	multilist_count=0;*/ //contraproductive amongst the hack!
	FILE * infile;
	infile=fopen(argv[1],"r+");
	input_fsm(infile);
	fclose(infile);
	svg_main("output.svg");
	exit(0);
	return 0;
}
