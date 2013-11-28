//Tells the Contents of a inf file. Needed for my thesis.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lendefs.h"
#include "cxxdata.h"
#define CDXMAKEINF_READONLY
#include "makeinf.hxx"
int main(int argc,char ** argv)
{
	makeinf_sortimentcount=0;
	#ifndef STRIP_ENDING
	makeinf(NULL,argv[1]);
	#else
	char cutstring[512];
	strcpy(cutstring,argv[1]);
	for (int ilv1=strlen(cutstring)-1;ilv1>0;ilv1--)
	{
		if (cutstring[ilv1]='.')
		{
			cutstring[ilv1]=0;
			goto cutit;
		}
	}
	cutit:
	makeinf(NULL,cutstring);
	#endif
	#ifndef MACHINE_READABLE
	printf("Frames:%i,Sortiments:%i\nWidth:%f,Height:%f\n",makeinf_frame_count,makeinf_sortimentcount,makeinf_width,makeinf_height);
	printf("Listing Frames:\n");
	for (int ilv1=0;ilv1<makeinf_frame_count;ilv1++)
	{
		printf("number:%i,X:%f,Y:%f,width:%f,height:%f\n",makeinf_frame[ilv1].number,makeinf_frame[ilv1].posx,makeinf_frame[ilv1].posy,makeinf_frame[ilv1].width,makeinf_frame[ilv1].height);
	}
	printf("Listing Sortiments:\n");
	for (int ilv1=0;ilv1<makeinf_sortimentcount;ilv1++)
	{
		printf("Listing Sortiment %i\n:",ilv1);
		printf("width:%f,height:%f\n",makeinf_sortiments[ilv1].width,makeinf_sortiments[ilv1].height);
		for (int ilv2=0;ilv2<makeinf_sortiments[ilv1].sortiment_length;ilv2++)
		{
			printf("%i",makeinf_sortiments[ilv1].sortiment[ilv2].number);
		}
	}
	#else
	printf("%i %i",(int)(makeinf_width*3),(int)(makeinf_height*3));
	#endif
}
