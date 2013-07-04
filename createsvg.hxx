#include <math.h>
float Pi=3.141592654;
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
void svg_main(const char * filename)
{
	FILE * outfile;
	outfile=fopen(filename,"w+");
	fprintf(outfile,"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>");
	fprintf(outfile,"<svg version=\"1.0\" width=\"637.79523\" height=\"637.79987\">\n");
	multilist<b_instance> * i_b_multilist=retrievemultilist<b_instance>();
	multilist<color_instance> * i_color_multilist=retrievemultilist<color_instance>();
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
			float cangle=getangle((*endnode).p.x-(*startnode).p.x,(*endnode).p.y-(*startnode).p.y)+Pi/2;
			fprintf(outfile,"<path d=\"M %f %f L %f %f \" style=\"stroke:black\"/>",(*startnode).p.x+bonddist*cos(cangle),(*startnode).p.y+bonddist*sin(cangle),(*endnode).p.x+bonddist*cos(cangle),(*endnode).p.y+bonddist*sin(cangle));
		}
	}
	multilist<graphic_instance> * i_graphic_multilist=retrievemultilist<graphic_instance>();
	for (int ilv1=0;ilv1<(*i_graphic_multilist).filllevel;ilv1++)
	{
		cdx_Rectangle iBBX=((*i_graphic_multilist).bufferlist)[ilv1].BoundingBox;
		int colornr=((*i_graphic_multilist).bufferlist)[ilv1].color;
		fprintf(outfile,"<path d=\"M %f %f L %f %f \" style=\"stroke:black\"/>",iBBX.left,iBBX.top,iBBX.right,iBBX.bottom);
	}
//	for (int ilv1=0;ilv1<(*i_
	fprintf(outfile,"</svg>");
	fclose(outfile);
}
