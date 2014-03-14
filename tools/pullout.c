#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
FILE * stringfile;//a binary
FILE * structfile;//contains what is left of the menuitem structures and control code.
FILE * functionfile;//contains automatically generated functions of menu items
FILE * reflectfile;
//TODO: create a reflection block, which lists up ANY automatically generated file structure
typedef struct menuitem
{
	int x,y;
	int picno;
	char description[80];
	int lmbmode;//0: nothing 1: select a tool 2: toggle variable 3: execute a function
	int rmbmode;//0: nothing 1: select a tool 2: toggle variable 3: execute a function
	int toolnr;
	char explanation[1024];
	char variablename[80];
	char LMBfunction[80];//LMB: Name of the variable which is influenced/function name on lmb
	char RMBfunction[80];//same When clicking with RMB
	int bgcolor; //color of the lights when pressed
}menuitem;
typedef struct reflective_head
{
	int type;// 1: menuitem TODO: 2: xmldata structure 3: 
	char name[20];
}reflectfile_head;
char pulloutlisting_string[]="\n"
"typedef int __attribute__((sysv_abi))(*AUTOSTRUCT_GETSET_FUNCTYPE)(char state,int input);//1: set 2: get 4: toggle\n"
"typedef struct AUTOSTRUCT_PULLOUTLISTING_\n"
"{\n"
"	int x,y;\n"
"	int picno;\n"
"	char * description;\n"
"	int lmbmode;\n"
"	int rmbmode;\n"
"	int toolnr;\n"
"	char * explanation;\n"
"	char * variablename;\n"
"	char * LMBfunction;\n"
"	char * RMBfunction;\n"
"	int bgcolor;\n"
"	AUTOSTRUCT_GETSET_FUNCTYPE getflag;\n"
"}AUTOSTRUCT_PULLOUTLISTING_;\n";
#include "../toolbox.pullout.hxx"
int stringlist_count=0;

void addstring(char * input)
{
	int ilv1=0;
	fprintf(stringfile,"\"");
	iback:
	if (input[ilv1]=='\n')
	{
		fprintf(stringfile,"\\n\"\n\"");
		stringlist_count+=1;
		goto idone;
	}
	if ((input[ilv1]<0x20) && (input[ilv1]>=0)) goto ibyte;
	if ((input[ilv1]=='"') || (input[ilv1]==0x7F) || (input[ilv1]=='\\')) goto ibyte;
	fprintf(stringfile,"%c",input[ilv1]);
	stringlist_count++;
	goto idone;
	ibyte:
	fprintf(stringfile,"\\x%02X",input[ilv1]);
	stringlist_count+=5;
	idone:
	if (input[ilv1]==0)
	{
		fprintf(stringfile,"\"\n");
		return;
	}
	ilv1++;goto iback;
}
void domenu(menuitem * input,int count,char * name)
{
	int ilv1;
	fprintf(reflectfile,"{1,%s},",name);
	fprintf(structfile,"AUTOSTRUCT_PULLOUTLISTING_ AUTOSTRUCT_PULLOUTLISTING_%s[]={\n",name);
	for (ilv1=0;ilv1<count;ilv1++)
	{
		fprintf(structfile,"{%i,%i,%i,\nAUTOSTRUCT_STRINGLIST_PULLOUT+%i\n,%i,%i,%i,\n",input[ilv1].x,input[ilv1].y,input[ilv1].picno,
		stringlist_count,input[ilv1].lmbmode,input[ilv1].rmbmode,input[ilv1].toolnr);
		addstring(input[ilv1].description);
		fprintf(structfile,"NULL,NULL,NULL,NULL,0x%06X},",input[ilv1].bgcolor);
	}
	fprintf(structfile,"};\n");
	return;
}


void main(int argc,char ** argv)
{
	stringfile=fopen("./generated/pullout_stringfile.hxx","w+");
	structfile=fopen("./generated/pullout_structfile.hxx","w+");
	reflectfile=fopen("./generated/pullout_reflectfile.hxx","w+");
	fprintf(structfile,"%s",pulloutlisting_string);
	fprintf(stringfile,"char AUTOSTRUCT_STRINGLIST_PULLOUT[]=\n");
	domenu(pullout_toolbox,sizeof(pullout_toolbox)/sizeof(menuitem),"toolbox");
	fprintf(stringfile,";\n");
	
	
	fclose(structfile);
	fclose(stringfile);
	fclose(reflectfile);
}
