#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
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
char helpbuffer[65536];
char * helpbufferpos;
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
	strcpy(properties_types[properties_count],"intl");
	fread(&ihv1,1,1,infile);
	if (ihv1==';')
	{
		goto propertiesdone;
	}
	switch(ihv1)
	{
		case 's' : strcpy(properties_types[properties_count],"string");break;
		case '0' : strcpy(properties_types[properties_count],"intl");break;
		case '1' : strcpy(properties_types[properties_count],"float");break;
		case '2' : strcpy(properties_types[properties_count],"cdx_point");break;
		case '4' : strcpy(properties_types[properties_count],"cdx_box");break;
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
		properties_types[properties_count][thisnamelength]=0;
		goto start_symbolreading3;
	}
	fread(&ihv1,1,1,infile);//should be a blank
	if (ihv1!=' ') {printf("Something went wrong - no space after type!");exit(1);};
	start_symbolreading3:
	properties_length[properties_count]=0;
	symbolback3:
	fread(&ihv1,1,1,infile);
	if (ihv1==';')
	{
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
	goto propertiesback;
	propertiesdone:
	fprintf(outfile,"struct %s_instance:basic_instance\n{\n",name);
	for (int ilv1=0;ilv1<contents_count;ilv1++)
	{
		fprintf(outfile,"        basicmultilistreference * %s;\n",contents[ilv1],contents[ilv1]);
	}
	for (int ilv1=0;ilv1<properties_count;ilv1++)
	{
		fprintf(outfile,"        %s %s;\n",properties_types[ilv1],properties[ilv1]);
	}
	fprintf(outfile,"        AUTOSTRUCT_GET_ROUTINE(contents)\n        AUTOSTRUCT_GET_ROUTINE(properties)\n        %s_instance();\n        ~%s_instance(){}\n};\nsuperconstellation %s_instance::contents[]={\n",name,name,name);
	for (int ilv1=0;ilv1<contents_count;ilv1++)
	{
		fprintf(outfile,"{\"%s\",offsetof(%s_instance,%s)}%s\n",contents[ilv1],name,contents[ilv1],(ilv1==properties_count-1) ? "" : ",");
	}
	fprintf(outfile,"};\nsuperconstellation %s_instance::properties[]={\n",name);
	for (int ilv1=0;ilv1<properties_count;ilv1++)
	{
		fprintf(outfile,"{\"%s\",offsetof(%s_instance,%s)}%s\n",properties[ilv1],name,properties[ilv1],(ilv1==properties_count-1) ? "" : ",");
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
