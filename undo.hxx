struct undo_singlebuffer
{
	char * buffer;//When==NULL, this buffer is empty, apply the buffer of the parent
	char * contentbuffer;//NULL if no buffer.
	char imultilist[sizeof(multilist<basic_instance>)];
	TELESCOPE_buffer bufferhead;
};
struct undo_undostep_
{
	undo_singlebuffer handles[sizeof(STRUCTURE_OBJECTTYPE_List)/sizeof(trienum)];
	char commandname[80];
	int parent;//Number of the step it arose from. -1 for "is the first parent"
};
undo_undostep_ undosteps[4000];
TELESCOPE_buffer glob_contentbuffer[sizeof(STRUCTURE_OBJECTTYPE_List)/sizeof(trienum)];
char * undo_retrievebuffer(intl start,intl list,intl * auxno=NULL)
{
	intl current=start;
	char * wert;
	iback:;
	if ((wert=undosteps[current].handles[list].buffer)!=NULL)
	{
		return wert;
	}
	else
	{
		current=undosteps[current].parent;
	}
	if (current==-1) return NULL;
	goto iback;
}
TELESCOPE_buffer * undo_retrievecontentbuffer(intl start,intl list,intl * auxno)
{
	intl current=start;
	char * wert;
	iback:;
	if ((wert=undosteps[current].handles[list].contentbuffer)!=NULL)
	{
		return &(undosteps[current].handles[list].bufferhead);
	}
	else
	{
		current=undosteps[current].parent;
	}
	if (current==-1) return NULL;
	goto iback;
}
int init_buffers()
{
	for (int ilv1=0;ilv1<sizeof(STRUCTURE_OBJECTTYPE_List)/sizeof(trienum);ilv1++)
	{
		glob_contentbuffer[ilv1].buffer=(char*)malloc(LHENDRAW_buffersize);
		glob_contentbuffer[ilv1].max=LHENDRAW_buffersize;
		glob_contentbuffer[ilv1].count=0;
	}
	return 0;
}
int undo_trackredo(int param)
{
	for (int ilv1=undosteps_count-1;ilv1>=0;ilv1--)
	{
		if (undosteps[ilv1].parent==currentundostep)
		{
			currentundostep=ilv1;
			return 1;
		}
	}
//	printf("-%i\n",currentundostep);
	return -1;
}
int undo_trackundo()
{
	int lastundostep=currentundostep;
	if (currentundostep==-1) return -1;
	currentundostep=undosteps[currentundostep].parent;
	if (currentundostep==-1)
	{
		currentundostep=lastundostep;
	}
//	printf("%i\n",currentundostep);
	return 0;
}
int storeundo(_u32 flags)
{
	intl imax=0;
	basicmultilist * tl_multilist;
	if (undosteps_count>=4000) return -1;//TODO: remove one UNDO level to free memory!
	for (int ilv1=1;ilv1<sizeof(STRUCTURE_OBJECTTYPE_List)/sizeof(trienum);ilv1++)
	{
		if (tl_multilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name))
		{
			if (flags & (1<<ilv1))
			{
				undosteps[undosteps_count].handles[ilv1].buffer=(char*)malloc(LHENDRAW_buffersize);
				undosteps[undosteps_count].handles[ilv1].contentbuffer=(char*)malloc(LHENDRAW_buffersize);
				imax=min(LHENDRAW_buffersize,glob_contentbuffer[ilv1].count)/sizeof(intl);
				for (int ilv3=0;ilv3<imax;ilv3++)
				{
					((intl*)(undosteps[undosteps_count].handles[ilv1].contentbuffer))[ilv3]=((intl*)(glob_contentbuffer[ilv1].buffer))[ilv3];
				}
				for (int ilv2=0;ilv2<sizeof(multilist<basic_instance>);ilv2++)
				{
					((char*)(undosteps[undosteps_count].handles[ilv1].imultilist))[ilv2]=((char*)(tl_multilist))[ilv2];
				}
				imax=min(LHENDRAW_buffersize,(tl_multilist->itemsize*tl_multilist->filllevel+sizeof(intl)-1))/sizeof(intl);
				for (int ilv3=0;ilv3<imax;ilv3++)
				{
					((intl*)(undosteps[undosteps_count].handles[ilv1].buffer))[ilv3]=((intl*)((*tl_multilist).pointer))[ilv3];
				}
				undosteps[undosteps_count].handles[ilv1].bufferhead.count=glob_contentbuffer[ilv1].count;
			}
			else
			{
				undosteps[undosteps_count].handles[ilv1].buffer=NULL;
				undosteps[undosteps_count].handles[ilv1].contentbuffer=NULL;
			}
			undosteps[undosteps_count].handles[ilv1].bufferhead.max=LHENDRAW_buffersize;
			undosteps[undosteps_count].handles[ilv1].bufferhead.buffer=undosteps[undosteps_count].handles[ilv1].contentbuffer;
		}
	}
	undosteps[undosteps_count].parent=currentundostep;
	currentundostep=undosteps_count;
