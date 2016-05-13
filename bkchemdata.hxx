//This defines BKCHEM-Typical data structures
#define constants_bkchemids_max 10000
bienum bkchemids[constants_bkchemids_max];//TODO: make size dynamic;
int bkchemids_count=0;
typedef _i32 bkchem_id ;
int __attribute__((sysv_abi))CDXMLWRITE_bkchem_id(char * input,void * output)
{
	fprintf((FILE*)output,"atom%i",*(_i32*)input);
	return 0;
}
int __attribute__((sysv_abi))CDXMLREAD_bkchem_id(char * input,void * output)
{
	for (int ilv1=0;ilv1<bkchemids_count;ilv1++)
	{
		if (strcmp(input,bkchemids[ilv1].name)==0)
		{
			*(_i32*)output=bkchemids[ilv1].number;
			return 0;
		}
	}
	if (bkchemids_count<constants_bkchemids_max)
	{
		bkchemids[bkchemids_count].number=bkchemids_count;
		strncpy(bkchemids[bkchemids_count].name,input,39);
		*(_i32*)output=bkchemids[bkchemids_count].number;
		printf(">%i\n",*(_i32*)output);
		bkchemids_count++;
		return 0;
	}
	else
	{
		error("bkchem numbers overflow");
	}
}
int __attribute__((sysv_abi))CDXMLWRITE_BIN_bkchem_id(char * input,void * output)
{
	error("what is a BINARY cdml file that you want to write????");
}
int __attribute__((sysv_abi))CDXMLREAD_BIN_bkchem_id(char * input,void * output)
{
	error("what is a BINARY cdml file that you want to read????");
}
void bkchem_undiscriminate()
{
	for (int ilv1=0;ilv1<multilist_count;ilv1++)
	{
		if (multilistlist[ilv1].instance->pointer==NULL)
		{
			basicmultilist * tl_multilist=multilistlist[ilv1].instance;
			memory_alloc(&(tl_multilist->pointer),2);
			memory_alloc((char**)&(tl_multilist->dependants),1);
		}
	}
}
