/*THIS IS THE MAIN PROGRAM

L      H   H EEEEE N    N DDD   RRR     A    W           W
L      H   H E     NN   N D  D  R  R   A A   W     W     W
L      HHHHH EEEEE N N  N D   D R  R  A   A  W     W     W
L      H   H E     N  N N D   D RRR   AAAAA   W   W W   W
L      H   H E     N   NN D  D  R  R A     A   W W   W W
LLLLLL H   H EEEEE N    N DDD   R  R A     A    W     W

*/
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <typeinfo>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include "lendefs.h"
#include "cxxdata.h"
#include "xmldata.hxx"

#include "enums.hxx"
#include "./generated/filestructure.hxx"
#include "definitionlist.h"
#include "janitor.hxx"
#include "ellipeq.h"
#include "quartic.hxx"
#include <time.h>
#include <SDL.h>
#include <unistd.h>
#include "graphics_variables.hxx"
#include "sdlgfx.hxx"
#include "createsvg.hxx"
#include "hatch.hxx"

#include "xmlparse.hxx"

#include "makeinf.hxx"
int main(int argc,char * * argv)
{
	if (argc!=2)
	{
		printf("no file specified or to many ones!");
		exit(1);
	}
	infile=fopen(argv[1],"r+");
	input_fsm(infile);
	fclose(infile);
	svg_findaround();
	getframes();
	sdl_output("outsdl.svg");
}
