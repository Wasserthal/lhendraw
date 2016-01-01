#ifndef NODEBUG
#define print(...) printf(__VA_ARGS__)
#else
void print(...)
{
}
#endif
#define error(...) {fprintf(stderr,__VA_ARGS__);fprintf(stderr,"\n");exit(1);};
#define error2(MACRO_NUMBER,...) {fprintf(stderr,__VA_ARGS__);exit(MACRO_NUMBER);};
#ifndef NODEBUG
const char *progname=NULL;
void Signal(int signum)
{
	if (control_GUI)
	{
		char localbuf[512+1];
		snprintf(localbuf,512,"gdb %s %d",progname,getpid());
		system(localbuf);
	}
	else
	fprintf(stderr,"FILE crashes:%s\n",control_filename);exit(1);
}
#endif
