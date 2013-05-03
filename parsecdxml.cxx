#include <stdlib.h>
#include <stdio.h>
#include <typeinfo>
#include <string.h>
#define intl int
#ifndef stringlength
#define stringlength 256
#endif
#define bufferlength 256
#define bufferlistsize 256
#define multilistlength 256
#define REGISTER_content the_template.possible_contents.add
struct stringstruct
{
	char a[stringlength+1];
	stringstruct(const char * in) {strcpy(a,in);}
	stringstruct(){};
};

char tagnamestring[stringlength+1];
char attstring[bufferlength+1];

/*stringstruct operator = (char* input)
{
	stringstruct wert;
	strncpy(wert.a,input);
	return wert;
};*/
class basic_xml_element_set;
struct multilistlist_
{
	stringstruct names[multilistlength];
	void * instances[multilistlength];
	
} multilistlist;
intl multilist_count = 0;
template <class whatabout> class  multilistreference;
template <class whatabout> class multilist
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
	intl insert(whatabout input,intl position,intl mynumber)
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
multilist<stringstruct> stringlist;

template <class whatabout> multilist<whatabout> * registermultilist(const char * thetypesname)
{
	for (int ilv1=0;ilv1<multilist_count;ilv1++)
	{
		if (strcmp(thetypesname,multilistlist.names[ilv1].a))
		{
			return (multilist<whatabout> *) multilistlist.instances[ilv1];
		}
	}
}
template <class whatabout> class multilistreference
{
	public:
	multilist<whatabout> * instances;
	intl start_in_it;
	intl count_in_it;
	intl mynumber;
	void add(whatabout input)
	{
		(*instances).insert(input,start_in_it+count_in_it,mynumber);
	}
	multilistreference()
	{
		if (typeid(whatabout)==typeid(stringstruct))
		{
			multilistreferenx(&stringlist);
		}
		else
		{
			instances=registermultilist<whatabout>(typeid(whatabout).name());
			/*multilistreference(backvalue);*/
			mynumber=(*instances).getme(this);
		}
	};
	void multilistreferenx(multilist<whatabout> * input)
	{
		instances=input;
		mynumber=(*instances).getme(this);
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

class basic_xml_element_set
{
	public:
	xml_template_element the_template;
	multilistreference<stringstruct> possible_contents;
	basic_xml_element_set() 
	{
		possible_contents=*new(multilistreference<stringstruct>);
	};
	~basic_xml_element_set() {};
};

class basic_instance
{
	public:
	basic_instance * master;
	basic_instance(){master=NULL;};
	~basic_instance(){};
};

struct cdxml_instance:basic_instance
{
};

struct page_instance:basic_instance
{
};
struct group_instance:basic_instance
{
};
struct fragment_instance:basic_instance
{
};
template <class whatabout> class xml_element_set:basic_xml_element_set
{
	public:
	multilistreference<whatabout> instances;
	xml_element_set() 
	{
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
	};
	~xml_element_set(){};
};

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

void entertag(){};
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
	intl fsmint=0; //0: in_nothing. 1: bracket-opening 2: Tagreading 3: attstringreading 4: bracket-closing 5: Qmark-ignoring 6: waiting_for_tagname
	char ichar='A';
	intl tagnamestring_length;
	intl attstring_length;
	iback:
	fread(&ichar,1,1,infile);
	switch (fsmint)
	{
		case 0:
			if (ichar=='<')
			{
				fsmint=1;break;
			}
		break;
		case 1:
			if (sentenumeric(ichar))
			{
				tagnamestring[0]=ichar;
				tagnamestring_length=1;
			}
		break;
		case 2:
			if (sentenumeric(ichar))
			{
				tagnamestring[tagnamestring_length]=ichar;
				tagnamestring_length++;
			}
			else
			{
				tagnamestring[tagnamestring_length]=0;
				entertag();
				fsmint=6;
			}
		break;
		case 5:
			if (ichar=='>')
			{
				fsmint=1;break;	
			}
		break;
		case 6:
			if (ichar=='/')
			{
				exittag();
				break;
			}
			if (ichar=='>')
			{
				
			}
			if (spaciatic(ichar))
			{
			}
			else
			{
				attstring_length=0;//TODO****
			}
		break;
	}
	if (!feof(infile))
	{
		goto iback;
	}
}


int main(int argc, char * * argv)
{
	stringlist=*new(multilist<stringstruct>);
	multilist_count=0;
	return 0;
}
