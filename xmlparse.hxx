//this is the xml parsing unit. requires xmldata.hxx
//few routines are also needed for binary cdx.
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
	suboffset=(currentinstance->getcontents(tagnamestring));
	if (suboffset!=-1)
	{
		nextinstance_list=*(basicmultilistreference**)(((char*)currentinstance)+suboffset);
		nextinstance=(basic_instance*)nextinstance_list->addnew();
	}
	else
	{
//		printf("%s has no member named %s at %X!\n",currentinstance->getName(),tagnamestring,debugcounter);
/*		printf("instead it got:");
		printf("%s\n",page_instance::contents[5].name);*/
		nextinstance=new(gummydummy_instance);
	}
	(*nextinstance).master=currentinstance;
	currentinstance=nextinstance;
};

void grow_pctext()
{
	CDXMLREAD_functype thisfunc;
	int txtoffset=currentinstance->getproperties("PCTEXT",&thisfunc);
	if (txtoffset==-1)
	{
		return;
	}
	char * txtpos=((char*)currentinstance)+txtoffset;
	thisfunc(parameterstring,txtpos);
}

inline void concludepctext()
{
	parameterstring[parameterstring_length]=0;
	parameterstring_length=0;
	grow_pctext();
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
	int iitemnumber;
	CDXMLREAD_functype thisfunc;
	if (strcmp(parameterstring,"PCTEXTcounter")==0)
	{
		fprintf(stderr,"Hacking attempt detected (PCSTRING)!");exit(1);
	}
	int suboffset=(currentinstance->getproperties(parameterstring,&thisfunc,&iitemnumber));
	if (suboffset!=-1)
	{
		AUTOSTRUCT_EXISTS_SET((*currentinstance),iitemnumber);
		thisfunc(paramvaluestring,((char*)currentinstance)+suboffset);
	}
	else
	{
//		fprintf(stderr,"%s has no parameter named %s\n",currentinstance->getName(),parameterstring);
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
	intl fsmint=0; //0: in_nothing. 1: bracket-opening 2: Tagreading 3: parameterstringreading 4: bracket-closing,i.e. start PCTEXT reading 5: Qmark-ignoring 7: waiting_for_tag_end 8: Addstring - Hyphenation 9: After equals symbol 10 PCTEXTstringreading.
	char ichar='A';
	bool bexittag=0;
	tagnamestring_length=0;
	parameterstring_length=0;
	paramvaluestring_length=0;
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
					fprintf(stderr,"Error: only slash in brackets!");exit(1);
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
						fprintf(stderr,"Error: The Tag to end was defined by another /");exit(1);
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
					fprintf(stderr,"error: exiting tag had also exit on end");exit(1);
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
					fprintf(stderr,"Error: Starting the Hypenation too late");exit(1);
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
				parameterstring[0]=ichar;
				parameterstring_length=1;
				fsmint=10;
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
			fprintf(stderr,"Error: \">\" expected at end of tag");
			#ifdef DEBUG
				fprintf(stderr,"%%-> %llX <-i\n",debugcounter);
			#endif
			for (int ilv1=0;ilv1<15;ilv1++)
			{
				fread(&ichar,1,1,infile);
				fprintf(stderr,"%c",ichar);
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
			paramvaluestring[paramvaluestring_length++]=ichar;//TODO: length restriction
		break;
		case 10:
			if (ichar=='<')
			{
				concludepctext();
				fsmint=1;
			}
			else
			{
				parameterstring[parameterstring_length++]=ichar;//TODO: length restriction
//TODO:				upon length restriction, don't forget that ampersand escapes may not be interrupted.
			}
		break;
		default:
		fprintf(stderr,"Error: Internal error!Invalid fsmint!!!!:%llX",fsmint);exit(1);
	}
	if (!feof(infile))
	{
		goto iback;
	}
	if (strcmp((*currentinstance).getName(),"Total_Document")!=0)
	{
		fprintf(stderr,"Error: Premature End of File! You are still in a %s",(*currentinstance).getName());exit(1);
	}
}
char valuestring[stringlength+1];
long long fullu64=0xFFFFFFFFFFFFFFFF;
void output_object(FILE * outfile,basic_instance * iinstance)
{
	_i32 propertycount=(*iinstance)._->properties_count;
	_u32 * ipointer=(*iinstance).getINTERNALPropertyexistflags();
	_u32 existflags;
	if (ipointer==NULL) existflags=*(_u32*)&fullu64; else existflags=*ipointer;
	for (int ilv1=0;ilv1<propertycount;ilv1++)
	{
		if (existflags & (1<<ilv1))
		{
			valuestring[stringlength]=0;
			strncpy(valuestring,"0",stringlength);
			fprintf(outfile,"%s=\"%s\"\n ",(*iinstance)._->properties[ilv1].name,valuestring);
		}
	}
}
struct n_instance;
extern multilist<n_instance> * glob_n_multilist;
void output_fsm(FILE * outfile,int usage)
{
	for (int ilv1=0;ilv1<multilist_count;ilv1++)
	{
		if (multilistlist[ilv1].usage==usage)
		{
			for (int ilv2=0;ilv2<multilistlist[ilv1].instance->filllevel;ilv2++)
			{
				output_object(outfile,&((*(multilistlist[ilv1].instance))[ilv2]));
			}
		}
	}
}
