/*THIS IS THE MAIN PROGRAM

L      H   H EEEEE N    N DDD   RRR     A    W           W
L      H   H E     NN   N D  D  R  R   A A   W     W     W
L      HHHHH EEEEE N N  N D   D R  R  A   A  W     W     W
L      H   H E     N  N N D   D RRR   AAAAA   W   W W   W
L      H   H E     N   NN D  D  R  R A     A   W W   W W
LLLLLL H   H EEEEE N    N DDD   R  R A     A    W     W

*/
#include <new>
#ifdef DEBUG
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
#include <setjmp.h>
#ifndef NOCLIPBOARD
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xmu/Atoms.h>
#endif
#include <SDL.h>
#include <time.h>
#ifndef NOPOSIX
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif
#include "lendefs.h"
#include "debug.hxx"
#define CAMBRIDGEPREFIX(content) CAMBRIDGE_ ## content
#define CAMBRIDGEPREFIX2(content,content2) content ## _CAMBRIDGE_ ## content2
#include "memory.hxx"
#include "elements.hxx"
#include "xmldata.hxx"
#include "definitionlist.h"
#include "cdxdata.hxx"
#include "bkchemdata.hxx"
#include "./generated/reflection_headers.hxx"
#include "telescope.hxx"
#include "xmlparse.hxx"
#include "internal_enum.hxx"

#include "enums.hxx"
#include "./generated/structure.hxx"
#include "./generated/configfilestructure.hxx"
#include "./generated/cambridgestructure.hxx"
#include "./generated/bkchemstructure.hxx"
//HERE, the program-specific includes begin
int undo_getbufferfromstructure(basicmultilist * input,TELESCOPE_buffer * * bufferptr);
#include "variables.hxx"
#include "janitor.hxx"
#include "ellipeq.h"
#include "quartic.hxx"
#include "draw_variables.hxx"
void automatic_init() {
#include "./generated/initialization_lhendraw.hxx"
}
superconstellation AUTOSTRUCTURE_ctype_propertylist[]={
#include "./generated/propertylist_lhendraw.hxx"
};
superconstellation_directory AUTOSTRUCTURE_ctype_directory[]={
#include "./generated/propertydirectory_lhendraw.hxx"
};
#include "text.h"
#include "text_freetype.h"

#ifdef DEBUG
#include <time.h>
struct timespec ts;
long long counter1=0;
long long counter2=0;
long long counter3=0;
long long counter4=0;
long long counter5=0;
int clockid=0;
#endif

#include "sdlgfx.hxx"
#include "glyf.hxx"
#ifndef NOCLIPBOARD
#include "clipboard.hxx"
#endif
#include "conv_config_internalconfig.hxx"
#include "selection.hxx"
extern int edit_getBoundingBoxMode(graphic_instance * iinstance);
#include "conv_cambridge_internal.hxx"
#include "conv_internal_cambridge.hxx"
#include "conv_bkchem_internal.hxx"
extern int storeundo(_u32 flags,const char * iname);
extern int restoreundo(_u32 flags,_u32 orderflags);
extern int undo_trackundo();
extern int undo_trackredo(int variable);
extern int undo_storcatch(_u32 flags,const char * iname);
#include "analysis.hxx"
#include "HQ.h"
#include "edit.hxx"
#include "pivot.hxx"
#include "./generated/pullout_stringfile.hxx"
#include "./generated/pullout_structfile.hxx"
#include "help.hxx"
#include "sdlctrl.hxx"
#include "menugfx.hxx"
#include "filedlg.hxx"
#define MACRO_DRAWPREFIX(content) gfx_ ## content
#include "draw.hxx"
#undef MACRO_DRAWPREFIX
#include "svg.hxx"
#define MACRO_DRAWPREFIX(content) svg_ ## content
#include "draw.hxx"
#undef MACRO_DRAWPREFIX
#include "ps.hxx"
#define MACRO_DRAWPREFIX(content) ps_ ## content
#include "draw.hxx"
#undef MACRO_DRAWPREFIX
//#include "hatch.hxx" //TODO SUBJECT HATCH needs the moleculefill buffer


