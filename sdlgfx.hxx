#define gfx_screensizex 640
#define gfx_screensizey 480
#define gfx_canvassizex 320
#define gfx_canvassizey 240
#define gfx_canvasminx 160
#define gfx_canvasminy 120
#define gfx_canvasmaxx 480
#define gfx_canvasmaxy 360
#define gfx_depth 4
float SDL_scrollx=0,SDL_scrolly=0;
float SDL_zoomx,SDL_zoomy;
//_u8 screen[gfx_screensizex*gfx_screensizey*gfx_depth];
_u32 * screen;
_u32 * canvas;
_u32 SDL_color;

int get_colorstringv(int number)
{
	if (number==0)
	{
		SDL_color=0;
		return 0;
	}
	if (number==1)
	{
		SDL_color=0xFFFFFF;
		return 0;
	}
	if (number-2>=(*glob_color_multilist).filllevel)
	{	
		SDL_color=0;
		return -1;
	}
	SDL_color=((_u8)(((*glob_color_multilist).bufferlist)[number-2].r)<<16)+
	((_u8)(((*glob_color_multilist).bufferlist)[number-2].g)<<8)+
	((_u8)(((*glob_color_multilist).bufferlist)[number-2].b));
	return 0;
}

void expressline(float ileft,float itop,float iright,float ibottom,_u32 icolor)
{
	int x=ileft;
	int y=itop;
	int x2=iright;
	int y2=ibottom;
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
		if (x2==x)//it can be that still x width is zero.
		{
			canvas[gfx_screensizex*y2+x]=icolor;
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
			canvas[gfx_screensizex*vertical+ilv1+x]=icolor;
		}
	}
	else
	{
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
			canvas[gfx_screensizex*(ilv1+y)+horizontal]=icolor;
		}
	}
	return;
}
 SDL_Surface *video;

void screenclear(_u32 icolor)
{
	for (int ilv1=0;ilv1<640*480;ilv1++)
	{
		screen[ilv1]=icolor;
	}
}
int gfx()
{
	video = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
	if ( video == NULL ) 
	{
		fprintf(stderr, "Ich konnte kein Fenster mit der Auflösung 640*480 öffnen: %s\n", SDL_GetError());
		exit(1);
	}
}
int gfxstart()
{
	if ( SDL_MUSTLOCK(video) ) 
	{
		if ( SDL_LockSurface(video) < 0 ) 
		{
			return(1);
		}
	}
	screen=(unsigned int*)(*video).pixels;//You know, that is a pointer to a n byte structure, n=colordepth
	canvas=screen+gfx_canvasminx+gfx_screensizex*gfx_canvasminy;
}
int gfxstop()
{
	if ( SDL_MUSTLOCK(video) ) 
	{
		SDL_UnlockSurface(video);
	}
	SDL_UpdateRect(video,0,0,640,480);
}

int expresstriangle(intl ifx1,intl ify1,intl ifx2,intl ify2,intl ifx3,intl ify3)
{
	float ibrakelist[10];
	intl ibrakelist_count;
	intl ix1;
	intl iy1;
	intl ix2;
	intl iy2;
	intl ix3;
	intl iy3;
	ix1=(ifx1-SDL_scrollx)*SDL_zoomx;
	iy1=(ify1-SDL_scrolly)*SDL_zoomy;
	ix2=(ifx2-SDL_scrollx)*SDL_zoomx;
	iy2=(ify2-SDL_scrolly)*SDL_zoomy;
	ix3=(ifx3-SDL_scrollx)*SDL_zoomx;
	iy3=(ify3-SDL_scrolly)*SDL_zoomy;
	intl iminx,imaxx,iminy,imaxy;
	float m1,m2,m3;
	if (iminx>ix1)iminx=ix1;
	if (iminx>ix2)iminx=ix2;
	if (iminx>ix3)iminx=ix3;
	if (iminy>iy1)iminy=iy1;
	if (iminy>iy2)iminy=iy2;
	if (iminy>iy3)iminy=iy3;
	if (imaxx<ix1)imaxx=ix1;
	if (imaxx<ix2)imaxx=ix2;
	if (imaxx<ix3)imaxx=ix3;
	if (imaxy<iy1)imaxy=iy1;
	if (imaxy<iy2)imaxy=iy2;
	if (imaxy<iy3)imaxy=iy3;
	if (iminx<0)iminx=0;
	if (imaxx>gfx_canvassizex)imaxx=gfx_canvassizex;
	if (iminy<0)iminy=0;
	if (imaxy>gfx_canvassizey)imaxy=gfx_canvassizey;
	if (imaxx<iminx){return 0;}
	if (imaxy<iminy){return 0;}
	{
		int tlvert;
		tlvert=(iy2-iy1);
		if (abs(tlvert)>=1)
		m1=(ix2-ix1)/tlvert;
	}
	for (int ilv1=iminy;ilv1<=imaxy;ilv1++)
	{
		ibrakelist_count=0;
		if (((iy1<ilv1) && (iy2>=ilv1)) || ((iy2<ilv1) && (iy1>=ilv1)))
		{
			ibrakelist[ibrakelist_count++]=m1*(ilv1-iy1)+ix1;
		}
		for (int ilv1=iminx;ilv1<=imaxx;ilv1++)
		{
			
		}
	}
}
void sdl_output()
{
	if ( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0 ) {
		fprintf(stderr, "SDL konnte nicht initialisiert werden: %s\n", SDL_GetError());
		exit(1);
	}
	srand(4);
	gfx();
	gfxstart();
	float x,y,spin;
	float impx,impy;
	impx=0;
	impy=0;
	x=0;
	y=0;
	back:
	spin+=1;
	impx+=((rand()%1000)-500)/1000.0;
	impx*=1.001;
	impy+=((rand()%1000)-500)/1000.0;
	impy*=1.001;
	screenclear(0xFFFFFF);
	x+=impx;
	y+=impy;
	if (x>320){x=320;impx=-fabs(impx)*0.5;}
	if (x<0){x=0;impx=fabs(impx)*0.5;}
	if (y>320){y=320;impy=-fabs(impy)*0.5;}
	if (y<0){y=0;impy=fabs(impy)*0.5;}
	gfxstart();
	expressline(x,y,x+100*cos(spin/36),y+100*sin(spin/36),0xFF00FF);
	expressline(x,y,x+100*cos(Pi/2+spin/36),y+100*sin(Pi/2+spin/36),0xFF00FF);
	expressline(x,y,x+100*cos(Pi+spin/36),y+100*sin(Pi+spin/36),0xFF00FF);
	expressline(x,y,x+100*cos(-Pi/2+spin/36),y+100*sin(-Pi/2+spin/36),0xFF00FF);
	gfxstop();
	goto back;
}
