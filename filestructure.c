//This generates the hxx file with the filestructure object definitions from filestructure.draft.
//Because this renders objects somehow obsolete, The main routine theoretically could be converted to ansi c99.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define stringlength 256
#define maxunits 256
#define bufferlength 65536
#ifdef BITMODE64
#define intl long long
#endif
#ifdef BITMODE32
#define intl long
#endif
char contents[maxunits][stringlength+1];
char properties[maxunits][stringlength+1];
char name[stringlength+1];
int namelength=0;
int properties_count=0;
int contents_count=0;
int contents_length[maxunits];
int properties_length[maxunits];
char properties_types[maxunits][stringlength+1];
intl properties_type_nrs[maxunits];
char enums_registered[maxunits][stringlength+1];
int enums_registered_count=0;
char helpbuffer[65536];
char resetmode_outline[][31]={{"%1$n"},{"%2$s=0;\n%1$n"},{"clear_%3$s(%2$s);\n%1$n"},{"%2$s.a=0;\n%1$n"},{"%2$s=1;\n%1$n"},{"%2$s=0;\n%1$n"},{"%2$s.count=0;\n%1$n"}};//very problematic: each operation must tell its length!
//ATTENTION: the second matrix array size is critical: if it is too low, the strings are simply cut off!
char * helpbufferpos;

char register_enum(const char * input)
{
	int ilv1;
	for (int ilv1=0;ilv1<enums_registered_count;ilv1++)
	{
		if (strcmp(enums_registered[ilv1],input)==0)
		{
			return 0;
		}
	}
	strcpy(enums_registered[enums_registered_count],input);
	enums_registered_count++;
	return 1;
}

