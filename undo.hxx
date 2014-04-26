
char storeundo(_u32 flags)
{
	int ilv0;
	for (int ilv1=0;ilv1<sizeof(multilist<n_instance>);ilv1++)
	{
		glob_n_undo_buffer[ilv1]=((char*)(glob_n_multilist))[ilv1];
	}
	for (int ilv1=0,ilv0=sizeof(multilist<n_instance>);ilv1<sizeof(n_instance)*bufferlistsize;ilv1++,ilv0++)
	{
		glob_n_undo_buffer[ilv0]=((char*)((*glob_n_multilist).bufferlist))[ilv1];
	}
	for (int ilv1=0;ilv1<1000000;ilv1++)
	{
		glob_n_undo_contentbuffer[ilv1]=internalstructure_n_buffer.buffer[ilv1];
	}
	internalstructure_n_undobuffer.max=internalstructure_n_buffer.max;
	internalstructure_n_undobuffer.count=internalstructure_n_buffer.count;
	for (int ilv1=0;ilv1<sizeof(multilist<b_instance>);ilv1++)
	{
		glob_b_undo_buffer[ilv1]=((char*)(glob_b_multilist))[ilv1];
	}
	for (int ilv1=0,ilv0=sizeof(multilist<b_instance>);ilv1<sizeof(b_instance)*bufferlistsize;ilv1++,ilv0++)
	{
		glob_b_undo_buffer[ilv0]=((char*)((*glob_b_multilist).bufferlist))[ilv1];
	}
	for (int ilv1=0;ilv1<1000000;ilv1++)
	{
		glob_b_undo_contentbuffer[ilv1]=internalstructure_b_buffer.buffer[ilv1];
	}
	internalstructure_b_undobuffer.max=internalstructure_b_buffer.max;
	internalstructure_b_undobuffer.count=internalstructure_b_buffer.count;
/*	for (int ilv1=0;ilv1<sizeof(multilist<text_instance>);ilv1++)
	{
		glob_text_undo_buffer[ilv1]=((char*)(glob_text_multilist))[ilv1];
	}
	for (int ilv1=0,ilv0=sizeof(multilist<text_instance>);ilv1<sizeof(text_instance)*bufferlistsize;ilv1++,ilv0++)
	{
		glob_text_undo_buffer[ilv0]=((char*)((*glob_text_multilist).bufferlist))[ilv1];
	}
	for (int ilv1=0;ilv1<1000000;ilv1++)
	{
		glob_text_undo_contentbuffer[ilv1]=internalstructure_text_buffer.buffer[ilv1];
	}
	internalstructure_text_undobuffer.max=internalstructure_text_buffer.max;
	internalstructure_text_undobuffer.count=internalstructure_text_buffer.count;*/
	for (int ilv1=0;ilv1<sizeof(multilist<arrow_instance>);ilv1++)
	{
		glob_arrow_undo_buffer[ilv1]=((char*)(glob_arrow_multilist))[ilv1];
	}
	for (int ilv1=0,ilv0=sizeof(multilist<arrow_instance>);ilv1<sizeof(arrow_instance)*bufferlistsize;ilv1++,ilv0++)
	{
		glob_arrow_undo_buffer[ilv0]=((char*)((*glob_arrow_multilist).bufferlist))[ilv1];
	}
	for (int ilv1=0;ilv1<1000000;ilv1++)
	{
		glob_arrow_undo_contentbuffer[ilv1]=internalstructure_arrow_buffer.buffer[ilv1];
	}
	internalstructure_arrow_undobuffer.max=internalstructure_arrow_buffer.max;
	internalstructure_arrow_undobuffer.count=internalstructure_arrow_buffer.count;
	return 1;
}
char restoreundo(_u32 flags,_u32 orderflags/*bit0: restore count only*/)//doesn't discard the old undo state, loads ONLY
{
	int ilv0;
	for (int ilv1=0;ilv1<sizeof(multilist<n_instance>);ilv1++)
	{
		((char*)(glob_n_multilist))[ilv1]=glob_n_undo_buffer[ilv1];
	}
	if ((orderflags & 0)==0)
	{
		for (int ilv1=0,ilv0=sizeof(multilist<n_instance>);ilv1<sizeof(n_instance)*bufferlistsize;ilv1++,ilv0++)
		{
			((char*)((*glob_n_multilist).bufferlist))[ilv1]=glob_n_undo_buffer[ilv0];
		}
		for (int ilv1=0;ilv1<1000000;ilv1++)
		{
			internalstructure_n_buffer.buffer[ilv1]=glob_n_undo_contentbuffer[ilv1];
		}
	}
	internalstructure_n_buffer.max=internalstructure_n_undobuffer.max;
	internalstructure_n_buffer.count=internalstructure_n_undobuffer.count;
	for (int ilv1=0;ilv1<sizeof(multilist<b_instance>);ilv1++)
	{
		((char*)(glob_b_multilist))[ilv1]=glob_b_undo_buffer[ilv1];
	}
	if ((orderflags & 0)==0)
	{
		for (int ilv1=0,ilv0=sizeof(multilist<b_instance>);ilv1<sizeof(b_instance)*bufferlistsize;ilv1++,ilv0++)
		{
			((char*)((*glob_b_multilist).bufferlist))[ilv1]=glob_b_undo_buffer[ilv0];
		}
		for (int ilv1=0;ilv1<1000000;ilv1++)
		{
			internalstructure_b_buffer.buffer[ilv1]=glob_b_undo_contentbuffer[ilv1];
		}
	}
	internalstructure_b_buffer.max=internalstructure_b_undobuffer.max;
	internalstructure_b_buffer.count=internalstructure_b_undobuffer.count;
	for (int ilv1=0;ilv1<sizeof(multilist<arrow_instance>);ilv1++)
	{
		((char*)(glob_arrow_multilist))[ilv1]=glob_arrow_undo_buffer[ilv1];
	}
	if ((orderflags & 0)==0)
	{
		for (int ilv1=0,ilv0=sizeof(multilist<arrow_instance>);ilv1<sizeof(arrow_instance)*bufferlistsize;ilv1++,ilv0++)
		{
			((char*)((*glob_arrow_multilist).bufferlist))[ilv1]=glob_arrow_undo_buffer[ilv0];
		}
		for (int ilv1=0;ilv1<1000000;ilv1++)
		{
			internalstructure_arrow_buffer.buffer[ilv1]=glob_arrow_undo_contentbuffer[ilv1];
		}
	}
	internalstructure_arrow_buffer.max=internalstructure_arrow_undobuffer.max;
	internalstructure_arrow_buffer.count=internalstructure_arrow_undobuffer.count;
}
