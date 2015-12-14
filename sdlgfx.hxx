//Graphic routines. Callers of those should be in createsvg.hxx
#define gfx_STANDARDX 800
#define gfx_STANDARDY 600
int gfx_screensizex=gfx_STANDARDX;
int gfx_screensizey=gfx_STANDARDY;
int gfx_canvassizex=gfx_STANDARDX-320;
int gfx_canvassizey=gfx_STANDARDY-240;
#define gfx_standardcanvassizex 704
#define gfx_standardcanvassizey 528
int gfx_canvasminx=160;
int gfx_canvasminy=120;
int gfx_canvasmaxx=gfx_STANDARDX-160;
int gfx_canvasmaxy=gfx_STANDARDY-120;
int gfx_depth=4;
int gfx_initialized=0;
float SDL_scrollx=0,SDL_scrolly=0;
float SDL_zoomx=1,SDL_zoomy=1;
int SDL_txcursorx=0;int SDL_txcursory=0;
float SDL_txangle=0;
int SDL_glyfstartx=0;int SDL_glyfstarty=0;
int SDL_old_txcursorx=0;int SDL_old_txcursory=0;
char SDL_text_fallback=1;
int framenumber=0;
_u32 * screen;
_u32 * canvas;
_u32 SDL_color;
_u8 gfx_txselectmode=0;//bit0: text to draw is selected /bit1: currently drawing between the selection positions
struct gfx_bufferset_
{
	int screensizex,screensizey,canvassizex,canvassizey,canvasminx,canvasminy,canvasmaxx,canvasmaxy;
	_u32 *screen,*canvas;
	float scrollx,scrolly,zoomx,zoomy;
	int depth;
};
gfx_bufferset_ gfx_old_bufferset;
void gfx_store_bufferset(gfx_bufferset_ * target)
{
	target->screensizex=gfx_screensizex;
	target->screensizey=gfx_screensizey;
	target->canvassizex=gfx_canvassizex;
	target->canvassizey=gfx_canvassizey;
	target->canvasminx=gfx_canvasminx;
	target->canvasminy=gfx_canvasminy;
	target->canvasmaxx=gfx_canvasmaxx;
	target->canvasmaxy=gfx_canvasmaxy;
	target->screen=screen;
	target->canvas=canvas;
	target->scrollx=SDL_scrollx;
	target->scrolly=SDL_scrolly;
	target->zoomx=SDL_zoomx;
	target->zoomy=SDL_zoomy;
	target->depth=gfx_depth;
}
void gfx_restore_bufferset(gfx_bufferset_ * target)
{
	gfx_screensizex=target->screensizex;
	gfx_screensizey=target->screensizey;
	gfx_canvassizex=target->canvassizex;
	gfx_canvassizey=target->canvassizey;
	gfx_canvasminx=target->canvasminx;
	gfx_canvasminy=target->canvasminy;
	gfx_canvasmaxx=target->canvasmaxx;
	gfx_canvasmaxy=target->canvasmaxy;
	screen=target->screen;
	canvas=target->canvas;
	SDL_scrollx=target->scrollx;
	SDL_scrolly=target->scrolly;
	SDL_zoomx=target->zoomx;
	SDL_zoomy=target->zoomy;
	gfx_depth=target->depth;
}
int SDL_linestyle;

int gfx_get_colorstringv(int number)
{
	SDL_color=number;
	return 0;
}
void gfx_express_txinit(char ialignment,float iposx,float iposy,float iatomfontheight,float iangle)
{
	SDL_text_fallback=0;
	SDL_txangle=iangle;
	if ((SDL_zoomx>1.01) || (SDL_zoomx<0.99))
	{
		SDL_text_fallback=0;
	}
	if ((iangle>0.01) || (iangle<-0.01))
	{
		SDL_text_fallback=1;
	}
	if (SDL_text_fallback)
	{
		SDL_txcursorx=-500;SDL_txcursory=666.666666;
		SDL_glyfstartx=iposx;
		SDL_glyfstarty=iposy;
	}
	else
	{
		SDL_txcursorx=(iposx-SDL_scrollx)*SDL_zoomx-3;SDL_txcursory=(iposy-SDL_scrolly)*SDL_zoomy+4;
	}
	SDL_old_txcursorx=SDL_txcursorx;SDL_old_txcursory=SDL_txcursory;
}
inline void gfx_express_text_tail()
{
	gfx_txselectmode=0;
}
inline void putpixel(int iposx,int iposy)
{
	if ((iposx>=0) && (iposx<gfx_canvassizex))
	{
		if ((iposy>=0) && (iposy<gfx_canvassizey))
		{
			canvas[gfx_screensizex*(iposy)+iposx]=SDL_color;
		}
	}
}
inline void gfx_stylegenestring(int flags,unsigned int fillcolor=0) //1: stroke 2: fill 4: bold 8: dashed
{
	SDL_linestyle=flags;
}

void gfx_expressfillrectangle(int startx,int starty,int endx,int endy,_u32 color)
{
	if (endx<startx)
	{
		int swap=endx;
		endx=startx;
		startx=swap;
	}
	if (endy<starty)
	{
		int swap=endy;
		endy=starty;
		starty=swap;
	}
	startx=max(startx,0);
	endx=min(endx,gfx_canvassizex);
	starty=max(starty,0);
	endy=min(endy,gfx_canvassizey);
	for (int ilv1=starty;ilv1<endy;ilv1++)
	{
		_u32 * pointer=canvas+ilv1*gfx_screensizex+startx;
		int count=endx-startx;
		for (int ilv2=0;ilv2<count;ilv2++)
		{
			*(pointer++)=color;
		}
	}
}
void gfx_expressbeziertrack(cdx_Bezierpoints * ipoints)
{
	float ishare;
	float iminusshare;
	int ix,iy;
	for (int ilv1=0;ilv1<(*ipoints).count;ilv1+=3)
	{
		float stepsbecausex=2*(abs((*ipoints).a[ilv1+3].x-(*ipoints).a[ilv1].x)+abs((*ipoints).a[ilv1+2].x-(*ipoints).a[ilv1+1].x));
		float stepsbecausey=2*(abs((*ipoints).a[ilv1+3].y-(*ipoints).a[ilv1].y)+abs((*ipoints).a[ilv1+2].y-(*ipoints).a[ilv1+1].y));
		if (stepsbecausey>stepsbecausex) {stepsbecausex=stepsbecausey;}
		for (int ilv2=0;ilv2<stepsbecausex;ilv2++)
		{
			ishare=(float(ilv2)/stepsbecausex);
			iminusshare=1-ishare;
			ix=(((*ipoints).a[ilv1].x*ishare+(*ipoints).a[ilv1+1].x*iminusshare)*ishare+((*ipoints).a[ilv1+1].x*ishare+(*ipoints).a[ilv1+2].x*iminusshare)*iminusshare)*ishare+
			(((*ipoints).a[ilv1+1].x*ishare+(*ipoints).a[ilv1+2].x*iminusshare)*ishare+((*ipoints).a[ilv1+2].x*ishare+(*ipoints).a[ilv1+3].x*iminusshare)*iminusshare)*iminusshare;
			iy=(((*ipoints).a[ilv1].y*ishare+(*ipoints).a[ilv1+1].y*iminusshare)*ishare+((*ipoints).a[ilv1+1].y*ishare+(*ipoints).a[ilv1+2].y*iminusshare)*iminusshare)*ishare+
			(((*ipoints).a[ilv1+1].y*ishare+(*ipoints).a[ilv1+2].y*iminusshare)*ishare+((*ipoints).a[ilv1+2].y*ishare+(*ipoints).a[ilv1+3].y*iminusshare)*iminusshare)*iminusshare;
			putpixel(ix,iy);
		}
	}
}
void gfx_expressxbezier(int icount,...)
{
	cdx_Bezierpoints tlpoints;
	tlpoints.count=icount;
	va_list i_valist;
	va_start(i_valist,icount);
	float iks;float yps;
	
	for (int ilv1=0;ilv1<icount;ilv1++)
	{
		tlpoints.a[ilv1].x=(va_arg(i_valist,double)-SDL_scrollx)*SDL_zoomx;
		tlpoints.a[ilv1].y=(va_arg(i_valist,double)-SDL_scrolly)*SDL_zoomy;
	}
	va_end(i_valist);
	gfx_expressbeziertrack(&tlpoints);
}

