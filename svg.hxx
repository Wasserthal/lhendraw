#define svg_text_buffer_max 1048576
char svg_text_buffer[svg_text_buffer_max+1];
int svg_text_buffer_count=0;
int svg_get_colorstringv(int number)
{
	sprintf(colorstring,"%06X",number);
	return 0;
}
void svg_stylegenestring(int flags,unsigned int fillcolor=0)//0x1: line 0x2: filled 0x4: bold 0x8 dashed
{
	int stylestringlength=0;
	stylestring[0]=0;//redundant
	int formatreturnvalue;
	char * waspar=emptystring;
	if (flags!=0)
	{
		sprintf(stylestring,"style=\"%n",&formatreturnvalue);
		stylestringlength+=formatreturnvalue;
		if (flags & 1)
		{
			sprintf(stylestring+stylestringlength,"%sstroke:#%s%n",waspar,colorstring,&formatreturnvalue);
			stylestringlength+=formatreturnvalue;
			waspar=semicolonstring;
		}
		if ((fillcolor & 0xFF000000)==0)
		{
			if (flags & 2)
			{
				sprintf(stylestring+stylestringlength,"%sfill:#%s%n",waspar,colorstring,&formatreturnvalue);
				stylestringlength+=formatreturnvalue;
				waspar=semicolonstring;
			}
			else
			{
				sprintf(stylestring+stylestringlength,"%sfill:none%n",waspar,&formatreturnvalue);
				stylestringlength+=formatreturnvalue;
				waspar=semicolonstring;
			}
		}
		else
		{
			sprintf(stylestring+stylestringlength,"%sfill:#%06X%n",waspar,fillcolor & 0xFFFFFF,&formatreturnvalue);
			stylestringlength+=formatreturnvalue;
			waspar=semicolonstring;
		}
		if (flags & 4)
		{
			sprintf(stylestring+stylestringlength,"%sstroke-width:5;stroke-linecap:round%n",waspar,&formatreturnvalue);
			stylestringlength+=formatreturnvalue;
			waspar=semicolonstring;
		}
		if (flags & 8)
		{
			sprintf(stylestring+stylestringlength,"%sstroke-dasharray:2,2%n",waspar,&formatreturnvalue);
			stylestringlength+=formatreturnvalue;
			waspar=semicolonstring;
		}
		#ifdef LENNARD_HACK
		if (flags & 0x10)
		{
			sprintf(stylestring+stylestringlength,"%sfill:url(#%s)%n",waspar,current_hack_annotation,&formatreturnvalue);
			stylestringlength+=formatreturnvalue;
			waspar=semicolonstring;
		}
		#endif
		sprintf(stylestring+stylestringlength,"\"%n",&formatreturnvalue);
		stylestringlength+=formatreturnvalue;
	}
	stylestring[stylestringlength]=0;
}
void svg_expressline(float ileft,float itop,float iright,float ibottom)
{
	fprintf(outfile,"<path d=\"M %f %f L %f %f \" %s/>\n",ileft+SVG_currentshiftx,itop+SVG_currentshifty,iright+SVG_currentshiftx,ibottom+SVG_currentshifty,stylestring);
}
void svg_expressspinellipse(float ix,float iy,float irx,float iry,float iangle)
{
	fprintf(outfile,"<ellipse cx=\"%f\" cy=\"%f\" rx=\"%f\" ry=\"%f\" transform=\"rotate(%f,%f,%f)\" %s/>\n",ix+SVG_currentshiftx,iy+SVG_currentshifty,irx,iry,(iangle*180)/Pi,ix+SVG_currentshiftx,iy+SVG_currentshifty,stylestring);
}
void svg_expressellipse(float ix,float iy,float irx,float iry)
{
	fprintf(outfile,"<ellipse cx=\"%f\" cy=\"%f\" rx=\"%f\" ry=\"%f\" %s/>\n",ix+SVG_currentshiftx,iy+SVG_currentshifty,irx,iry,stylestring);
}
void svg_expressinfinityangle(int count)
{
	fprintf(outfile,"<path d=\"M");
	for (int ilv1=0;ilv1<count;ilv1++)
	{
		fprintf(outfile," %f %f %c",LHENDRAW_inficorn[ilv1].x+SVG_currentshiftx,LHENDRAW_inficorn[ilv1].y+SVG_currentshifty,(ilv1==count-1)?'z':'L');
	}
	fprintf(outfile," \" %s/>",stylestring);
}
void svg_expressarc(float centerx,float centery,float radiusx,float radiusy,float startangle,float endangle)
{
	float startx,starty;
	float endx,endy;
	startx=centerx+radiusx*cos(startangle)+SVG_currentshiftx;
	starty=centery+radiusy*sin(startangle)+SVG_currentshifty;
	endx=centerx+radiusx*cos(endangle)+SVG_currentshiftx;
	endy=centery+radiusy*sin(endangle)+SVG_currentshifty;
	fprintf(outfile,"<path d=\"M %f,%f A %f,%f %i %i %i %f %f\" %s />",startx,starty,radiusx,radiusy,(int)0,(int)(fabs(startangle-endangle)>=Pi),(int)(startangle-endangle)<0,endx,endy,stylestring);
}
void svg_expressarc_enhanced(float centerx,float centery,float radiusx,float radiusy,float startangle,float endangle,float tiltangle)
{
	float startx,starty;
	float endx,endy;
	startx=centerx+(radiusx*cos(startangle)*cos(tiltangle)-radiusy*sin(startangle)*sin(tiltangle))+SVG_currentshiftx;
	starty=centery+(radiusy*sin(startangle)*cos(tiltangle)+radiusx*cos(startangle)*sin(tiltangle))+SVG_currentshifty;
	endx=centerx+(radiusx*cos(endangle)*cos(tiltangle)-radiusy*sin(endangle)*sin(tiltangle))+SVG_currentshiftx;
	endy=centery+(radiusy*sin(endangle)*cos(tiltangle)+radiusx*cos(endangle)*sin(tiltangle))+SVG_currentshifty;
	fprintf(outfile,"<path d=\"M %f,%f A %f,%f %i %i %i %f %f\" %s />",startx,starty,radiusx,radiusy,(int)(tiltangle*180.0/Pi),(int)(fabs(startangle-endangle)>=Pi),(int)(startangle-endangle)<0,endx,endy,stylestring);
}
void svg_expressbow(float ileft,float itop,float iright,float ibottom,float irelradius)
{
	float tldist=sqrt(fsqr(iright-ileft)+fsqr(itop-ibottom));
	fprintf(outfile,"<path d=\"M %f,%f A %f,%f %i %i %i %f %f\" %s />",ileft+SVG_currentshiftx,itop+SVG_currentshifty,tldist*irelradius,tldist*irelradius,0,0,1,iright+SVG_currentshiftx,ibottom+SVG_currentshifty,stylestring);
}
void svg_expressbezier(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4)
{
	fprintf(outfile,"<path d=\"M %f %f C %f %f %f %f %f %f \" %s/>\n",x1+SVG_currentshiftx,y1+SVG_currentshifty,x2+SVG_currentshiftx,y2+SVG_currentshifty,x3+SVG_currentshiftx,y3+SVG_currentshifty,x4+SVG_currentshiftx,y4+SVG_currentshifty,stylestring);
}
void svg_expressbezier2(float x1,float y1,float x2,float y2,float x3,float y3)
{
	fprintf(outfile,"<path d=\"M %f %f Q %f %f %f %f \" %s/>\n",x1+SVG_currentshiftx,y1+SVG_currentshifty,x2+SVG_currentshiftx,y2+SVG_currentshifty,x3+SVG_currentshiftx,y3+SVG_currentshifty,stylestring);
}

