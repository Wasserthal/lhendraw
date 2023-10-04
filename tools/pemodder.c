#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#define _u8 unsigned char
#define _u16 unsigned short
#define _u32 unsigned int
typedef struct
{
	_u32 idx;
	char*name;
	char*shortname;
}SECTIONFLAGS_;
SECTIONFLAGS_ SECTIONFLAGS[]={
{5,"IMAGE_SCN_CNT_CODE","Code"},
{6,"IMAGE_SCN_CNT_INITIALIZED_DATA","Idata"},
{7,"IMAGE_SCN_CNT_UNINITIALIZED_DATA","Udata"},
{9,"IMAGE_SCN_LNK_INFO","Lnkinfo"},
{11,"IMAGE_SCN_LNK_REMOVE","Lnkremove"},
{12,"IMAGE_SCN_LNK_COMDAT","Lnkcomdat"},
{15,"IMAGE_SCN_MEM_FARDATA","Far"},
{17,"IMAGE_SCN_MEM_PURGEABLE","Pur"},
{18,"IMAGE_SCN_MEM_LOCKED","Lock"},
{19,"IMAGE_SCN_MEM_PRELOAD","Prel"},
{24,"IMAGE_SCN_LNK_NRELOC_OVFL","Ovfl"},
{25,"IMAGE_SCN_MEM_DISCARDABLE","D"},
{26,"IMAGE_SCN_MEM_NOT_CACHED","Nc"},
{27,"IMAGE_SCN_MEM_NOT_PAGED","Np"},
{28,"IMAGE_SCN_MEM_SHARED","S"},
{29,"IMAGE_SCN_MEM_EXECUTE","X"},
{30,"IMAGE_SCN_MEM_READ","R"},
{31,"IMAGE_SCN_MEM_WRITE","W"},
};
char*DIRECTORYNAMES[]={
"IMAGE_DIRECTORY_ENTRY_EXPORT",
"IMAGE_DIRECTORY_ENTRY_IMPORT",
"IMAGE_DIRECTORY_ENTRY_RESOURCE",
"IMAGE_DIRECTORY_ENTRY_EXCEPTION",
"IMAGE_DIRECTORY_ENTRY_SECURITY",
"IMAGE_DIRECTORY_ENTRY_BASERELOC",
"IMAGE_DIRECTORY_ENTRY_DEBUG",
"IMAGE_DIRECTORY_ENTRY_COPYRIGHT",
"IMAGE_DIRECTORY_ENTRY_GLOBALPTR",
"IMAGE_DIRECTORY_ENTRY_TLS",
"IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG",
"IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT",
"IMAGE_DIRECTORY_ENTRY_IAT",
"13",
"14",
"15",
};
typedef struct
{
        char*name;
        void*pos;
        int size;
}headertype_;
typedef struct
{
_u32 Signature;
_u16 Machine;
_u16 NumberOfSections;
_u32 TimeDateStamp;
_u32 PointerToSymbolTable;
_u32 NumberOfSymbols;
_u16 SizeOfOptionalHeader;
_u16 Characteristics;
_u16 Magic;
_u8 MajorLinkerVersion;
_u8 MinorLinkerVersion;
_u32 SizeOfCode;
_u32 SizeOfInitializedData;
_u32 SizeOfUninitializedData;
_u32 AddressOfEntryPoint;
_u32 BaseOfCode;
_u32 BaseOfData;
_u32 ImageBase;
_u32 SectionAlignment;
_u32 FileAlignment;
_u16 MajorOperatingSystemVersion;
_u16 MinorOperatingSystemVersion;
_u16 MajorImageVersion;
_u16 MinorImageVersion;
_u16 MajorSubsystemVersion;
_u16 MinorSubsystemVersion;
_u32 Win32VersionValue;
_u32 SizeOfImage;
_u32 SizeOfHeaders;
_u32 CheckSum;
_u16 Subsystem;
_u16 DllCharacteristics;
_u32 SizeOfStackReserve;
_u32 SizeOfStackCommit;
_u32 SizeOfHeapReserve;
_u32 SizeOfHeapCommit;
_u32 LoaderFlags;
_u32 NumberOfRvaAndSizes;
}header_;
header_*header1,header2;
_u32 file_to_analyze;
_u32 PEpos=0;
_u32 original_length=0;
_u8*original_buffer=NULL;
headertype_ headertype[]={
{"Signature",&((*((header_*)0)).Signature),2},
{"Machine",&((*((header_*)0)).Machine),2},
{"NumberOfSections",&((*((header_*)0)).NumberOfSections),2},
{"TimeDateStamp",&((*((header_*)0)).TimeDateStamp),4},
{"PointerToSymbolTable",&((*((header_*)0)).PointerToSymbolTable),4},
{"NumberOfSymbols",&((*((header_*)0)).NumberOfSymbols),4},
{"SizeOfOptionalHeader",&((*((header_*)0)).SizeOfOptionalHeader),2},
{"Characteristics",&((*((header_*)0)).Characteristics),2},

{"Magic",&((*((header_*)0)).Magic),2},
{"MajorLinkerVersion",&((*((header_*)0)).MajorLinkerVersion),1},
{"MinorLinkerVersion",&((*((header_*)0)).MinorLinkerVersion),1},
{"SizeOfCode",&((*((header_*)0)).SizeOfCode),4},
{"SizeOfInitializedData",&((*((header_*)0)).SizeOfInitializedData),4},
{"SizeOfUninitializedData",&((*((header_*)0)).SizeOfUninitializedData),4},
{"AddressOfEntryPoint",&((*((header_*)0)).AddressOfEntryPoint),4},
{"BaseOfCode",&((*((header_*)0)).BaseOfCode),4},
{"BaseOfData",&((*((header_*)0)).BaseOfData),4},
{"ImageBase",&((*((header_*)0)).ImageBase),4},
{"SectionAlignment",&((*((header_*)0)).SectionAlignment),4},
{"FileAlignment",&((*((header_*)0)).FileAlignment),4},
{"MajorOperatingSystemVersion",&((*((header_*)0)).MajorOperatingSystemVersion),2},
{"MinorOperatingSystemVersion",&((*((header_*)0)).MinorOperatingSystemVersion),2},
{"MajorImageVersion",&((*((header_*)0)).MajorImageVersion),2},
{"MinorImageVersion",&((*((header_*)0)).MinorImageVersion),2},
{"MajorSubsystemVersion",&((*((header_*)0)).MajorSubsystemVersion),2},
{"MinorSubsystemVersion",&((*((header_*)0)).MinorSubsystemVersion),2},
{"Win32VersionValue",&((*((header_*)0)).Win32VersionValue),4},
{"SizeOfImage",&((*((header_*)0)).SizeOfImage),4},
{"SizeOfHeaders",&((*((header_*)0)).SizeOfHeaders),4},
{"CheckSum",&((*((header_*)0)).CheckSum),4},
{"Subsystem",&((*((header_*)0)).Subsystem),2},
{"DllCharacteristics",&((*((header_*)0)).DllCharacteristics),2},
{"SizeOfStackReserve",&((*((header_*)0)).SizeOfStackReserve),4},
{"SizeOfStackCommit",&((*((header_*)0)).SizeOfStackCommit),4},
{"SizeOfHeapReserve",&((*((header_*)0)).SizeOfHeapReserve),4},
{"SizeOfHeapCommit",&((*((header_*)0)).SizeOfHeapCommit),4},
{"LoaderFlags",&((*((header_*)0)).LoaderFlags),4},
{"NumberOfRvaAndSizes",&((*((header_*)0)).NumberOfRvaAndSizes),4},
};
#define SizeOfOptionalHeader 0xe0
typedef struct
{
	char name[8];
	_u32 PHY_ADDR;
	_u32 VADDR;
	_u32 SIZE;
	_u32 PRAW;
	_u32 RELO;
	_u32 CHRXTICS;
	_u32 Q;
	_u32 R;
}section_;
section_ l_section;
_u32 lv1,lv2,lv3;
_u32 hxout(_u32 in,_u32 size)
{
	_u8 val=0;
	_u32 shift=(size*8-4);
	while (shift<32)
	{
		val=(in>>shift)&0xF;
		if (val>=10)
		{
			val+=0x37;
		}
		else
		{
			val+='0';
		}
		printf("%c",val);
		shift-=4;
	}
}
_u32 xtoi(char*i_in)
{
	_u32 val=0;
	_u32 l_lv1=0;
	while (i_in[l_lv1]!=0)
	{
		val*=16;
		if ((i_in[l_lv1]>='0')&&(i_in[l_lv1]<='9'))
		{
			val+=i_in[l_lv1]-'0';
		}
		if ((i_in[l_lv1]>='A')&&(i_in[l_lv1]<='Z'))
		{
			val+=i_in[l_lv1]-'7';
		}
		if ((i_in[l_lv1]>='a')&&(i_in[l_lv1]<='z'))
		{
			val+=i_in[l_lv1]-'W';
		}
		l_lv1++;
	}
	return val;
}
_u32 print_var32(char*string,_u32 hex)
{
	printf("%s",string);
	printf(" ");
	hxout(hex,4);
	printf(" ");
}
_u32 writeSp()
{
	printf(" ");
}
_u32 writeLn()
{
	printf("\n");
}
_u32 getheader_obtainmenttype=0;
section_*sectionstart;
_u32 placeholder0_pos;
_u32 placeholder1_pos;
_u32 placeholder_old_pos;
_u32 placeholder_new_pos;
_u32 placeholder_movestart;
_u32 getheader(char*i_name)
{
	//obtainmenttype: 1: entry in header 2: entry in directories 3: entry in sections 4: position in binary file
	if (i_name[0]=='0') i_name++;
	if (i_name[0]==0)
	{
		getheader_obtainmenttype=4;
		return 0;
	}
	if ((i_name[0]>='1')&&(i_name[0]<='9'))
	{
		getheader_obtainmenttype=4;
		return atoi(i_name);
	}
	_u32 l_lv1=0;
	for (l_lv1=0;l_lv1<sizeof(headertype)/sizeof(headertype_);l_lv1++)
	{
		if (strcmp(headertype[l_lv1].name,i_name)==0)
		{
			getheader_obtainmenttype=1;
			return PEpos+(_u32)headertype[l_lv1].pos;
		}
	}
	for (l_lv1=0;l_lv1<sizeof(DIRECTORYNAMES)/sizeof(char*);l_lv1++)
	{
		if (strcmp(DIRECTORYNAMES[l_lv1],i_name)==0)
		{
			getheader_obtainmenttype=2;
			return PEpos+sizeof(header_)+(l_lv1*8);
		}
	}
	if (strncmp("SECTION",i_name,7)==0)
	{
		getheader_obtainmenttype=3;
		return PEpos+sizeof(header_)+(16*8)+(40*xtoi(i_name+8))+((i_name[7]-'0')*4);
	}
	if (strncmp("SECTname0",i_name,9)==0)
	{
		getheader_obtainmenttype=3;
		for (l_lv1=0;l_lv1<(*header1).NumberOfSections;l_lv1++)
		{
			if (strncmp((_u8*)(sectionstart+l_lv1),i_name+10,8)==0)
			{
				return PEpos+sizeof(header_)+(16*8)+(40*l_lv1)+((i_name[9]-'0')*4);
			}
		}
		fprintf(stderr,"Section for delta 0 not found!");
		exit(1);
	}
	if (strncmp("SECTname1",i_name,9)==0)
	{
		getheader_obtainmenttype=3;
		for (l_lv1=0;l_lv1<(*header1).NumberOfSections;l_lv1++)
		{
			if (strncmp((_u8*)(sectionstart+l_lv1),i_name+10,8)==0)
			{
				return PEpos+sizeof(header_)+(16*8)+(40*(l_lv1+1))+((i_name[9]-'0')*4);
			}
		}
		fprintf(stderr,"Section for delta 0 not found!");
		exit(1);
	}
	if (strncmp("SECTname_",i_name,9)==0)
	{
		getheader_obtainmenttype=3;
		for (l_lv1=0;l_lv1<(*header1).NumberOfSections;l_lv1++)
		{
			if (strncmp((_u8*)(sectionstart+l_lv1),i_name+10,8)==0)
			{
				return PEpos+sizeof(header_)+(16*8)+(40*(l_lv1-1))+((i_name[9]-'0')*4);
			}
		}
		fprintf(stderr,"Section for delta -1 not found!");
		exit(1);
	}
	if (strncmp("x",i_name,1)==0)
	{
		getheader_obtainmenttype=4;
		return xtoi(i_name+1);
	}
	if (strcmp("{_}",i_name)==0)
	{
		getheader_obtainmenttype=3;
		return (placeholder0_pos*2)-placeholder1_pos;
	}
	if (strcmp("{0}",i_name)==0)
	{
		getheader_obtainmenttype=3;
		return placeholder0_pos;
	}
	if (strcmp("{1}",i_name)==0)
	{
		getheader_obtainmenttype=3;
		return placeholder1_pos;
	}
	if (strcmp("{x0}",i_name)==0)
	{
		getheader_obtainmenttype=4;
		return placeholder_old_pos;
	}
	if (strcmp("{x1}",i_name)==0)
	{
		getheader_obtainmenttype=4;
		return placeholder_new_pos;
	}
	if (strcmp("{xm}",i_name)==0)
	{
		getheader_obtainmenttype=4;
		return placeholder_movestart;
	}
	fprintf(stderr,"Header type %s not recognized",i_name);
	exit(1);
}
_u32 searchsection(_u32 i_size)
{
	_u32 cursor1=PEpos+0x18+SizeOfOptionalHeader+(40*(*header1).NumberOfSections);
	while(cursor1<original_length)
	{
		_u32 cursor2=0;
		while (cursor2<original_length)
		{
			if (original_buffer[cursor1+cursor2]!=0)
			{
				cursor1+=cursor2;
				cursor1=cursor1-(cursor1%0x200);
				goto l_not_this_time;
			}
			cursor2++;
			if (cursor2==i_size)
			{
				return cursor1;
			}
		}
		return 0;
		l_not_this_time:;
		cursor1+=0x200;
	}
	return 0;
}
int main(int argc,char**argv)
{
	file_to_analyze=open(argv[1],O_RDONLY);
	original_length=lseek(file_to_analyze,0,SEEK_END);
	original_buffer=malloc(original_length);
	lseek(file_to_analyze,0,SEEK_SET);
	read(file_to_analyze,original_buffer,original_length);
	close(file_to_analyze);
	PEpos=*(_u32*)(original_buffer+0x3C);
	print_var32("PEpos",PEpos);
	writeLn();
	header1=(header_*)(original_buffer+PEpos);
	sectionstart=(section_*)(original_buffer+PEpos+sizeof(header_)+(16*8));
	if (argc>2)
	{
		if (argv[2][0]=='-')
		{
			_u8*argvpointer=argv[2]+1;
			_u32 argvcursor=3;
			_u32 argv_repeat_tooling_active=0;
			_u8*argv_repeat_tooling_argvpointer=NULL;
			_u32 argv_repeat_tooling_argvcursor=0x7FFFFFFF;
			_u32 argv_repeat_tooling_section=0;
			goto skip_argv_repeat_tooling;
			argv_repeat_tooling_hook:;
			while(argv_repeat_tooling_section<(*header1).NumberOfSections)
			{
				if ((sectionstart[argv_repeat_tooling_section].PRAW==0))
				{
					placeholder0_pos=((_u8*)(sectionstart+argv_repeat_tooling_section))-original_buffer;
					placeholder1_pos=((_u8*)(sectionstart+argv_repeat_tooling_section+1))-original_buffer;
					argvpointer=argv_repeat_tooling_argvpointer;
					argvcursor=argv_repeat_tooling_argvcursor;
					goto argv_repeat_tooling_go;
				}
				argv_repeat_tooling_section++;
			}
			goto argv_repeat_tooling_continue;
			skip_argv_repeat_tooling:;
			file_to_analyze=open(argv[1],O_RDWR);
			argv_repeat_tooling_go:;
			while ((*argvpointer)!=0)
			{
				switch (*argvpointer)
				{
					case '0': //activate argv_repeat_tooling
					{
						argv_repeat_tooling_active=1;
						argv_repeat_tooling_argvpointer=argvpointer+1;
						argv_repeat_tooling_argvcursor=argvcursor;
						argv_repeat_tooling_section=0;
						goto argv_repeat_tooling_hook;
					}
					case '>':
					{
						_u32 tl_lv1;
						for (tl_lv1=0;tl_lv1<(*header1).NumberOfSections;tl_lv1++)
						{
							if (sectionstart[tl_lv1].SIZE==0)
							{
								sectionstart[tl_lv1].SIZE=sectionstart[tl_lv1].PHY_ADDR;
								while (sectionstart[tl_lv1].SIZE%0x200)
								{
									sectionstart[tl_lv1].SIZE+=1;
								}
								sectionstart[tl_lv1].PRAW=searchsection(sectionstart[tl_lv1].SIZE);
							}
						}
						break;
					}
					case 'X':
					{
						memcpy(&l_section,original_buffer+getheader(argv[argvcursor]),40);
						if (getheader_obtainmenttype!=3)
						{
							fprintf(stderr,"Error! Must SPECIFY SECTION0<hex number of section>\n");
							exit(1);
						}
						memcpy(original_buffer+getheader(argv[argvcursor]),original_buffer+getheader(argv[argvcursor+1]),40);
						memcpy(original_buffer+getheader(argv[argvcursor+1]),&l_section,40);
						argvcursor+=2;
						break;
					}
					case 's':
					{
						_u32 tl_value=getheader(argv[argvcursor+1]);
						*((_u32*)(original_buffer+getheader(argv[argvcursor])))=tl_value;
						if (getheader_obtainmenttype == 2)
						{
							_u32 value=getheader(argv[argvcursor+2]);
							*((_u32*)(original_buffer+getheader(argv[argvcursor])+4))=tl_value;
							argvcursor++;
						}
						argvcursor+=2;
						break;
					}
					case 'c':
					{
						memcpy(original_buffer+getheader(argv[argvcursor]),original_buffer+getheader(argv[argvcursor+1]),getheader(argv[argvcursor+2]));
						argvcursor+=3;
						break;
					}
					case 'i': //add indirect
					{
						_u32 tl_value=*((_u32*)(original_buffer+getheader(argv[argvcursor])));
						tl_value+=*((_u32*)(original_buffer+getheader(argv[argvcursor+1])));
						*((_u32*)(original_buffer+getheader(argv[argvcursor])))=tl_value;
						argvcursor+=2;
						break;
					}
					case 'd': //add indirect
					{
						_u32 tl_value=*((_u32*)(original_buffer+getheader(argv[argvcursor])));
						tl_value-=*((_u32*)(original_buffer+getheader(argv[argvcursor+1])));
						*((_u32*)(original_buffer+getheader(argv[argvcursor])))=tl_value;
						argvcursor+=2;
						break;
					}
					case 'R': //round up
					{
						_u32 tl_value=*((_u32*)(original_buffer+getheader(argv[argvcursor])));
						tl_value+=getheader(argv[argvcursor+1])-1;
						tl_value/=getheader(argv[argvcursor+1]);
						tl_value*=getheader(argv[argvcursor+1]);
						*((_u32*)(original_buffer+getheader(argv[argvcursor])))=tl_value;
						argvcursor+=2;
						break;
					}
					case 'm': //moves the physical data the section refers to (and everything after) in the file. TODO: move the following sections as well
					{
						section_*tl_sectionpos=(section_*)(original_buffer+getheader(argv[argvcursor]));
						placeholder_movestart=(*tl_sectionpos).PRAW;
						if (placeholder_movestart==0)goto move_skip;//empty sections will not be moved
						printf("[31mMOVING %08X %08X[0m\n",placeholder_old_pos,placeholder_new_pos);
						_u32 tl_SIZE=(*tl_sectionpos).SIZE;
						_u32 tl_nextpos=getheader(argv[argvcursor+1]);
						(*tl_sectionpos).PRAW=tl_nextpos;
						if (tl_nextpos+tl_SIZE>original_length)
						{
							original_buffer=realloc(original_buffer,tl_nextpos+tl_SIZE);
							sectionstart=(section_*)(original_buffer+PEpos+sizeof(header_)+(16*8));
							header1=(header_*)(original_buffer+PEpos);
							original_length=tl_nextpos+tl_SIZE;
						}
						memmove(original_buffer+tl_nextpos,original_buffer+placeholder_movestart,tl_SIZE);
						move_skip:;
						argvcursor+=2;
						break;
					}
					case 'z':
					{
						_u32 tl_end=getheader(argv[argvcursor+1]);
						_u32 tl_lv1;
						if (getheader(argv[argvcursor])==0)goto zero_skip;
						for (tl_lv1=getheader(argv[argvcursor]);tl_lv1<tl_end;tl_lv1++)
						{
							original_buffer[tl_lv1]=0;
						}
						zero_skip:;
						argvcursor+=2;
						break;
					}
					case 'F': //fuses a section with the prior one
					{
						section_*tl_to_keep=(section_*)(original_buffer+getheader(argv[argvcursor]));
						section_*tl_to_consume=(section_*)(original_buffer+getheader(argv[argvcursor])+40);
						placeholder_old_pos=(*tl_to_keep).PRAW+(*tl_to_keep).SIZE;
						(*tl_to_consume).SIZE=(*tl_to_consume).PHY_ADDR;
						(*tl_to_consume).SIZE+=((*header1).FileAlignment-1);
						(*tl_to_consume).SIZE/=(*header1).FileAlignment;
						(*tl_to_consume).SIZE*=(*header1).FileAlignment;
						(*tl_to_keep).SIZE=(*tl_to_consume).VADDR-(*tl_to_keep).VADDR+(*tl_to_consume).SIZE;
						if ((*tl_to_consume).VADDR==0)
						{
							fprintf(stderr,"Fuse with VADDR 0\n");
							exit(1);
						}
						placeholder_new_pos=(*tl_to_keep).PRAW+(*tl_to_keep).SIZE;
						(*tl_to_keep).PHY_ADDR=(*tl_to_keep).SIZE;
						memmove(tl_to_consume,tl_to_consume+1,((_u8*)(sectionstart+((*header1).NumberOfSections)))-((_u8*)tl_to_consume)-40);
						(*header1).NumberOfSections--;
						argvcursor+=1;
						break;
					}
					case 'f': //fuses a bss section with the prior one
					{
						section_*tl_to_keep=(section_*)(original_buffer+getheader(argv[argvcursor]));
						section_*tl_to_consume=(section_*)(original_buffer+getheader(argv[argvcursor])+40);
						placeholder_old_pos=(*tl_to_keep).PRAW+(*tl_to_keep).SIZE;
						if ((*tl_to_consume).VADDR==(*(tl_to_consume+1)).VADDR)
						{
							placeholder_new_pos=placeholder_old_pos;
							goto done;
						}
						(*tl_to_keep).PHY_ADDR+=((*header1).SectionAlignment-1);
						(*tl_to_keep).PHY_ADDR/=(*header1).SectionAlignment;
						(*tl_to_keep).PHY_ADDR*=(*header1).SectionAlignment;
						(*tl_to_keep).PHY_ADDR+=(*tl_to_consume).PHY_ADDR;
						placeholder_new_pos=(*tl_to_keep).PRAW+(*tl_to_keep).SIZE;
						done:;
						memmove(tl_to_consume,tl_to_consume+1,((_u8*)(sectionstart+((*header1).NumberOfSections)))-((_u8*)tl_to_consume)-40);
						(*header1).NumberOfSections--;
						argvcursor+=1;
						break;
					}
				}
				argvpointer++;
			}
			if (argv_repeat_tooling_active)
			{
				goto argv_repeat_tooling_hook;
			}
			argv_repeat_tooling_continue:;
			file_to_analyze=open(argv[1],O_RDWR,0660);
			_u32 backval=write(file_to_analyze,original_buffer,original_length);
			close(file_to_analyze);
			return 0;
		}
	}
	for (lv1=0;lv1<sizeof(headertype)/sizeof(headertype_);lv1++)
	{
		_u8*l_scanpos=(_u8*)(((_u32)((_u8*)header1))+headertype[lv1].pos);
		printf("%s",headertype[lv1].name);
		writeSp();
		hxout(*(_u32*)l_scanpos,headertype[lv1].size);
		writeLn();
	}
	for (lv1=0;lv1<16;lv1++)
	{
		_u32 tl_address=*(_u32*)(((_u8*)header1)+sizeof(header_)+(lv1*8));
		_u32 tl_size=*(_u32*)(((_u8*)header1)+sizeof(header_)+4+(lv1*8));
		if ((tl_address!=0)||(tl_size!=0))
		{
			printf("%s",DIRECTORYNAMES[lv1]);
			printf(":");
			writeSp();
			hxout(tl_address,4);
			writeSp();
			hxout(tl_size,4);
			writeLn();
		}
	}
	for (lv1=0;lv1<(*header1).NumberOfSections;lv1++)
	{
		memcpy(&l_section,original_buffer+PEpos+0x18+SizeOfOptionalHeader+(40*lv1),40);
		printf("name: '");
		for (lv2=0;lv2<8;lv2++) if (l_section.name[lv2]!=0) printf("%c",l_section.name[lv2]); else printf("\\x00");
		printf("'\n");
		print_var32("PHY_ADDR",l_section.PHY_ADDR);
		print_var32("VADDR",l_section.VADDR);
		print_var32("SIZE",l_section.SIZE);
		print_var32("PRAW",l_section.PRAW);
		print_var32("RELO",l_section.RELO);
		writeLn();
		for (lv2=0;lv2<32;lv2++)
		{
			if (l_section.R&(1<<lv2))
			{
				for (lv3=0;lv3<sizeof(SECTIONFLAGS)/sizeof(SECTIONFLAGS_);lv3++)
				{
					if (SECTIONFLAGS[lv3].idx==lv2)
					{
						printf("%s",SECTIONFLAGS[lv3].shortname);
						goto found;
					}
				}
				printf("? ");
				found:;
			}
		}
		print_var32("CHRXTICS",l_section.CHRXTICS);
		print_var32("Q",l_section.Q);
		print_var32("R",l_section.R);
		writeLn();
	}
	return 0;
}
