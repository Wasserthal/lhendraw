#include <math.h>
float Pi=3.141592654;
#define minfloat -1.0e20;
#define maxfloat 1.0e20;
float getangle(float dx,float dy)
{
	float frac;
	float val;
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
float bonddist=4;
float arrowdepth=10;
float arrowthickness=10;

multilist<color_instance> * glob_color_multilist;
char colorstring[7]="AABBCC";
color_instance * get_color(int number)
{
	if (number==0)
	{
		return NULL;
	}
	if (number==1)
	{
		return NULL;
	}
	if (number-2>=(*glob_color_multilist).filllevel)
	{
		return NULL;
	}
	return &(((*glob_color_multilist).bufferlist)[number-2]);//is this really the best method? After all, colors have no IDs.
}

int get_colorstring(int number)
{
	if (number==0)
	{
		strcpy(colorstring,"000000");
	}
	if (number==1)
	{
		strcpy(colorstring,"FFFFFF");
	}
	if (number-2>=(*glob_color_multilist).filllevel)
	{	
		strcpy(colorstring,"000000");
		return -1;
	}
	snprintf(colorstring,7,"%02hhX%02hhX%02hhX",char(((*glob_color_multilist).bufferlist)[number-2].r*255),char(((*glob_color_multilist).bufferlist)[number-2].g*255),char(((*glob_color_multilist).bufferlist)[number-2].b*255)); //is this really the best method? After all, colors have no IDs.
	return 0;
}

void getcaptions(float * width,float * height)
{
	float minx=maxfloat;
	float miny=maxfloat;
	float maxx=minfloat;
	float maxy=minfloat;
	multilist<n_instance> * i_n_multilist=retrievemultilist<n_instance>();
	multilist<graphic_instance> * i_graphic_multilist=retrievemultilist<graphic_instance>();
	for (int ilv1=0;ilv1<(*i_n_multilist).filllevel;ilv1++)
	{
		if ((*i_n_multilist).bufferlist[ilv1].p.x>maxx)
		{
			maxx=(*i_n_multilist).bufferlist[ilv1].p.x;
		}
		if ((*i_n_multilist).bufferlist[ilv1].p.y>maxy)
		{
			maxy=(*i_n_multilist).bufferlist[ilv1].p.y;
		}
		if ((*i_n_multilist).bufferlist[ilv1].p.x<minx)
		{
			minx=(*i_n_multilist).bufferlist[ilv1].p.x;
		}
		if ((*i_n_multilist).bufferlist[ilv1].p.y<miny)
		{
			miny=(*i_n_multilist).bufferlist[ilv1].p.y;
		}
	}
	for (int ilv1=0;ilv1<(*i_graphic_multilist).filllevel;ilv1++)
	{
		if ((*i_graphic_multilist).bufferlist[ilv1].BoundingBox.left>maxx)
		{
			maxx=(*i_graphic_multilist).bufferlist[ilv1].BoundingBox.left;
		}
		if ((*i_graphic_multilist).bufferlist[ilv1].BoundingBox.top>maxy)
		{
			maxy=(*i_graphic_multilist).bufferlist[ilv1].BoundingBox.top;
		}
		if ((*i_graphic_multilist).bufferlist[ilv1].BoundingBox.left<minx)
		{
			minx=(*i_graphic_multilist).bufferlist[ilv1].BoundingBox.left;
		}
		if ((*i_graphic_multilist).bufferlist[ilv1].BoundingBox.top<miny)
		{
			miny=(*i_graphic_multilist).bufferlist[ilv1].BoundingBox.top;
		}
		if ((*i_graphic_multilist).bufferlist[ilv1].BoundingBox.right>maxx)
		{
			maxx=(*i_graphic_multilist).bufferlist[ilv1].BoundingBox.right;
		}
		if ((*i_graphic_multilist).bufferlist[ilv1].BoundingBox.bottom>maxy)
		{
			maxy=(*i_graphic_multilist).bufferlist[ilv1].BoundingBox.bottom;
		}
		if ((*i_graphic_multilist).bufferlist[ilv1].BoundingBox.right<minx)
		{
			minx=(*i_graphic_multilist).bufferlist[ilv1].BoundingBox.right;
		}
		if ((*i_graphic_multilist).bufferlist[ilv1].BoundingBox.bottom<miny)
		{
			miny=(*i_graphic_multilist).bufferlist[ilv1].BoundingBox.bottom;
		}
	}
	(*width)=maxx;
	(*height)=maxy;
}

void svg_main(const char * filename)
{
	FILE * outfile;
	float cangle;
	float langle;
	float width, height;
	outfile=fopen(filename,"w+");
	fprintf(outfile,"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>");
	getcaptions(&width,&height);
	width+=10;
	height+=10;
	fprintf(outfile,"<svg version=\"1.0\" width=\"%f\" height=\"%f\">\n",width,height);
	glob_color_multilist=retrievemultilist<color_instance>();
	multilist<b_instance> * i_b_multilist=retrievemultilist<b_instance>();
	multilist<n_instance> * i_n_multilist=retrievemultilist<n_instance>();
	n_instance * startnode, * endnode;
	for (int ilv1=0;ilv1<(*i_b_multilist).filllevel;ilv1++)
	{
		for (int ilv2=0;ilv2<(*i_n_multilist).filllevel;ilv2++)
		{
			if (((*i_n_multilist).bufferlist)[ilv2].id==((*i_b_multilist).bufferlist)[ilv1].E)
			{
				endnode=&(((*i_n_multilist).bufferlist)[ilv2]);
			}
			if (((*i_n_multilist).bufferlist)[ilv2].id==((*i_b_multilist).bufferlist)[ilv1].B)
			{
				startnode=&(((*i_n_multilist).bufferlist)[ilv2]);
			}
		}
		fprintf(outfile,"<path d=\"M %f %f L %f %f \" style=\"stroke:black\"/>",(*startnode).p.x,(*startnode).p.y,(*endnode).p.x,(*endnode).p.y);
		if (((*i_b_multilist).bufferlist)[ilv1].Order==2)
		{
			cangle=getangle((*endnode).p.x-(*startnode).p.x,(*endnode).p.y-(*startnode).p.y)+Pi/2;
			fprintf(outfile,"<path d=\"M %f %f L %f %f \" style=\"stroke:black\"/>",(*startnode).p.x+bonddist*cos(cangle),(*startnode).p.y+bonddist*sin(cangle),(*endnode).p.x+bonddist*cos(cangle),(*endnode).p.y+bonddist*sin(cangle));
		}
	}
	multilist<graphic_instance> * i_graphic_multilist=retrievemultilist<graphic_instance>();
	for (int ilv1=0;ilv1<(*i_graphic_multilist).filllevel;ilv1++)
	{
		cdx_Rectangle iBBX=((*i_graphic_multilist).bufferlist)[ilv1].BoundingBox;
		int colornr=((*i_graphic_multilist).bufferlist)[ilv1].color;
		get_colorstring(colornr);
		fprintf(outfile,"<path d=\"M %f %f L %f %f \" style=\"stroke:#%s\"/>",iBBX.left,iBBX.top,iBBX.right,iBBX.bottom,colorstring);
		cangle=getangle(iBBX.right-iBBX.left,iBBX.top-iBBX.bottom)+Pi/2;
		langle=getangle(iBBX.right-iBBX.left,iBBX.top-iBBX.bottom);
		if (((*i_graphic_multilist).bufferlist)[ilv1].ArrowType==2)
		{
		fprintf(outfile,"<path d=\"M %f %f L %f %f \" style=\"stroke:#%s fill:#%s\"/>",iBBX.left,iBBX.top,iBBX.left+cos(langle)*arrowdepth+cos(cangle)*arrowthickness,iBBX.top+sin(langle)*arrowdepth+sin(cangle)*arrowthickness,colorstring,colorstring);
		fprintf(outfile,"<path d=\"M %f %f L %f %f \" style=\"stroke:#%s fill:#%s\"/>",iBBX.left,iBBX.top,iBBX.left+cos(langle)*arrowdepth-cos(cangle)*arrowthickness,iBBX.top+sin(langle)*arrowdepth-sin(cangle)*arrowthickness,colorstring,colorstring);
		}
	}
	multilist<t_instance> * i_t_multilist=retrievemultilist<t_instance>();
	multilist<s_instance> * i_s_multilist=retrievemultilist<s_instance>();
	for (int ilv1=0;ilv1<(*i_t_multilist).filllevel;ilv1++)
	{
		fprintf(outfile,"<text fill=\"black\" stroke=\"none\" transform=\"translate(%f,%f)\" font-size=\"10\">",((*i_t_multilist).bufferlist)[ilv1].p.x,((*i_t_multilist).bufferlist)[ilv1].p.y);
		intl start,end;
		start=(*(((*i_t_multilist).bufferlist)[ilv1].s)).start_in_it;
		end=start+(*(((*i_t_multilist).bufferlist)[ilv1].s)).count_in_it;
		printf("start:%i,end:%i",start,end);
		for (int ilv2=start;ilv2<end;ilv2++)
		{
			fprintf(outfile,"%s",(((*i_s_multilist).bufferlist))[ilv2].PCTEXT.a);
		}
		fprintf(outfile,"</text>");
	}
	
//	for (int ilv1=0;ilv1<(*i_
	fprintf(outfile,"</svg>");
	fclose(outfile);
}
