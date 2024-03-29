float ps_txposx,ps_txposy;
float ps_fontsize;
_u32 ps_linestyle=0;
float ps_currentx;
float ps_currenty;
float ps_color_red;
float ps_color_green;
float ps_color_blue;
int ps_sizex;
int ps_sizey;
CLEANUPRELEVANT int ps_RESX=600;
CLEANUPRELEVANT int ps_RESY=842;
float ps_scalex=1;
float ps_scaley=1;
float ps_offset_x=0;
float ps_offset_y=0;
#ifndef CROFTOIDAL
char ps_buffer[65537];
#else
char ps_overbuffer[65539];
char * ps_buffer=ps_overbuffer+2;
#endif
void print_print(const char * input);
void ps_printf()
{
	if (ps_printmode)
	{
		print_print(ps_buffer);
	}
	else
	{
		fprintf(outfile,"%s",ps_buffer);
	}
	ps_buffer[0]=0;
}
void ps_newmatrix(int format)
{
	ps_scalex=1;
	ps_scaley=1;
	#ifdef CROFTOIDAL
	sprintf(ps_buffer,"initmatrix\n");
	ps_printf();
	#endif
	if (format==1) goto portrait;
	if (format==2) goto landscape;
	if (SVG_height>=SVG_width)
	{
		portrait:;
		if (SVG_height>ps_RESY)
		{
			ps_scaley=((float)ps_RESY)/SVG_height;
			ps_scalex=ps_scaley;
			printf("ps_scaley=%f\n",ps_scaley);
		}
		if ((SVG_width*ps_scalex)>ps_RESX)
		{
			ps_scalex=ps_RESX/SVG_width;
			ps_scaley=ps_scalex;
		}
		sprintf(ps_buffer, "[%f 0 0 -%f 0 0] concat\n",ps_scalex,ps_scaley);
		ps_printf();
		ps_offset_x=0;
		ps_offset_y=-ps_RESY/ps_scaley;
	}
	else
	{
		landscape:;
		if (SVG_height>ps_RESX)
		{
			ps_scaley=((float)ps_RESX)/SVG_height;
			ps_scalex=ps_scaley;
			printf("ps_scaley=%f\n",ps_scaley);
		}
		if ((SVG_width*ps_scalex)>ps_RESY)
		{
			ps_scalex=ps_RESY/SVG_width;
			ps_scaley=ps_scalex;
		}
		sprintf(ps_buffer, "[0 %f %f 0 0 0] concat\n",ps_scalex,ps_scaley);
		ps_printf();
		ps_offset_x=0;
		ps_offset_y=0;
	}
}
void ps_express_text_tail()
{
	sprintf(ps_buffer,"pop pop\n");
	ps_printf();;
}
void ps_express_txinit(char ialignment,float iposx,float iposy,float iatomfontheight,float angle)
{
	SDL_txcursorx=0;
	SDL_txcursory=0;
	SDL_glyfstartx=iposx;
	SDL_glyfstarty=iposy;
	ps_txposx=iposx-4;
	ps_txposy=ps_offset_y+iposy+4;
	ps_fontsize=iatomfontheight;
	SDL_text_fallback=1;
	sprintf(ps_buffer,"\nnewpath\n %f %f moveto\n0 0\n",ps_txposx,ps_txposy);
	ps_printf();
}
void ps_expressarc_enhanced(float centerx,float oldcentery,float radiusx,float radiusy,float startangle,float endangle,float tiltangle)
{
	sprintf(ps_buffer,"\nnewpath\n");
	ps_printf();
	sprintf(ps_buffer, "initmatrix\n");
	ps_printf();
	double centery=ps_offset_y+oldcentery;
	double tiltcos=cos(tiltangle);
	double tiltsin=sin(tiltangle);
	double xeffonx=tiltcos*radiusx;
	double xeffony=-tiltsin*radiusx;
	double yeffonx=tiltsin*radiusy;
	double yeffony=tiltcos*radiusy;
	double newcenterx=centerx*xeffonx+centery*yeffonx;
	double newcentery=centerx*xeffony+centery*yeffony;
	sprintf(ps_buffer, "[%f %f %f %f %f %f] concat\n",xeffonx,xeffony,yeffonx,yeffony,centerx-newcenterx,centery-newcentery);
	ps_printf();
	sprintf(ps_buffer,"%f %f 1 %f %f arc\n",centerx,centery,-startangle/Pi*180.0,-endangle/Pi*180.0);
	ps_printf();
	sprintf(ps_buffer,"%1.6f %1.6f %1.6f setrgbcolor\n",ps_color_red,ps_color_green,ps_color_blue);
	ps_printf();
	if (ps_linestyle & 2)
	{
		sprintf(ps_buffer,"fill\n");
		ps_printf();
	}
	else
	{
		sprintf(ps_buffer,"%1.6f setlinewidth\n",(ps_linestyle&4)?0.1:0.025);
		ps_printf();
		sprintf(ps_buffer,"stroke\n");
		ps_printf();
	}
	sprintf(ps_buffer,"initmatrix\n");
	ps_printf();
}
void ps_expressarc(float centerx,float centery,float radiusx,float radiusy,float startangle,float endangle)
{
	ps_expressarc_enhanced(centerx,centery,radiusx,radiusy,startangle,endangle,0);
}
void ps_expressbezier(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4)
{
}
void ps_expressbezier2(float x1,float y1,float x2,float y2,float x3,float y3)
{
}
void ps_expressbow(float ileft,float itop,float iright,float ibottom,float irelradius)
{
}
void ps_expressinfinityangle(int count)
{
	sprintf(ps_buffer,"\nnewpath\n%f %f moveto\n",LHENDRAW_inficorn[0].x,ps_offset_y+LHENDRAW_inficorn[0].y);
	ps_printf();
	for (int ilv1=0;ilv1<count;ilv1++)
	{
		sprintf(ps_buffer,"\n%f %f lineto\n",LHENDRAW_inficorn[(ilv1+1)%count].x,ps_offset_y+LHENDRAW_inficorn[(ilv1+1)%count].y);
		ps_printf();
	}
	sprintf(ps_buffer,"%1.6f %1.6f %1.6f setrgbcolor\n",ps_color_red,ps_color_green,ps_color_blue);
	ps_printf();
	if (ps_linestyle & 2)
	{
		sprintf(ps_buffer,"fill\n");
		ps_printf();
	}
	else
	{
		sprintf(ps_buffer,"%i setlinewidth\n",(ps_linestyle&4)?4:1);
		ps_printf();
		sprintf(ps_buffer,"stroke\n");
		ps_printf();
	}
}
void ps_expressline(float ileft,float itop,float iright,float ibottom)
{
	itop=ps_offset_y+itop;
	ibottom=ps_offset_y+ibottom;
	sprintf(ps_buffer,"newpath\n%f %f moveto\n%f %f lineto\n%1.6f %1.6f %1.6f setrgbcolor\n%i setlinewidth\nstroke\n",ileft,itop,iright,ibottom,ps_color_red,ps_color_green,ps_color_blue,(ps_linestyle&4)?4:1);
	ps_printf();
}
void ps_expressspinellipse(float ix,float iy,float irx,float iry,float iangle)
{
	ps_expressarc_enhanced(ix,iy,irx,iry,2*Pi,0,iangle);
}
void ps_expressellipse(float ix,float iy,float irx,float iry)
{
	ps_expressspinellipse(ix,iy,irx,iry,0);
}
void ps_expressxbezier(int icount,...)
{
}
int ps_get_colorstringv(int number)
{
	_u8 int_red=(number & 0xFF0000)>>16;
	_u8 int_green=(number & 0xFF00)>>8;
	_u8 int_blue=number & 0xFF;
	ps_color_red=int_red/255.0;
	ps_color_green=int_green/255.0;
	ps_color_blue=int_blue/255.0;
	return 0;
}
void ps_printformatted(const char * iinput,const char * parms,int imode,int start,int end)
{
	if (imode==0x20)
	{
		sprintf(ps_buffer,"0 -6 rmoveto\n");
		ps_printf();
	}
	if (imode==0x40)
	{
		sprintf(ps_buffer,"0 6 rmoveto\n");
		ps_printf();
	}
	sprintf(ps_buffer,"pop\n");
	ps_printf();
	sprintf(ps_buffer,"/Helvetica findfont\n12 scalefont\nsetfont\n(");
	ps_printf();
	for (int ilv1=start;ilv1<end;ilv1++)
	{
		char ihv1;
		ihv1=iinput[ilv1];
		if (strchr("\\)(	",ihv1)!=NULL)
		{
			sprintf(ps_buffer,"\\%c",ihv1);goto ifertig;
			ps_printf();
		}
		if ((((_u8)ihv1) & 0xE0)==0xC0)
		{
			sprintf(ps_buffer,"\\%03hho",(_u8)(((ihv1 & 0x3)<<6)|(iinput[ilv1+1] & 0x3F)));goto ifertig;
			ps_printf();
		}
		if ((((_u8)ihv1) & 0x80)!=0) goto ifertig;
		fwrite(&ihv1,1,1,outfile);
		ifertig:;
	}
	sprintf(ps_buffer,") dup show stringwidth pop add 0\n");
	ps_printf();
	if (imode==0x20)
	{
		sprintf(ps_buffer,"0 6 rmoveto\n");
		ps_printf();
	}
	if (imode==0x40)
	{
		sprintf(ps_buffer,"0 -6 rmoveto\n");
		ps_printf();
	}
}
void ps_stylegenestring(int flags,unsigned int fillcolor=0)
{
	ps_linestyle=flags;
}
int ps_text_rewind(const _u8 * sizestring,int length,int imode)
{
	sprintf(ps_buffer,"exch 0 exch sub exch rmoveto\n");
	ps_printf();
	sprintf(ps_buffer,"(");
	ps_printf();
	fwrite(sizestring,1,length,outfile);
	sprintf(ps_buffer,") stringwidth exch 0 exch sub exch rmoveto\n");
	ps_printf();
	sprintf(ps_buffer,"0 0\n");
	ps_printf();
	return 0;
}
int __attribute__((warn_unused_result)) ps_expressgeometry_start(float left,float top,float right,float bottom)
{
	sprintf(ps_buffer,"newpath\n");
	ps_printf();
	return 1;
}
void ps_expressgeometry_begin(float x,float y)
{
	sprintf(ps_buffer,"%1.6f %1.6f moveto\n",x,ps_offset_y+y);
	ps_printf();
	gfx_geometry.startx=x;
	gfx_geometry.starty=y;
	ps_currentx=x;
	ps_currenty=y;
}
void ps_expressgeometry_arc_enhanced(float centerx,float centery,float radiusx,float radiusy,float startangle,float endangle,float tiltangle)
{
}
void ps_expressgeometry_end()
{
	sprintf(ps_buffer,"%1.6f %1.6f %1.6f setrgbcolor\n",ps_color_red,ps_color_green,ps_color_blue);
	ps_printf();
	if (ps_linestyle & 2)
	{
		sprintf(ps_buffer,"fill\n");
		ps_printf();
	}
	else
	{
		sprintf(ps_buffer,"%i setlinewidth\n",(ps_linestyle&4)?4:1);
		ps_printf();
		sprintf(ps_buffer,"stroke\n");
		ps_printf();
	}
}
void ps_expressgeometry_line(float x,float y)
{
	sprintf(ps_buffer,"%f %f lineto\n",x,ps_offset_y+y);
	ps_printf();
	ps_currentx=x;
	ps_currenty=y;
}
void ps_expressgeometry_bezier2(float x1,float y1,float x2,float y2)
{
	double ax=(ps_currentx+x1*2)/3.0;
	double ay=(ps_currenty+y1*2)/3.0;
	double bx=(x2+x1*2)/3.0;
	double by=(y2+y1*2)/3.0;
	sprintf(ps_buffer,"%1.9f %1.9f %1.9f %1.9f %1.9f %1.9f curveto\n",ax,ps_offset_y+ay,bx,ps_offset_y+by,x2,ps_offset_y+y2);
	ps_printf();
	ps_currentx=x2;
	ps_currenty=y2;
	if (((ps_offset_y+y2)==350.0) && (x2==0))
	{
		sprintf(ps_buffer,"%%ursprungspunktfehler quadratisch\n");
		ps_printf();
	}
}
void ps_expressgeometry_bezier3(float x1,float y1,float x2,float y2,float x3,float y3)
{
	sprintf(ps_buffer,"%1.6f %1.6f %1.6f %1.6f %1.6f %1.6f curveto\n",x1,ps_offset_y+y1,x2,ps_offset_y+y2,x3,ps_offset_y+y3);
	ps_printf();
	ps_currentx=x3;
	ps_currenty=y3;
	if (((ps_offset_y+y3)==350.0) && (x3==0))
	{
		sprintf(ps_buffer,"%%ursprungspunktfehler kubisch\n");
		ps_printf();
	}
}
void ps_expressgeometry_backline()
{
	sprintf(ps_buffer,"%1.6f %1.6f lineto\n",gfx_geometry.startx,ps_offset_y+gfx_geometry.starty);
	ps_printf();
}
