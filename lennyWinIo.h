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
char * text_outtext=NULL;
int text_outtext_left=-1;
int text_outtext_right=-1;
#define DESTRET(BACKVAL) \
{\
	__asm__ __volatile__ ("LEAVE\npop %%ebx\nret\n": :"a"(BACKVAL):"ebx");\
	return;\
}
void __attribute__((__cdecl__)) text_plot(_i8 input)
{
	if (text_outtext_left!=-1)
	{
		if (text_outtext_left==0)
		{
			DESTRET(text_outtext_right);
		}
	}
	*text_outtext=input;
	*(++text_outtext)=0;
	text_outtext_right++;
}
void (*text_output)(_i8)=text_plot;
int __attribute__((__cdecl__)) text_snprintf(const char ** inputpointer)
{
	int nprinted=0;
	int nread=0;
	const char * input=*inputpointer;
	inputpointer++;
	const char * input2;
	int nread2;
	while(input[nread]!=0)
	{
		if (input[nread]=='%')
		{
			int tl_bytes=4;
			iback:
			nread++;
			if (input[nread]=='h')
			{
				tl_bytes/=2;
				goto iback;
			}
			if (input[nread]=='l')
			{
				tl_bytes*=2;
				goto iback;
			}
			if (input[nread]=='s')
			{
				goto skip;
				input2=*inputpointer;
				inputpointer++;
				nread2=0;
				while ((input2[nread2]!=0) and (nread2<2))
				{
					text_output(input2[nread2]);
					nread2++;
				}
				nread++;
			}
			else if ((input[nread]=='i') || (input[nread]=='u') || (input[nread]=='x') || (input[nread]=='X'))
			{
				skip:;
				int ilv1;
				input2=(char*)inputpointer;
				inputpointer++;
				long long tl_zuverwoschtendes=0;
				for (ilv1=0;ilv1<tl_bytes;ilv1++)
				{
					(*(((char*)&tl_zuverwoschtendes)+ilv1))=(*(input2+ilv1));
				}
				if ((*(((char*)&tl_zuverwoschtendes)+ilv1-1))&0x80)
				{
					for (ilv1=tl_bytes;ilv1<sizeof(tl_zuverwoschtendes);ilv1++)
					{
						(*(((char*)&tl_zuverwoschtendes)+ilv1))=0xFF;
					}
				}
				if (tl_zuverwoschtendes<0)
				{
					tl_zuverwoschtendes=-tl_zuverwoschtendes;
					text_output('-');
				}
				while (tl_zuverwoschtendes>0)
				{
					int zahl=(tl_zuverwoschtendes%10);
					text_output('0'+zahl);
					tl_zuverwoschtendes/=10;
				}
			}
			else if (input[nread]=='n')//here comes the Endgegner
			{
				(**(int**)inputpointer)=text_outtext_right;
				inputpointer++;
			}
			else if (input[nread]=='%')
			{
				text_output('%');
			}
			else
			{
				inputpointer++;
			}
		}
		else
		{
			text_output(input[nread]);
		}
		nread++;
	}
	return text_outtext_right;
}
int __attribute__((__cdecl__)) snprintf(char * output,_uXX size,const char * input,...)
{
	text_output=text_plot;
	text_outtext=output;
	text_outtext_left=size;
	text_outtext_right=0;
	return text_snprintf(&input);
}
int __attribute__((__cdecl__)) sprintf(char * output,const char * input,...)
{
	text_output=text_plot;
	text_outtext=output;
	text_outtext_left=-1;
	text_outtext_right=0;
	return text_snprintf(&input);
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
	(*ifile).cursor+=num;
	return num;
}
_uXX fread(void * buffer,int blocksize,int blockcount,FILE * ifile)
{
	long unsigned int num=0;
	if (checkfilevalidity(ifile)<1) return 0;
	ReadFile((*ifile).W32handle,buffer,blocksize*blockcount,&num,NULL);
	(*ifile).cursor+=num;
	return num;
}

int fseek(FILE * ifile,int TYPE,int offset)
{
	int relationpoint=0;
	switch (TYPE)
	{
		case SEEK_SET: relationpoint=0;(*ifile).cursor=offset;break;
		case SEEK_CUR: relationpoint=1;(*ifile).cursor=-offset;break;
		case SEEK_END: relationpoint=2;if ((*ifile).length==-1) exit(2);(*ifile).cursor=(*ifile).length+offset;break;
	}
	SetFilePointer((*ifile).W32handle,(*ifile).cursor,NULL,0);
}
_uXX ftell(FILE * ifile)
{
	if (checkfilevalidity(ifile)<1) exit(2);
	(*ifile).cursor=SetFilePointer((*ifile).W32handle,0,NULL,1);
	return (*ifile).cursor;
}
