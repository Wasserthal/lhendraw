/*THIS IS THE MAIN PROGRAM

L      H   H EEEEE N    N DDD   RRR     A    W           W
L      H   H E     NN   N D  D  R  R   A A   W     W     W
L      HHHHH EEEEE N N  N D   D R  R  A   A  W     W     W
L      H   H E     N  N N D   D RRR   AAAAA   W   W W   W
L      H   H E     N   NN D  D  R  R A     A   W W   W W
LLLLLL H   H EEEEE N    N DDD   R  R A     A    W     W

*/
#include <new>
#define _XOPEN_SOURCE 700
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <typeinfo>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#define CAMBRIDGEPREFIX(content) CAMBRIDGE_ ## content
#define CAMBRIDGEPREFIX2(content,content2) content ## _CAMBRIDGE_ ## content2
#include "lendefs.h"
#include "elements.hxx"
#include "xmldata.hxx"
#include "telescopic_list.hxx"
#include "cdxdata.hxx"
#include "./generated/reflection_headers.hxx"
#include "internal_enum.hxx"

#include "enums.hxx"
#include "./generated/structure.hxx"
#include "./generated/configfilestructure.hxx"
#include "./generated/cambridgestructure.hxx"
#include "definitionlist.h"
#include "janitor.hxx"
#include "ellipeq.h"
#include "quartic.hxx"
#include <time.h>
#include <SDL.h>
#include <unistd.h>
#include "draw_variables.hxx"
void automatic_init() {
#include "./generated/initialization_lhendraw.hxx"
}
superconstellation AUTOSTRUCTURE_ctype_propertylist[]{
#include "./generated/propertylist_lhendraw.hxx"
};
superconstellation_directory AUTOSTRUCTURE_ctype_directory[]{
#include "./generated/propertydirectory_lhendraw.hxx"
};
#include "text_freetype.h"

#include <time.h>
struct timespec ts;
long long counter1=0;
long long counter2=0;
long long counter3=0;
long long counter4=0;
long long counter5=0;
int clockid=0;

#include "sdlgfx.hxx"
#include "parsecdxml_variables.hxx"
#include "conv_cambridge_internal.hxx"
#include "conv_config_internalconfig.hxx"
#include "selection.hxx"
#include "undo.hxx"
#include "edit.hxx"
#include "./generated/pullout_stringfile.hxx"
#include "./generated/pullout_structfile.hxx"
#include "sdlctrl.hxx"
#include "menugfx.hxx"
#include "draw.hxx"
//#include "hatch.hxx" //TODO SUBJECT HATCH needs the moleculefill buffer

#include "xmlparse.hxx"

#include "lhendraw_files.hxx"
#include "resources_init.hxx"
#include "./generated/reflection.hxx"
structenum REFLECTION_List[]{
#include "./generated/pullout_reflectfile.hxx"
};
int REFLECTION_ListSize=sizeof(REFLECTION_List)/sizeof(structenum);
structenum * searchreflectedstruct(const char * input)
{
	for (int ilv1=0;ilv1<REFLECTION_ListSize;ilv1++)
	{
		if (strcmp(REFLECTION_List[ilv1].name,input)==0)
		{
			return &(REFLECTION_List[ilv1]);
		}
	}
	return NULL;
}
int main(int argc,char * * argv)
{
	clock_getcpuclockid(getpid(),&clockid);
	initmemory();
	automatic_init();
	resources_init(argv[0]);
	for (int ilv1=0;ilv1<control_toolcount;ilv1++)
	{
		clickabilitymatrix_tooldependent[ilv1]=selection_clickabilitymatrix;
	}
	conv_config_internalconfig();
	if (argc==2)
	{
		infile=fopen(argv[1],"r");
		currentinstance=new(CAMBRIDGEPREFIX(Total_Document_instance));
		input_fsm(infile);
		fclose(infile);
		CAMBRIDGECONV_maintointernal();
	}
	else
	{
		FILE_NEW(NULL,NULL);
	}
	svg_findaround();
	sdl_init();
	SDL_EnableUNICODE(1);
	SDL_ShowCursor(0);
	mainloop:
	sdl_control();
	gfx_gfxstart();
	gfx_output();
	sdl_canvasframedraw();
	sdl_commonmenudraw();
	sdl_selectiondraw();
	gfx_gfxstop();
	usleep(1000);
	if (!LHENDRAW_leave) goto mainloop;
	sdl_outit();
	printf("Time consumption:\nInit:%llX\nfindaround:%llX\ngetatoms:%llX\ninitZlist:%llX\ndrawing:%llX\n",counter1,counter2,counter3,counter4,counter5);
}
