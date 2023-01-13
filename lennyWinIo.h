HGLOBAL LHENDRAW_clipboardhandle;
typedef struct FILE
{
	_u8 exist=2;
	HANDLE W32handle;
	_uXX cursor;
	char * startposition;
        _iXX length=-1;//-1 means unknown
}FILE;
char * internaladdress=NULL;
#define W32_FILE_max 5
FILE W32_FILEHEAP[W32_FILE_max+4];
FILE * W32_FILE=W32_FILEHEAP+3;
FILE * stderr=W32_FILE-3;
FILE * stdout=W32_FILE-2;
FILE * stdin=W32_FILE-4;
FILE * win_internal=W32_FILE-1;
extern "C" char binary_gfx_buttons_bmp_start[];
extern "C" char binary_hotkeys_xml_start[];
extern "C" char binary_LiberationMono_Regular_bin_start[];
extern "C" char binary_LiberationMono_Regular_lennardfont_start[];
extern "C" char binary_gfx_buttons_bmp_end[];
extern "C" char binary_hotkeys_xml_end[];
extern "C" char binary_LiberationMono_Regular_bin_end[];
extern "C" char binary_LiberationMono_Regular_lennardfont_end[];
extern char * LHENDRAW_clipboardbuffer;
extern int LHENDRAW_clipboardbuffer_max;
int checkfilevalidity(FILE * ifile)
{
	for (int ilv1=-4;ilv1<W32_FILE_max;ilv1++)
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
extern "C" int binary_gfx_buttons_bmp_size;
FILE * fopen(const char * name,const char * mode)
{
	int readmode=0;
	int createmode=0;
	if ((name[0])==0)
	{
		if (strcmp(name+1,"clipboard")==0)
		{
			W32_FILE[-1].exist=1;
			W32_FILE[-1].length=LHENDRAW_clipboardbuffer_max;
			W32_FILE[-1].cursor=0;
			W32_FILE[-1].W32handle=(HANDLE)NULL;
			W32_FILE[-1].startposition=(char*)LHENDRAW_clipboardbuffer;
			return W32_FILE-1;
		}
		if (strcmp(name+1,"/gfx/buttons.bmp")==0)
		{
			W32_FILE[-1].exist=1;
			W32_FILE[-1].length=&binary_gfx_buttons_bmp_end[0]-&binary_gfx_buttons_bmp_start[0];
			W32_FILE[-1].cursor=0;
			W32_FILE[-1].W32handle=(HANDLE)NULL;
			W32_FILE[-1].startposition=(char*)binary_gfx_buttons_bmp_start;
			return W32_FILE-1;
		}
		if (strcmp(name+1,"/hotkeys.xml")==0)
		{
			W32_FILE[-1].exist=1;
			W32_FILE[-1].length=&binary_hotkeys_xml_end[0]-&binary_hotkeys_xml_start[0];
			W32_FILE[-1].cursor=0;
			W32_FILE[-1].W32handle=(HANDLE)NULL;
			W32_FILE[-1].startposition=(char*)binary_hotkeys_xml_start;
			return W32_FILE-1;
		}
		if (strcmp(name+1,"/LiberationMono-Regular.bin")==0)
		{
			W32_FILE[-1].exist=1;
			W32_FILE[-1].length=&binary_LiberationMono_Regular_bin_end[0]-&binary_LiberationMono_Regular_bin_start[0];
			W32_FILE[-1].cursor=0;
			W32_FILE[-1].W32handle=(HANDLE)NULL;
			W32_FILE[-1].startposition=(char*)binary_LiberationMono_Regular_bin_start;
			return W32_FILE-1;
		}
		if (strcmp(name+1,"/LiberationMono-Regular.lennardfont")==0)
		{
			W32_FILE[-1].exist=1;
			W32_FILE[-1].length=&binary_LiberationMono_Regular_lennardfont_start[0]-&binary_LiberationMono_Regular_lennardfont_end[0];
			W32_FILE[-1].cursor=0;
			W32_FILE[-1].W32handle=(HANDLE)NULL;
			W32_FILE[-1].startposition=(char*)binary_LiberationMono_Regular_lennardfont_start;
			return W32_FILE-1;
		}
		return NULL;
	}
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
	else if (strcmp(mode,"w+")==0)
	{
		createmode=CREATE_ALWAYS;
		readmode=GENERIC_WRITE|GENERIC_READ;
	}
	else
	{
		__asm__("int3\n");
		exit(2);//opening type not used yet
		return 0;
	}
	(*wertfile).W32handle=CreateFileA(name,readmode,1,NULL,createmode,FILE_ATTRIBUTE_NORMAL,NULL);
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
	ifile->exist=0;
	CloseHandle((*ifile).W32handle);
}
int feof(FILE * ifile)
{
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
	if (checkfilevalidity(ifile)<1) return 0;
	return 0;
}
void printf(const char * input,...)
{
	return;//TODO: we say nothing 
}
char * text_outtext=NULL;
int text_outtext_left=-1;
int text_outtext_right=-1;
int (*text_outtext_realloc)()=NULL;
FILE * text_outtext_file=NULL;
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
		text_outtext_left--;
	}
	*text_outtext=input;
	*(++text_outtext)=0;
	text_outtext_right++;
}
void __attribute__((__cdecl__)) text_plot_ramfile(_i8 input)
{
	if (text_outtext_left!=-1)
	{
		if (text_outtext_left==0)
		{
			if (text_outtext_realloc()>0) {text_outtext_left=text_outtext_file->length-text_outtext_file->cursor-text_outtext_right; goto could_realloc;}
			DESTRET(text_outtext_right);
		}
		could_realloc:;
		text_outtext_left--;
	}
	*text_outtext=input;
	*(++text_outtext)=0;
	text_outtext_right++;
}
void (*text_output)(_i8)=text_plot;
char text_nullstring[]="(NULL)";
int __attribute__((__cdecl__)) text_snprintf(const char ** inputpointer)
{
	int nprinted=0;
	int nread=0;
	const char * input=*inputpointer;
	inputpointer++;
	const char * input2;
	int nread2;
	char i_outputbuffer[25];
	char i_outputbufferpos;
	while(input[nread]!=0)
	{
		if (input[nread]=='%')
		{
			_u8 definemode=0;//spacereserved,precision,commagiven,fillwithzeroes
			int spacereserved=0;
			int precision=0;
			int tl_bytes=4;
			int basis=10;
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
			if (input[nread]=='.')
			{
				definemode|=4;
				goto iback;
			}
			if ((input[nread]>='0') && (input[nread]<='9'))
			{
				if (((definemode&7)==0) && (input[nread]=='0'))
				{
					definemode|=8;
				}
				if (definemode&4)
				{
					precision*=10;
					precision+=input[nread]-'0';
					definemode|=2;
				}
				else
				{
					spacereserved*=10;
					spacereserved+=input[nread]-'0';
					definemode|=1;
				}
				goto iback;
			}
			if (input[nread]=='*')
			{
				input2=*inputpointer;
				inputpointer++;
				if (definemode&4)
				{
					precision=(int)input2;
					definemode|=2;
				}
				else
				{
					spacereserved=(int)input2;
					definemode|=1;
				}
				goto iback;
			}
			if (tl_bytes>4) tl_bytes/=2;//because one l is always swallowed in both 32- and 64-bit mode
			if (input[nread]=='s')
			{
				input2=*inputpointer;
				inputpointer++;
				if (input2==NULL) {for (int ilv1=0;ilv1<strlen(text_nullstring);ilv1++) text_output(text_nullstring[ilv1]);goto idone;}
				nread2=0;
				while ((input2[nread2]!=0) and ((nread2<precision)||((definemode&4)==0)))
				{
					text_output(input2[nread2]);
					nread2++;
				}
				idone:;
			}
			else if (input[nread]=='c')
			{
				text_output(*(char*)inputpointer);
				inputpointer++;
			}
			else if ((input[nread]=='i') || (input[nread]=='u') || (input[nread]=='x') || (input[nread]=='X'))
			{
				if ((input[nread]=='x') || (input[nread]=='X'))
				{
					basis=16;
				}
				int ilv1;
				input2=(char*)inputpointer;
				inputpointer++;
				long long tl_zuverwoschtendes=0;
				for (ilv1=0;ilv1<tl_bytes;ilv1++)
				{
					(*(((char*)&tl_zuverwoschtendes)+ilv1))=(*(input2+ilv1));
				}
				if (input[nread]=='i')
				{
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
				}
				i_outputbufferpos=0;
				do
				{
					int zahl=(tl_zuverwoschtendes%basis);
					_u8 wert='0'+zahl;
					if (wert>'9') wert+=((input[nread]=='X')?'A':'a')-'9'-1;
					i_outputbuffer[i_outputbufferpos]=wert;
					i_outputbufferpos++;
					tl_zuverwoschtendes/=basis;
				} while (tl_zuverwoschtendes>0);
				for (int ilv1=spacereserved-1;ilv1>=i_outputbufferpos;ilv1--)
				{
					text_output((definemode&8)?'0':' ');
				}
				for (int ilv1=i_outputbufferpos-1;ilv1>=0;ilv1--)
				{
					text_output(i_outputbuffer[ilv1]);
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
			else if (input[nread]=='f')
			{
				if ((definemode&2)==0)
				{
					precision=10;
				}
				double tl_zuverwoschtendes=*(double*)inputpointer;
				inputpointer++;
				inputpointer++;
				if (tl_zuverwoschtendes<0)
				{
					tl_zuverwoschtendes=-tl_zuverwoschtendes;
					text_output('-');
				}
				int tl_integral=trunc(tl_zuverwoschtendes);
				double tl_integralf;
				double tl_fractional=modf(tl_zuverwoschtendes,&tl_integralf);
				i_outputbufferpos=0;
				do
				{
					int zahl=(tl_integral%10);
					i_outputbuffer[i_outputbufferpos]='0'+zahl;
					i_outputbufferpos++;
					tl_integral/=10;
				} while (tl_integral>0);
				for (int ilv1=i_outputbufferpos-1;ilv1>=0;ilv1--)
				{
					text_output(i_outputbuffer[ilv1]);
				}
				text_output('.');
				for (int ilv1=0;ilv1<precision;ilv1++)
				{
					tl_fractional*=10.0;
					text_output(((int)trunc(tl_fractional))+'0');
					tl_fractional=modf(tl_fractional,&tl_integralf);
				}
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
	text_outtext[0]=0;
	return text_snprintf(&input);
}
int __attribute__((__cdecl__)) sprintf(char * output,const char * input,...)
{
	text_output=text_plot;
	text_outtext=output;
	text_outtext_left=-1;
	text_outtext_right=0;
	text_outtext[0]=0;
	return text_snprintf(&input);
}
int vsnprintf(char * output,_uXX size,const char * &input,va_list ap)
{
	text_output=text_plot;
	text_outtext=output;
	text_outtext_left=-1;
	text_outtext_right=0;
	text_outtext[0]=0;
	int backval=text_snprintf(&input);
}
void __attribute__((__cdecl__)) text_plot_to_file(_i8 input)
{
	if (text_outtext_left!=-1)
	{
		if (text_outtext_left==0)
		{
			DESTRET(text_outtext_right);
		}
		text_outtext_left--;
	}
	_uXX num=0;
	WriteFile((*text_outtext_file).W32handle,&input,1,&num,NULL);
	(*text_outtext_file).cursor+=num;
	text_outtext_right++;
}
int fprintf(FILE * ifile,const char * input,...)
{
	if (ifile==stderr)
	{
		return 0;
	}
	if (checkfilevalidity(ifile)>0)
	{
		text_output=text_plot_to_file;
		text_outtext_file=ifile;
		text_outtext_left=-1;
		text_outtext_right=0;
		long unsigned int num=1;
		return text_snprintf(&input);
	}
	else
	{
		if (checkfilevalidity(ifile)==-1)
		{
			text_output=text_plot_ramfile;
			text_outtext=ifile->startposition+ifile->cursor;
			text_outtext_left=ifile->length-ifile->cursor;//TODO
			text_outtext_file=ifile;
			text_outtext_right=0;
			text_snprintf(&input);
			ifile->cursor+=text_outtext_right;
			return text_outtext_right;
		}
	}
	__asm__("int3\n");
	exit(2);
}
extern "C" _uXX fwrite(const void * buffer,int blocksize,int blockcount,FILE * ifile)
{
	long unsigned int num=0;
	if (ifile==(W32_FILE-1))
	{
		int num=blocksize*blockcount;
		iback:;
		if ((*ifile).length!=-1)
		{
			if ((*ifile).length-(*ifile).cursor<num)
			{
				text_outtext_realloc();
				goto iback;//You can never know if it is sufficient now, what if it is still not sufficient for all bytes?
				num=(*ifile).length-(*ifile).cursor;
			}
		}
		memcpy((*ifile).startposition+(*ifile).cursor,buffer,num);
		(*ifile).cursor+=num;
		return num;
	}
	if (checkfilevalidity(ifile)<1) return 0;
	WriteFile((*ifile).W32handle,buffer,blocksize*blockcount,&num,NULL);
	(*ifile).cursor+=num;
	return num;
}
_uXX fread(void * buffer,int blocksize,int blockcount,FILE * ifile)
{
	long unsigned int num=0;
	if (ifile==(W32_FILE-1))
	{
		int num=blocksize*blockcount;
		if ((*ifile).length!=-1)
		{
			if ((*ifile).length-(*ifile).cursor<num)
			{
				num=(*ifile).length-(*ifile).cursor;
			}
		}
		memcpy(buffer,(*ifile).startposition+(*ifile).cursor,num);
		(*ifile).cursor+=num;
		return num;
	}
	if (checkfilevalidity(ifile)<1) return 0;
	ReadFile((*ifile).W32handle,buffer,blocksize*blockcount,&num,NULL);
	(*ifile).cursor+=num;
	return num;
}

int fseek(FILE * ifile,int offset,int TYPE)
{
	int relationpoint=0;
	switch (TYPE)
	{
		case SEEK_SET: relationpoint=0;(*ifile).cursor=offset;break;
		case SEEK_CUR: relationpoint=1;(*ifile).cursor+=offset;break;
		case SEEK_END: relationpoint=2;if ((*ifile).length==-1) exit(2);(*ifile).cursor=(*ifile).length+offset;break;
	}
	if (ifile>=W32_FILE)
	{
		SetFilePointer((*ifile).W32handle,(*ifile).cursor,NULL,0);
	}
}
_uXX ftell(FILE * ifile)
{
	if (ifile==(W32_FILE-1))
	{
		return((*ifile).cursor);
	}
	if (checkfilevalidity(ifile)<1) exit(2);
	(*ifile).cursor=SetFilePointer((*ifile).W32handle,0,NULL,1);
	return (*ifile).cursor;
}
#ifdef FULLCROSS
extern "C" int fputc(_u32 i_c,FILE*i_file)
{
	long unsigned int num=0;
	WriteFile((*i_file).W32handle,&i_c,1,&num,NULL);
	return num;
}
extern "C" int fputs(const char*i_s,FILE*i_file)
{
	long unsigned int num=0;
	WriteFile((*i_file).W32handle,i_s,1,&num,NULL);
	return num;
}
extern "C" _u32 __assert_fail()
{
	return 0;
}
extern "C" _u32 __stack_chk_fail_local()
{
	return 0;
}
extern "C" _u32 __sprintf_chk()
{
	return 0;
}
extern "C" _u32 __libc_start_main()
{
	return 0;
}
extern "C" _u32 __libc_csu_init()
{
	return 0;
}
extern "C" _u32 __libc_csu_fini()
{
	return 0;
}
extern "C" _u32 dl_iterate_phdr()
{
	return 0;
}
extern "C" _u32 write()
{
	return 0;
}
char W32_path_name_extended_with_asterisk[1024+2];
extern "C" DIR*opendir(const char*i_name)
{
	snprintf(W32_path_name_extended_with_asterisk,1024+2,"%s/*",i_name);
	W32_DIR_instance.windowshandle=FindFirstFileA(W32_path_name_extended_with_asterisk,&(W32_DIR_instance.windowsstruct));
	W32_DIR_instance.linuxstruct.d_ino=0;
	if (W32_DIR_instance.windowshandle!=0xFFFFFFFF)
	{
		W32_DIR_instance.fsm=1;
	}
	else
	{
		W32_DIR_instance.fsm=3;
	}
	return &W32_DIR_instance;
}
extern "C" dirent*readdir(DIR*i_DIR)
{
	if ((*i_DIR).fsm==3)
	{
		return NULL;
	}
	if ((*i_DIR).fsm==2)
	{
		if (FindNextFileA((*i_DIR).windowshandle,&(*i_DIR).windowsstruct))
		{
			goto l_found;
		}
		return NULL;
	}
	if ((*i_DIR).fsm==1)
	{
		(*i_DIR).fsm=2;
		l_found:;
		(*i_DIR).linuxstruct.d_ino++;
		(*i_DIR).linuxstruct.d_name=(*i_DIR).windowsstruct.cFileName;
		(*i_DIR).linuxstruct.d_namelen=strlen((*i_DIR).windowsstruct.cFileName);
		return &(*i_DIR).linuxstruct;
	}
	if ((*i_DIR).fsm==0)
	{
		WriteConsoleA(GetStdHandle(-11),"Directory not opened",20,0,0);
		return NULL;
	}
}
extern "C" void closedir(DIR*i_DIR)
{
	FindClose((*i_DIR).windowshandle);
}
char**W32_argpointer;
int W32_argpointer_count;
int main(int argc,char**argv);
void W32_strshorten(char*o_char)
{
	_u32 l_strlen=strlen(o_char);
	for (_u32 tl_lv1=0;tl_lv1<l_strlen;tl_lv1++)
	{
		o_char[tl_lv1]=o_char[tl_lv1+1];
	}
}
extern "C" _u32 W32_constants_sweet_escape=1381322579;
extern "C" _u32 W32_main()
{
	__asm__("\n\
	pushl	%ebp\n\
	movl	%esp, %ebp\n\
	subl	$24, %esp\n\
	movl	$65535, 4(%esp)\n\
	movl	$1, (%esp)\n\
	call	_Z41__static_initialization_and_destruction_0ii\n\
	leave\n\
	");
	char*l_commandline=GetCommandLineA();
	WriteConsoleA(GetStdHandle(-11),l_commandline,strlen(l_commandline),0,0);
	_u32 l_length=strlen(l_commandline);
	_u32 W32_argpointer_count=0;
	_u32 W32_argpointer_index=0;
	for (_u32 pass=0;pass<=1;pass++)
	{
		_u32 tl_fsm=0;
		if (pass==1)
		{
			W32_argpointer_count=W32_argpointer_index+1;
			W32_argpointer=(char**)malloc(W32_argpointer_count*4);
			tl_fsm=2;
		}
		W32_argpointer_index=0;
		for (_u32 tl_lv1=0;tl_lv1<l_length;tl_lv1++)
		{
			if (tl_fsm==2)
			{
				tl_fsm=0;
				if (pass==1)
				{
					W32_argpointer[W32_argpointer_index]=l_commandline+tl_lv1;
				}
			}
			if (l_commandline[tl_lv1]=='"')
			{
				if (tl_fsm!=1)
				{
					tl_fsm=1;
				}
				else
				{
					tl_fsm=0;
				}
			}
			else
			{
				if (tl_fsm==1)
				{
				}
				else if (tl_fsm==2)
				{
					tl_fsm=0;
				}
				else
				{
					if (l_commandline[tl_lv1]==' ')
					{
						W32_argpointer_index++;
						tl_fsm=2;
						if (pass==1)
						{
							l_commandline[tl_lv1]=0;
						}
					}
				}
			}
		}
	}
	for (_u32 tl_lv1=0;tl_lv1<W32_argpointer_count;tl_lv1++)
	{
		if (W32_constants_sweet_escape==1413830483)
		{
			_u32 tl_fsm=0;
			_u32 tl_lv2=0;
			_u32 old_strlen=strlen(W32_argpointer[tl_lv1]);
			while ((W32_argpointer[tl_lv1][tl_lv2]!=0)&&(tl_lv2<old_strlen))
			{
				if (tl_fsm==0)
				{
					if (W32_argpointer[tl_lv1][tl_lv2]=='"')
					{
						W32_strshorten(W32_argpointer[tl_lv1]+tl_lv2);
						continue;
					}
					else if (W32_argpointer[tl_lv1][tl_lv2]=='/')
					{
						W32_strshorten(W32_argpointer[tl_lv1]+tl_lv2);
						tl_fsm=1;
						continue;
					}
				}
				else if (tl_fsm==1)
				{
					tl_fsm=0;
				}
				tl_lv2++;
			}
		}
		else
		{
			if (W32_argpointer[tl_lv1][0]=='"')
			{
				W32_argpointer[tl_lv1]++;
			}
			if (strlen(W32_argpointer[tl_lv1])>0)
			{
				if (W32_argpointer[tl_lv1][strlen(W32_argpointer[tl_lv1])-1]=='"')
				{
					W32_argpointer[tl_lv1][strlen(W32_argpointer[tl_lv1])-1]=0;
				}
			}
		}
	}
	return main(W32_argpointer_count,W32_argpointer);
}
#endif
