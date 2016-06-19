#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL.h>
#include "lendefs.h"
#include "font.hxx"
int gfx_initialized=0;
_u32 * screen;
SDL_Surface *video;
SDL_Window *window;
int gfx()
{
	window=SDL_CreateWindow("Lhendraw",0,0,800,600,0);
	video=SDL_GetWindowSurface(window);
	if ( video == NULL ) 
	{
		fprintf(stderr,"Ich konnte kein Fenster mit der Auflösung 800*600 öffnen: %s\n",SDL_GetError());
		exit(1);
	}
	return 1;
}
void sdl_init()
{
	if (SDL_Init(SDL_INIT_VIDEO)<0)
	{
		fprintf(stderr, "SDL konnte nicht initialisiert werden: %s\n", SDL_GetError());
		exit(1);
	}
	gfx();
	gfx_initialized=1;
}
int gfx_gfxstart()
{
	if (gfx_initialized==0)
	{
		sdl_init();
	}
	if ( SDL_MUSTLOCK(video) ) 
	{
		if ( SDL_LockSurface(video) < 0 ) 
		{
			return(-1);
		}
	}
	screen=(unsigned int*)(*video).pixels;//You know, that is a pointer to a n byte structure, n=colordepth
	return 1;
}
void gfx_gfxstop()
{
	if ( SDL_MUSTLOCK(video) ) 
	{
		SDL_UnlockSurface(video);
	}
	SDL_UpdateWindowSurface(window);
}
void putpixel(int x,int y)
{
	if ((x<0) || (y<0))return;
	if ((x>=800) || (y>=600))return;
	screen[x+y*800]=0xFFFFFFFF;
}
void purtpixel(int x,int y,_u32 color,char prematureend)
{
	if (x<0)x=0;
	if (x>=800-7)x=800-8;
	if (y<0)y=0;
	if (y>=600-7)y=600-8;
	if ((x<0) || (y<0))return;
	if ((x>=800-7) || (y>=600-7))return;
	screen[x+3+(y+3)*800]=color;
	screen[x+6+(y+4)*800]=color;
	screen[x+6+(y+3)*800]=color;
	screen[x+6+(y+2)*800]=color;
	screen[x+0+(y+4)*800]=color;
	screen[x+0+(y+3)*800]=color;
	screen[x+0+(y+2)*800]=color;
	screen[x+4+(y+6)*800]=color;
	screen[x+3+(y+6)*800]=color;
	screen[x+2+(y+6)*800]=color;
	screen[x+4+(y+0)*800]=color;
	screen[x+3+(y+0)*800]=color;
	screen[x+2+(y+0)*800]=color;
	if (prematureend) return;

	screen[x+5+(y+4)*800]=color;
	screen[x+5+(y+3)*800]=color;
	screen[x+5+(y+2)*800]=color;
	screen[x+1+(y+4)*800]=color;
	screen[x+1+(y+3)*800]=color;
	screen[x+1+(y+2)*800]=color;
	screen[x+4+(y+5)*800]=color;
	screen[x+3+(y+5)*800]=color;
	screen[x+2+(y+5)*800]=color;
	screen[x+4+(y+1)*800]=color;
	screen[x+3+(y+1)*800]=color;
	screen[x+2+(y+1)*800]=color;

	screen[x+4+(y+4)*800]=color;
	screen[x+4+(y+3)*800]=color;
	screen[x+4+(y+2)*800]=color;
	screen[x+2+(y+4)*800]=color;
	screen[x+2+(y+3)*800]=color;
	screen[x+2+(y+2)*800]=color;
	screen[x+4+(y+4)*800]=color;
	screen[x+3+(y+4)*800]=color;
	screen[x+2+(y+4)*800]=color;
	screen[x+4+(y+2)*800]=color;
	screen[x+3+(y+2)*800]=color;
	screen[x+2+(y+2)*800]=color;

	screen[x+5+(y+5)*800]=color;
	screen[x+5+(y+1)*800]=color;
	screen[x+1+(y+5)*800]=color;
	screen[x+1+(y+1)*800]=color;
	screen[x+5+(y+5)*800]=color;
	screen[x+1+(y+5)*800]=color;
	screen[x+5+(y+1)*800]=color;
	screen[x+1+(y+1)*800]=color;
}
void arc(int startx,int starty,int midx,int midy,int endx,int endy)
{
	for (int ilv1=0;ilv1<200;ilv1++)
	{
		float master1x=startx+((float)(midx-startx))*(ilv1/200.0);
		float master1y=starty+((float)(midy-starty))*(ilv1/200.0);
		float master2x=midx+((float)(endx-midx))*(ilv1/200.0);
		float master2y=midy+((float)(endy-midy))*(ilv1/200.0);
		float slavex=master1x+(master2x-master1x)*(ilv1/200.0);
		float slavey=master1y+(master2y-master1y)*(ilv1/200.0);
		putpixel(slavex,slavey);
	}
}
void line(int startx,int starty,int endx,int endy)
{
	float m,t;
	int swap;
	if (abs(endx-startx)>abs(endy-starty))
	{
		if (endx<startx)
		{
			swap=startx;
			startx=endx;
			endx=swap;
			swap=starty;
			starty=endy;
			endy=swap;
		}
		m=((float)(endy-starty))/(endx-startx);
		t=endy-endx*m;
		for (int ilv1=startx;ilv1<=endx;ilv1++)
		{
			putpixel(ilv1,t+m*ilv1);
		}
	}
	else
	{
		if (endy<starty)
		{
			swap=startx;
			startx=endx;
			endx=swap;
			swap=starty;
			starty=endy;
			endy=swap;
		}
		m=((float)(endx-startx))/(endy-starty);
		t=endx-endy*m;
		for (int ilv1=starty;ilv1<=endy;ilv1++)
		{
			putpixel(t+m*ilv1,ilv1);
		}
	}
}
char lineto_donebefore=0;
int lineto_startx,lineto_starty;
int lineto_firstx,lineto_firsty;
void arcto(int x,int y,int x2,int y2,char reset)
{
	if (!lineto_donebefore)//Should not occur
	{
		fprintf(stderr,"Error! start with off-line\n");//exit(1);//DEBUG
		lineto_startx=x2;lineto_starty=y2;
		lineto_firstx=x;lineto_firsty=y;
/*		lineto_firstx=0;
		lineto_firsty=0;*/
		lineto_donebefore=1;
		return;
	}
	if (reset)
	{
		arc(lineto_startx,lineto_starty,x,y,lineto_firstx,lineto_firsty);//TODO: x2 and y2 read from invalid memoryposition, then!
		lineto_donebefore=0;
		lineto_startx=lineto_firstx;lineto_starty=lineto_firsty;
	}
	else
	{
		arc(lineto_startx,lineto_starty,x,y,x2,y2);
		lineto_startx=x2;lineto_starty=y2;
	}
}
void lineto(int x,int y,char reset)
{
	if (!lineto_donebefore)
	{
		lineto_startx=x;lineto_starty=y;
		lineto_firstx=x;lineto_firsty=y;
		lineto_donebefore=1;
		return;
	}
	line(lineto_startx,lineto_starty,x,y);
	lineto_startx=x;lineto_starty=y;
	if (reset)
	{
		line(lineto_startx,lineto_starty,lineto_firstx,lineto_firsty);
		lineto_donebefore=0;
	}
}
int main(int argc,char ** argv)
{
	int selected=0;
	font_initlist();
	font_start();
	sdl_init();
	int aim=1;
	if (argc>=2) aim=atoi(argv[1]);
	int bookstavecount=0;
	int backval;
	iback:
	backval=font();
	if (backval==0) return 0;
	selectpointback:;
	gfx_gfxstart();
	for (int ilv1=0;ilv1<800*600;ilv1++)
	{
		screen[ilv1]=0;
	}
	int endpointcursor=0;
	lineto_donebefore=0;
	for (int ilv1=0;ilv1<glyph.maxcount;ilv1++)
	{
		char endpoint=0;
		printf("{%i:%i,%02hhX}\n",glyph.simple.donecoordinates[ilv1].x,glyph.simple.donecoordinates[ilv1].y,glyph.simple.donecoordinates[ilv1].flags);
		if (glyph.simple.donecoordinates[ilv1].flags & 1)
		{
			if (ilv1>=glyph.simple.endpoints[endpointcursor])
			{
				endpoint=1;
				endpointcursor++;
			}
			lineto(((float)(glyph.simple.donecoordinates[ilv1].x)-glyph.minx)*799.0/(glyph.maxx-glyph.minx),599-((glyph.simple.donecoordinates[ilv1].y-glyph.miny)*599.0/(glyph.maxy-glyph.miny)),endpoint);
			purtpixel(((float)(glyph.simple.donecoordinates[ilv1].x)-glyph.minx)*799.0/(glyph.maxx-glyph.minx),599-((glyph.simple.donecoordinates[ilv1].y-glyph.miny)*599.0/(glyph.maxy-glyph.miny)),(endpoint)?0xFF00FF:0xFF,(ilv1==selected));
		}
		else
		{
			if (ilv1>=glyph.simple.endpoints[endpointcursor])
			{
				endpoint=1;
				endpointcursor++;
			}
			arcto(((float)(glyph.simple.donecoordinates[ilv1].x)-glyph.minx)*799.0/(glyph.maxx-glyph.minx),599-((glyph.simple.donecoordinates[ilv1].y-glyph.miny)*599.0/(glyph.maxy-glyph.miny)),((float)(glyph.simple.donecoordinates[ilv1+1].x)-glyph.minx)*799.0/(glyph.maxx-glyph.minx),599-((glyph.simple.donecoordinates[ilv1+1].y-glyph.miny)*599.0/(glyph.maxy-glyph.miny)),endpoint);
			purtpixel(((float)(glyph.simple.donecoordinates[ilv1].x)-glyph.minx)*799.0/(glyph.maxx-glyph.minx),599-((glyph.simple.donecoordinates[ilv1].y-glyph.miny)*599.0/(glyph.maxy-glyph.miny)),0x00FF00,(ilv1==selected));
			if (endpoint==0)
			{
				printf("{%i:%i,%02hhX}\n",glyph.simple.donecoordinates[ilv1+1].x,glyph.simple.donecoordinates[ilv1+1].y,glyph.simple.donecoordinates[ilv1+1].flags);
				purtpixel(((float)(glyph.simple.donecoordinates[ilv1+1].x)-glyph.minx)*799.0/(glyph.maxx-glyph.minx),599-((glyph.simple.donecoordinates[ilv1+1].y-glyph.miny)*599.0/(glyph.maxy-glyph.miny)),(glyph.simple.donecoordinates[ilv1+1].flags & 0x80)?0xFFFFFFFF:0xFF0000,(ilv1+1==selected));
				if (ilv1+1<glyph.simple.endpoints[endpointcursor])
				{
					ilv1++;
				}
			}
		}
	}
	gfx_gfxstop();
	bookstavecount++;
	if (bookstavecount==aim) scanf("%i",&aim);
	if (aim>=1000)
	{
		selected=aim-1000;
		aim=bookstavecount+1;
		for (int ilv1=0;ilv1<glyph.units;ilv1++)
		{
			printf("END#%i=%i\n",ilv1,glyph.simple.endpoints[ilv1]);
		}
		printf("FLAGS=%02hhx\n",glyph.simple.donecoordinates[selected].flags);
		goto selectpointback;
	}
	if (aim==1) aim=bookstavecount+1;
	if (aim!=0)
	goto iback;
	fclose(fontfile);
	return 0;
}
