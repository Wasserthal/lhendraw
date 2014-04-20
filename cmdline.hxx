char argmeanings[1000];
char parametersthatwantfilename[]={"oslcn"};
char parametersexecutedimmediately[]={"Io"};
char * parameter_filename;
char * parameter_filetype;
int * parameter_argc;
char ** parameter_argv;
char parameter_cmd1[stringsize];
char parameter_cmd2[stringsize];
int executeparameter(char which,int parameter,int posinparameter)
{
	switch(which)
	{
		case 'I' :      control_GUI=0; control_.interactive=0;control_.force=1;break;
		case 'f' :      control_.force|=1;break;
		case 'F' :      control_.force|=0;break;
		case 'o' :      strncpy(control_.filename,getparameter(parameter,posinparameter,parameter_filetype),stringlength-1);control_filename[stringlength-1]=0;strncpy(control_filetype,parameter_filetype,stringlength-1);control_filetype[stringlength-1]=0;control_.saveuponexit=1;break;
		case 'f' :      strncpy(control_.filename,getparameter(parameter,posinparameter,parameter_filetype),stringlength-1);control_filename[stringlength-1]=0;strncpy(control_filetype,parameter_filetype,stringlength-1);control_filetype[stringlength-1]=0;break;
		case 'w' :      parameter_filename=getparameter(parameter,posinparameteri,control_.filetype);SAVEAS(parameter_filename,parameter_filetype);break;
		case 'l' :      parameter_filename=getparameter(parameter,posinparameteri,control_.filetype);LOADAS(parameter_filename,parameter_filetype);break;
		case '-' :      if ((argv[parameter][posinparameter])!=0)
				{
				//TODO: add your multi-symbol commands here
				}//or it was a --, so not interpreted.
				break;
		case 'c' :      for (int ilv1=0;ilv1<REFLECTION_FUNCTION_LISTSIZE;ilv1++)
				{
					if (strcmp(REFLECTION_FUNCTION_LIST[ilv1].command,getparameter(parameter,posinparameter,parameter_cmd1,parameter_cmd2)==0)
					{
						REFLECTION_FUNCTION_LISTSIZE.function(parameter_cmd1,parameter_cmd2);
					}
				}
				break;
		case 'N' :      NEW("","");break;
		case 'h' :      printf(
"Usage:\n"
"lhendraw [-I] [-o output] [-<parameter> <parameter_filename>] (...) <filename> (...)\n"
"filenames and parameter_filename combinations may be mixed\n"
"the \"output\" file is always saved when the program exits\n"
"Its name is stored in the same register as the \"current\"\n"
"The -I command prevents loading the graphical user interface, making the program all command-line type.\n"
"All filenames stored outside the registers are read in additively, merging those documents\n"
"All other parameters are processed AFTERWARDS(sic)\n"
"\n"
"Parameters:\n"
"-I      : No gui. implies f\n"
"-o<fn>  : specify output-filename/filetype. Will be saved at end. Is stored as the current document's filename\n"
"-f      : run non-interactively\n"
"-F      : Run interactively. overrides the f of -I\n"
"Commands, processed in order:\n"
"-w<fn>  : saves to filename. Set no current filename\n"
"-l<fn>  : loads. Set no current filename\n"
"-e<fn>  : loads. Set current filename, overrides -o's filename\n"
"-q<fn>  : exits. saves if -o specified\n"
"-c<cmd>(<prm>,<prm>) : executes command in the form COMMAND(valuea,valueb)\n"
"     Example: -cSAVEAS(SUCKER.xml,.cdxml)\n"
"     saves the file as the name SUCKER.xml, but with the filetype .cdxml\n"
"--      : Interpret the following parameters as filenames, to be additively loaded at start\n"
"-N<fn>  : Un-loads the current file and un-sets the current filename, just as New File. Defeats -o.\n"
"-n<fn>  : searches, in the specified file, for the chemical pattern which is loaded before\n"
"          files with matches are printed on stdout. One match per file.\n"
"--search: searches, like -n, but in all follwing files. All following parameters are treated as input files.\n"
"\n"
"Example:\n"
"lhendraw -I -o.cdx shirt.cdx super.cdxml duper.cdx -ccheckupinconsistencies() -w.svg shirt.txt -cSELECT(true) -cROTATE(z,-90)\n"
"Superimposes super.cdxml and duper.cdx. concatenates all overlapping atoms.\n Saves a picture of the current state, as svg, into shirt.txt.\n Spins the result CCW at 90° and saves the spun result as shirt.cdx\n"
"lhendraw -I pattern.cdx --search FUJI0001.cdx FUJI0002.cdx FUJI0003.cdx FUJI0004.cdx FUJI0005.cdx FUJI0006.cdx LÖNNEBERG0001.cdxml LÖNNEBERG0002.cdxml\n"
"Searches the files FUJI.... and LÖNNE.... for the pattern in pattern.cdxml, then exits\n"
				);
				break;
	}
}

void cmdline(int argc,char ** argv)
{
	parameter_argc=argc;parameter_argv=argv;
	char wfn=0;//parameter wants filename
	char toomanyarguments=0;
	if (argc>1000) {toomanyarguments=1;argc=1000;}
	for (int ilv1=1;ilv1<argc;ilv1++)
	{
		argmeanings[ilv1]=0;
		if (wfn)
		{
			argmeanings[ilv1]=0;
			wfn&=2;
		}
		else
		{
			if (argv[ilv1][0]=='-')
			{
				argmeanings[ilv1]=1;
				if (argv[ilv1][1]=='-')
				{
					if (argv[ilv1][2]==0)
					{
						wfn=2;//want only filenames, forever
					}
					//TODO: add your multi-symbol commands here.
				}
				for (int ilv2=1;argv[ilv1][ilv2]!=0;ilv2++)
				{
					if (strchr(argv[ilv1][ilv2],parametersthatwantfilename))
					{
						ilv2++;
						if (argv[ilv1][ilv2]=='.')
						{
							wfn=1;
							for (;argv[ilv1][ilv2]!=0;ilv2++)//resumed-loop
							{
								if ((argv[ilv1][ilv2]=='=') && (argv[ilv1][ilv2+1]!=0))
								{
									wfn=0;
								}
							}
						}
						else
						{
							wfn=0;
						}
					}
					if (strchr(argv[ilv1][ilv2],parametersexecutedimmediately))
					{
						executeparameter(argv[ilv1][ilv2],ilv1,ilv2);
					}
					goto ifertig;
				}
				ifertig:;
			}
			else
			{
				argmeanings=2;//Filename, to be read at start
			}
		}
	}
	if (toomanyarguments)
	{
		if (control_force & 1)
		{
		}
		else
		{
			fprintf(stderr,"too many arguments");exit(10);
		}
	}
	for (int ilv1=1;ilv1<argc;ilv1++)
	{
		if (argmeanings[ilv1]==2)
		{
			LOADAS(argv[ilv1],"");
		}
	}
}
