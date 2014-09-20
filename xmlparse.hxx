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
		printf("%s vs. %s !\n",tagnamestring,currentinstance->getName());
		nextinstance=new(gummydummy_instance);
		nextinstance->_=NULL;
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
void scoopparam_bin()
{
	int iitemnumber;
	CDXMLREAD_functype thisfunc;
	if (strcmp(parameterstring,"PCTEXTcounter")==0)
	{
		fprintf(stderr,"Hacking attempt detected (PCSTRING)!");exit(1);
	}
	if (currentinstance->_==NULL) return;
	superconstellation * tl_superconstellation=getsuperconstellation_p(currentinstance->_,parameterstring,&iitemnumber);
	if (tl_superconstellation!=NULL)
	{
		AUTOSTRUCT_EXISTS_SET((*currentinstance),iitemnumber);
		tl_superconstellation->binreaddelegate(paramvaluestring,((char*)currentinstance)+tl_superconstellation->ref);
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
	if (strcmp(currentinstance->getName(),"Total_Document")==0)
	{
		currentinstance=NULL;
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
	if (currentinstance==NULL) return;
	if (!feof(infile))
	{
		goto iback;
	}
	if (strcmp((*currentinstance).getName(),"Total_Document")!=0)
	{
		fprintf(stderr,"Error: Premature End of File! You are still in a %s",(*currentinstance).getName());exit(1);
	}
}
int debug_demonstratefsm_recursion_depth=0;
void indent()
{
	for (int ilv1=0;ilv1<debug_demonstratefsm_recursion_depth;ilv1++)
	{
		printf("  ");
	}
}
int input_recursion(FILE * infile)
{
	//TODO: cdx generic list
	_i16 itype=0;
	_i32 iid=0;
	paramvaluestring_length=0;
	debug_demonstratefsm_recursion_depth+=1;
	fread(&itype,2,1,infile);
	if (itype==0) {debug_demonstratefsm_recursion_depth-=1; return 0;}
	if ((itype&0x8000)==0)
	{
		paramvaluestring_length=0;
		fread(&paramvaluestring_length,2,1,infile);//ENDIAN
		indent();
		char * tl_name=lookup_bienum(CDXML_propertycodes_List,CDXML_propertycodes_ListSize,itype);
		if (paramvaluestring_length<0) return 0;
		if (tl_name)
		{
			strcpy(parameterstring,tl_name);
			if (paramvaluestring_length>stringlength) {fprintf(stderr,"File overflow!");*(char*)0=0;exit(1);}
			fread(&paramvaluestring,paramvaluestring_length,1,infile);
			_u8 padding=0x00;
			for (int ilv1=0;ilv1<sizeof(list_padlist)/sizeof(_i32);ilv1++)
			{
				if (itype==list_padlist[ilv1]) if (paramvaluestring[paramvaluestring_length-1] & 0x80) padding=0xFF;
			}
			for (int ilv1=paramvaluestring_length;ilv1<min(paramvaluestring_length+4,stringlength+1);ilv1++)
			{
				paramvaluestring[ilv1]=padding;
			}
			scoopparam_bin();
			printf("%s:%i:%llX\n",tl_name,(int)paramvaluestring_length,*(_u64*)paramvaluestring);
		}
		else
		{
			fread(&paramvaluestring,paramvaluestring_length,1,infile);
			printf("%04hX:%i:%llX\n",itype,(int)paramvaluestring_length,*(_u64*)paramvaluestring);
		}
		debug_demonstratefsm_recursion_depth-=1;
		return 1;
	}
	//From here, it is known to be an object
	fread(&iid,4,1,infile);
	indent();
	char * tl_name=lookup_bienum(CDXML_objectcodes_List,CDXML_objectcodes_ListSize,itype);
	if (tl_name)
	{
		strcpy(tagnamestring,tl_name);
		printf("\e[31m%s:\e[0m%i\n",tl_name,iid);
	}
	else
	{
		strcpy(tagnamestring,"gummydummy");
		printf("\e[31m%04hX:\e[0m%i\n",itype,iid);
	}
	tagnamestring_length=strlen(tagnamestring);
	entertag();
	strcpy(parameterstring,"id");memcpy(paramvaluestring,(char*)&iid,4);
	scoopparam_bin();
	indent();
	printf("you are in a %s\n",(*currentinstance).getName());
	while (input_recursion(infile))
	{
	}
	exittag();
	debug_demonstratefsm_recursion_depth-=1;
	return 1;
}
void input_bin(FILE* infile)
{
	fseek(infile,12,SEEK_SET);
	parameterstring_length=0;
	paramvaluestring_length=0;
	tagnamestring_length=0;
	while (!input_recursion(infile));
}
long long fullu64=0xFFFFFFFFFFFFFFFF;
void output_object(FILE * outfile,basic_instance * iinstance)
{
	char hadhadcontent=0;
	char haspctext=0;
	char * PCTEXT_pointer;
	_i32 propertycount=(*iinstance)._->properties_count;
	_i32 contentcount=(*iinstance)._->contents_count;
	_u32 * ipointer=(*iinstance).getINTERNALPropertyexistflags();
	_u32 existflags;
	if (ipointer==NULL) existflags=*(_u32*)&fullu64; else existflags=*ipointer;
	fprintf(outfile,"<%s\n",(*iinstance).getName());
	for (int ilv1=0;ilv1<propertycount;ilv1++)
	{
		if (existflags & (1<<ilv1))
		{
			int ipropertypos=(*iinstance)._->properties[ilv1].ref;
			char * name=(*iinstance)._->properties[ilv1].name;
			if (strcmp(name,"PCTEXT")==0)
			{
				haspctext=1;
				PCTEXT_pointer=((char*)iinstance)+ipropertypos;
			}
			else
			{
				fprintf(outfile," %s=\"",name);
				(*iinstance)._->properties[ilv1].writedelegate(((char*)iinstance)+ipropertypos,outfile);
				fprintf(outfile,"\"\n");
			}
		}
	}
	if (haspctext)
	{
		fprintf(outfile,">");
		writefrombuffer(outfile,*(char**)PCTEXT_pointer);
	}
	hadhadcontent=haspctext;
	for (int ilv1=0;ilv1<contentcount;ilv1++)
	{
		basicmultilistreference * tlmultilistreference=*((basicmultilistreference**)(((char*)iinstance)+((*iinstance)._->contents[ilv1].ref)));
		int tlcount=(*tlmultilistreference).count_in_it;
		for (int ilv2=0;ilv2<tlcount;ilv2++)
		{
			if (hadhadcontent==0)
			{
				fprintf(outfile,">");
				hadhadcontent=1;
			}
			output_object(outfile,(*tlmultilistreference)[ilv2]);
		}
	}
	if (hadhadcontent)
	{
		fprintf(outfile,"</%s>",(*iinstance).getName());
	}
	else
	{
		fprintf(outfile,"/>");
	}
	return;
}
int output_object_bin(FILE * outfile,basic_instance * iinstance)
{
	char * PCTEXT_pointer;
	intl dummy=0;
	char * idatapointer;
	superconstellation * isuperconstellation;
	_i32 propertycount=(*iinstance)._->properties_count;
	_i32 contentcount=(*iinstance)._->contents_count;
	_u32 * ipointer=(*iinstance).getINTERNALPropertyexistflags();
	_u32 existflags;
	if (ipointer==NULL) existflags=*(_u32*)&fullu64; else existflags=*ipointer;
	dummy=get_bienum(CDXML_objectcodes_List,(*iinstance).getName(),CDXML_objectcodes_ListSize);
	if (dummy==0)
	{
		basic_instance * imaster=(*iinstance).master;
		isuperconstellation=getsuperconstellation_p(imaster->_,(*iinstance).getName(),NULL);
		if (isuperconstellation) 
		{
			idatapointer=((char*)imaster)+isuperconstellation->ref;
			dummy=get_bienum(CDXML_propertycodes_List,(*iinstance).getName(),CDXML_propertycodes_ListSize);
			fwrite(&dummy,2,1,outfile);
			isuperconstellation->binwritedelegate(idatapointer,outfile);
		}
		return 0;
	}
	fwrite(&dummy,2,1,outfile);
	dummy=0;
	idatapointer=(char*)&dummy;
	isuperconstellation=getsuperconstellation_p(iinstance->_,"id",NULL);
	if (isuperconstellation) 
	{
		idatapointer=((char*)iinstance)+isuperconstellation->ref;
	}
	fwrite(idatapointer,4,1,outfile);
	for (int ilv1=0;ilv1<propertycount;ilv1++)
	{
		if (existflags & (1<<ilv1))
		{
			int ipropertypos=(*iinstance)._->properties[ilv1].ref;
			char * name=(*iinstance)._->properties[ilv1].name;
			for (int ilv1=0;ilv1<CDXML_propertycodes_ListSize;ilv1++)
			{
				if (strcmp(name,CDXML_propertycodes_List[ilv1].name)==0)
				{
					dummy=CDXML_propertycodes_List[ilv1].number;
					fwrite(&dummy,2,1,outfile);
					goto idone;
				}
			}
			if (strcmp(name,"PCTEXT")==0)
			{
				goto ignore_item;
			}
			if (strcmp(name,"id")==0)
			{
				goto ignore_item;
			}
			if (strcmp(name,"IGOTYOU")==0)
			{
				goto ignore_item;
			}
			printf("Unknown number of Element named %s\n",name);goto ignore_item;
			idone:;
			(*iinstance)._->properties[ilv1].binwritedelegate(((char*)iinstance)+ipropertypos,outfile);
			ignore_item:;
		}
	}
	for (int ilv1=0;ilv1<contentcount;ilv1++)
	{
		basicmultilistreference * tlmultilistreference=*((basicmultilistreference**)(((char*)iinstance)+((*iinstance)._->contents[ilv1].ref)));
		int tlcount=(*tlmultilistreference).count_in_it;
		for (int ilv2=0;ilv2<tlcount;ilv2++)
		{
			if (output_object_bin(outfile,(*tlmultilistreference)[ilv2])==0) goto ioneforall;
		}
		ioneforall:;
	}
	dummy=0;
	fwrite(&dummy,2,1,outfile);
	return 1;
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
void output_fsm_bin(FILE * outfile,int usage)
{
	for (int ilv1=0;ilv1<multilist_count;ilv1++)
	{
		if (multilistlist[ilv1].usage==usage)
		{
			for (int ilv2=0;ilv2<multilistlist[ilv1].instance->filllevel;ilv2++)
			{
				output_object_bin(outfile,&((*(multilistlist[ilv1].instance))[ilv2]));
			}
		}
	}
}