void gfx_expressbezier2(float x1,float y1,float x2,float y2,float x3,float y3)
{
	float ishare;
	float iminusshare;
	float ix,iy;
	x1=((x1-SDL_scrollx)*SDL_zoomx);
	x2=((x2-SDL_scrollx)*SDL_zoomx);
	x3=((x3-SDL_scrollx)*SDL_zoomx);
	y1=((y1-SDL_scrolly)*SDL_zoomy);
	y2=((y2-SDL_scrolly)*SDL_zoomy);
	y3=((y3-SDL_scrolly)*SDL_zoomy);
	float stepsbecausex=2.2*max(max(x1,x2),x3)-min(min(x1,x2),x3);
	float stepsbecausey=2.2*max(max(y1,y2),y3)-min(min(y1,y2),y3);
	if (stepsbecausey>stepsbecausex) {stepsbecausex=stepsbecausey;}
	for (float ilv2=0;ilv2<=stepsbecausex;ilv2++)
	{
		ishare=(float(ilv2)/stepsbecausex);
		iminusshare=1-ishare;
		ix=(x1+(x2-x1)*ishare)*iminusshare+(x2+(x3-x2)*ishare)*ishare;
		iy=(y1+(y2-y1)*ishare)*iminusshare+(y2+(y3-y2)*ishare)*ishare;
		putpixel(ix,iy);
	}
}
void gfx_expressbezier(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4)
{
	float ishare;
	float iminusshare;
	float ix,iy;
	x1=((x1-SDL_scrollx)*SDL_zoomx);
	x2=((x2-SDL_scrollx)*SDL_zoomx);
	x3=((x3-SDL_scrollx)*SDL_zoomx);
	x4=((x4-SDL_scrollx)*SDL_zoomx);
	y1=((y1-SDL_scrolly)*SDL_zoomy);
	y2=((y2-SDL_scrolly)*SDL_zoomy);
	y3=((y3-SDL_scrolly)*SDL_zoomy);
	y4=((y4-SDL_scrolly)*SDL_zoomy);
	float stepsbecausex=3.3*max(max(x1,x2),max(x3,x4))-min(min(x1,x2),min(x3,x4));
	float stepsbecausey=3.3*max(max(y1,y2),max(y3,y4))-min(min(y1,y2),min(y3,y4));
	if (stepsbecausey>stepsbecausex) {stepsbecausex=stepsbecausey;}
	for (int ilv2=0;ilv2<stepsbecausex;ilv2++)
	{
		ishare=(float(ilv2)/stepsbecausex);
		iminusshare=1-ishare;
		ix=((x1*ishare+x2*iminusshare)*ishare+(x2*ishare+x3*iminusshare)*iminusshare)*ishare+
		((x2*ishare+x3*iminusshare)*ishare+(x3*ishare+x4*iminusshare)*iminusshare)*iminusshare;
		iy=((y1*ishare+y2*iminusshare)*ishare+(y2*ishare+y3*iminusshare)*iminusshare)*ishare+
		((y2*ishare+y3*iminusshare)*ishare+(y3*ishare+y4*iminusshare)*iminusshare)*iminusshare;
		putpixel(ix,iy);
	}
}

void gfx_expressline(float ileft,float itop,float iright,float ibottom)
{
	int x=(ileft-SDL_scrollx)*SDL_zoomx;
	int y=(itop-SDL_scrolly)*SDL_zoomy;
	int x2=(iright-SDL_scrollx)*SDL_zoomx;
	int y2=(ibottom-SDL_scrolly)*SDL_zoomy;
	int x3;
	int y3;
	if (x<0)
	{
		if (x2<0)
		{
			return;
		}
	}
	if (x>gfx_canvassizex)
	{
		if (x2>gfx_canvassizex)
		{
			return;
		}
	}
	float m;float t;
	if (x>x2)
	{
		x3=x;
		x=x2;
		x2=x3;
		y3=y;
		y=y2;
		y2=y3;
	}
	if (x!=x2)
	{
		if (x<0)
		{
			float m=(float(y2)-y)/(x2-x);
			float t=y-x*m;
			x=0;
			y=t;
		}
		if (x2>gfx_canvassizex)
		{
			float m=(float(y2)-y)/(x2-x);
			float t=y-x*m;
			x2=gfx_canvassizex;
			y2=t+m*gfx_canvassizex;
		}
	}
	if (y<0)
	{
		if (y2<0)
		{
			return;
		}
	}
	if (y>gfx_canvassizey)
	{
		if (y2>gfx_canvassizey)
		{
			return;
		}
	}
	if (y>y2)
	{
		y3=y;
		y=y2;
		y2=y3;
		x3=x;
		x=x2;
		x2=x3;
	}
	if (y!=y2)
	{
		if (y<0)
		{
			float m=(float(x2)-x)/(y2-y);
			float t=x-y*m;
			y=0;
			x=t;
		}
		if (y2>gfx_canvassizey)
		{
			float m=(float(x2)-x)/(y2-y);
			float t=x-y*m;
			y2=gfx_canvassizey;
			x2=t+m*gfx_canvassizey;
		}
	}
	if (x<0)
	{
		x=0;
	}
	if (x>gfx_canvassizex)
	{
		x=gfx_canvassizey;
	}
	if (y2<0)
	{
		y2=0;
	}
	if (y2>gfx_canvassizey)
	{
		y2=gfx_canvassizey;
	}
	if (abs(x2-x)>=abs(y2-y))
	{
		if (SDL_linestyle&4)
		{
			SDL_linestyle&=~4;
			gfx_expressline(ileft,itop-1,iright,ibottom-1);
			gfx_expressline(ileft,itop+1,iright,ibottom+1);
			SDL_linestyle|=4;
		}
		if (x2==x)//it can be that still x width is zero.
		{
			canvas[gfx_screensizex*y2+x]=SDL_color;
		}
		if (x2<x)
		{
			y3=y;
			y=y2;
			y2=y3;
			x3=x;
			x=x2;
			x2=x3;
		}
		float slope=float(y2-y)/(x2-x);
		for (int ilv1=0;ilv1<x2-x;ilv1++)
		{
			int vertical=y+ilv1*slope;
			canvas[gfx_screensizex*vertical+ilv1+x]=SDL_color;
			if (SDL_linestyle & 4)
			{
				_u32 * place=canvas+(gfx_screensizex*(vertical-1)+ilv1+x);
				if (vertical>0)
				{
					*place=SDL_color;
				}
				place+=(gfx_screensizex<<1);
				if (vertical<(gfx_canvassizey-1))
				{
					*place=SDL_color;
				}
			}
		}
	}
	else
	{
		if (SDL_linestyle&4)
		{
			SDL_linestyle&=~4;
			gfx_expressline(ileft-1,itop,iright-1,ibottom);
			gfx_expressline(ileft+1,itop,iright+1,ibottom);
			SDL_linestyle|=4;
		}
		if (y2<y)
		{
			x3=x;
			x=x2;
			x2=x3;
			y3=y;
			y=y2;
			y2=y3;
		}
		float slope=float(x2-x)/(y2-y);
		for (int ilv1=0;ilv1<y2-y;ilv1++)
		{
			int horizontal=x+ilv1*slope;
			canvas[gfx_screensizex*(ilv1+y)+horizontal]=SDL_color;
			if (SDL_linestyle & 4)
			{
				_u32 * place=canvas+(gfx_screensizex*(ilv1+y)+horizontal-1);
				if ((horizontal>0))
				{
					*place=SDL_color;
				}
				place+=2;
				if (horizontal<(gfx_canvassizex-1))
				{
					*place=SDL_color;
				}
			}
		}
	}
	return;
}
SDL_Surface *video;
#ifdef SDL2
SDL_Window *window;
#endif
#ifndef NOCLIPBOARD
Window gfx_Window;
Display * gfx_Display;
Atom clipboard_sseln = XA_PRIMARY;
Atom clipboard_target = XA_STRING;
#endif
char * LHENDRAW_clipboardbuffer=NULL;
long unsigned int LHENDRAW_clipboardbuffer_count;

