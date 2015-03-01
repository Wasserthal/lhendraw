float ps_txposx,ps_txposy;
float ps_fontsize;
void ps_express_text_tail()
{
	fprintf(outfile,"pop pop\n");
}
void ps_express_txinit(char ialignment,float iposx,float iposy,float iatomfontheight)
{
	ps_txposx=iposx-4;
	ps_txposy=SVG_height-iposy-4;
	ps_fontsize=iatomfontheight;
	fprintf(outfile,"\nnewpath\n %f %f moveto\n0 0\n",ps_txposx,ps_txposy);
}
void ps_expressarc(float centerx,float centery,float radiusx,float radiusy,float startangle,float endangle)
{
}
void ps_expressarc_enhanced(float centerx,float centery,float radiusx,float radiusy,float startangle,float endangle,float tiltangle)
{
}
void ps_expressbezier(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4)
{
}
void ps_expressbow(float ileft,float itop,float iright,float ibottom,float irelradius)
{
}
void ps_expressellipse(float ix,float iy,float irx,float iry)
{
}
void ps_expressinfinityangle(int count)
{
}
void ps_expressline(float ileft,float itop,float iright,float ibottom)
{
	itop=SVG_height-itop;
	ibottom=SVG_height-ibottom;
	fprintf(outfile,"newpath\n%f %f moveto\n%f %f lineto\n 1 setlinewidth\nstroke\n",ileft,itop,iright,ibottom);
}
void ps_expressspinellipse(float ix,float iy,float irx,float iry,float iangle)
{
}
void ps_expressxbezier(int icount,...)
{
}
int ps_get_colorstringv(int number)
{
	return 0;
}
void ps_printformatted(const char * iinput,const char * parms,int imode,int start,int end)
{
	if (imode==1)
	{
		fprintf(outfile,"0 -6 rmoveto\n");
	}
	if (imode==4)
	{
		fprintf(outfile,"0 6 rmoveto\n");
	}
	fprintf(outfile,"pop\n");
	fprintf(outfile,"/Helvetica findfont\n12 scalefont\nsetfont\n(");
	for (int ilv1=start;ilv1<end;ilv1++)
	{
		char ihv1;
		ihv1=iinput[ilv1];
		if (strchr("\\)(	",ihv1)!=NULL)
		{
			fprintf(outfile,"\\%c",ihv1);goto ifertig;
		}
		if ((((_u8)ihv1) & 0xE0)==0xC0)
		{
			fprintf(outfile,"\\%03hho",(_u8)(((ihv1 & 0x3)<<6)|(iinput[ilv1+1] & 0x3F)));goto ifertig;
		}
		if ((((_u8)ihv1) & 0x80)!=0) goto ifertig;
		fwrite(&ihv1,1,1,outfile);
		ifertig:;
	}
	fprintf(outfile,") dup show stringwidth pop add 0\n");
	if (imode==1)
	{
		fprintf(outfile,"0 6 rmoveto\n");
	}
	if (imode==4)
	{
		fprintf(outfile,"0 -6 rmoveto\n");
	}
}
void ps_stylegenestring(int flags,unsigned int fillcolor=0)
{
}
int ps_text_rewind(const _u8 * sizestring,int length)
{
	fprintf(outfile,"exch 0 exch sub exch rmoveto\n");
	fprintf(outfile,"(");
	fwrite(sizestring,1,length,outfile);
	fprintf(outfile,") stringwidth exch 0 exch sub exch rmoveto\n");
	fprintf(outfile,"0 0\n");
	return 0;
}
int __attribute__((warn_unused_result)) ps_expressgeometry_start(float left,float top,float right,float bottom)
{
	//TODO
	return 0;
}
void ps_expressgeometry_begin(float x,float y)
{
	//TODO
}
void ps_expressgeometry_end()
{
	//TODO
}
void ps_expressgeometry_line(float x,float y)
{
	//TODO
}
void ps_expressgeometry_bezier2(float x1,float y1,float x2,float y2)
{
	//TODO
}
void ps_expressgeometry_backline()
{
	//TODO
}
