#include <stdio.h>
#include <string.h>
char options_line[1000];
FILE * outfile;
int currentitemcount=0;
char structname[1000];
char sentenumeric(char input)
{
	if ((input>='A') && (input<='Z')) return 1;
	if ((input>='a') && (input<='z')) return 1;
	if ((input>='0') && (input<='9')) return 1;
	if ((input=='_') || (input=='$')) return 1;
	return 0;
}
void dynamic_menu(char * substring)
{
	char variablename[1000];
	char * stringend=substring+strlen(substring);
	while ((*stringend)!=';') stringend--;
	*stringend=0;
	sprintf(variablename,"&(%s.%s)",structname,substring);
currentitemcount++;
fprintf(outfile,"\n\
{\n\
	320,%i,\n\
//	int x,y;\n\
	1,\n\
//	int picno;\n\
	\"%s\",\n\
//	char * name;\n\
	0x302,\n\
//	int lmbmode;\n\
	0x302,\n\
//	int rmbmode;\n\
	0,\n\
//	int toolnr;\n\
	\"%s\",\n\
//	char * explanation;\n\
	\"%s.%s\",\n\
//	char * variablename;\n\
	NULL,\n\
//	catalogized_command_functype LMB_function;\n\
	NULL,\n\
//	catalogized_command_functype RMB_function;\n\
	0xFF00,\n\
//	int bgcolor;\n\
	%s,\n\
//	void * variable;\n\
	480,%i,\n\
//	int maxx,maxy;\n\
},",currentitemcount*16,substring,substring,structname,substring,variablename,(currentitemcount+1)*16);

}
int main(int argc,char**argv)
{
	FILE * infile;
	outfile=fopen("./generated/pullout_structfile.hxx","a");
	fprintf(outfile,"%s","AUTOSTRUCT_PULLOUTLISTING_ AUTOSTRUCT_PULLOUTLISTING_options[]={");
	int iinput_fsm;
	int paramcounter=1;
	ifilerepeat:;
	infile=fopen(argv[paramcounter],"r");
	iinput_fsm=0;
	char * substring;
	iback:;
	for (int ilv1=0;ilv1<1000;ilv1++)
	{
		int tl_backval=fread(options_line+ilv1,1,1,infile);
		if (tl_backval==0) goto finish;
		if (options_line[ilv1]==10)
		{
			options_line[ilv1]=0;
			goto ilinedone;
		}
	}
	ilinedone:;
	switch (iinput_fsm)
	{
		case 0:
		if (strncmp("struct",options_line,6)==0)
		{
			int ilv1;
			strcpy(structname,options_line+7);
			for (ilv1=strlen(structname);structname[ilv1]!='_';ilv1--)
			{
				if ((sentenumeric(structname[ilv1]>='a')) || (ilv1==0))
				{
					iinput_fsm=0;
					goto iback;
				}
			}
			structname[ilv1]=0;
			iinput_fsm=1;
		}
		break;
		case 1:
		if (strstr(options_line,"{")!=NULL)
		{
			iinput_fsm=2;
		}
		break;
		case 2:
		if (strncmp(options_line,"	_i32 ",6)==0)
		{
			substring=options_line+6;
			dynamic_menu(substring);
			break;
		}
		if (strncmp(options_line,"	int ",5)==0)
		{
			substring=options_line+5;
			dynamic_menu(substring);
			break;
		}
		if (strstr(options_line,"}")!=NULL)
		{
			iinput_fsm=0;
		}
		break;
	}
	goto iback;
	finish:;
	fclose(infile);
	paramcounter++;
	if (paramcounter<argc) goto ifilerepeat;
	fprintf(outfile,"%s","};int AUTOSTRUCT_PULLOUTLISTING_options_Size=sizeof(AUTOSTRUCT_PULLOUTLISTING_options)/sizeof(AUTOSTRUCT_PULLOUTLISTING_);");
	fclose(outfile);
	outfile=fopen("./generated/pullout_reflectfile.hxx","a");
	fprintf(outfile,"{\"options\",%i,%i,&AUTOSTRUCT_PULLOUTLISTING_options,sizeof(AUTOSTRUCT_PULLOUTLISTING_options)},",currentitemcount,currentitemcount);
	fclose(outfile);
	return 0;
}
