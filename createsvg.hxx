void svg_main(const char * filename)
{
	FILE * outfile;
	outfile=fopen(filename,"w+");
	fprintf(outfile,"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>");
	fprintf(outfile,"<svg version=\"1.0\" width=\"637.79523\" height=\"637.79987\">\n");
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
	}
//	for (int ilv1=0;ilv1<(*i_
	fprintf(outfile,"</svg>");
	fclose(outfile);
}
