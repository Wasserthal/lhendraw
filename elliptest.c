#include <stdio.h>
#include <math.h>
#include "lendefs.h"
#include "ellipeq.h"

FILE* framebuffer;
void putpixel(int x, int y,int color)
{
	x+=960;
	y+=540;
	fseek(framebuffer,(1920*y+x)*4,SEEK_SET);
	fwrite(&color,4,1,framebuffer);
}
void ellipse(double scalex,double scaley,int color)
{
	for (int ilv1=0;ilv1<360;ilv1++)
	{
		putpixel(cos(ilv1*Pi/180.0)*scalex,sin(ilv1*Pi/180.0)*scaley,color);
	}
}
void shearellipse(double scalex,double scaley,double shear,int color)
{
	for (int ilv1=0;ilv1<360;ilv1++)
	{
		putpixel(cos(ilv1*Pi/180.0)*scalex+shear*sin(ilv1*Pi/180.0)*scaley,sin(ilv1*Pi/180.0)*scaley,color);
	}
}
void turnellipse(double scalex,double scaley,double turn,int color)
{
	for (int ilv1=0;ilv1<360;ilv1++)
	{
		putpixel(cos(ilv1*Pi/180.0)*scalex*cos(turn)-sin(ilv1*Pi/180.0)*scaley*sin(turn),cos(ilv1*Pi/180.0)*scalex*sin(turn)+sin(ilv1*Pi/180.0)*scaley*cos(turn),color);
	}
}
struct cdx_Point3D
{
	double x;
	double y;
};
double getangle(double dx,double dy)
{
	double frac;
	double val;
	if (dx!=0)
	{
		frac=dy/dx;
	}
	else
	{
		if (dy>=0)
		{
			val=Pi/2;
		}
		else
		{
			val=3*Pi/2;
		}
		return val;
	}
	val=atan(frac);
	if (dx<0)
	{
		val+=Pi;
	}
	if (val<0)
	{
		val+=2*Pi;
	}
	return val;
}
struct ellipsoid_
{
	double majx,majy;
	double minx,miny;
	double normalizedmajx,normalizedmajy;
	double normalizedminx,normalizedminy;
	double radiusx,radiusy;
	double internal1,internal2;
	double internalrad,internalangle;
	double axangle;
	char shearsharepositive;
	void create(cdx_Point3D iCenter,cdx_Point3D iMajor,cdx_Point3D iMinor)
	{
		majx=iMajor.x-iCenter.x;
		majy=iMajor.y-iCenter.y;
		minx=iMinor.x-iCenter.x;
		miny=iMinor.y-iCenter.y;
		radiusx=sqrt(fsqr(majx)+fsqr(majy));
		radiusy=sqrt(fsqr(minx)+fsqr(miny));
/*		if (radiusx<radiusy)
		{
			double swap;
			swap=majx;
			majx=majy;
			majy=swap;
			swap=minx;
			minx=miny;
			miny=swap;
			swap=radiusx;
			radiusx=radiusy;
			radiusy=swap;
		}*/
		if (radiusx>0.0000001)
		{
			normalizedmajx=majx/radiusx;
			normalizedmajy=majy/radiusx;
		}
		if (radiusy>0.0000001)
		{
			normalizedminx=minx/radiusy;
			normalizedminy=miny/radiusy;
		}
		axangle=getangle(majx,majy);
		double shearshare=(normalizedmajx*minx+normalizedmajy*miny);
		printf("%f\n",shearshare);
		printf("%f\n",radiusy);
		radiusy=radiusy*sqrt(1-fsqr(shearshare/radiusy));
		printf("%f\n",radiusy);
		double oldvol=fabs(radiusx*radiusy);
		shearsharepositive=(shearshare>0);
		shearshare=fabs(shearshare/radiusy);
		if (fabs(shearshare)>=1e-6)
		{
			double tiltangle;
			double tilttangens;
			tiltangle=acos(NORMALIZEELLIPSE(1,radiusy/radiusx,shearshare))/2;
			tilttangens=tan(tiltangle);
			double flatness=(radiusy/radiusx);
			printf("%f\n",tilttangens);
			printf(">>%f\n",(1/(flatness*((1/tilttangens)-shearshare))));
			double criticaliks=sqrt(fabs(1/((1/(flatness*(shearshare-(1/tilttangens))))-1)));
			printf("%f\n",criticaliks);
			double criticalyps=sqrt(1-fsqr(criticaliks));
			radiusx=radiusx*sqrt(fsqr(criticalyps*shearshare*flatness+criticaliks)+fsqr(criticalyps*flatness));
			printf("%f\n",radiusx);
			radiusy=oldvol/radiusx;
			axangle+=shearsharepositive?tiltangle:-tiltangle;
		}
	}
	void reset()
	{
		normalizedmajx=1;
		normalizedminx=0;
		normalizedmajy=0;
		normalizedminy=1;
		axangle=0;
	}
	void fill(double x,double y)
	{
		internal1=(normalizedmajx*x+normalizedmajy*y)/radiusx;
		internal2=(normalizedminx*x+normalizedminy*y)/radiusy;
		internalrad=sqrt(fsqr(internal1)+fsqr(internal2));
		internalangle=getangle(internal1,internal2);
	}
} ellipsoid;
int main(int argc,char**argv)
{
	framebuffer=fopen("/dev/fb0","r+");
	double shear=1;
	float ifactorx=200;
	float ifactory=100;
	ellipse(ifactorx,ifactorx,0xFF0000);
	shearellipse(ifactorx,ifactory,shear,0xFF0000);
	double backval1;
	backval1=NORMALIZEELLIPSE(1,ifactory/ifactorx,shear);
	for (int ilv1=-200;ilv1<200;ilv1++)
	{
		putpixel(backval1*ifactorx,ilv1,0xFFFF00);
	}
	ellipsoid.create(cdx_Point3D{0,0},cdx_Point3D{ifactorx,0},cdx_Point3D{ifactory*shear,ifactory});
	turnellipse(ellipsoid.radiusx,ellipsoid.radiusy,ellipsoid.axangle,0x0000FF);
	fclose(framebuffer);
	return 0;
}
