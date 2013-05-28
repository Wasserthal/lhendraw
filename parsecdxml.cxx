#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <typeinfo>
#include <string.h>
#include <stdarg.h>
#define intl int
#ifndef stringlength
#define stringlength 256
#endif
#define bufferlength 256
#define bufferlistsize 256
#define multilistlength 256
#define REGISTER_content the_template.possible_contents.add
//This is vomittingly expensive. Why cant I make the contents[] "virtual static" ? :G
#define AUTOSTRUCT_GET_ROUTINE(AUTOSTRUCT_MACRONAME) static superconstellation AUTOSTRUCT_MACRONAME[]; \
        virtual	int get ## AUTOSTRUCT_MACRONAME(char * name) \
	{ \
		for (int ilv1=0;ilv1<sizeof(AUTOSTRUCT_MACRONAME[0])/sizeof(superconstellation);ilv1++) \
		{ \
			if (strcmp(name,AUTOSTRUCT_MACRONAME[ilv1].name)==0) \
			{ \
				return AUTOSTRUCT_MACRONAME[ilv1].ref; \
			} \
		} \
		return 0; \
	}
struct stringstruct
{
	char a[stringlength+1];
	stringstruct(const char * in) {strcpy(a,in);}
	stringstruct(){};
};

char tagnamestring[stringlength+1];
char attstring[bufferlength+1];
intl tagnamestring_length;
intl attstring_length;

/*stringstruct operator = (char* input)
{
	stringstruct wert;
	strncpy(wert.a,input);
	return wert;
};*/
class basic_xml_element_set;
struct multilistlist_
{
	char names[multilistlength][stringlength+1];
	void * instances[multilistlength];
	
} multilistlist;
intl multilist_count = 0;
template <class whatabout> class multilistreference;

class basicmultilist
{
	public:
	basicmultilist(){};
	~basicmultilist(){};
};

template <class whatabout> class multilist : public basicmultilist
{
	public:
	whatabout * bufferlist;
	multilistreference<whatabout> * dependants[bufferlistsize];
	intl filllevel;
	intl ourcount;
	multilist()
	{
		filllevel=0;
		ourcount=0;
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
			(*dependants[ilv1]).start_in_it++;
		}
		bufferlist[position]=input;
		filllevel++;
		return &bufferlist[position];
	}
	intl getme(multilistreference<whatabout> * input)
	{
		(*input).start_in_it=filllevel;
		(*input).count_in_it=0;
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

template <class whatabout> multilist<whatabout> * registermultilist(const char * thetypesname)
{
	for (int ilv1=0;ilv1<multilist_count;ilv1++)
	{
		if (strcmp(thetypesname,multilistlist.names[ilv1]))
		{
			return (multilist<whatabout> *) multilistlist.instances[ilv1];
		}
	}
	multilistlist.instances[multilist_count]=new(multilist<whatabout>);
	strcpy(multilistlist.names[multilist_count++],thetypesname);
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
	}
	void * addnew()
	{
		whatabout tempwert=whatabout();
		return (*((multilist<whatabout>*)instances)).insert(tempwert,start_in_it+count_in_it,mynumber);
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
	char name[10];
	int ref;
};

struct basic_instance
{
	public:
	virtual int getcontents(char * name)
	{
		return 0;
	};
	virtual int getproperties(char * name)
	{
		return 0;
	};
	basic_instance * master;
	char myname[stringlength+1];
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
		/*
		if (typeid(whatabout)==typeid(cdxml_instance))
		{
			REGISTER_content("page");
		}
		if (typeid(whatabout)==typeid(page_instance))
		{
			REGISTER_content("fragment");
			REGISTER_content("group");
		}
		if (typeid(whatabout)==typeid(group_instance))
		{
			REGISTER_content("fragment");
			REGISTER_content("group");
		}
		if (typeid(whatabout)==typeid(fragment_instance))
		{
			REGISTER_content("fragment");
			REGISTER_content("group");
		}
		*/
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
#include "filestructure.hxx"

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

void entertag()
{
	basicmultilistreference * nextinstance_list;
	basic_instance * nextinstance;
	tagnamestring[tagnamestring_length]=0;
	nextinstance_list=*reinterpret_cast<basicmultilistreference**>((((char*)currentinstance)+currentinstance->getcontents(tagnamestring)));
	printf("%s",typeid(nextinstance_list->addnew()).name());
	printf("enter %s\n",tagnamestring);
};

void concludeattstring()
{
	attstring[tagnamestring_length]=0;
};

void exittag(){};
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

void input_fsm(FILE* infile)
{
	intl fsmint=0; //0: in_nothing. 1: bracket-opening 2: Tagreading 3: attstringreading 4: bracket-closing 5: Qmark-ignoring 7: waiting_for_tag_end 8: Addstring - Hyphenation 9: After equals symbol.
	char ichar='A';
	bool bexittag=0;
	currentinstance=new(Total_Document_instance);
	#ifdef DEBUG
	intl debugcounter=0;
	#endif
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
			if (bexittag==1)
			{
				if (ichar=='/')
				{
					printf("Error: The Tag to end was defined by another /");exit(1);
					break;
				}
				if (ichar=='>')
				{
					if (bexittag)
					{
						printf("Error: Only slash in Brackets!");exit(1);
						exittag();
					}
					break;
				}
				if (spaciatic(ichar))
				{
					break;
				}
			}
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
				bexittag=0;
				fsmint=8;
			}
			printf("Error: invalid beginning of tag!");exit(1);
		break;
		case 2:
			if (sentenumeric(ichar))
			{
				tagnamestring[tagnamestring_length]=ichar;
				tagnamestring_length++;
			}
			else
			{
				entertag();
				fsmint=3;
			}
			if (ichar=='/')
			{
				exittag();
				fsmint=7;
				break;
			}
		break;
		case 3:
			if (ichar=='=')
			{
				fsmint=9;
				break;
			}
			if (ichar=='"')
			{
				fsmint=8;
				break;
			}
			if (ichar=='>')
			{
				bexittag=0;
				concludeattstring();
				fsmint=4;
			}
		break;
		case 8:
			if (ichar=='"')
			{
				fsmint=3;
				break;
			}
		break;
		case 4:
			if (ichar=='<')
			{
				printf("start ");
				fsmint=1;
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
			for (int ilv1=0;ilv1<15;ilv1++)
			{
				#ifdef DEBUG
					printf("%%-> %llX <-i\n",debugcounter);
				#endif
				fread(&ichar,1,1,infile);
				printf("%c",ichar);
			}
			exit(1);
		break;
		case 9:
			if (ichar=='"')
			{
				fsmint=8;
				break;
			}
			if (ichar=='>')
			{
				printf("Error:\">\" directly after equals");exit(1);
			}
		break;
		default:
		printf("Invalid fsmint!!!!");
		exit(1);
	}
	if (!feof(infile))
	{
		goto iback;
	}
}


int main(int argc, char * * argv)
{
	xml_set_register_count=0;
	multilist_count=0;
	FILE * infile;
	infile=fopen(argv[1],"r+");
	input_fsm(infile);
	fclose(infile);
	return 0;
}
