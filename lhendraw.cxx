/*THIS IS THE MAIN PROGRAM

L      H   H EEEEE N    N DDD   RRR     A    W           W
L      H   H E     NN   N D  D  R  R   A A   W     W     W
L      HHHHH EEEEE N N  N D   D R  R  A   A  W     W     W
L      H   H E     N  N N D   D RRR   AAAAA   W   W W   W
L      H   H E     N   NN D  D  R  R A     A   W W   W W
LLLLLL H   H EEEEE N    N DDD   R  R A     A    W     W

*/
#include <new>
#ifndef NODEBUG
#include <signal.h>
#endif
#define _XOPEN_SOURCE 700
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
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
#include "definitionlist.h"
#include "cdxdata.hxx"
#include "xmlparse.hxx"
#include "./generated/reflection_headers.hxx"
#include "internal_enum.hxx"

#include "enums.hxx"
#include "./generated/structure.hxx"
#include "./generated/configfilestructure.hxx"
#include "./generated/cambridgestructure.hxx"
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
#include "memory.hxx"
#include "conv_config_internalconfig.hxx"
#include "selection.hxx"
#include "conv_cambridge_internal.hxx"
#include "conv_internal_cambridge.hxx"
int storeundo(_u32 flags);
int restoreundo(_u32 flags,_u32 orderflags);
int undo_trackundo();
int undo_trackredo(int variable);
#include "edit.hxx"
#include "pivot.hxx"
#include "./generated/pullout_stringfile.hxx"
#include "./generated/pullout_structfile.hxx"
#include "help.hxx"
#include "sdlctrl.hxx"
#include "menugfx.hxx"
#include "filedlg.hxx"
#include "draw.hxx"
//#include "hatch.hxx" //TODO SUBJECT HATCH needs the moleculefill buffer


#include "resources_init.hxx"
multilistlist_ multilistlist[]=
{
#include "./generated/multilistlistfile.hxx"
};
#include "./generated/reflection.hxx"
structenum REFLECTION_List[]{
#include "./generated/pullout_reflectfile.hxx"
};
int REFLECTION_ListSize=sizeof(REFLECTION_List)/sizeof(structenum);
#include "cmdline.hxx"
#include "undo.hxx"
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
void test_routine()
{
	printf("ggg:%llX---%llX\n",(long long)&(((*glob_CAMBRIDGE_t_multilist).bufferlist[7])._vptr),(long long)(((*glob_CAMBRIDGE_t_multilist).bufferlist[7])._vptr));
}
#ifndef NODEBUG
const char *progname=NULL;
void Signal(int signum)
{
	if (control_GUI)
	{
		char localbuf[512+1];
		snprintf(localbuf,512,"gdb %s %d",progname,getpid());
		system(localbuf);
	}
	else
	fprintf(stderr,"FILE crashes:%s\n",control_filename);exit(1);
}
#endif
int main(int argc,char * * argv)
{
#ifndef NODEBUG
	progname=argv[0];
	signal(SIGSEGV,&Signal);
	signal(SIGILL,&Signal);
	signal(SIGFPE,&Signal);
	signal(SIGSYS,&Signal);
	signal(SIGPIPE,&Signal);
#endif
	memory_alloc(&tagnamestring);
	memory_alloc(&parameterstring);
	memory_alloc(&paramvaluestring);
	clock_getcpuclockid(getpid(),&clockid);
	multilist_count=sizeof(multilistlist)/sizeof(multilistlist_);
	for (int ilv1=0;ilv1<multilist_count;ilv1++)
	{
		multilistlist[ilv1].instance->index=ilv1;
	}
	initmemory();
	automatic_init();
	resources_init(argv[0]);
	init_buffers();
	for (int ilv1=0;ilv1<control_toolcount;ilv1++)
	{
		clickabilitymatrix_tooldependent[ilv1]=selection_clickabilitymatrix;
	}
	conv_config_internalconfig();
	FILE_NEW(NULL,NULL);
	cmdline(argc,argv);
	if (control_GUI)
	{
		svg_findaround();
		sdl_init();
		SDL_EnableUNICODE(1);
//		SDL_ShowCursor(0);
		mainloop:
		control_normal();
		gfx_gfxstart();
		screenclear(0xFFFFFF);
		if (LHENDRAW_filedlgmode==0)
		{
			gfx_output();
			sdl_canvasframedraw();
			sdl_commonmenudraw();
			sdl_selectiondraw();
			draw_reticle();
		}
		else
		{
			control_filedlg();
			sdl_filemenudraw();
			sdl_menuframe();
			draw_reticle();
		}
		gfx_gfxstop();
		usleep(1000);
		control_doubleclickenergy-=1;
		if (control_doubleclickenergy<0) control_doubleclickenergy=0;
		if (!LHENDRAW_leave) goto mainloop;
		sdl_outit();
		
		printf("Time consumption:\nInit:%llX\nfindaround:%llX\ngetatoms:%llX\ninitZlist:%llX\ndrawing:%llX\n",counter1,counter2,counter3,counter4,counter5);
	}
}
