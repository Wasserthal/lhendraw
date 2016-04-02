jmp_buf debug_crashhandler;
#ifdef DEBUG
#include <mcheck.h>
void debug_abortfunc(mcheck_status istatus)
{
	printf("Mcheck failed!\n");exit(1);
}
#endif
#ifdef DEBUG
#define print(...) printf(__VA_ARGS__)
#else
void print(...)
{
}
#endif
#define error(...) {fprintf(stderr,__VA_ARGS__);fprintf(stderr,"\n");exit(1);};
#define error2(MACRO_NUMBER,...) {fprintf(stderr,__VA_ARGS__);exit(MACRO_NUMBER);};
#ifdef DEBUG
const char *progname=NULL;
extern int control_GUI;
extern char control_filenamehead[256];
void Signal(int signum)
{
	if (control_GUI)
	{
		char localbuf[512+1];
		snprintf(localbuf,512,"gdb %s %d",progname,getpid());
		system(localbuf);
	}
	else
	fprintf(stderr,"FILE crashes:%s\n",control_filenamehead);exit(1);
}
#endif
