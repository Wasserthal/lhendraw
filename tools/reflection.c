#include <stdio.h>
#include <string.h>
FILE * infile;
FILE * outfile;
FILE * outfile2;
char buffer[1024];
char namestring[40];
char glob_started=0;
int backval;
int getline()
{
	char ihv1;
	char wasvalid=0;
	for (int ilv1=0;ilv1<1024;ilv1++)
	{
		backval=fread(&ihv1,1,1,infile);
		if (backval==0)
		{
			if (wasvalid==0)
			{
				return 0;
			}
			else
			{
				goto idone;
			}
		}
		if ((ihv1==10) && (wasvalid==0))
		{
			ilv1=0;
			goto loopdone;
		}
		wasvalid=1;
		buffer[ilv1]=ihv1;
		if ((ihv1==13) || (ihv1==10))
		{
			idone:
			buffer[ilv1]=0;
			return 1;
		}
		loopdone:;
	}
}
int main(int argc,char ** argv)
{
	int ihv1;
	int iindex;
	outfile=fopen("./generated/reflection.hxx","a");
	outfile2=fopen("./generated/reflection_headers.hxx","a");
	fprintf(outfile2,"extern fuenum REFLECTION_FUNCTION_List[];\nextern int REFLECTION_FUNCTION_ListSize;\n");
	fclose(outfile2);
	fprintf(outfile,"fuenum REFLECTION_FUNCTION_List[]{\n");
	for (int ilv1=1;ilv1<argc;ilv1++)
	{
		infile=fopen(argv[ilv1],"r");
		while (getline())
		{
			backval=sscanf(buffer,"catalogized_command_funcdef(%[A-Za-z0-9_])",namestring);
			if (backval>0)
			{
				fprintf(outfile,"{%i,\"%s\",(catalogized_command_functype)%s},\n",ilv1,namestring,namestring);
			}
		}
		fclose(infile);
	}
	fprintf(outfile,"};\nint REFLECTION_FUNCTION_ListSize=sizeof(REFLECTION_FUNCTION_List)/sizeof(fuenum);\n");
	fclose(outfile);
	return 0;
}
