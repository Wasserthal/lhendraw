#include "stdio.h"
#include "stdlib.h"
#define PARSECDXML_IS_AUXPROG
#include "parsecdxml.cxx"
#include "makeinf.hxx"
int createsortiment(char sortiment_count,char * * sortimentstart)
{
	for (int ilv1=0;ilv1<sortiment_count;ilv1++)
	{
		makeinf_sortiment[ilv1]=atoi(sortimentstart[ilv1]);
		//TODO: calculate scaling...
	}
}
int main(int argc,char * * argv)
{
	FILE * infile;
	setvbuf(stdout,NULL,_IONBF,0);
	makeinf_frame_count=0;
	makeinf_sortimentcount=0;
	createsortiment(argc-3,argv+3);
	makeinf(namestring,"makeinf_test.inf");
	//TODO: calculate "raw" file to find text fragments!
}
