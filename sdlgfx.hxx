//Graphic routines. Callers of those should be in createsvg.hxx
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
float SDL_zoomx=1,SDL_zoomy=1;
//_u8 screen[gfx_screensizex*gfx_screensizey*gfx_depth];
_u32 * screen;
_u32 * canvas;
_u32 SDL_color;
int SDL_linestyle;

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
	printf("cn %i,%i\n",(*glob_color_multilist).filllevel,number);
	if (number-2>=(*glob_color_multilist).filllevel)
	{	
		SDL_color=0;
		return -1;
	}
	SDL_color=((_u8)(((*glob_color_multilist).bufferlist)[number-2].r*255)<<16)+
	((_u8)(((*glob_color_multilist).bufferlist)[number-2].g*255)<<8)+
	((_u8)(((*glob_color_multilist).bufferlist)[number-2].b*255));
	printf("co %llX",SDL_color);
	return 0;
}

inline void stylegenestring(int flags) //1: stroke 2: fill 4: bold 8: dashed
{
	SDL_linestyle=flags;
}

void expressline(float ileft,float itop,float iright,float ibottom)
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
	int ibrakelist_bks[256];
	int ibrakelist_pps[256];//1: end 2: start 0: unknown 0x4: other than 8 0x8: other than 0x4 0x10: deaf(horz)
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
	iminx=10000;
	iminy=10000;
	imaxx=-10000;
	imaxy=-10000;
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
	printf("SDL:%llX,%llX,%llX,%llX\n",iminx,iminy,imaxx,imaxy);
	if (imaxx<iminx){return 0;}
	if (imaxy<iminy){return 0;}
	{
		int tlvert;
		tlvert=(iy2-iy1);
		if (abs(tlvert)>=1)
		m1=(float(ix2)-ix1)/tlvert;
	}
	{
		int tlvert;
		tlvert=(iy3-iy2);
		if (abs(tlvert)>=1)
		m2=(float(ix3)-ix2)/tlvert;
	}
	{
		int tlvert;
		tlvert=(iy1-iy3);
		if (abs(tlvert)>=1)
		m3=(float(ix1)-ix3)/tlvert;
	}
	for (int ilv1=iminy;ilv1<=imaxy;ilv1++)
	{
		ibrakelist_count=0;
		if (((iy1<ilv1) && (iy2>=ilv1)) || ((iy2<ilv1) && (iy1>=ilv1)))
		{
			ibrakelist_pps[ibrakelist_count]=(iy2>iy1)?4:8;
			ibrakelist_bks[ibrakelist_count++]=m1*(ilv1-iy1)+ix1;
		}
		if (((iy2<ilv1) && (iy3>=ilv1)) || ((iy3<ilv1) && (iy2>=ilv1)))
		{
			ibrakelist_pps[ibrakelist_count]=(iy3>iy2)?4:8;
			ibrakelist_bks[ibrakelist_count++]=m2*(ilv1-iy2)+ix2;
		}
		if (((iy3<ilv1) && (iy1>=ilv1)) || ((iy1<ilv1) && (iy3>=ilv1)))
		{
			ibrakelist_pps[ibrakelist_count]=(iy1>iy3)?4:8;
			ibrakelist_bks[ibrakelist_count++]=m3*(ilv1-iy3)+ix3;
		}
		char tlchanged;
		tlsortback:
		tlchanged=0;
		for (int ilv2=0;ilv2<ibrakelist_count;ilv2++)
		{
			for (int ilv3=ilv2+1;ilv3<ibrakelist_count;ilv3++)
			{
				if (ibrakelist_bks[ilv2]>ibrakelist_bks[ilv3])
				{
					float temp=ibrakelist_bks[ilv2];
					ibrakelist_bks[ilv2]=ibrakelist_bks[ilv3];
					ibrakelist_bks[ilv3]=temp;
					tlchanged=1;
				}
			}
		}
		if (tlchanged) goto tlsortback;
		char sideness=(ibrakelist_bks[0]==4)?1:2;
		char tlonness;
		tlonness=0;
		int tlbrakesthrough=0;
		for (int ilv2=iminx;ilv2<=imaxx;ilv2++)
		{
			while (ibrakelist_bks[tlbrakesthrough]<ilv2)
			{
				tlbrakesthrough++;
				if (tlbrakesthrough==ibrakelist_count)
				{
					goto ilinefertig;
				}
				tlonness=((ibrakelist_pps[tlbrakesthrough-1]&4) && (sideness&1)) || ((ibrakelist_pps[tlbrakesthrough-1]&8) && (sideness&2));
			}
			if (tlonness)
			{
				canvas[gfx_screensizex*(ilv1)+ilv2]=SDL_color;
			}
			ilinefertig:
			;
		}
	}
}

