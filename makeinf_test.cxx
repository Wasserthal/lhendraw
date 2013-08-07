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
	makeinf(NULL,argv[1]);
	printf("Frames:%i,Sortiments:%i\nWidth:%f,Height:%f\n",makeinf_frame_count,makeinf_sortimentcount,makeinf_width,makeinf_height);
	printf("Listing Frames:");
	for (int ilv1=0;ilv1<makeinf_frame_count;ilv1++)
	{
		printf("number:%i,X:%f,Y:%f,width:%f,height:%f\n",makeinf_frame[ilv1].number,makeinf_frame[ilv1].posx,makeinf_frame[ilv1].posy,makeinf_frame[ilv1].width,makeinf_frame[ilv1].height);
	}
}
