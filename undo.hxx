//This is the unit which processes undo'es. The undoes are stored in a Fifo buffer. Fifo in the sense: oldest items are more probably discarded.
//Among a TOTAL undo buffer, which indexes the aaftermentioned buffers, following buffers are used:
//For each undostep and each multilist, a buffer item for its contents
//If that multilist-item has contents, a contentbuffer holding its contents.
//Needed for buffers:

//undoes:
//The total undo list!
//All undo buffers!
//All undo contentbuffers!


struct undo_undostep_
{
	undo_singlebuffer handles[sizeof(STRUCTURE_OBJECTTYPE_List)/sizeof(trienum)];
	char commandname[80];
	int parent;//Number of the step it arose from. -1 for "is the first parent"
};
undo_undostep_ undosteps[constants_undostep_max];
TELESCOPE_buffer glob_contentbuffer[sizeof(STRUCTURE_OBJECTTYPE_List)/sizeof(trienum)];
char undo_nextcommandname[80]="ROOT";
char * undo_retrievebuffer(intl start,intl list)
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
undo_singlebuffer * undo_retrievehandle(intl start,intl list)
{
	intl current=start;
	char * wert;
	iback:;
	if ((undosteps[current].handles[list].buffer)!=NULL)
	{
		undo_singlebuffer * isinglebuffer=undosteps[current].handles+list;
		(*(basicmultilist*)((*isinglebuffer).imultilist)).pointer=(*isinglebuffer).buffer;
		return isinglebuffer;
	}
	else
	{
		current=undosteps[current].parent;
	}
	if (current==-1) return NULL;
	goto iback;
}
TELESCOPE_buffer * undo_retrievecontentbuffer(intl start,intl list)
{
	intl current=start;
	char * wert;
	iback:;
	if ((wert=undosteps[current].handles[list].contentbuffer)!=NULL)
	{
		undosteps[current].handles[list].bufferhead.buffer=undosteps[current].handles[list].contentbuffer;
		//set buffer_max here when not using LHENDRAW_buffersize
		return &(undosteps[current].handles[list].bufferhead);
	}
	else
	{
		current=undosteps[current].parent;
	}
	if (current==-1) return NULL;
	goto iback;
}
int undo_getbufferfromstructure(basicmultilist * input,TELESCOPE_buffer * * bufferptr)
{
	for (int ilv1=0;ilv1<undosteps_count;ilv1++)
	{
		for (int ilv2=1;ilv2<STRUCTURE_OBJECTTYPE_ListSize;ilv2++)
		{
			if ((undosteps[ilv1].handles[ilv2].imultilist)==(char*)input)
			{
				*bufferptr=undo_retrievecontentbuffer(ilv1,ilv2);
				return 1;
			}
		}
	}
	return 0;
}
int init_buffers()
{
	for (int ilv1=0;ilv1<sizeof(STRUCTURE_OBJECTTYPE_List)/sizeof(trienum);ilv1++)
	{
		memory_alloc(&(glob_contentbuffer[ilv1].buffer),3);
		//set buffer_max here when not using LHENDRAW_buffersize
		glob_contentbuffer[ilv1].count=0;
	}
	return 0;
}
int undo_trackredo(int param)
{
	if (undo_undodirty==0)
	{
		for (int ilv1=undosteps_count-1;ilv1>=0;ilv1--)
		{
			if (undosteps[ilv1].parent==currentundostep)
			{
				currentundostep=ilv1;
				return 1;
			}
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
void drawundo()//TEST routine
{
	for (int ilv1=0;ilv1<undosteps_count;ilv1++)
	{
		printf("\e[32m%4i\e[0m",undosteps[ilv1].parent);
	}
	printf("\n");
}
int slayredo()
{
	int oldparent=-1;
	for (int ilv1=undosteps_count-1;ilv1>=0;ilv1--)
	{
		if (ilv1==currentundostep) goto cantdeletethis;
		if (undosteps[ilv1].parent<=-1) goto cantdeletethis;
		for (int ilv2=undosteps_count-1;ilv2>ilv1;ilv2--)
		{
			if (undosteps[ilv2].parent==ilv1) goto cantdeletethis;
		}
		oldparent=undosteps[ilv1].parent;
		for (int ilv3=1;ilv3<sizeof(STRUCTURE_OBJECTTYPE_List)/sizeof(trienum);ilv3++)
		{
			if (undosteps[ilv1].handles[ilv3].buffer!=NULL)
			{
				memory_free(undosteps[ilv1].handles[ilv3].buffer);
			}
			if (undosteps[ilv1].handles[ilv3].contentbuffer!=NULL)
			{
				memory_free(undosteps[ilv1].handles[ilv3].contentbuffer);
			}
		}
		for (int ilv2=ilv1;ilv2<undosteps_count-1;ilv2++)
		{
			undosteps[ilv2]=undosteps[ilv2+1];
		}
		memory_areamoved((char*)&(undosteps[2]),(char*)&(undosteps[1]),sizeof(undosteps[0])*(undosteps_count-1-ilv1));
		if (currentundostep>ilv1)
		{
			currentundostep--;
		}
		undosteps_count--;
		for (int ilv2=ilv1;ilv2<undosteps_count;ilv2++)
		{
			if (undosteps[ilv2].parent>ilv1) undosteps[ilv2].parent--; else if (undosteps[ilv2].parent==ilv1) undosteps[ilv2].parent=oldparent;//== case should be unnecessary
		}
		return 1;
		cantdeletethis:;
	}
	printf("Slayredo failed!\n");
	return -1;
}
int slayundo()
{
	//TODO: make independent of slayredo being complete
	intl imax;
	basicmultilist * tl_multilist;
	if (currentundostep<=1) return -1;
	for (int ilv1=1;ilv1<sizeof(STRUCTURE_OBJECTTYPE_List)/sizeof(trienum);ilv1++)
	{
		tl_multilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
		if (tl_multilist)
		{
			if (undosteps[1].handles[ilv1].buffer!=0)
			{
				imax=min(LHENDRAW_buffersize,(tl_multilist->itemsize*((basicmultilist*)(undosteps[1].handles[ilv1].imultilist))->filllevel+sizeof(intl)-1))/sizeof(intl);
				for (int ilv2=1;ilv2<imax;ilv2++)
				{
					((intl*)(undosteps[0].handles[ilv1].buffer))[ilv2]=((intl*)(undosteps[1].handles[ilv1].buffer))[ilv2];
				}
				memory_free(undosteps[1].handles[ilv1].buffer);
				for (int ilv2=0;ilv2<sizeof(basicmultilist);ilv2++)
				{
					undosteps[0].handles[ilv1].imultilist[ilv2]=undosteps[1].handles[ilv1].imultilist[ilv2];
				}
			}
			if (undosteps[1].handles[ilv1].contentbuffer!=0)
			{
				imax=min(LHENDRAW_buffersize,undosteps[1].handles[ilv1].bufferhead.count+sizeof(intl)-1)/sizeof(intl);
				for (int ilv2=1;ilv2<imax;ilv2++)
				{
					((intl*)(undosteps[0].handles[ilv1].contentbuffer))[ilv2]=((intl*)(undosteps[1].handles[ilv1].contentbuffer))[ilv2];
				}
				undosteps[0].handles[ilv1].bufferhead=undosteps[1].handles[ilv1].bufferhead;
				undosteps[0].handles[ilv1].bufferhead.buffer=undosteps[0].handles[ilv1].contentbuffer;
				memory_free(undosteps[1].handles[ilv1].contentbuffer);
			}
		}
	}
	for (int ilv2=1;ilv2<undosteps_count-1;ilv2++)
	{
		undosteps[ilv2]=undosteps[ilv2+1];
	}
	memory_areamoved((char*)&(undosteps[2]),(char*)&(undosteps[1]),sizeof(undosteps[0])*(undosteps_count-2));
	if (currentundostep>0)
	{
		currentundostep--;
	}
	undosteps_count--;
	for (int ilv2=1;ilv2<undosteps_count;ilv2++)
	{
		if (undosteps[ilv2].parent>=1) undosteps[ilv2].parent--;
	}
	return 1;
}
int storeundo(_u32 flags,const char * iname)
{
	if (undo_undodirty==0)
	{
		undo_undodirty=1;
		strncpy(undo_nextcommandname,iname,79);
		return 1;
	}
	if (undosteps_count==0) flags=~0;
	intl imax=0;
	basicmultilist * tl_multilist;
	if (undosteps_count>=constants_undostep_max)
	{
		if (slayredo()<=0)
		{
			if (slayundo()<=0)
			{
				return -1;
			}
		}
	}
	for (int ilv1=1;ilv1<sizeof(STRUCTURE_OBJECTTYPE_List)/sizeof(trienum);ilv1++)
	{
		tl_multilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
		if (tl_multilist)
		{
			if (flags & (1<<ilv1))
			{
				memory_alloc(&(undosteps[undosteps_count].handles[ilv1].buffer),2);
				memory_alloc(&(undosteps[undosteps_count].handles[ilv1].contentbuffer),3);
				imax=min(LHENDRAW_buffersize,glob_contentbuffer[ilv1].count+sizeof(intl)-1)/sizeof(intl);
				for (int ilv3=0;ilv3<imax;ilv3++)
				{
					((intl*)(undosteps[undosteps_count].handles[ilv1].contentbuffer))[ilv3]=((intl*)(glob_contentbuffer[ilv1].buffer))[ilv3];
				}
				for (int ilv2=0;ilv2<sizeof(basicmultilist);ilv2++)
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
			//set buffer_max here when not using LHENDRAW_buffersize
			#ifdef DEBUG
			undosteps[undosteps_count].handles[ilv1].bufferhead.buffer=NULL;
			#else
			undosteps[undosteps_count].handles[ilv1].bufferhead.buffer=undosteps[undosteps_count].handles[ilv1].contentbuffer;
			#endif
		}
	}
	undosteps[undosteps_count].parent=currentundostep;
	strncpy(undosteps[undosteps_count].commandname,undo_nextcommandname,79);
	strncpy(undo_nextcommandname,iname,79);
	currentundostep=undosteps_count;
//	printf("\e[31m%i,%i\e[0m\n",currentundostep,undosteps_count);
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
	intl thatundostep;//TODO: this variable is unnecessary
//	printf("\e[34m%i\e[0m\n",currentundostep);
	for (int ilv1=1;ilv1<sizeof(STRUCTURE_OBJECTTYPE_List)/sizeof(trienum);ilv1++)
	{
		tl_multilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
		if (tl_multilist)
		{
			if (flags & (1<<ilv1))
			{
				char * ipointer;
				thatundostep=currentundostep;
				if (orderflags & 1)
				{
					ipointer=(*tl_multilist).pointer;
					for (int ilv2=0;ilv2<sizeof(basicmultilist);ilv2++)
					{
						((char*)(tl_multilist))[ilv2]=((char*)(undosteps[thatundostep].handles[ilv1].imultilist))[ilv2];
					}
					(*tl_multilist).pointer=ipointer;
					goto count_only;
				}
				imax=min(LHENDRAW_buffersize,undosteps[currentundostep].handles[ilv1].bufferhead.count+sizeof(intl)-1)/sizeof(intl);
				tl_buffer=(intl*)(undosteps[currentundostep].handles[ilv1].contentbuffer);
				if (tl_buffer==NULL)
				{
					tl_buffer=(intl*)(undo_retrievecontentbuffer(currentundostep,ilv1)->buffer);
				}
				for (int ilv3=0;ilv3<imax;ilv3++)
				{
					((intl*)(glob_contentbuffer[ilv1].buffer))[ilv3]=tl_buffer[ilv3];
				}
				tl_buffer=(intl*)(undosteps[currentundostep].handles[ilv1].buffer);
				if (tl_buffer==NULL)
				{
					tl_buffer=(intl*)(undo_retrievebuffer(currentundostep,ilv1));
				}
				ipointer=(*tl_multilist).pointer;
				for (int ilv2=0;ilv2<sizeof(basicmultilist);ilv2++)
				{
					((char*)(tl_multilist))[ilv2]=((char*)(undosteps[thatundostep].handles[ilv1].imultilist))[ilv2];
				}
				(*tl_multilist).pointer=ipointer;
				imax=min(LHENDRAW_buffersize,(tl_multilist->itemsize*((basicmultilist*)(undosteps[thatundostep].handles[ilv1].imultilist))->filllevel+sizeof(intl)-1))/sizeof(intl);
				for (int ilv3=0;ilv3<imax;ilv3++)
				{
					((intl*)((*tl_multilist).pointer))[ilv3]=(tl_buffer)[ilv3];
				}
				count_only:;
				glob_contentbuffer[ilv1].count=undosteps[currentundostep].handles[ilv1].bufferhead.count;
			}
			//set buffer_max here when not using LHENDRAW_buffersize
			#ifdef DEBUG
			undosteps[currentundostep].handles[ilv1].bufferhead.buffer=NULL;
			#else
			undosteps[currentundostep].handles[ilv1].bufferhead.buffer=undosteps[currentundostep].handles[ilv1].contentbuffer;
			#endif
		}
	}
	undo_undodirty=0;
	return 1;
}
intl undo_memory_needs()
{
	intl needs=0;
	intl current=0;
	basicmultilist * tl_multilist;
	needs=constants_undostep_max*sizeof(undo_undostep_);//and if constants_undostep_max is increased, you must check if there is enough memory
	for (int ilv1=0;ilv1<undosteps_count;ilv1++)
	{
		for (int ilv2=1;ilv2<STRUCTURE_OBJECTTYPE_ListSize;ilv2++)
		{
			tl_multilist=((basicmultilist*)&(undosteps[ilv1].handles[ilv2].imultilist));
			current=(*tl_multilist).itemsize*(*tl_multilist).filllevel;
			if (needs<current) needs=current;
			current=(undosteps[ilv1].handles[ilv2].bufferhead.count);
			if (needs<current) needs=current;
		}
	}
	for (int ilv1=1;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
	{
		tl_multilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
		current=(*tl_multilist).itemsize*(*tl_multilist).filllevel;
		if (needs<current) needs=current;
		current=glob_contentbuffer[ilv1].count;
		if (needs<current) needs=current;
	}
/*	for (int ilv1=0;ilv1<memory_bufferstructure_count;ilv1++)
	{
		intl current=0;
		switch(memory_bufferstructure[ilv1].type)
		{
			case 0: 
			current=memory_bufferstructure_count*sizeof(memory_bufferstructure_);
			break;
			case 1: 
			//TODO urgent
			break;
			case 2: 
//			current=(basicmultilist*)//How the fuck can I sync them ???? _._ °n°
			//I guess, I better sync "backwards"
			break;
		}
		if (current>needs) needs=current;
	}*/
	return needs;
}
void printundostats()
{
	printf("%lli\n",glob_n_multilist->filllevel);
	for (int ilv1=0;ilv1<constants_undostep_max;ilv1++)
	{
		printf("O%s%2i,l%p=%p,b%p=%p:%lli\n",(ilv1==currentundostep)?"\e[32m:\e[0m":"\e[0m:",undosteps[ilv1].parent,undosteps[ilv1].handles[1].buffer,((basicmultilist*)(undosteps[ilv1].handles[1].imultilist))->pointer,undosteps[ilv1].handles[1].contentbuffer,undosteps[ilv1].handles[1].bufferhead.buffer,((basicmultilist*)(undosteps[ilv1].handles[1].imultilist))->filllevel);
	}
}
int undo_storcatch(_u32 flags,const char * iname)
{
	int backval=storeundo(flags,iname);
	if (setjmp(memory_catch_overflow)>=1)
	{
		restoreundo(~0,1);
		error_reset();
		getatoms();
	}
	return backval;
}