int expresstetrangle(intl ifx1,intl ify1,intl ifx2,intl ify2,intl ifx3,intl ify3,intl ifx4,intl ify4)
{
	int ibrakelist_bks[256];
	int ibrakelist_pps[256];//1: end 2: start 0: unknown 0x4: other than 8 0x8: other than 0x4 0x10: deaf(horz)
	intl ibrakelist_count;
	intl ix1;
	intl iy1;
	intl ix2;
	intl iy2;
	intl ix3;
	intl iy3;
	intl ix4;
	intl iy4;
	ix1=(ifx1-SDL_scrollx)*SDL_zoomx;
	iy1=(ify1-SDL_scrolly)*SDL_zoomy;
	ix2=(ifx2-SDL_scrollx)*SDL_zoomx;
	iy2=(ify2-SDL_scrolly)*SDL_zoomy;
	ix3=(ifx3-SDL_scrollx)*SDL_zoomx;
	iy3=(ify3-SDL_scrolly)*SDL_zoomy;
	ix4=(ifx4-SDL_scrollx)*SDL_zoomx;
	iy4=(ify4-SDL_scrolly)*SDL_zoomy;
	intl iminx,imaxx,iminy,imaxy;
	iminx=10000;
	iminy=10000;
	imaxx=-10000;
	imaxy=-10000;
	float m1,m2,m3,m4;
	if (iminx>ix1)iminx=ix1;
	if (iminx>ix2)iminx=ix2;
	if (iminx>ix3)iminx=ix3;
	if (iminx>ix4)iminx=ix4;
	if (iminy>iy1)iminy=iy1;
	if (iminy>iy2)iminy=iy2;
	if (iminy>iy3)iminy=iy3;
	if (iminy>iy4)iminy=iy4;
	if (imaxx<ix1)imaxx=ix1;
	if (imaxx<ix2)imaxx=ix2;
	if (imaxx<ix3)imaxx=ix3;
	if (imaxx<ix4)imaxx=ix4;
	if (imaxy<iy1)imaxy=iy1;
	if (imaxy<iy2)imaxy=iy2;
	if (imaxy<iy3)imaxy=iy3;
	if (imaxy<iy4)imaxy=iy4;
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
		m1=(float(ix2)-ix1)/tlvert;
	}
	{
		int tlvert;
		tlvert=(iy3-iy2);
		if (abs(tlvert)>=1)
		m2=(float(ix3)-ix2)/tlvert;
	}
	{
		int tlvert;
		tlvert=(iy4-iy3);
		if (abs(tlvert)>=1)
		m3=(float(ix4)-ix3)/tlvert;
	}
	{
		int tlvert;
		tlvert=(iy1-iy4);
		if (abs(tlvert)>=1)
		m4=(float(ix1)-ix4)/tlvert;
	}
	for (int ilv1=iminy;ilv1<=imaxy;ilv1++)
	{
		ibrakelist_count=0;
		if (((iy1<ilv1) && (iy2>=ilv1)) || ((iy2<ilv1) && (iy1>=ilv1)))
		{
			ibrakelist_pps[ibrakelist_count]=(iy2>iy1)?4:8;
			ibrakelist_bks[ibrakelist_count++]=m1*(ilv1-iy1)+ix1;
		}
		if (((iy2<ilv1) && (iy3>=ilv1)) || ((iy3<ilv1) && (iy2>=ilv1)))
		{
			ibrakelist_pps[ibrakelist_count]=(iy3>iy2)?4:8;
			ibrakelist_bks[ibrakelist_count++]=m2*(ilv1-iy2)+ix2;
		}
		if (((iy3<ilv1) && (iy4>=ilv1)) || ((iy4<ilv1) && (iy3>=ilv1)))
		{
			ibrakelist_pps[ibrakelist_count]=(iy4>iy3)?4:8;
			ibrakelist_bks[ibrakelist_count++]=m3*(ilv1-iy3)+ix3;
		}
		if (((iy4<ilv1) && (iy1>=ilv1)) || ((iy1<ilv1) && (iy4>=ilv1)))
		{
			ibrakelist_pps[ibrakelist_count]=(iy1>iy4)?4:8;
			ibrakelist_bks[ibrakelist_count++]=m4*(ilv1-iy4)+ix4;
		}
		char tlchanged;
		tlsortback:
		tlchanged=0;
		for (int ilv2=0;ilv2<ibrakelist_count;ilv2++)
		{
			for (int ilv3=ilv2+1;ilv3<ibrakelist_count;ilv3++)
			{
				if (ibrakelist_bks[ilv2]>ibrakelist_bks[ilv3])
				{
					float temp=ibrakelist_bks[ilv2];
					ibrakelist_bks[ilv2]=ibrakelist_bks[ilv3];
					ibrakelist_bks[ilv3]=temp;
					tlchanged=1;
				}
			}
		}
		if (tlchanged) goto tlsortback;
		char sideness=(ibrakelist_pps[0]==4)?1:2;
		char tlonness;
		tlonness=0;
		int tlbrakesthrough=0;
		for (int ilv2=iminx;ilv2<=imaxx;ilv2++)
		{
			while (ibrakelist_bks[tlbrakesthrough]<ilv2)
			{
				tlbrakesthrough++;
				if (tlbrakesthrough==ibrakelist_count)
				{
					goto ilinefertig;
				}
				tlonness=((ibrakelist_pps[tlbrakesthrough-1]&4) && (sideness&1)) || ((ibrakelist_pps[tlbrakesthrough-1]&8) && (sideness&2));
			}
			if (tlonness)
			{
				canvas[gfx_screensizex*(ilv1)+ilv2]=SDL_color;
			}
		}
		ilinefertig:
		;
	}
}

void svg_main(const char * filename);
void sdl_output(const char * filename)
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
	SDL_color=0xFF00FF;
	expresstriangle(10,10,10,80,80,10);
	svg_main(filename);
	gfxstop();
	sleep(10);
}
