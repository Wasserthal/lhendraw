//This is the main unit of the headless version, most C++ code is contained in xmldata.hxx and xmlparse.hxx, and inserts the other units in the middle
//Be aware that filestructure.hxx is dynamically generated from filestructure.draft by filestructure_maker (filestructuremaker.c)
#define CAMBRIDGEPREFIX(content) content
#define CAMBRIDGEPREFIX2(content,content2) content ## _ ## content2
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <typeinfo>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include "lendefs.h"
#include "xmldata.hxx"
#include "cxxdata.h"

#include "enums.hxx"
#include "./generated/filestructure.hxx"
#include "./generated/internalstructure.hxx"
#include "./generated/initialization_lhendraw.hxx"
#include "telescopic_list.hxx"
#include "internal_enum.hxx"
#include "./generated/propertylist_lhendraw.hxx"
#include "./generated/propertydirectory_lhendraw.hxx"
#include "parsecdxml_variables.hxx"
#include "definitionlist.h"
#include "janitor.hxx"
#include "ellipeq.h"
#include "quartic.hxx"
#include "graphics_variables.hxx"
#include "createsvg.hxx"
#include "hatch.hxx"

#include "xmlparse.hxx"

#include "makeinf.hxx"
char pseudoparams[255*10+1];
char * pseudoparamsidx[255];
int main(int argc, char * * argv)
{
	setvbuf(stdout,NULL,_IONBF,0);
	/*xml_set_register_count=0;
	multilist_count=0;*/ //contraproductive amongst the hack!
	FILE * infile;
	infile=fopen(argv[1],"r+");
	if (infile==NULL)
	{
		fprintf(stderr,"CDXML Error! file %s not found!",argv[1]);exit(1);
	}
	input_fsm(infile);
	fclose(infile);
	if (argc<3)
	{
		svg_main("output.svg");
	}
	else
	{
		if (argc==3)
		{
			svg_findaround();
			getframes();
			if (makeinf_frame_count>0)
			{
				pseudoparams[0]=0;
				int tlpos=0;
				for (int ilv1=0;ilv1<makeinf_frame_count;ilv1++)
				{
					int tlbackval;
					pseudoparamsidx[ilv1]=(pseudoparams+tlpos);
					sprintf((pseudoparams+tlpos),"%i%n",ilv1+1,&tlbackval);
					tlpos+=tlbackval+1;
				}
				svg_main2(argv[2],makeinf_frame_count,pseudoparamsidx);
			}
			else
			{
				svg_main(argv[2]);
			}
		}
		if (argc>3)
		{
			svg_findaround();
			getframes();
			svg_main2(argv[2],argc-3,argv+3);
		}
	}
	exit(0);
	return 0;
}
