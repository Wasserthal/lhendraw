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
int SDL_txcursorx=0;int SDL_txcursory=0;
//_u8 screen[gfx_screensizex*gfx_screensizey*gfx_depth];
_u32 * screen;
_u32 * canvas;
_u32 SDL_color;
int SDL_linestyle;

int get_colorstringv(int number)
{
	SDL_color=number;
	return 0;
}
void express_txinit(char ialignment,float iposx,float iposy,float iatomfontheight)
{
	SDL_txcursorx=(iposx-SDL_scrollx)*SDL_zoomx;SDL_txcursory=(iposy-SDL_scrolly)*SDL_zoomy;
}
inline void express_text_tail()
{
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
inline void stylegenestring(int flags,unsigned int fillcolor=0) //1: stroke 2: fill 4: bold 8: dashed
{
	SDL_linestyle=flags;
}

void expressbeziertrack(cdx_Bezierpoints * ipoints)
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
void expressxbezier(int icount,...)
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
	expressbeziertrack(&tlpoints);
}

void expressbezier(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4)
{
	float ishare;
	float iminusshare;
	float stepsbecausex=2*(abs(x4-x1)+abs(x3-x2));
	float stepsbecausey=2*(abs(y4-x1)+abs(y3-y2));
	float ix,iy;
	x1=((x1-SDL_scrollx)*SDL_zoomx);
	x2=((x2-SDL_scrollx)*SDL_zoomx);
	x3=((x3-SDL_scrollx)*SDL_zoomx);
	x4=((x4-SDL_scrollx)*SDL_zoomx);
	y1=((y1-SDL_scrolly)*SDL_zoomy);
	y2=((y2-SDL_scrolly)*SDL_zoomy);
	y3=((y3-SDL_scrolly)*SDL_zoomy);
	y4=((y4-SDL_scrolly)*SDL_zoomy);
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

typedef struct inficorn
{
	float x,y;
	float m;
	int ix,iy;
	float type;//1: line 3: cubic bezier 0x100: bezier part, order depending on the last preceding
	int bks;//not necessarily in the same order as the other part of the structures!
	int pps;//not necessarily in the same order as the other part of the structures!
};

int expressinfinityangle(inficorn * iworkarray,int count)
{
	intl iminx,imaxx,iminy,imaxy;
	intl ibrakelist_count;
	iminx=10000;
	iminy=10000;
	imaxx=-10000;
	imaxy=-10000;
	for (int ilv1=0;ilv1<count;ilv1++)
	{
		iworkarray[ilv1].ix=(iworkarray[ilv1].x-SDL_scrollx)*SDL_zoomx;
		iworkarray[ilv1].iy=(iworkarray[ilv1].y-SDL_scrolly)*SDL_zoomy;
		if (iminx>iworkarray[ilv1].ix)iminx=iworkarray[ilv1].ix;
		if (iminy>iworkarray[ilv1].iy)iminy=iworkarray[ilv1].iy;
		if (imaxx<iworkarray[ilv1].ix)imaxx=iworkarray[ilv1].ix;
		if (imaxy<iworkarray[ilv1].iy)imaxy=iworkarray[ilv1].iy;
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
		tlvert=(iworkarray[(ilv1+1)%count].iy-iworkarray[ilv1].iy);
		if (abs(tlvert)>=1)
		iworkarray[ilv1].m=(float(iworkarray[(ilv1+1)%count].ix)-iworkarray[ilv1].ix)/tlvert;
	}
	for (int ilv1=iminy;ilv1<=imaxy;ilv1++)
	{
		ibrakelist_count=0;
		for (int ilv2=0;ilv2<count;ilv2++)
		{
			if (((iworkarray[ilv2].iy<ilv1) && (iworkarray[(ilv2+1)%count].iy>=ilv1)) || ((iworkarray[(ilv2+1)%count].iy<ilv1) && (iworkarray[ilv2].iy>=ilv1)))
			{
				iworkarray[ibrakelist_count].pps=(iworkarray[(ilv2+1)%count].iy>iworkarray[ilv2].iy)?4:8;
				iworkarray[ibrakelist_count++].bks=iworkarray[ilv2].m*(ilv1-iworkarray[ilv2].iy)+iworkarray[ilv2].ix;
			}
		}
		char tlchanged;
		tlsortback:
		tlchanged=0;
		for (int ilv2=0;ilv2<ibrakelist_count;ilv2++)
		{
			for (int ilv3=ilv2+1;ilv3<ibrakelist_count;ilv3++)
			{
				if (iworkarray[ilv2].bks>iworkarray[ilv3].bks)
				{
					float temp=iworkarray[ilv2].bks;
					iworkarray[ilv2].bks=iworkarray[ilv3].bks;
					iworkarray[ilv3].bks=temp;
					tlchanged=1;
				}
			}
		}
		if (tlchanged) goto tlsortback;
		char sideness=(iworkarray[0].pps==4)?1:2;
		char tlonness;
		tlonness=0;
		int tlbrakesthrough=0;
		for (int ilv2=iminx;ilv2<=imaxx;ilv2++)
		{
			while (iworkarray[tlbrakesthrough].bks<ilv2)
			{
				tlbrakesthrough++;
				if (tlbrakesthrough==ibrakelist_count)
				{
					goto ilinefertig;
				}
				tlonness=((iworkarray[tlbrakesthrough-1].pps&4) && (sideness&1)) || ((iworkarray[tlbrakesthrough-1].pps&8) && (sideness&2));
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

void expressspinellipse(float ix,float iy,float radiusx,float radiusy, float axangle)
{
	ix=(ix-SDL_scrollx)*SDL_zoomx;
	iy=(iy-SDL_scrolly)*SDL_zoomy;
	int isteps=radiusx*Pi*2;
	if (radiusy>radiusx)
	{
		isteps=radiusy*Pi*2;
	}
	float tlsaxx=cos(axangle);float tlsaxy=sin(axangle);
	for (int ilv1=0;ilv1<isteps;ilv1++)
	{
		float tlangle=(ilv1/float(isteps))*2*Pi;
		putpixel(ix+tlsaxx*radiusx*cos(tlangle)-tlsaxy*radiusy*sin(tlangle),iy+tlsaxy*radiusx*cos(tlangle)+tlsaxx*radiusy*sin(tlangle));
	}
}
void expressarc_enhanced(float centerx,float centery,float radiusx,float radiusy,float startangle,float endangle,float tiltangle)
{
	centerx=(centerx-SDL_scrollx)*SDL_zoomx;
	centery=(centery-SDL_scrolly)*SDL_zoomy;
	int isteps=radiusx*Pi*2;
	if (startangle>endangle)
	{
/*		float swapangle;
		swapangle=startangle;
		startangle=endangle;
		endangle=swapangle;*/
		endangle+=2*Pi;
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
inline void expressarc(float centerx,float centery,float radiusx,float radiusy,float startangle,float endangle)
{
	expressarc_enhanced(centerx,centery,radiusx,radiusy,startangle,endangle,0);
}
void expressbow(float ileft,float itop,float iright,float ibottom,float irelradius)
{
	float dx=iright-ileft;
	float dy=ibottom-itop;
	float angle=getangle(dx,dy);
	angle=fmod(angle+(Pi/2),2*Pi);
	float tangle=asin(0.5/irelradius);
	float tangent=0.5/tan(tangle);
	float tldist=sqrt(fsqr(iright-ileft)+fsqr(ibottom-itop));
	expressarc((iright+ileft)/2+tangent*cos(angle)*tldist,(ibottom+itop)/2+tangent*sin(angle)*tldist,irelradius*tldist,irelradius*tldist,fmod(angle-tangle+Pi,Pi*2),fmod(angle+tangle+Pi,Pi*2));
}
void expressellipse(float centerx,float centery,float radiusx,float radiusy)
{
	centerx=(centerx-SDL_scrollx)*SDL_zoomx;
	centery=(centery-SDL_scrolly)*SDL_zoomy;
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

void expresshexangle(float ix1,float iy1,float ix2,float iy2,float ix3,float iy3,float ix4,float iy4,float ix5,float iy5,float ix6,float iy6)
{
	inficorn tlinficorn[6];
	tlinficorn[0].x=ix1;
	tlinficorn[1].x=ix2;
	tlinficorn[2].x=ix3;
	tlinficorn[3].x=ix4;
	tlinficorn[4].x=ix5;
	tlinficorn[5].x=ix6;
	tlinficorn[0].y=iy1;
	tlinficorn[1].y=iy2;
	tlinficorn[2].y=iy3;
	tlinficorn[3].y=iy4;
	tlinficorn[4].y=iy5;
	tlinficorn[5].y=iy6;
	expressinfinityangle(tlinficorn,6);
}
void text_print_bitmap(int * posx,int * posy,fontpixinf_ * ifontpixinf)
{
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
	if (scanx<0) {mempos-=scanx;scanx=0;}
	if (scany<0) {mempos-=scany*skip;scany=0;}
	if (maxx>=gfx_canvassizex) maxx=gfx_canvassizex-1;
	if (maxy>=gfx_canvassizey) maxy=gfx_canvassizey-1;
	skip-=maxx-scanx;
	(*posx)+=(*ifontpixinf).deltax;
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
			//	putpixel(ilv2,ilv1);
			}
			icanvas++;
			mempos++;
		}
		mempos+=skip;
		icanvas+=icanvasskip;
	}
}
void printformatted(const char * iinput,const char * parms,int imode,int start,int end)
{
	int ilv4=start;
	char linebreak;
	thatwasatemporaryskip:
	linebreak=0;
	for (;ilv4<end;ilv4++)
	{
		if (iinput[ilv4]==10)
		{
			ilv4++;
			linebreak=1;
			goto skipfornow;
		}
		text_print_bitmap(&SDL_txcursorx,&SDL_txcursory,&fontpixinf[indexfromunicode(((unsigned char)iinput[ilv4]))]);
//		print_bitmap(&SDL_txcursorx,&SDL_txcursory,&fontpixinf[indexfromunicode(((unsigned char)iinput[ilv4]))]);
	}
	skipfornow:
	if (linebreak) {if (ilv4<end) goto thatwasatemporaryskip;}//a line break;
}

void sdl_init()
{
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		fprintf(stderr, "SDL konnte nicht initialisiert werden: %s\n", SDL_GetError());
		exit(1);
	}
	gfx();
}
void sdl_outit()
{
}
void svg_findaround();
void getatoms();
void svg_controlprocedure(bool irestriction,bool hatches);
void svg_main(const char * filename);
void sdl_output()
{
	clock_gettime(clockid,&ts);
	counter1-=ts.tv_nsec+1000000000*ts.tv_sec;
	gfxstart();
	clock_gettime(clockid,&ts);
	counter1+=ts.tv_nsec+1000000000*ts.tv_sec;
	screenclear(0xFFFFFF);
	clock_gettime(clockid,&ts);
	counter2-=ts.tv_nsec+1000000000*ts.tv_sec;
	svg_findaround();
	clock_gettime(clockid,&ts);
	counter2+=ts.tv_nsec+1000000000*ts.tv_sec;
	clock_gettime(clockid,&ts);
	counter3-=ts.tv_nsec+1000000000*ts.tv_sec;
	getatoms();
	clock_gettime(clockid,&ts);
	counter3+=ts.tv_nsec+1000000000*ts.tv_sec;
	clock_gettime(clockid,&ts);
	counter4-=ts.tv_nsec+1000000000*ts.tv_sec;
	initZlist();
	clock_gettime(clockid,&ts);
	counter4+=ts.tv_nsec+1000000000*ts.tv_sec;
	clock_gettime(clockid,&ts);
	counter5-=ts.tv_nsec+1000000000*ts.tv_sec;
	svg_controlprocedure(0,1);
	svg_controlprocedure(0,0);
	clock_gettime(clockid,&ts);
	counter5+=ts.tv_nsec+1000000000*ts.tv_sec;
	clock_gettime(clockid,&ts);
	counter1-=ts.tv_nsec+1000000000*ts.tv_sec;
	gfxstop();
	clock_gettime(clockid,&ts);
	counter1+=ts.tv_nsec+1000000000*ts.tv_sec;
}