void main(void)
{
	FILE * infile,*outfile;
	char ihv1;
	int helpbufferreturnvalue;
	infile=fopen("filestructure.draft","r");
	outfile=fopen("filestructure.hxx","w");
	helpbufferpos=&helpbuffer[0];
	helpbuffer[0]=0;
	lineback:
	namelength=0;
	properties_count=0;
	contents_count=0;
	symbolback1:
	fread(&(name[namelength]),1,1,infile);
	if (feof(infile))
	{
		goto done;
	}
	if (name[namelength]!=';')
	{
		namelength++;
		goto symbolback1;
	}
	name[namelength]=0;
	contentsback:
	contents_length[contents_count]=0;
	symbolback2:
	fread(&ihv1,1,1,infile);
	if (ihv1==';')
	{
		goto contentsdone;
	}
	contents[contents_count][contents_length[contents_count]]=ihv1;
	if (contents[contents_count][contents_length[contents_count]]!=' ')
	{
		contents_length[contents_count]++;
		goto symbolback2;
	}
	contents[contents_count][contents_length[contents_count]]=0;
	contents_count++;
	goto contentsback;
	contentsdone:
	propertiesback:
	strcpy(properties_types[properties_count],"_i32");
	properties_type_nrs[properties_count]=0;
	fread(&ihv1,1,1,infile);
	if (ihv1==';')
	{
		goto propertiesdone;
	}
	switch(ihv1)
	{
		case 's' : strcpy(properties_types[properties_count],"cdx_String");properties_type_nrs[properties_count]=2;break;
		case '0' : strcpy(properties_types[properties_count],"_i32");properties_type_nrs[properties_count]=1;break;
		case '1' : strcpy(properties_types[properties_count],"float");properties_type_nrs[properties_count]=1;break;
		case '2' : strcpy(properties_types[properties_count],"cdx_Point2D");properties_type_nrs[properties_count]=2;break;
		case '3' : strcpy(properties_types[properties_count],"cdx_Point3D");properties_type_nrs[properties_count]=2;break;
		case '4' : strcpy(properties_types[properties_count],"cdx_Rectangle");properties_type_nrs[properties_count]=2;break;
		case '5' : strcpy(properties_types[properties_count],"cdx_Coordinate");properties_type_nrs[properties_count]=1;break;
		case '#' : strcpy(properties_types[properties_count],"_i32");properties_type_nrs[properties_count]=4;break;
		case '~' : strcpy(properties_types[properties_count],"cdx_Bezierpoints");properties_type_nrs[properties_count]=6;break;
		case '!' : strcpy(properties_types[properties_count],"_i32");properties_type_nrs[properties_count]=5;break;//an ENUM
		case '\\' : 
		;
		int thisnamelength=0;
		propertiesname_back:
		fread(&properties_types[properties_count][thisnamelength],1,1,infile);
		if (properties_types[properties_count][thisnamelength]!=' ')
		{
			thisnamelength++;
			goto propertiesname_back;
		}
		properties_types[properties_count][thisnamelength]=0;properties_type_nrs[properties_count]=0;
		goto start_symbolreading3;
	}
	fread(&ihv1,1,1,infile);//should be a blank
	if (ihv1!=' ') {printf("Something went wrong defining %s - no space after type! ",name);exit(1);};
	start_symbolreading3:
	properties_length[properties_count]=0;
	symbolback3:
	fread(&ihv1,1,1,infile);
	if (ihv1==';')
	{
		if (properties_length[properties_count]>0)
		{
			printf("Something went wrong defining %s - no space after typename!",name);exit(1);
		}
		goto propertiesdone;
	}
	properties[properties_count][properties_length[properties_count]]=ihv1;
	if (properties[properties_count][properties_length[properties_count]]!=' ')
	{
		properties_length[properties_count]++;
		goto symbolback3;
	}
	properties[properties_count][properties_length[properties_count]]=0;
	properties_count++;
	printf("..%s..\n",properties[properties_count-1]);
	goto propertiesback;
	propertiesdone:
	fprintf(outfile,"struct %s_instance:basic_instance\n{\n        char * getName(){static char name[]=\"%s\";return (char*)&name;}\n",name,name);
	for (int ilv1=0;ilv1<contents_count;ilv1++)
	{
		fprintf(outfile,"        basicmultilistreference * %s;\n",contents[ilv1],contents[ilv1]);
	}
	for (int ilv1=0;ilv1<properties_count;ilv1++)
	{
		fprintf(outfile,"        %s %s;\n",properties_types[ilv1],properties[ilv1]);
	}
	fprintf(outfile,"        AUTOSTRUCT_GET_ROUTINE(contents,%i)\n        AUTOSTRUCT_PROPERTY_ROUTINE(%i)\n        %s_instance();\n        ~%s_instance(){}\n};\nsuperconstellation %s_instance::contents[]={\n",contents_count,properties_count,name,name,name);
	for (int ilv1=0;ilv1<contents_count;ilv1++)
	{
		fprintf(outfile,"{\"%s\",offsetof(%s_instance,%s)}%s\n",contents[ilv1],name,contents[ilv1],(ilv1==contents_count-1) ? "" : ",");
	}
	fprintf(outfile,"};\n");
	for (int ilv1=0;ilv1<properties_count;ilv1++)
	{
		if ((properties_type_nrs[ilv1]==5) || (properties_type_nrs[ilv1]==4))
		{
			printf("HHXHX");
			if (register_enum(properties[ilv1]))
			{
				fprintf(outfile,"int __attribute__((sysv_abi))CDXMLREAD_ENUM_%s(char * input,void * output)\n{\n        \
	*((_i32 *)output)=get_bienum(CDXML_%s,input,CDXML_%s_max);\n}\n",properties[ilv1],properties[ilv1],properties[ilv1]);
			}
		}
	}
	fprintf(outfile,"superconstellation %s_instance::properties[]={\n",name);
	for (int ilv1=0;ilv1<properties_count;ilv1++)
	{
		if ((properties_type_nrs[ilv1]==5) || (properties_type_nrs[ilv1]==4))
		{
			fprintf(outfile,"{\"%s\",offsetof(%s_instance,%s),CDXMLREAD_ENUM_%s}%s\n",properties[ilv1],name,properties[ilv1],properties[ilv1],(ilv1==properties_count-1) ? "" : ",");
		}
		else
		{
			fprintf(outfile,"{\"%s\",offsetof(%s_instance,%s),CDXMLREAD_%s}%s\n",properties[ilv1],name,properties[ilv1],properties_types[ilv1],(ilv1==properties_count-1) ? "" : ",");
		}
	}
	fprintf(outfile,"};\n");
	sprintf(helpbufferpos,"%s_instance::%s_instance()\n{\n%n",name,name,&helpbufferreturnvalue);
	helpbufferpos+=helpbufferreturnvalue;
	(*helpbufferpos)=0;
	for (int ilv1=0;ilv1<contents_count;ilv1++)
	{
		sprintf(helpbufferpos,"        %s=new(multilistreference<%s_instance>);\n%n",contents[ilv1],contents[ilv1],&helpbufferreturnvalue);
		helpbufferpos+=helpbufferreturnvalue;
		(*helpbufferpos)=0;
	}
	for (int ilv1=0;ilv1<properties_count;ilv1++)
	{
		sprintf(helpbufferpos,"        %n",&helpbufferreturnvalue);
		helpbufferpos+=helpbufferreturnvalue;
		(*helpbufferpos)=0;
		sprintf(helpbufferpos,resetmode_outline[properties_type_nrs[ilv1]],&helpbufferreturnvalue,properties[ilv1],properties_types[ilv1]);
		helpbufferpos+=helpbufferreturnvalue;
		(*helpbufferpos)=0;
	}
	sprintf(helpbufferpos,"}\n%n",&helpbufferreturnvalue);
	helpbufferpos+=helpbufferreturnvalue;
	(*helpbufferpos)=0;
	sprintf(helpbufferpos,"xml_element_set<%s_instance> %s_list=xml_element_set<%s_instance>();\n%n",name,name,name,&helpbufferreturnvalue);
	helpbufferpos+=helpbufferreturnvalue;
	(*helpbufferpos)=0;
	if (fread(&ihv1,1,1,infile)==0){goto done;};
	if (ihv1!='\n') {if(!feof(infile)){printf("no breakline");exit(1);}else goto done;}
	if (!feof(infile))
	{
		goto lineback;
	}
	done:
	fprintf(outfile,"%s",helpbuffer);
	fclose(infile);
	fclose(outfile);
}