#include "resources_init.hxx"
multilistlist_ multilistlist[]=
{
#include "./generated/multilistlistfile.hxx"
};
#include "./generated/reflection.hxx"
structenum REFLECTION_List[]={
#include "./generated/pullout_reflectfile.hxx"
};
int REFLECTION_ListSize=sizeof(REFLECTION_List)/sizeof(structenum);
#include "cmdline.hxx"
#include "undo.hxx"
#include "undotree.hxx"
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
#ifdef DEBUG
	progname=argv[0];
	signal(SIGSEGV,&Signal);
	signal(SIGILL,&Signal);
	signal(SIGFPE,&Signal);
	signal(SIGSYS,&Signal);
	signal(SIGPIPE,&Signal);
#endif
	if (sizeof(bond_actual_node_)>sizeof(b_instance))
	{
		error("Conceptual programming error, Helpstructure b > structure b");
	}
	if (sizeof(atom_actual_node_)>sizeof(n_instance))
	{
		error("Conceptual programming error, Helpstructure n > structure n");
	}
	if (sizeof(text_actual_node_)>sizeof(t_instance))
	{
		error("Conceptual programming error, Helpstructure t > structure t");
	}
	if (setjmp(debug_crashhandler)>0)
	{
		error("Crash cannot be handled during startup! - config files too long? Then compile with a larger blocksize\n");
	}
	memcpy(memory_catch_overflow,debug_crashhandler,sizeof(jmp_buf));
	memory_init();
	memory_alloc(&tagnamestring,10);
	memory_alloc(&parameterstring,10);
	memory_alloc(&paramvaluestring,10);
	memory_alloc((char**)&atom_actual_node,4);
	memory_alloc((char**)&bond_actual_node,4);
	memory_alloc((char**)&text_actual_node,4);
	#ifdef DEBUG
	clock_getcpuclockid(getpid(),&clockid);
	#endif
	multilist_count=sizeof(multilistlist)/sizeof(multilistlist_);
	for (int ilv1=0;ilv1<multilist_count;ilv1++)
	{
		multilistlist[ilv1].instance->index=ilv1;
	}
	for (int ilv1=1;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
	{
		basicmultilist * tl_multilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
		if (tl_multilist)
		{
			(*tl_multilist).numberinlist=ilv1;
		}
	}
	initmemory();
	automatic_init();
	resources_init();
	init_buffers();
	for (int ilv1=0;ilv1<control_toolcount;ilv1++)
	{
		control_clickabilitymatrixes[ilv1]=selection_clickabilitymatrix;
	}
	control_clickabilitymatrixes[7]=({clickabilitymatrix_ tl_clickabilitymatrix={2,0,0,
	{0xFFFFFFFF,0x6,0x6,0x0,0x6,0x6,0x0,0xFFFFFFFF}};tl_clickabilitymatrix;});
	control_clickabilitymatrixes[9]=({clickabilitymatrix_ tl_clickabilitymatrix={2,0,0,
	{0xFFFFFFFF,0x70,0x70,0x70,0x70,0x70,0x70,0xFFFFFFFF}};tl_clickabilitymatrix;});
	control_clickabilitymatrixes[10]=({clickabilitymatrix_ tl_clickabilitymatrix={2,0,0,
	{0xFFFFFFFF,0x2,0x22,0x2,0x22,0x2,0x0,0xFFFFFFFF}};tl_clickabilitymatrix;});
	control_clickabilitymatrixes[10]=({clickabilitymatrix_ tl_clickabilitymatrix={2,0,0,
	{0xFFFFFFFF,0x6,0x22,0x2,0x22,0x2,0x0,0xFFFFFFFF}};tl_clickabilitymatrix;});
	control_clickabilitymatrixes[11]=({clickabilitymatrix_ tl_clickabilitymatrix={2,0,0,
	{0xFFFFFFFF,0xA,0xA,0x8,0xA,0xA,0x0,0xFFFFFFFF}};tl_clickabilitymatrix;});
	control_clickabilitymatrixes[17]=({clickabilitymatrix_ tl_clickabilitymatrix={2,0,0,
	{0xFFFFFFFF,0x6,0x6,0x0,0x6,0x6,0x0,0xFFFFFFFF}};tl_clickabilitymatrix;});
	control_clickabilitymatrixes[6]=({clickabilitymatrix_ tl_clickabilitymatrix={2,0,0,
	{0xFFFFFFFF,0x2,0x2,0x0,0x2,0x2,0x0,0xFFFFFFFF}};tl_clickabilitymatrix;});
	control_clickabilitymatrixes[5]=({clickabilitymatrix_ tl_clickabilitymatrix={2,0,0,
	{0xFFFFFFFF,0x0,0x0,0x0,0x0,0x0,0x0,0xFFFFFFFF}};tl_clickabilitymatrix;});
	control_clickabilitymatrixes[1]=({clickabilitymatrix_ tl_clickabilitymatrix={2,0,0,
	{0xFFFFFFFF,0x0,0x0,0x0,0x0,0x0,0x0,0xFFFFFFFF}};tl_clickabilitymatrix;});
	conv_config_internalconfig();
	FILE_NEW(NULL,NULL);
	if (setjmp(debug_crashhandler)>0)
	{
		error_reset();
	}
	memcpy(memory_catch_overflow,debug_crashhandler,sizeof(jmp_buf));
	cmdline(argc,argv);
	print("LISTSIZE:%i,MAX:%i\n",glob_CONFIGBRIDGE_Hotkey_multilist->filllevel,glob_CONFIGBRIDGE_Hotkey_multilist->getmaxitems());
	if (control_GUI)
	{
		svg_findaround();
		sdl_init();
		if (setjmp(debug_crashhandler)>0)
		{
			error_reset();
		}
		memcpy(memory_catch_overflow,debug_crashhandler,sizeof(jmp_buf));
#ifndef SDL2
		SDL_WM_SetCaption(control_filenamehead,control_filenamehead);
		resources_set_icon();
		SDL_EnableUNICODE(1);
		SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);
//		SDL_ShowCursor(0);
#endif
		mainloop:
		#ifndef NOCLIPBOARD
		service_clipboard();
		#endif
		switch (LHENDRAW_filedlgmode)
		{
		case 0:
			control_normal();
			gfx_gfxstart();
			screenclear(0xFFFFFF);
			if (control_displayproperties.outofarea==2)
			{
				if (SDL_scrollx<0) SDL_scrollx=0;
				if (SDL_scrolly<0) SDL_scrolly=0;
			}
			if (control_displayproperties.outofarea & 1) sdl_outofareadraw();
			gfx_output();
			sdl_canvasframedraw();
			sdl_commonmenucommon();
			sdl_menudraw();
			sdl_selectiondraw();
			draw_reticle();
		break;
		case 1:
			control_filedlg_datastorages();
			sdl_filemenucommon();
			control_filedlg();
			gfx_gfxstart();
			screenclear(0xFFFFFF);
			sdl_filemenucommon();
			sdl_menudraw();
			sdl_menuframe();
			draw_reticle();
		break;
		case 2:
			sdl_optionsmenucommon();
			control_filedlg();
			gfx_gfxstart();
			screenclear(0xFFFFFF);
			sdl_optionsmenucommon();
			sdl_menudraw();
			sdl_menuframe();
			draw_reticle();
		break;
		case 3:
			control_undotree();
			gfx_gfxstart();
			screenclear(0xFFFFFF);
			draw_undotree(0);
			draw_reticle();
		break;
		}
		gfx_gfxstop();
		usleep(1000);
		control_doubleclickenergy-=1;
		control_doublekeypressenergy-=1;
		if (control_doubleclickenergy<0) control_doubleclickenergy=0;
		if (control_doublekeypressenergy<0) control_doublekeypressenergy=0;
		if (!LHENDRAW_leave) goto mainloop;
		sdl_outit();
		
		#ifdef DEBUG
		printf("Time consumption:\nInit:%llX\nfindaround:%llX\ngetatoms:%llX\ninitZlist:%llX\ndrawing:%llX\n",counter1,counter2,counter3,counter4,counter5);
		#endif
	}
	return 0;
}
