#define FILE int
int win_stderr=-2;
int win_stdout=-1;
int win_stdin=-3;
#define stderr &win_stderr
#define stdout &win_stdout
#define stdin &win_stdin
FILE * fopen(const char * name,const char * mode)
{
}
void fclose(FILE * ifile)
{
}
int feof(FILE * ifile)
{
	return 0;
}
void printf(const char * input,...)
{
}
void sprintf(char * output,const char * input,...)
{
}
void snprintf(char * output,_uXX size,const char * input,...)
{
}
void vsnprintf(char * output,_uXX size,const char * input,va_list ap)
{
}

void fprintf(FILE * ifile,const char * input,...)
{
}
void fwrite(const void * buffer,int blocksize,int blockcount,FILE * ifile)
{
}
int fread(void * buffer,int blocksize,int blockcount,FILE * ifile)
{
}

int fseek(FILE * ifile,int TYPE,int offset)
{
}
int ftell(FILE * ifile)
{
}