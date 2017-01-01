#include <stdio.h>
#include <stdlib.h>
int main(int argc,char ** argv)
{
	FILE * file1=fopen(argv[1],"r");
	FILE * file2=fopen(argv[2],"w");
	int ilhv1=0;
	int readcount=0;
	iback:
	readcount=fread(&ilhv1,1,4,file1);
	fprintf(file2,"%i,",ilhv1);
	if (feof(file1))
	{
		fclose(file1);
		fclose(file2);
		return 0;
	}
	if (readcount!=0) goto iback;
	return 0;
}
