#include <stdio.h>
#include <string.h>
char buffer[1024];
char name[512];
char group[512];
char searchstring[512];
char glob_started=0;
int main(void)
{
	FILE * infile;
	FILE * outfile;
	FILE * outfile2;
	int ihv1;
	int iindex;
	int icount=0;
	infile=fopen("./internal_enum.hxx","r");
	outfile=fopen("./generated/reflection.hxx","w+");
	outfile2=fopen("./generated/reflection_headers.hxx","w+");
	iback:
	;
	int backval=fscanf(infile,"%[^\n]",buffer);
	if (backval==1)
	{
		if (strncmp(buffer,"//",2)!=0)
		{
			int backval=sscanf(buffer,searchstring,name,&iindex);
			if (backval==2)
			{
				icount++;
				fprintf(outfile,"{\"%s\",%i,\n#ifdef STRUCTUREDEFINED_%s_instance\nsizeof(%s_instance)\n#else\n0\n#endif\n},\n",name,iindex,name,name);
			}
		}
		else
		{
			if (strncmp(buffer,"//--",4)==0)
			{
				if (glob_started) {fprintf(outfile,"};\n");fprintf(outfile2,"#define %s_ListSize %i\n",group,icount);}
				int backval=sscanf(buffer,"//--%[^\n \x0D]",group);
				icount=0;
				sprintf(searchstring,"#define %s_%%[^\n ] %%i",group);
				fprintf(outfile,"trienum %s_List[]={\n",group);
				fprintf(outfile2,"extern trienum %s_List[];\n",group);
				glob_started=1;
			}
		}
		fread(&ihv1,1,1,infile);
		if (ihv1==13){fread(&ihv1,1,1,infile);}
		goto iback;
	}
	if (glob_started) {fprintf(outfile,"};\n");fprintf(outfile2,"#define %s_ListSize %i\n",group,icount);}
	fclose(infile);
	fclose(outfile);
	fclose(outfile2);
	return 0;
}
