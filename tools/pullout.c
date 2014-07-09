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
	int lmbmode;//0: nothing 1: select a tool 2: toggle variable 3: execute a function 4: set/clear a multi-boolean variable 5: set single boolean of multi-boolean variable 6: set variable to value 0x1XX click-drag button
	int rmbmode;//0: nothing 1: select a tool 2: toggle variable 3: execute a function 4: set/clear a multi-boolean variable
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
"	char * name;\n"
"	int lmbmode;\n"
"	int rmbmode;\n"
"	int toolnr;\n"
"	char * explanation;\n"
"	char * variablename;\n"
"	char * LMBfunction;\n"
"	char * RMBfunction;\n"//Either called on RMB or, amongst sliders, for every item after calling LMBfunction
"	catalogized_command_functype LMB_function;\n"
"	catalogized_command_functype RMB_function;\n"
"	int bgcolor;\n"
"	void * variable;\n//Either char, at 2 or _u32 at 4 \n"
"	int maxx,maxy;\n"
"}AUTOSTRUCT_PULLOUTLISTING_;\n"
"_u32 nope;\n";
#include "../toolbox.pullout.hxx"
#include "../toolspecific_BOND.pullout.hxx"
#include "../toolspecific_ATTRIBUTES.pullout.hxx"
#include "../submenu_toolbox_M1.pullout.hxx"
#include "../submenu_toolbox_M2.pullout.hxx"
#include "../submenu_toolbox_M3.pullout.hxx"
#include "../submenu_toolbox_M4.pullout.hxx"
#include "../submenu_toolbox_M5.pullout.hxx"
#include "../submenu_toolbox_M6.pullout.hxx"
#include "../filedlg_buttons_save.pullout.hxx"
#include "../filedlg_buttons_load.pullout.hxx"
#include "../filedlg_lists.pullout.hxx"
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
		fprintf(structfile,"NULL,NULL,\nAUTOSTRUCT_STRINGLIST_PULLOUT+%i,\n",stringlist_count);
		addstring(input[ilv1].LMBfunction);
		fprintf(structfile,"\nAUTOSTRUCT_STRINGLIST_PULLOUT+%i\n,%s,%s,0x%08X,&(%s),%i,%i},",stringlist_count,stringifnull(input[ilv1].LMBfunction),stringifnull(input[ilv1].RMBfunction),input[ilv1].bgcolor,stringifnope(input[ilv1].variablename),input[ilv1].maxx,input[ilv1].maxy);
		addstring(input[ilv1].RMBfunction);
	}
	fprintf(reflectfile,"{\"%s\",%i,%i,&AUTOSTRUCT_PULLOUTLISTING_%s,sizeof(AUTOSTRUCT_PULLOUTLISTING_%s)},",name,count,count,name,name);
	fprintf(structfile,"};\nint AUTOSTRUCT_PULLOUTLISTING_%s_Size=sizeof(AUTOSTRUCT_PULLOUTLISTING_%s)/sizeof(AUTOSTRUCT_PULLOUTLISTING_);",name,name);
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
	domenu(pullout_toolspecific_BOND,sizeof(pullout_toolspecific_BOND)/sizeof(menuitem),"toolspecific_BOND");
	domenu(pullout_toolspecific_ATTRIBUTES,sizeof(pullout_toolspecific_ATTRIBUTES)/sizeof(menuitem),"toolspecific_ATTRIBUTES");
	domenu(pullout_submenu_toolbox_M1,sizeof(pullout_submenu_toolbox_M1)/sizeof(menuitem),"submenu_toolbox_M1");
	domenu(pullout_submenu_toolbox_M2,sizeof(pullout_submenu_toolbox_M2)/sizeof(menuitem),"submenu_toolbox_M2");
	domenu(pullout_submenu_toolbox_M3,sizeof(pullout_submenu_toolbox_M3)/sizeof(menuitem),"submenu_toolbox_M3");
	domenu(pullout_submenu_toolbox_M4,sizeof(pullout_submenu_toolbox_M4)/sizeof(menuitem),"submenu_toolbox_M4");
	domenu(pullout_submenu_toolbox_M5,sizeof(pullout_submenu_toolbox_M5)/sizeof(menuitem),"submenu_toolbox_M5");
	domenu(pullout_submenu_toolbox_M6,sizeof(pullout_submenu_toolbox_M6)/sizeof(menuitem),"submenu_toolbox_M6");
	domenu(pullout_filedlg_buttons_save,sizeof(pullout_filedlg_buttons_save)/sizeof(menuitem),"filedlg_buttons_save");
	domenu(pullout_filedlg_buttons_load,sizeof(pullout_filedlg_buttons_load)/sizeof(menuitem),"filedlg_buttons_load");
	domenu(pullout_filedlg_lists,sizeof(pullout_filedlg_lists)/sizeof(menuitem),"filedlg_lists");
	fprintf(stringfile,";\n");
	
	
	fclose(structfile);
	fclose(stringfile);
	fclose(reflectfile);
}