void screenclear(_u32 icolor)
{
	for (int ilv1=0;ilv1<gfx_screensizex*gfx_screensizey;ilv1++)
	{
		screen[ilv1]=icolor;
	}
}
int gfx()
{
	#ifdef SDL2
	window=SDL_CreateWindow("Lhendraw",0,0,gfx_STANDARDX,gfx_STANDARDY,0);
	video=SDL_GetWindowSurface(window);
	#else
	video=SDL_SetVideoMode(gfx_screensizex,gfx_screensizey,32,SDL_SWSURFACE);
	#endif
	#ifndef NOCLIPBOARD
	gfx_Display=XOpenDisplay(NULL);
	clipboard_sseln=XA_CLIPBOARD(gfx_Display);
	gfx_Window=XCreateSimpleWindow(gfx_Display,DefaultRootWindow(gfx_Display),0,0,1,1,0,0,0);
	XSelectInput(gfx_Display,gfx_Window,PropertyChangeMask);
	#endif
	if ( video == NULL ) 
	{
		fprintf(stderr,"Ich konnte kein Fenster mit der Auflösung %i*%i öffnen: %s\n",gfx_STANDARDX,gfx_STANDARDY,SDL_GetError());
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
	canvas=screen+gfx_canvasminx+gfx_screensizex*gfx_canvasminy;
	return 1;
}
void gfx_gfxstop()
{
	if ( SDL_MUSTLOCK(video) ) 
	{
		SDL_UnlockSurface(video);
	}
	#ifdef SDL2
	SDL_UpdateWindowSurface(window);
	#else
	SDL_UpdateRect(video,0,0,gfx_screensizex,gfx_screensizey);
	#endif
}

#define if_gfx_geo if ((SDL_linestyle & 2)==0)

int gfx_bks[bezierpointmax];//Worst case, every curve making an s
float gfx_type;//1: line 3: cubic bezier 0x100: bezier part, order depending on the last preceding
float gfx_m[bezierpointmax];//Only for polygons
char * gfx_linewisebuffer;
int ibrakelist_count;

struct gfx_geometry_
{
	char type;//0: at start 1: line drawn 2: loop closed
	float startx,starty;
	int currentx,currenty;
	int left,right,top,bottom;
	int lastdirection;//1: downward 2: upward
	int firstdirection;//0: none, save to "firstdirection" 1: downward 2: upward
};
gfx_geometry_ gfx_geometry;

int __attribute__((warn_unused_result)) gfx_expressgeometry_start(float left,float top,float right,float bottom)
{
	gfx_geometry.type=0;
	if_gfx_geo return 1;
	gfx_geometry.left=(left-SDL_scrollx)*SDL_zoomx;
	gfx_geometry.right=(right-SDL_scrollx)*SDL_zoomx;
	gfx_geometry.top=(top-SDL_scrolly)*SDL_zoomy;
	gfx_geometry.bottom=(bottom-SDL_scrolly)*SDL_zoomy;
	if (fmod(bottom,1)!=0) gfx_geometry.bottom++;
	if (fmod(right,1)!=0) gfx_geometry.right++;
	if (gfx_geometry.right>gfx_canvassizex) {gfx_geometry.right=gfx_canvassizex; if (gfx_geometry.left>=gfx_canvassizex) return 0;}
	if (gfx_geometry.bottom>gfx_canvassizey) {gfx_geometry.bottom=gfx_canvassizey; if (gfx_geometry.top>=gfx_canvassizey) return 0;}
	if (gfx_geometry.left<0) {gfx_geometry.left=0; if (gfx_geometry.right<0) return 0;}
	if (gfx_geometry.top<0) {gfx_geometry.top=0; if (gfx_geometry.bottom<0) return 0;}
	for (int ilv1=gfx_geometry.top;ilv1<gfx_geometry.bottom;ilv1++)
	{
		char * tl_buffer=gfx_linewisebuffer+(ilv1*gfx_canvassizex);
		for (int ilv2=gfx_geometry.left;ilv2<gfx_geometry.right;ilv2++)
		{
			tl_buffer[ilv2]=0;
		}
	}
	return 1;
}
#define GFX_GEONEXT(MACROPARAM)\
{\
	gfx_geometry.lastdirection=(MACROPARAM);\
	if (gfx_geometry.firstdirection==0) {fprintf(stderr,"WARNsdlgfx.hxx:536");}\
}
void gfx_expressgeometry_neutro(int currentdirection,char halt=0)
{
	if_gfx_geo return;
	if (gfx_geometry.firstdirection==0) {gfx_geometry.firstdirection=currentdirection;return;}
	
	if (gfx_geometry.lastdirection!=currentdirection) return;
	if ((gfx_geometry.currenty)<gfx_geometry.top) return;
	if ((gfx_geometry.currenty)>=gfx_geometry.bottom) return;
	if (gfx_geometry.currentx>=gfx_geometry.right) return;
	int medix=gfx_geometry.currentx;
	if (medix<gfx_geometry.left) medix=gfx_geometry.left;
	canvas[gfx_geometry.currenty*gfx_screensizex+medix+1]=0xFF00;
	if ((gfx_geometry.currenty>=0) && (gfx_geometry.currenty<gfx_geometry.bottom))
	{
		gfx_linewisebuffer[gfx_geometry.currenty*gfx_canvassizex+medix]^=1;
	}
}
void gfx_expressgeometry_begin(float x,float y)
{
	if (gfx_geometry.type!=0) gfx_expressgeometry_neutro(gfx_geometry.firstdirection);
	gfx_geometry.type=1;
	if_gfx_geo
	{
		gfx_geometry.startx=x;
		gfx_geometry.starty=y;
		gfx_geometry.currentx=x;
		gfx_geometry.currenty=y;
		return;
	}
	gfx_geometry.currentx=(x-SDL_scrollx)*SDL_zoomx;
	gfx_geometry.currenty=(y-SDL_scrolly)*SDL_zoomy;
	gfx_geometry.startx=x;
	gfx_geometry.starty=y;
	gfx_geometry.firstdirection=0;
	gfx_geometry.lastdirection=0;
}
void gfx_expressgeometry_end()
{
	if_gfx_geo return;
	if (gfx_geometry.type==1) gfx_expressgeometry_neutro(gfx_geometry.firstdirection);
	for (int ilv1=gfx_geometry.top;ilv1<gfx_geometry.bottom;ilv1++)
	{
		char state=0;
		for (int ilv2=gfx_geometry.left;ilv2<gfx_geometry.right;ilv2++)
		{
			state^=gfx_linewisebuffer[ilv1*gfx_canvassizex+ilv2];
			if (state & 1)
			{
				canvas[ilv1*gfx_screensizex+ilv2]=SDL_color;
			}
				if (state & 0x2)
				canvas[ilv1*gfx_screensizex+ilv2]^=0xFF;
		}
	}
	gfx_geometry.type=2;
}
void gfx_expressgeometry_line(float x,float y)
{
	if (gfx_geometry.type!=1)
	{
		gfx_expressgeometry_begin(x,y);
		return;
	}
	if_gfx_geo
	{
		gfx_expressline(gfx_geometry.currentx,gfx_geometry.currenty,x,y);
		gfx_geometry.currentx=x;
		gfx_geometry.currenty=y;
		return;
	}
	int ix=(x-SDL_scrollx)*SDL_zoomx;
	int iy=(y-SDL_scrolly)*SDL_zoomy;
	char swapped=0;
	if (gfx_geometry.currenty>iy)
	{
		int swap=gfx_geometry.currenty;
		gfx_expressgeometry_neutro(2);
		gfx_geometry.currenty=iy;
		iy=swap;
		swap=gfx_geometry.currentx;
		gfx_geometry.currentx=ix;
		ix=swap;
		swapped=1;
	}
	else
	{
		if (iy!=gfx_geometry.currenty)
		gfx_expressgeometry_neutro(1);
	}
	if (iy!=gfx_geometry.currenty)
	{
		float m=(ix-gfx_geometry.currentx)/((float)(iy-gfx_geometry.currenty));
		float t=gfx_geometry.currentx-(gfx_geometry.currenty*m);
		for (int ilv1=gfx_geometry.currenty;ilv1<=iy;ilv1++)
		{
			int medix=(ilv1-gfx_geometry.currenty)*m+gfx_geometry.currentx;
			if (medix<gfx_geometry.left) medix=gfx_geometry.left;
			if (medix>=gfx_geometry.right) medix=gfx_geometry.right-1;
			if ((ilv1>=0) && (ilv1<gfx_geometry.bottom))
			{
				gfx_linewisebuffer[ilv1*gfx_canvassizex+medix]^=1;
			}
		}
	}
	if (iy!=gfx_geometry.currenty)
	GFX_GEONEXT((3*swapped)^1);
	if (!swapped)
	{
		gfx_geometry.currentx=ix;
		gfx_geometry.currenty=iy;
	}
}
void gfx_expressgeometry_bezier2(float inx1,float iny1,float x2,float y2)
{
	if_gfx_geo
	{
		gfx_expressbezier2(gfx_geometry.currentx,gfx_geometry.currenty,inx1,iny1,x2,y2);
		gfx_geometry.currentx=x2;
		gfx_geometry.currenty=y2;
		return;
	}
	int startx=gfx_geometry.currentx;
	int starty=gfx_geometry.currenty;
	x2=(x2-SDL_scrollx)*SDL_zoomx;
	y2=(y2-SDL_scrolly)*SDL_zoomy;
	int x0=x2;
	int y0=y2;
	int x1=(int)((inx1-SDL_scrollx)*SDL_zoomx);
	int y1=(int)((iny1-SDL_scrolly)*SDL_zoomy);
	char swapped=0;
	int lasty;//make sure it's smaller than the actual start in first place!
	int endy;
	float fragment;
	float currentlevel;
	float lastlevel;
	int medix,mediy;
	char hasotherhalf;
	if (starty>y0)
	{
		int swap=starty;
		starty=y0;
		y0=swap;
		swap=startx;
		startx=x0;
		x0=swap;
		swapped=1;
	}
	hasotherhalf=0;
	iotherhalfback:;
	if ((y1>=starty) && (y1<=y0))
	{
		gfx_expressgeometry_neutro((3*swapped)^1);
		lasty=starty-1;

		endy=y0;
		currentlevel=0;
		lastlevel=1;
		GFX_GEONEXT((3*swapped)^1);
		if (gfx_geometry.lastdirection==2) gfx_geometry.lastdirection=2;
	}
	else
	{
		if (y1<starty)
		{
			if (hasotherhalf==1)
			{
				lasty=y1-1;
				endy=starty;
				currentlevel=(starty-y1)/(float)((y0-y1)+(starty-y1));
				lastlevel=0;
				hasotherhalf=2;
			}
			else
			{
				gfx_expressgeometry_neutro(2);
				lasty=y1-1;
				endy=y0;
				currentlevel=(starty-y1)/(float)((y0-y1)+(starty-y1));
				lastlevel=1;
				hasotherhalf=1;
				GFX_GEONEXT(1);
			}
		}
		else
		{
			if (hasotherhalf==1)
			{
				lasty=y0-1;
				endy=y1;
				currentlevel=1;
				lastlevel=(starty-y1)/(float)((y0-y1)+(starty-y1));
				hasotherhalf=2;
			}
			else
			{
				gfx_expressgeometry_neutro(1);
				lasty=starty-1;
				endy=y1;
				currentlevel=0;
				lastlevel=(starty-y1)/(float)((y0-y1)+(starty-y1));
				hasotherhalf=1;
				GFX_GEONEXT(2);
			}
		}
	}
	fragment=0.45*(lastlevel-currentlevel)/(float)(endy-lasty+1);
	if (fragment==0) {fragment=(hasotherhalf==2)?-0.001:0.001;}
	do
	{
		mediy=((starty+((y1-starty)*currentlevel))*(1-currentlevel))+((y1+(y0-y1)*currentlevel)*currentlevel);
		if ((mediy>lasty))
		{
			lasty=mediy;
			lastline:;
			medix=((startx+((x1-startx)*currentlevel))*(1-currentlevel))+((x1+(x0-x1)*currentlevel)*currentlevel);
			if ((lasty>=0) && (lasty<gfx_canvassizey) && (medix<gfx_canvassizex))
			{
				if (medix<gfx_geometry.left) medix=gfx_geometry.left;
				if (medix>=gfx_geometry.right) medix=gfx_geometry.right-1;
				if ((lasty>=0) && (lasty<gfx_geometry.bottom))
				{
					gfx_linewisebuffer[lasty*gfx_canvassizex+medix]^=1+(lasty>endy)*2;
				}
			}
		}
		currentlevel+=fragment;
	}
	while ((((currentlevel<=lastlevel) && (hasotherhalf!=2)) || ((currentlevel>=lastlevel) && (hasotherhalf==2))) && (lasty<endy));
	if (lasty!=endy)
	{
		currentlevel=lastlevel;
		lasty=endy;
		goto lastline;
	}
	if (hasotherhalf==1) goto iotherhalfback;
	gfx_geometry.currentx=x2;
	gfx_geometry.currenty=y2;
}
float gfx_expressgeometry_calcbezier3apogee(int which,int starty,int y1,int y2,int y0)//which=0: check if there are 2 points,retval +1 if yes, -1 if no 1: first point 2: second point
{
	float a=-3*starty+9*y1-9*y2+3*y0;
	float b=6*starty-12*y1+6*y2;
	float c=-3*starty+3*y1;
			//A calculation follows
//			starty*(1-currentlevel)*(1-currentlevel)*(1-currentlevel)+y1*currentlevel*(1-currentlevel)*(1-currentlevel)*3+y2*(1-currentlevel)*currentlevel*currentlevel*3+y0*currentlevel*currentlevel*currentlevel;
/*y=
			starty-3*starty*currentlevel+3*starty*currentlevel*currentlevel-starty*currentlevel*currentlevel*currentlevel+
			+3*y1*currentlevel-6*y1*currentlevel*currentlevel+3*y1*currentlevel*currentlevel*currentlevel+
			+3*y2*currentlevel*currentlevel-3*y2*currentlevel*currentlevel*currentlevel+
			+y0*currentlevel*currentlevel*currentlevel;
			*/
/*y'=
			-3*starty+6*starty*currentlevel-3*starty*currentlevel*currentlevel+
			+3*y1-12*y1*currentlevel+9*y1*currentlevel*currentlevel+
			+6*y2*currentlevel-9*y2*currentlevel*currentlevel+
			+3*y0*currentlevel*currentlevel=0
<=>			*/
/*c=3*starty+3*y1; b=6*starty-12*y1+6*y2; a=-3*starty+9*y1-9*y2+3*y0*/
	if ((a<0.00001) && (a>-0.00001))
	{
		//fitting with parabola
		//parabolaequation=-a*x*x+b*x
		//parabolaequation'=-2*a*x+b=0
		float a=(3)*(y1-((y0/3)+(starty*2/3)));
		if ((a<0.00001) && (a>-0.00001))
		{
			if (which==0) return -1;
			return 0;//good approximation for "NO" max/min
		}
		if (which==0) return -2;
		return ((y0-starty)/2/a)+0.5;
	}
	if ((b*b-(4*a*c))<0)
	{
		if (which==0) return -1; else return 0;
	}
	float resultat1=(-b-sqrt(b*b-(4*a*c)))/(2*a);
	float resultat2=(-b+sqrt(b*b-(4*a*c)))/(2*a);
	if (resultat2<resultat1)
	{
		float resultat3=resultat1;
		resultat1=resultat2;
		resultat2=resultat3;
	}
	if ((which==0) && ((resultat2>=1) || (resultat1<=0))) return -1;
	if (resultat2>1) resultat2=resultat1;
	if (resultat1<0) resultat1=resultat2;
	if (resultat1>1) resultat1=1;
	if (resultat2<0) resultat2=0;
	if (which==0) return 1;
	if (which==1) return resultat1; else return resultat2;
}
			
void gfx_expressgeometry_bezier3(float inx1,float iny1,float inx2,float iny2,float x3,float y3)
{
	if_gfx_geo
	{
		gfx_expressbezier(gfx_geometry.currentx,gfx_geometry.currenty,inx1,iny1,inx2,iny2,x3,y3);
		gfx_geometry.currentx=x3;
		gfx_geometry.currenty=y3;
		return;
	}
	int startx=gfx_geometry.currentx;
	int starty=gfx_geometry.currenty;
	int x2=(int)((inx2-SDL_scrollx)*SDL_zoomx);
	int y2=(int)((iny2-SDL_scrolly)*SDL_zoomy);
	x3=(x3-SDL_scrollx)*SDL_zoomx;
	y3=(y3-SDL_scrolly)*SDL_zoomy;
	int x0=x3;
	int y0=y3;
	int x1=(int)((inx1-SDL_scrollx)*SDL_zoomx);
	int y1=(int)((iny1-SDL_scrolly)*SDL_zoomy);
	char swapped=0;
	int lasty;//make sure it's smaller than the actual start in first place!
	int endy;
	float fragment;
	float currentlevel;
	float lastlevel;
	int medix,mediy;
	char hasotherhalf;
	if (starty>y0)
	{
		int swap=starty;
		starty=y0;
		y0=swap;
		swap=startx;
		startx=x0;
		x0=swap;
		swap=y1;
		y1=y2;
		y2=swap;
		swap=x1;
		x1=x2;
		x2=swap;
		swapped=1;
	}
	hasotherhalf=-1;
	float knotpattern=gfx_expressgeometry_calcbezier3apogee(0,starty,y1,y2,y0);
	iotherhalfback:;
	if ((knotpattern>0) && (y2<y1))//Total surge
	{
		if (hasotherhalf==1)
		{
			lasty=starty-1;
			endy=y1;
			currentlevel=0;
			lastlevel=gfx_expressgeometry_calcbezier3apogee(1,starty,y1,y2,y0);
			hasotherhalf=3;
		}
		else
		if (hasotherhalf==4)
		{
			lasty=y2-1;
			endy=y0;
			currentlevel=gfx_expressgeometry_calcbezier3apogee(2,starty,y1,y2,y0);
			lastlevel=1;
			hasotherhalf=1;
		}
		else
		{
			gfx_expressgeometry_neutro((3*swapped)^1);
			lasty=y2-1;
			endy=y1;
			currentlevel=gfx_expressgeometry_calcbezier3apogee(2,starty,y1,y2,y0);
			lastlevel=gfx_expressgeometry_calcbezier3apogee(1,starty,y1,y2,y0);
			hasotherhalf=4;
			GFX_GEONEXT((3*swapped)^1);
		}
	}
	else
	{
		if ((y1>starty) && (y2<y0))
		{
			if (y1>y0)
			{
				if (knotpattern<-1.5)
				{
					goto i_lower_bend;
				}
				else
				{
					goto i_linear;
				}
			}
			else
			{//Plain ramp
				i_linear:;
				gfx_expressgeometry_neutro((3*swapped)^1);
				lasty=starty-1;
				endy=y0;
				currentlevel=0;
				lastlevel=1;
				GFX_GEONEXT((3*swapped)^1);
				if (gfx_geometry.lastdirection==2) gfx_geometry.lastdirection=2;
			}
		}
		else
		{
			if (y1<=starty)
			{
				if (y2>y0)//WIDE SURGE
				{
					if (hasotherhalf==4)
					{
						lasty=y1-1;
						endy=starty;
						currentlevel=gfx_expressgeometry_calcbezier3apogee(1,starty,y1,y2,y0);
						lastlevel=0;
						hasotherhalf=2;
					}
					else
					if (hasotherhalf==1)
					{
						lasty=y0-1;
						endy=y2;
						currentlevel=1;
						lastlevel=gfx_expressgeometry_calcbezier3apogee(2,starty,y1,y2,y0);
						hasotherhalf=4;
					}
					else
					{
						gfx_expressgeometry_neutro((3*swapped)^2);
						lasty=y1-1;
						endy=y2;
						currentlevel=gfx_expressgeometry_calcbezier3apogee(1,starty,y1,y2,y0);
						lastlevel=gfx_expressgeometry_calcbezier3apogee(2,starty,y1,y2,y0);
						hasotherhalf=1;
						GFX_GEONEXT((3*swapped)^2);
					}
				}
				else
				{//upwards
					if (hasotherhalf==1)
					{
						lasty=min(y1,y2)-1;
						endy=starty;
						currentlevel=gfx_expressgeometry_calcbezier3apogee(1,starty,y1,y2,y0);
						lastlevel=0;
						hasotherhalf=2;
					}
					else
					{
						gfx_expressgeometry_neutro(2);
						lasty=min(y1,y2)-1;
						endy=y0;
						currentlevel=gfx_expressgeometry_calcbezier3apogee(1,starty,y1,y2,y0);
						lastlevel=1;
						hasotherhalf=1;
						GFX_GEONEXT(1);
					}
				}
			}
			else
			{
				i_lower_bend:;
				if (hasotherhalf==1)
				{
					lasty=y0-1;
					endy=max(y1,y2);
					currentlevel=1;
					lastlevel=gfx_expressgeometry_calcbezier3apogee(1,starty,y1,y2,y0);
					hasotherhalf=2;
				}
				else
				{
					gfx_expressgeometry_neutro(1);
					lasty=starty-1;
					endy=max(y1,y2);
					currentlevel=0;
					lastlevel=gfx_expressgeometry_calcbezier3apogee(1,starty,y1,y2,y0);
					hasotherhalf=1;
					GFX_GEONEXT(2);
				}
			}
		}
	}
	fragment=0.30*(lastlevel-currentlevel)/(float)(endy-lasty+1)*fabs(1.0/(float)(endy-lasty+1));
	if (fragment==0) {fragment=((hasotherhalf%2)==0)?-0.001:0.001;}
	do
	{
		mediy=starty-3*starty*currentlevel+3*starty*currentlevel*currentlevel-starty*currentlevel*currentlevel*currentlevel+
		+3*y1*currentlevel-6*y1*currentlevel*currentlevel+3*y1*currentlevel*currentlevel*currentlevel+
		+3*y2*currentlevel*currentlevel-3*y2*currentlevel*currentlevel*currentlevel+
		+y0*currentlevel*currentlevel*currentlevel;
		if ((mediy>lasty))
		{
			lasty=mediy;
			lastline:;
			medix=startx-3*startx*currentlevel+3*startx*currentlevel*currentlevel-startx*currentlevel*currentlevel*currentlevel+
			+3*x1*currentlevel-6*x1*currentlevel*currentlevel+3*x1*currentlevel*currentlevel*currentlevel+
			+3*x2*currentlevel*currentlevel-3*x2*currentlevel*currentlevel*currentlevel+
			+x0*currentlevel*currentlevel*currentlevel;
			if ((lasty>=0) && (lasty<gfx_canvassizey) && (medix<gfx_canvassizex))
			{
				if (medix<gfx_geometry.left) medix=gfx_geometry.left;
				if (medix>=gfx_geometry.right) medix=gfx_geometry.right-1;
				if ((lasty>=0) && (lasty<gfx_geometry.bottom))
				{
					gfx_linewisebuffer[lasty*gfx_canvassizex+medix]^=1+(lasty>endy)*2;
				}
			}
		}
//		printf("-%i,starty:%i,endy:%i-\n",mediy,starty,endy);
		currentlevel+=fragment;
	}
	while ((((currentlevel<=lastlevel) && ((hasotherhalf%2)!=0)) || ((currentlevel>=lastlevel) && ((hasotherhalf%2)==0))) && (lasty<endy));
	if (lasty!=endy)
	{
		currentlevel=lastlevel;
		lasty=endy;
		goto lastline;
	}
	if ((hasotherhalf==1) || (hasotherhalf==4)) goto iotherhalfback;
	gfx_geometry.currentx=x3;
	gfx_geometry.currenty=y3;
}
void gfx_expressgeometry_backline()
{
	if (gfx_geometry.type==1)
	{
		if_gfx_geo
		{
			gfx_expressline(gfx_geometry.currentx,gfx_geometry.currenty,gfx_geometry.startx,gfx_geometry.starty);
			return;
		}
		gfx_expressgeometry_line(gfx_geometry.startx,gfx_geometry.starty);
		gfx_expressgeometry_neutro(gfx_geometry.firstdirection);
		gfx_geometry.type=2;
		gfx_geometry.firstdirection=0;
	}
}

int gfx_expressinfinityangle(int count)
{
	intl iminx,imaxx,iminy,imaxy;
	if_gfx_geo
	{
		for (int ilv1=0;ilv1<count;ilv1++)
		{
			gfx_expressline(LHENDRAW_inficorn[ilv1].x,LHENDRAW_inficorn[ilv1].y,LHENDRAW_inficorn[(ilv1+1)%count].x,LHENDRAW_inficorn[(ilv1+1)%count].y);
		}
		return 1;
	}
	iminx=10000;
	iminy=10000;
	imaxx=-10000;
	imaxy=-10000;
	for (int ilv1=0;ilv1<count;ilv1++)
	{
		LHENDRAW_inficorn[ilv1].ix=(LHENDRAW_inficorn[ilv1].x-SDL_scrollx)*SDL_zoomx;
		LHENDRAW_inficorn[ilv1].iy=(LHENDRAW_inficorn[ilv1].y-SDL_scrolly)*SDL_zoomy;
		if (iminx>LHENDRAW_inficorn[ilv1].ix)iminx=LHENDRAW_inficorn[ilv1].ix;
		if (iminy>LHENDRAW_inficorn[ilv1].iy)iminy=LHENDRAW_inficorn[ilv1].iy;
		if (imaxx<LHENDRAW_inficorn[ilv1].ix)imaxx=LHENDRAW_inficorn[ilv1].ix;
		if (imaxy<LHENDRAW_inficorn[ilv1].iy)imaxy=LHENDRAW_inficorn[ilv1].iy;
	}
	if (iminx<0)iminx=0;
	if (imaxx>gfx_canvassizex)imaxx=gfx_canvassizex;
	if (iminy<0)iminy=0;
	if (imaxy>gfx_canvassizey)imaxy=gfx_canvassizey;
	if (imaxx<iminx){return 0;}
	if (imaxy<iminy){return 0;}
	for (int ilv1=0;ilv1<count;ilv1++)
	{
		int tlvert;
		tlvert=(LHENDRAW_inficorn[(ilv1+1)%count].iy-LHENDRAW_inficorn[ilv1].iy);
		if (abs(tlvert)>=1)
		gfx_m[ilv1]=
		(float(LHENDRAW_inficorn[(ilv1+1)%count].ix)-LHENDRAW_inficorn[ilv1].ix)/tlvert;
	}
	for (int ilv1=iminy;ilv1<=imaxy;ilv1++)
	{
		ibrakelist_count=0;
		for (int ilv2=0;ilv2<count;ilv2++)
		{
			if (((LHENDRAW_inficorn[ilv2].iy<ilv1) && (LHENDRAW_inficorn[(ilv2+1)%count].iy>=ilv1)) || ((LHENDRAW_inficorn[(ilv2+1)%count].iy<ilv1) && (LHENDRAW_inficorn[ilv2].iy>=ilv1)))
			{
				gfx_bks[ibrakelist_count++]=gfx_m[ilv2]*(ilv1-LHENDRAW_inficorn[ilv2].iy)+LHENDRAW_inficorn[ilv2].ix;
			}
		}
		char tlchanged;
		tlsortback:
		tlchanged=0;
		for (int ilv2=0;ilv2<ibrakelist_count;ilv2++)
		{
			for (int ilv3=ilv2+1;ilv3<ibrakelist_count;ilv3++)
			{
				if (gfx_bks[ilv2]>gfx_bks[ilv3])
				{
					float temp=gfx_bks[ilv2];
					gfx_bks[ilv2]=gfx_bks[ilv3];
					gfx_bks[ilv3]=temp;
					tlchanged=1;
				}
			}
		}
		if (tlchanged) goto tlsortback;
		char tlonness;
		tlonness=0;
		int tlbrakesthrough=0;
		if (ibrakelist_count==0) goto ilinefertig;
		for (int ilv2=iminx;ilv2<=imaxx;ilv2++)
		{
			while (gfx_bks[tlbrakesthrough]<ilv2)
			{
				tlbrakesthrough++;
				if (tlbrakesthrough>=ibrakelist_count)
				{
					goto ilinefertig;
				}
				tlonness=tlbrakesthrough & 1;
			}
			if (tlonness)
			{
				canvas[gfx_screensizex*(ilv1)+ilv2]=SDL_color;
			}
		}
		ilinefertig:
		;
	}
	return 1;
}

void gfx_expressspinellipse(float ix,float iy,float radiusx,float radiusy, float axangle)
{
	ix=(ix-SDL_scrollx)*SDL_zoomx;
	iy=(iy-SDL_scrolly)*SDL_zoomy;
	radiusx=radiusx*SDL_zoomx;
	radiusy=radiusy*SDL_zoomy;
	int isteps=radiusx*Pi*2;
	float tlsaxx=cos(axangle);float tlsaxy=sin(axangle);
	if (SDL_linestyle & 2)
	{
		if (fabs(tlsaxx)<0.00001)
		{
			float swap=radiusx;
			radiusx=radiusy;
			radiusy=swap;
			tlsaxx=1;tlsaxy=0;
		}
		while (axangle>Pi) axangle-=Pi;
		while (axangle<-Pi) axangle+=Pi;
		for (int ilv1=max(iy-max(radiusx,radiusy),0);ilv1<=min(iy+max(radiusx,radiusy),gfx_canvassizey);ilv1++)
		{
			float t=((ilv1-iy)/fabs(tlsaxx)/radiusy);
			float m=-((tlsaxy*radiusx)/tlsaxx)/radiusy;
			float tlangle=(ilv1/float(isteps))*2*Pi;
			float radicand=4*t*t*m*m-4*(1+m*m)*(t*t-1);
			if (radicand<=0) continue;
			radicand=sqrt(radicand);
			int leftsperre=ix+(-2*t*m-radicand)/2/(1+m*m)*(sqrt(1*radiusx*radiusx+m*m*radiusy*radiusy))-(ilv1-iy)*tlsaxy/tlsaxx;
			int rightsperre=ix+(-2*t*m+radicand)/2/(1+m*m)*(sqrt(1*radiusx*radiusx+m*m*radiusy*radiusy))-(ilv1-iy)*tlsaxy/tlsaxx;
			for (int ilv2=max(ix-max(radiusx,radiusy),0);ilv2<=min(ix+max(radiusx,radiusy),gfx_canvassizex);ilv2++)
			{
				if ((ilv2>=leftsperre) && (ilv2<=rightsperre))
				{
					putpixel(ilv2,ilv1);
				}
			}
		}
	}
	else
	{
		if (radiusy>radiusx)
		{
			isteps=radiusy*Pi*2;
		}
		for (int ilv1=0;ilv1<isteps;ilv1++)
		{
			float tlangle=(ilv1/float(isteps))*2*Pi;
			putpixel(ix+tlsaxx*radiusx*cos(tlangle)-tlsaxy*radiusy*sin(tlangle),iy+tlsaxy*radiusx*cos(tlangle)+tlsaxx*radiusy*sin(tlangle));
		}
	}
}
void gfx_expressarc_enhanced(float centerx,float centery,float radiusx,float radiusy,float startangle,float endangle,float tiltangle)
{
	centerx=(centerx-SDL_scrollx)*SDL_zoomx;
	centery=(centery-SDL_scrolly)*SDL_zoomy;
	radiusx=radiusx*SDL_zoomx;
	radiusy=radiusy*SDL_zoomx;
	int isteps=radiusx*Pi*2;
	if (startangle>endangle)
	{
		float swapangle;
		swapangle=startangle;
		startangle=endangle;
		endangle=swapangle;
//		endangle+=2*Pi;
	}
	if (radiusy>radiusx)
	{
		isteps=radiusy*Pi*2;
	}
	float tlsaxx=cos(tiltangle);float tlsaxy=sin(tiltangle);
	for (int ilv1=(isteps*startangle)/(Pi*2);ilv1<(isteps*endangle)/(Pi*2);ilv1++)
	{
		float tlangle=(ilv1/float(isteps))*2*Pi;
		putpixel(centerx+tlsaxx*radiusx*cos(tlangle)-tlsaxy*radiusy*sin(tlangle),centery+tlsaxy*radiusx*cos(tlangle)+tlsaxx*radiusy*sin(tlangle));
	}
}
inline void gfx_expressarc(float centerx,float centery,float radiusx,float radiusy,float startangle,float endangle)
{
	gfx_expressarc_enhanced(centerx,centery,radiusx,radiusy,startangle,endangle,0);
}
void gfx_expressbow(float ileft,float itop,float iright,float ibottom,float irelradius)
{
	float dx=iright-ileft;
	float dy=ibottom-itop;
	float angle=getangle(dx,dy);
	angle=fmod(angle+(Pi/2),2*Pi);
	float tangle=asin(0.5/irelradius);
	float tangent=0.5/tan(tangle);
	float tldist=sqrt(fsqr(iright-ileft)+fsqr(ibottom-itop));
	gfx_expressarc((iright+ileft)/2+tangent*cos(angle)*tldist,(ibottom+itop)/2+tangent*sin(angle)*tldist,irelradius*tldist,irelradius*tldist,fmod(angle-tangle+Pi,Pi*2),fmod(angle+tangle+Pi,Pi*2));
}
void gfx_expressellipse(float centerx,float centery,float radiusx,float radiusy)
{
	centerx=(centerx-SDL_scrollx)*SDL_zoomx;
	centery=(centery-SDL_scrolly)*SDL_zoomy;
	radiusx*=SDL_zoomx;
	radiusy*=SDL_zoomy;
	if (SDL_linestyle & 2)
	{
		for (int ilv1=max(0,centery-radiusy);ilv1<=min(centery+radiusy,gfx_canvassizey);ilv1++)
		{
			float radicand=1-(ilv1-centery)*(ilv1-centery)/radiusy/radiusy;
			if (radicand<=0) continue;
			radicand=sqrt(radicand)*radiusx;
			int rightborder=min(gfx_canvassizex,centerx+radicand);
			for (int ilv2=max(0,centerx-radicand);ilv2<rightborder;ilv2++)
			{
				putpixel(ilv2,ilv1);
			}
		}
	}
	else
	{
		int isteps=radiusx*Pi*2;
		if (radiusy>radiusx)
		{
			isteps=radiusy*Pi*2;
		}
		for (int ilv1=0;ilv1<isteps;ilv1++)
		{
			float tlangle=(ilv1/float(isteps))*2*Pi;
			putpixel(centerx+radiusx*cos(tlangle),centery+radiusy*sin(tlangle));
		}
	}
}

int gfx_text_rewind(unsigned char * windtext,int length,int imode)
{
	int ilv4;
	int backcount;
	int larger=((imode & 1)==1)*1+((imode & 8)==8)*2;
	SDL_txcursorx=SDL_old_txcursorx;
	SDL_txcursory=SDL_old_txcursory;
	for (ilv4=0;ilv4<length;ilv4+=backcount)
	{
		fontpixinf_ * ifontpixinf=&fontpixinf[indexfromunicode(utf8resolve((unsigned char*)windtext + ilv4,&backcount))];
		SDL_txcursorx-=(*ifontpixinf).deltax+larger;
		SDL_txcursory-=(*ifontpixinf).deltay;
		SDL_old_txcursorx=SDL_txcursorx;
		SDL_old_txcursory=SDL_txcursory;
	}
	return 1;
}
inline char text_print_bitmap_enhanced_getpos(int posx,int posy,char * memory,int skip,int maxx,int maxy)
{
	if ((posx>=0) && (posy>=0) && (posx<maxx) && (posy<maxy))
	{
		return ((memory[posy*skip+posx] & 0x80)!=0);
	}
	return 0;
}
void text_print_bitmap_enhanced(int * posx,int * posy,fontpixinf_ * ifontpixinf,int imode)
{
	int ilv1,ilv2;
	int scanx,scany;
	int maxx,maxy;
	int icanvasskip=gfx_screensizex;
	int slope=0;
	unsigned int * icanvas=canvas;
	int skip=(*ifontpixinf).sizex;
	char * mempos=(*ifontpixinf).memstart;
	scanx=(*posx)+(*ifontpixinf).pivotx;
	scany=(*posy)+(*ifontpixinf).pivoty;
	maxx=(*ifontpixinf).sizex;
	maxy=(*ifontpixinf).sizey;
	int larger=((imode & 1)==1)*1+((imode & 8)==8)*2;
	if (gfx_txselectmode & 2)
	{
		gfx_expressfillrectangle(*posx,(*posy)-12,(*posx)+(*ifontpixinf).deltax+larger,(*posy)+4,(SDL_color+0xC0C0) & 0xFFFFFF);
	}
	if (imode & 4)
	{
		gfx_expressline(*posx,(*posy)+4,(*posx)+(*ifontpixinf).deltax+larger,(*posy)+4);
	}
	(*posx)+=(*ifontpixinf).deltax+larger;
	(*posy)+=(*ifontpixinf).deltay;
	if ((imode & 0x8)==0)
	{
		for (ilv1=0;ilv1<maxy;ilv1++)
		{
			if (imode & 2) slope=1-(ilv1>>2);
			for (ilv2=0;ilv2<maxx+1;ilv2++)
			{
				if (text_print_bitmap_enhanced_getpos(ilv2,ilv1,mempos,skip,maxx,maxy) || (text_print_bitmap_enhanced_getpos(ilv2-1,ilv1,mempos,skip,maxx,maxy) && (imode & 1)))
				{
					putpixel(ilv2+slope+scanx,ilv1+scany);
				}
			}
		}
	}
	else //outline mode
	{
		slope=1;
		for (ilv1=-1;ilv1<maxy+1;ilv1++)
		{
			if (imode & 2) slope=2-(ilv1>>2);
			for (ilv2=-1;ilv2<maxx+2;ilv2++)
			{
				if (text_print_bitmap_enhanced_getpos(ilv2,ilv1,mempos,skip,maxx,maxy) || (text_print_bitmap_enhanced_getpos(ilv2-1,ilv1,mempos,skip,maxx,maxy) && (imode & 1)))
				{
					//do not paint anything when on letter but do not ask about pixels around it (for speed)
				}
				else
				{
					if (imode & 1)
					{
						if (text_print_bitmap_enhanced_getpos(ilv2-2,ilv1,mempos,skip,maxx,maxy)) putpixel(ilv2+slope+scanx,ilv1+scany);
						if (text_print_bitmap_enhanced_getpos(ilv2+1,ilv1,mempos,skip,maxx,maxy)) putpixel(ilv2+slope+scanx,ilv1+scany);
						if (text_print_bitmap_enhanced_getpos(ilv2-1,ilv1-1,mempos,skip,maxx,maxy)) putpixel(ilv2+slope+scanx,ilv1+scany);
						if (text_print_bitmap_enhanced_getpos(ilv2-1,ilv1+1,mempos,skip,maxx,maxy)) putpixel(ilv2+slope+scanx,ilv1+scany);
						if (text_print_bitmap_enhanced_getpos(ilv2,ilv1-1,mempos,skip,maxx,maxy)) putpixel(ilv2+slope+scanx,ilv1+scany);
						if (text_print_bitmap_enhanced_getpos(ilv2,ilv1+1,mempos,skip,maxx,maxy)) putpixel(ilv2+slope+scanx,ilv1+scany);
					}
					else
					{
						if (text_print_bitmap_enhanced_getpos(ilv2-1,ilv1,mempos,skip,maxx,maxy)) putpixel(ilv2+slope+scanx,ilv1+scany);
						if (text_print_bitmap_enhanced_getpos(ilv2+1,ilv1,mempos,skip,maxx,maxy)) putpixel(ilv2+slope+scanx,ilv1+scany);
						if (text_print_bitmap_enhanced_getpos(ilv2,ilv1-1,mempos,skip,maxx,maxy)) putpixel(ilv2+slope+scanx,ilv1+scany);
						if (text_print_bitmap_enhanced_getpos(ilv2,ilv1+1,mempos,skip,maxx,maxy)) putpixel(ilv2+slope+scanx,ilv1+scany);
					}
				}
			}
		}
	}
}
void text_print_bitmap(int * posx,int * posy,fontpixinf_ * ifontpixinf,int imode)
{
	if (imode & 0x1A) return text_print_bitmap_enhanced(posx,posy,ifontpixinf,imode);
	int ilv1,ilv2;
	int scanx,scany;
	int maxx,maxy;
	int icanvasskip=gfx_screensizex;
	unsigned int * icanvas=canvas;
	int skip=(*ifontpixinf).sizex;
	char * mempos=(*ifontpixinf).memstart;
	scanx=(*posx)+(*ifontpixinf).pivotx;
	scany=(*posy)+(*ifontpixinf).pivoty;
	maxx=(*ifontpixinf).sizex+scanx;
	maxy=(*ifontpixinf).sizey+scany;
	int larger=((imode & 1)==1)*1;
	if (scanx<0) {mempos-=scanx;scanx=0;}
	if (scany<0) {mempos-=scany*skip;scany=0;}
	if (maxx>=gfx_canvassizex) maxx=gfx_canvassizex-1-larger;
	if (maxy>=gfx_canvassizey) maxy=gfx_canvassizey-1;
	skip-=maxx-scanx;
	if (gfx_txselectmode & 2)
	{
		gfx_expressfillrectangle(*posx,(*posy)-12,(*posx)+(*ifontpixinf).deltax+larger,(*posy)+4,(SDL_color+0xC0C0) & 0xFFFFFF);
	}
	if (imode & 4)
	{
		gfx_expressline(*posx,(*posy)+4,(*posx)+(*ifontpixinf).deltax+larger,(*posy)+4);
	}
	(*posx)+=(*ifontpixinf).deltax+larger;
	(*posy)+=(*ifontpixinf).deltay;
	if ((scanx>=maxx) || (scany>=maxy))
	{
		return;
	}
	icanvas+=icanvasskip*scany+scanx;
	icanvasskip-=maxx-scanx;
	for (ilv1=scany;ilv1<maxy;ilv1++)
	{
		for (ilv2=scanx;ilv2<maxx;ilv2++)
		{
			if (*(mempos) & 0x80)
			{
				*(icanvas)=SDL_color;
				if (imode & 1) *(icanvas+1)=SDL_color;
			}
			icanvas++;
			mempos++;
		}
		mempos+=skip;
		icanvas+=icanvasskip;
	}
}
void gfx_printformatted(const char * iinput,const char * parms,int imode,int start,int end)
{
	int ilv4=start;
	char linebreak;
	int i_offsy=0;
	thatwasatemporaryskip:
	linebreak=0;
	_i32 backcount=0;
	for (;ilv4<end;ilv4+=backcount)
	{
		if ((iinput[ilv4]=='\xee') && (iinput[ilv4+1]=='\x80') && (((_u8)(iinput[ilv4+2] & 0xFE))==(_u8)'\x80'))
		{
			if (gfx_txselectmode & 1)
			{
				gfx_txselectmode^=2;
			}
			ilv4+=3;
			for (int ilv5=0;ilv5<16;ilv5++)
			{
				putpixel(SDL_txcursorx,SDL_txcursory-12+ilv5);
			}
			goto thatwasatemporaryskip;
		}
		if (iinput[ilv4]==10)
		{
			ilv4++;
			linebreak=1;
			goto skipfornow;
		}
		if (imode & 0x20) {i_offsy=4;}
		if (imode & 0x40) {i_offsy=-4;}
		SDL_txcursory+=i_offsy;
		text_print_bitmap(&SDL_txcursorx,&SDL_txcursory,&fontpixinf[indexfromunicode(utf8resolve((unsigned char*)iinput + ilv4,&backcount))],imode);
		SDL_txcursory-=i_offsy;
	}
	skipfornow:
	if (linebreak) {SDL_txcursory+=16;SDL_txcursorx=SDL_old_txcursorx;goto thatwasatemporaryskip;}//a line break;
}

void sdl_outit()
{
}
void svg_findaround();
void getatoms();
void gfx_controlprocedure(bool irestriction,char hatches);
void gfx_output(int mode=0)
{
	framenumber++;
	#ifndef NODEBUG
	clock_gettime(clockid,&ts);
	counter1-=ts.tv_nsec+1000000000*ts.tv_sec;
	clock_gettime(clockid,&ts);
	counter1+=ts.tv_nsec+1000000000*ts.tv_sec;
	clock_gettime(clockid,&ts);
	counter2-=ts.tv_nsec+1000000000*ts.tv_sec;
	#endif
	svg_findaround();
	#ifndef NODEBUG
	clock_gettime(clockid,&ts);
	counter2+=ts.tv_nsec+1000000000*ts.tv_sec;
	clock_gettime(clockid,&ts);
	counter3-=ts.tv_nsec+1000000000*ts.tv_sec;
	#endif
	getatoms();
	#ifndef NODEBUG
	clock_gettime(clockid,&ts);
	counter3+=ts.tv_nsec+1000000000*ts.tv_sec;
	clock_gettime(clockid,&ts);
	counter4-=ts.tv_nsec+1000000000*ts.tv_sec;
	#endif
	initZlist();
	#ifndef NODEBUG
	clock_gettime(clockid,&ts);
	counter4+=ts.tv_nsec+1000000000*ts.tv_sec;
	clock_gettime(clockid,&ts);
	counter5-=ts.tv_nsec+1000000000*ts.tv_sec;
	#endif
	if (mode)
	{
		gfx_controlprocedure(0,2);
	}
	else
	{
		gfx_controlprocedure(0,1);
		gfx_controlprocedure(0,0);
	}
	#ifndef NODEBUG
	clock_gettime(clockid,&ts);
	counter5+=ts.tv_nsec+1000000000*ts.tv_sec;
	clock_gettime(clockid,&ts);
	counter1-=ts.tv_nsec+1000000000*ts.tv_sec;
	clock_gettime(clockid,&ts);
	counter1+=ts.tv_nsec+1000000000*ts.tv_sec;
	#endif
}