void svg_expressxbezier(int icount,...)
{
	va_list i_valist;
	va_start(i_valist,icount);
	float iks;float yps;
	iks=va_arg(i_valist,double)+SVG_currentshiftx;
	yps=va_arg(i_valist,double)+SVG_currentshifty;
	fprintf(outfile,"<path d=\" M %f %f C ",iks,yps);
	for (int ilv1=0;ilv1<icount-1;ilv1++)
	{
		iks=va_arg(i_valist,double)+SVG_currentshiftx;
		yps=va_arg(i_valist,double)+SVG_currentshifty;
		fprintf(outfile,"%f %f ",iks,yps);
	}
	va_end(i_valist);
//	%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f 
	fprintf(outfile,"\" %s />",stylestring);
}
int svg_outstring(const char * iinput,...)
{
	va_list arglist;
	va_start(arglist,iinput);
	vsnprintf(svg_text_buffer+svg_text_buffer_count,svg_text_buffer_max-svg_text_buffer_count,iinput,arglist);
	va_end(arglist);
	svg_text_buffer[svg_text_buffer_max]=0;
	svg_text_buffer_count+=strlen(svg_text_buffer+svg_text_buffer_count);
	if (svg_text_buffer_count>=svg_text_buffer_max)
	{
		return -1;
	}
	return 1;
}
void svg_printformatted(const char * iinput,const char * parms,int imode,int start,int end)
{
	int ilv4=start;
	char linebreak;
	thatwasatemporaryskip:
	linebreak=0;
	imode&=0x60;
	if (imode==0x20)
	{
		svg_outstring("<tspan dy=\"+%f\" %s font-size=\"%f\" style=\"fill:#%s\">&#8202;",currentsetfontsize/6.0,parms,currentsetfontsize*0.77777,colorstring);
	}
	if ((imode==0) || (imode==0x60))
	{
		svg_outstring("<tspan %s font-size=\"%f\" style=\"fill:#%s\">",parms,currentsetfontsize,colorstring);
	}
	if (imode==0x40)
	{
		svg_outstring("<tspan dy=\"-%f\" %s font-size=\"%f\" style=\"fill:#%s\">&#8202;",currentsetfontsize/6.0,parms,currentsetfontsize*0.77777,colorstring);
	}
	for (;ilv4<end;ilv4++)
	{
		if (iinput[ilv4]==10)
		{
			ilv4++;
			linebreak=1;
			goto skipfornow;
		}
		for (int ilv2=0;ilv2<sizeof(list_xml)/sizeof(list_bookstavecode);ilv2++)
		{
			if (iinput[ilv4]==list_xml[ilv2].unicode[0])
			{
				svg_outstring("&%s;",list_xml[ilv2].name);
				goto ifertig;
			}
		}
		svg_outstring("%c",iinput[ilv4]);
		ifertig:;
	}
	skipfornow:
	if (linebreak) svg_outstring("\n");
	svg_outstring("</tspan>");
	if (ilv4>=end)
	{
		if (imode==0x20)
		{
			svg_outstring("<tspan %s dy=\"-%f\">&#8202;</tspan>",parms,currentsetfontsize/6.0);
		}
		if (imode==0x40)
		{
			svg_outstring("<tspan %s dy=\"%f\">&#8202;</tspan>",parms,currentsetfontsize/6.0);
		}
	}
	if (linebreak) {svg_outstring("<tspan dy=\"%f\" x=\"0\">&#8202;</tspan>",20.0/18.0*currentsetfontsize);if (ilv4<end) goto thatwasatemporaryskip;}//a line break;
}
void svg_express_txinit(char ialignment,float iposx,float iposy,float iatomfontheight,float angle)
{
	svg_text_buffer_count=0;
	svg_text_buffer[0]=0;
	SDL_text_fallback=0;
	iatomfontheight=12;
	if (ialignment & 2)//For atoms only
	{
		iposx+=(ialignment & 1)?((iatomfontheight*3)/4):(-(iatomfontheight*3)/4);
		iposy+=iatomfontheight/2;
	}
	fprintf(outfile,"<text fill=\"%s\" %s stroke=\"none\" transform=\"translate(%f,%f)\" font-size=\"%f\">",colorstring,(ialignment & 1) ? "text-anchor=\"end\" text-align=\"end\"" : "",iposx+SVG_currentshiftx,iposy+SVG_currentshifty,iatomfontheight);
}
void svg_express_text_tail()
{
	char finished=0;
	for (int ilv1=svg_text_buffer_count;(ilv1>=0 && (finished==0));)
	{
		ilv1--;
		if (ilv1<0) {finished=1; goto only_one_more_time;}
		if (svg_text_buffer[ilv1]==0)
		{
			only_one_more_time:;
			fprintf(outfile,"%s",svg_text_buffer+ilv1+1);
		}
	}
	fprintf(outfile,"</text>\n");
}
void svg_head(FILE * ifile,float width,float height)
{
	outfile=ifile;
	getatoms();
	fprintf(outfile,"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>");
	fprintf(outfile,"<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">");
//	fprintf(outfile,"<svg version=\"1.0\" width=\"%f\" height=\"%f\">\n",SVG_width-SVG_ileft,SVG_height-SVG_itop);
	fprintf(outfile,"<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"%f\" height=\"%f\">\n",width,height);
}
void svg_tail()
{
	fprintf(outfile,"</svg>");
	fclose(outfile);
}
int svg_text_rewind(const _u8 * sizestring,int length,int imode)
{
	if (svg_text_buffer_count>=svg_text_buffer_max-1)
	{
		return -1;
	}
	*(svg_text_buffer+svg_text_buffer_count)=0;
	svg_text_buffer_count++;
	return 1;
}
void svg_controlprocedure(bool irestriction,char clickcollisioncheck);
int svg_main(FILE * ifile)
{
	svg_findaround();
	svg_head(ifile,SVG_width-SVG_ileft,SVG_height-SVG_itop);
	SVG_currentshiftx=-SVG_ileft;
	SVG_currentshifty=-SVG_itop;
	SVG_currentfringex=((unsigned int)-1)>>1;
	SVG_currentfringey=((unsigned int)-1)>>1;
	#ifdef LENNARD_HACK
	Lennard_hatch();
	#endif
	initZlist();
	#ifdef LENNARD_HACK
	if (LENNARD_HACK_dokilltext)
	{
		LENNARD_HACK_killtext();
	}
	#endif
	svg_controlprocedure(0,0);
	svg_tail();
	return 1;
}
int __attribute__((warn_unused_result)) svg_expressgeometry_start(float left,float top,float right,float bottom)
{
	fprintf(outfile,"<path d=\"");
	return 1;
}
void svg_expressgeometry_begin(float x,float y)
{
	gfx_geometry.startx=x;
	gfx_geometry.starty=y;
	fprintf(outfile,"M %f,%f ",x,y);
}
void svg_expressgeometry_arc_enhanced(float centerx,float centery,float radiusx,float radiusy,float startangle,float endangle,float tiltangle)
{
	float startx,starty;
	float endx,endy;
	startx=centerx+(radiusx*cos(startangle)*cos(tiltangle)-radiusy*sin(startangle)*sin(tiltangle))+SVG_currentshiftx;
	starty=centery+(radiusy*sin(startangle)*cos(tiltangle)+radiusx*cos(startangle)*sin(tiltangle))+SVG_currentshifty;
	endx=centerx+(radiusx*cos(endangle)*cos(tiltangle)-radiusy*sin(endangle)*sin(tiltangle))+SVG_currentshiftx;
	endy=centery+(radiusy*sin(endangle)*cos(tiltangle)+radiusx*cos(endangle)*sin(tiltangle))+SVG_currentshifty;
	fprintf(outfile,"M %f,%f A %f,%f %i %i %i %f %f ",startx,starty,radiusx,radiusy,(int)(tiltangle*180.0/Pi),(int)(fabs(startangle-endangle)>=Pi),(int)(startangle-endangle)<0,endx,endy);
}
void svg_expressgeometry_end()
{
	fprintf(outfile,"\" %s/>\n",stylestring);
}
void svg_expressgeometry_line(float x,float y)
{
	fprintf(outfile,"L %f,%f ",x,y);
}
void svg_expressgeometry_bezier2(float x1,float y1,float x2,float y2)
{
	fprintf(outfile,"Q %f,%f %f,%f ",x1,y1,x2,y2);
}
void svg_expressgeometry_bezier3(float x1,float y1,float x2,float y2,float x3,float y3)
{
	fprintf(outfile,"C %f,%f %f,%f %f,%f ",x1,y1,x2,y2,x3,y3);
}
void svg_expressgeometry_backline()
{
	fprintf(outfile,"L %f,%f ",gfx_geometry.startx,gfx_geometry.starty);
}
