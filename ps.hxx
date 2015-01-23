float ps_txposx,ps_txposy;
float ps_fontsize;
void ps_express_text_tail()
{
}
void ps_express_txinit(char ialignment,float iposx,float iposy,float iatomfontheight)
{
	ps_txposx=iposx;
	ps_txposy=SVG_height-iposy;
	ps_fontsize=iatomfontheight;
	fprintf(outfile,"\nnewpath\n %f %f moveto\n",ps_txposx,ps_txposy);
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
	fprintf(outfile,"/Helvetica findfont\n12 scalefont\nsetfont\n \n(");
	fwrite(iinput+start,1,end-start,outfile);
	fprintf(outfile,") dup show stringwidth 0 rmoveto\n");
}
void ps_stylegenestring(int flags,unsigned int fillcolor=0)
{
}
int ps_text_rewind(const _u8 * sizestring,int length)
{
	return 0;
}
