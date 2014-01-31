char * filestructure_buffer;
intl filestructure_buffer_max;
intl filestructure_buffer_count;
int getbufferfromstructure(basicmultilist * input,char * * bufferptr,intl * ibufferlength,intl * * bufferfill)
{
	if (input==CAMBRIDGEPREFIX2(glob,s_multilist))
	{
		(*bufferptr)=filestructure_buffer;
		(*ibufferlength)=filestructure_buffer_max;
		(*bufferfill)=&filestructure_buffer_count;
		return 1;
	}
	if (input==CAMBRIDGEPREFIX2(glob,curve_multilist))
	{
		(*bufferptr)=filestructure_buffer;
		(*ibufferlength)=filestructure_buffer_max;
		(*bufferfill)=&filestructure_buffer_count;
		return 1;
	}
	return 0;
}
int initmemory()
{
	filestructure_buffer=(char*)malloc(1000000);
	filestructure_buffer_max=0;
	filestructure_buffer_count=0;
}