//	printf("\e[31m%i\e[0m\n",currentundostep);
	undosteps_count++;
	undo_undodirty=1;
	return 1;
}
int restoreundo(_u32 flags,_u32 orderflags/*bit0: restore count only*/)//doesn't discard the old undo state, loads ONLY
{
	intl imax=0;
	basicmultilist * tl_multilist;
	if (currentundostep==-1) return -1;
	intl * tl_buffer=NULL;
	intl thatundostep;
//	printf("\e[34m%i\e[0m\n",currentundostep);
	for (int ilv1=1;ilv1<sizeof(STRUCTURE_OBJECTTYPE_List)/sizeof(trienum);ilv1++)
	{
		if (tl_multilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name))
		{
			if (flags & (1<<ilv1))
			{
				imax=min(LHENDRAW_buffersize,undosteps[currentundostep].handles[ilv1].bufferhead.count)/sizeof(intl);
				tl_buffer=(intl*)(undosteps[currentundostep].handles[ilv1].contentbuffer);
				if (tl_buffer==NULL)
				{
					tl_buffer=(intl*)(undo_retrievecontentbuffer(currentundostep,ilv1)->buffer);
				}
				for (int ilv3=0;ilv3<imax;ilv3++)
				{
					((intl*)(glob_contentbuffer[ilv1].buffer))[ilv3]=tl_buffer[ilv3];
				}
				thatundostep=currentundostep;
				tl_buffer=(intl*)(undosteps[currentundostep].handles[ilv1].buffer);
				if (tl_buffer==NULL)
				{
					tl_buffer=(intl*)(undo_retrievebuffer(currentundostep,ilv1,&thatundostep));
				}
				for (int ilv2=0;ilv2<sizeof(multilist<basic_instance>);ilv2++)
				{
					((char*)(tl_multilist))[ilv2]=((char*)(undosteps[thatundostep].handles[ilv1].imultilist))[ilv2];
				}
				imax=min(LHENDRAW_buffersize,(tl_multilist->itemsize*tl_multilist->filllevel+sizeof(intl)-1))/sizeof(intl);
				for (int ilv3=0;ilv3<imax;ilv3++)
				{
					((intl*)((*tl_multilist).pointer))[ilv3]=(tl_buffer)[ilv3];
				}
				glob_contentbuffer[ilv1].count=undosteps[currentundostep].handles[ilv1].bufferhead.count;
			}
			undosteps[currentundostep].handles[ilv1].bufferhead.max=LHENDRAW_buffersize;
			undosteps[currentundostep].handles[ilv1].bufferhead.buffer=undosteps[currentundostep].handles[ilv1].contentbuffer;
		}
	}
	undo_undodirty=0;
	return 1;
}

