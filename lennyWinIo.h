#define stderr win_stderr
#define stdout win_stdout
#define stdin win_stdin
typedef struct FILE
{
	_u8 exist=2;
	HANDLE W32handle;
	_uXX cursor;
        _iXX length=-1;//-1 means unknown
}FILE;
#define W32_FILE_max 5
FILE W32_FILEHEAP[W32_FILE_max+3];
FILE * W32_FILE=W32_FILEHEAP+3;
FILE * win_stderr=W32_FILE-2;
FILE * win_stdout=W32_FILE-1;
FILE * win_stdin=W32_FILE-3;
int checkfilevalidity(FILE * ifile)
{
	for (int ilv1=-3;ilv1<W32_FILE_max;ilv1++)
	{
		if (ifile==(W32_FILE+ilv1))
		{
			if (ilv1<0) return ilv1;
			if ((*ifile).W32handle==NULL) return 0;
			return 1;
		}
	}
}
FILE * W32_summonfile()
{
	for (int ilv1=0;ilv1<W32_FILE_max;ilv1++)
	{
		if (W32_FILE[ilv1].exist!=1)
		{
			W32_FILE[ilv1].exist=1;
			W32_FILE[ilv1].length=-1;
			W32_FILE[ilv1].cursor=0;
			W32_FILE[ilv1].W32handle=(HANDLE)NULL;
			return W32_FILE+ilv1;
		}
	}
}
FILE * fopen(const char * name,const char * mode)
{
	int readmode=0;
	int createmode=0;
	FILE * wertfile=W32_summonfile();
	if (wertfile==NULL) return NULL;
	if (strcmp(mode,"r")==0)
	{
		createmode=OPEN_EXISTING;
		readmode=GENERIC_READ;
	}
	else if (strcmp(mode,"w")==0)
	{
		createmode=CREATE_ALWAYS;
		readmode=GENERIC_WRITE;
	}
	else
	{
		__asm__("int3\n");
		exit(2);//opening type not used yet
		return 0;
	}
	(*wertfile).W32handle=CreateFile(name,readmode,1,NULL,createmode,FILE_ATTRIBUTE_NORMAL,NULL);
	if ((*wertfile).W32handle==INVALID_HANDLE_VALUE)
	{
		(*wertfile).exist=0;
		return NULL;
	}
	return wertfile;
}
void fclose(FILE * ifile)
{
	if (checkfilevalidity(ifile)<1) return;
	CloseHandle((*ifile).W32handle);
}
int feof(FILE * ifile)
{
	if (checkfilevalidity(ifile)<1) return 0;
	if (((*ifile).cursor>=0) && ((*ifile).length>=0))
	{
		if ((*ifile).cursor>=(*ifile).length)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	return 0;
}
void printf(const char * input,...)
{
	return;//TODO: we say nothing 
}
void snprintf(char * output,_uXX size,const char * input,...)
{
	int nprinted=0;
}
void sprintf(char * output,const char * input,...)
{
}
void vsnprintf(char * output,_uXX size,const char * input,va_list ap)
{
	int nprinted=0;
}

int fprintf(FILE * ifile,const char * input,...)
{
	if (ifile==stderr)
	{
		return 0;
	}
	if (checkfilevalidity>0)
	{
		__asm__("int3\n");
		exit(2);
	}
		__asm__("int3\n");
	exit(2);
}
_uXX fwrite(const void * buffer,int blocksize,int blockcount,FILE * ifile)
{
	long unsigned int num=0;
	if (checkfilevalidity(ifile)<1) return 0;
	WriteFile((*ifile).W32handle,buffer,blocksize*blockcount,&num,NULL);
	return num;
}
_uXX fread(void * buffer,int blocksize,int blockcount,FILE * ifile)
{
	long unsigned int num=0;
	if (checkfilevalidity(ifile)<1) return 0;
	ReadFile((*ifile).W32handle,buffer,blocksize*blockcount,&num,NULL);
	return num;
}

int fseek(FILE * ifile,int TYPE,int offset)
{
}
_uXX ftell(FILE * ifile)
{
	if (checkfilevalidity(ifile)<1) exit(2);
	return (*ifile).cursor;
}
