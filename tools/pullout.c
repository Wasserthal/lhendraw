#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
FILE * stringfile;//a binary
FILE * structfile;//contains what is left of the menuitem structures and control code.
FILE * reflectfile;
//TODO: create a reflection block, which lists up ANY automatically generated file structure
typedef struct menuitem
{
	int x,y;
	int picno;
	char name[80];
	int lmbmode;//0: nothing 1: select a tool 2: toggle variable 3: execute a function 4: set/clear a multi-boolean variable 5: set single boolean of multi-boolean variable 6: set variable to value 7: act like 3 but look like 6. 8: set variable to value. when left-clicked, call LMBfunction with name as parameter and a string of toolnr as value. 9: like 8, but reset variable to 1, not to 0. 10: Like 8 but like 5. 0x50: File menu 0x1XX: click-drag button 0x107: Tool, immediately used for dragging 0x2XX: table 0x3XX: text input field 0x400: popup with file formats
	int rmbmode;//0: see above, but for right mouse button
	int toolnr;
	char explanation[1024];
	char variablename[80];
	char LMBfunction[80];//LMB: Name of the variable which is influenced/function name on lmb
	char RMBfunction[80];//same When clicking with RMB
	int bgcolor; //color of the lights when pressed
	int maxx,maxy;//right-bottom corner
}menuitem;
typedef struct reflective_head
{
	int type;// 1: menuitem TODO: 2: xmldata structure 3: 
	char name[20];
}reflectfile_head;
char pulloutlisting_string[]="\n"
"typedef struct AUTOSTRUCT_PULLOUTLISTING_\n"
"{\n"
"	int x,y;\n"
"	int picno;\n"
"	const char * name;\n"
"	int lmbmode;\n"
"	int rmbmode;\n"
"	int toolnr;\n"
"	const char * explanation;\n"
"	const char * variablename;\n"
"	catalogized_command_functype LMB_function;\n"
"	catalogized_command_functype RMB_function;\n"//Either called on RMB or, amongst sliders, for every item after calling LMBfunction
"	int bgcolor;\n"
"	void * variable;\n//Either char, at 2 or _u32 at 4 \n"
"	int maxx,maxy;\n"
"}AUTOSTRUCT_PULLOUTLISTING_;\n"
"_u32 nope;\n";
#include "../generated/pullout.hxx"
int stringlist_count=0;
char nullstring[]="NULL";
char nopestring[]="nope";
const char * stringifnull(const char * instring)
{
	if (instring!=NULL)
	{
		if (instring[0]!=0)
		{
			return instring;
		}
	}
	return nullstring;
}
const char * stringifnope(const char * instring)
{
	if (instring!=NULL)
	{
		if (instring[0]!=0)
		{
			return instring;
		}
	}
	return nopestring;
}
void addstring(char * input)
{
	int ilv1=0;
	fprintf(stringfile,"\"");
	iback:
	if (input[ilv1]=='\n')
	{
		fprintf(stringfile,"\\n\"\n\"");
		goto idone;
	}
	if ((input[ilv1]<0x20) && (input[ilv1]>=0)) goto ibyte;
	if ((input[ilv1]=='"') || (input[ilv1]==0x7F) || (input[ilv1]=='\\')) goto ibyte;
	fprintf(stringfile,"%c",input[ilv1]);
	goto idone;
	ibyte:
	fprintf(stringfile,"\\x%02X",input[ilv1]);
	idone:
	if (input[ilv1]==0)
	{
		stringlist_count+=ilv1+1;
		fprintf(stringfile,"\"\n");
		return;
	}
	ilv1++;
	goto iback;
}
void domenu(menuitem * input,int count,char * name)
{
	int ilv1;
	fprintf(structfile,"AUTOSTRUCT_PULLOUTLISTING_ AUTOSTRUCT_PULLOUTLISTING_%s[]={\n",name);
	for (ilv1=0;ilv1<count;ilv1++)
	{
		fprintf(structfile,"{%i,%i,%i,\nAUTOSTRUCT_STRINGLIST_PULLOUT+%i\n,%i,%i,%i,\n",input[ilv1].x,input[ilv1].y,input[ilv1].picno,
		stringlist_count,input[ilv1].lmbmode,input[ilv1].rmbmode,input[ilv1].toolnr);
		addstring(input[ilv1].name);
		fprintf(structfile,"AUTOSTRUCT_STRINGLIST_PULLOUT+%i,",stringlist_count);
		addstring(input[ilv1].explanation);
		fprintf(structfile,"AUTOSTRUCT_STRINGLIST_PULLOUT+%i,",stringlist_count);
		addstring(input[ilv1].variablename);
		fprintf(structfile,"%s,%s,0x%08X,&(%s),%i,%i},",stringifnull(input[ilv1].LMBfunction),stringifnull(input[ilv1].RMBfunction),input[ilv1].bgcolor,stringifnope(input[ilv1].variablename),input[ilv1].maxx,input[ilv1].maxy);
	}
	fprintf(reflectfile,"{\"%s\",%i,%i,&AUTOSTRUCT_PULLOUTLISTING_%s,sizeof(AUTOSTRUCT_PULLOUTLISTING_%s)},",name,count,count,name,name);
	fprintf(structfile,"};\nint AUTOSTRUCT_PULLOUTLISTING_%s_Size=sizeof(AUTOSTRUCT_PULLOUTLISTING_%s)/sizeof(AUTOSTRUCT_PULLOUTLISTING_);",name,name);
	return;
}


int main(int argc,char ** argv)
{
	stringfile=fopen("./generated/pullout_stringfile.hxx","w+");
	structfile=fopen("./generated/pullout_structfile.hxx","w+");
	reflectfile=fopen("./generated/pullout_reflectfile.hxx","w+");
	fprintf(structfile,"_i32 nope;");
	fprintf(stringfile,"char AUTOSTRUCT_STRINGLIST_PULLOUT[]=\n");
#include "../generated/pulloutmatic.hxx"
	fprintf(stringfile,";\n");
	
	
	fclose(structfile);
	fclose(stringfile);
	fclose(reflectfile);
	return 0;
}
