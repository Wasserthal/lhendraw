void ps_controlprocedure(bool irestriction,char clickcollisioncheck);
#ifndef NOPRINT
#ifndef CROFTOIDAL
#include <cups/cups.h>
int print_start_job(const char * destname,int partly)
{
	int job_id;
	job_id=cupsCreateJob(CUPS_HTTP_DEFAULT,destname,"Lhendraw",0,NULL);
	int backval1=cupsStartDocument(CUPS_HTTP_DEFAULT,destname,job_id,"lhendraw",CUPS_FORMAT_POSTSCRIPT,1);
	edit_fileoperationrefersonlytopartofdocument=partly;
	ps_newmatrix(0);
	ps_controlprocedure(0,0);
	edit_fileoperationrefersonlytopartofdocument=0;
	int backval3=cupsFinishDocument(CUPS_HTTP_DEFAULT,destname);
	return job_id;
}
const char * printer_default_name()
{
	cups_dest_t *dests;
	int num_dests = cupsGetDests(&dests);
	for (int ilv1=0;ilv1<num_dests;ilv1++)
	{
		if (dests[ilv1].is_default)
		{
			return dests[ilv1].name;
		}
	}
	return "";
}
void print_print(const char * input)
{
	cupsWriteRequestData(CUPS_HTTP_DEFAULT,input,strlen(input));
}
#else
int print_start_job(const char * destname,int partly)
{
	memset(&W32_pd,0,sizeof(PRINTDLG));
	W32_pd.lStructSize=sizeof(PRINTDLG);
	W32_pd.hwndOwner=NULL;
	W32_pd.Flags=PD_RETURNDC;
	if (PrintDlgA(&W32_pd))
	{
		DOCINFOA di;
		memset(&di,0,sizeof(DOCINFOA));
		di.cbSize=sizeof(DOCINFOA);
		di.lpszDocName="lhendraw";
		ps_sizex=GetDeviceCaps(W32_pd.hDC,HORZRES);
		ps_sizey=GetDeviceCaps(W32_pd.hDC,VERTRES);
		StartDocA(W32_pd.hDC,&di);
		StartPage(W32_pd.hDC);
		ps_printmode=1;
		edit_fileoperationrefersonlytopartofdocument=partly;
		ps_newmatrix(1);
		ps_controlprocedure(0,0);
		ps_printmode=0;
		edit_fileoperationrefersonlytopartofdocument=0;
		EndPage(W32_pd.hDC);
		EndDoc(W32_pd.hDC);
	}
}
void print_print(const char * input)
{
	if (input!=ps_buffer)
	{
		MessageBoxA(NULL,"Windows-Programmierfehler 1\nFalscher Postscript-Puffer benutzt","__1__",0);
		exit(1);
	}
	*((_u16*)ps_overbuffer)=strlen(input);
	ExtEscape(W32_pd.hDC,POSTSCRIPT_PASSTHROUGH,strlen(input)+2,ps_overbuffer,0,NULL);
}
const char * printer_default_name()
{
	_u32 default_printer_name_size=256;
	GetDefaultPrinterA((char*)W32_default_printer_name,&default_printer_name_size);
	W32_default_printer_name[sizeof(W32_default_printer_name)-1]=0;
	return (char*)W32_default_printer_name;
}
#endif
#else
int print_start_job(char * destname,partly)
{
	char control_totalfilename[stringlength+1];
	sprintf(control_totalfilename,"mkdir -p %s/.clipboard",getenv("HOME"));
	system(control_totalfilename);
	edit_fileoperationrefersonlytopartofdocument=partly;
	edit_file_always_overwrite=1;
	ps_printmode=0;
	ps_buffer[0]=0;
	sprintf(control_totalfilename,"%s/.clipboard/clipboard.png",getenv("HOME"));
	SAVE_TYPE(control_totalfilename,".png");
	edit_fileoperationrefersonlytopartofdocument=0;
	edit_file_always_overwrite=0;
	LHENDRAW_clipboardmode=0;
	sprintf(control_totalfilename,"lp %s/.clipboard/clipboard.png",getenv("HOME"));
	system(control_totalfilename);
	return 1;//printed
}
const char * printer_default_name()
{
	return "";
}
void print_print(const char * input)//DUMMY, may stay empty
{}
#endif
catalogized_command_funcdef(PRINT)
{
	ps_printmode=1;
	ps_buffer[0]=0;
	if (value==NULL) goto leerer_parameter;
	if (strcmp(value,"0")==0) goto leerer_parameter;
	if (strcmp(value,"")==0)
	{
		leerer_parameter:;
		print_start_job(printer_default_name(),atoi(parameter)^1);
	}
	else
	{
		print_start_job(value,atoi(parameter)^1);
	}
	return 1;
}
catalogized_command_funcdef(PORTRAIT)
{
	return 1;
}
