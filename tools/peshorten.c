#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <lendefs.h>
#include <string.h>
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
section_*section;
_u16 nos=0;
_u32 lv1,lv2,lv3;
void des(_u32 which)
{
	_u32 ilv1;
	for (ilv1=which;ilv1<nos;ilv1++)
	{
		if ((ilv1+1)<nos)
		{
			section[ilv1]=section[ilv1+1];
		}
		else
		{
			memset(section+ilv1,0,40);
		}
	}
	nos--;
}
int main(int argc,char**argv)
{
	section_ l_section;
	if (argc!=2)
	{
		fprintf(stderr,"need filename to read\n");
		return 1;
	}
	_u32 file_to_analyze=open(argv[1],O_RDWR);
	lseek(file_to_analyze,0x3C,SEEK_CUR);
	_u32 PEpos=0;
	read(file_to_analyze,&PEpos,4);
	lseek(file_to_analyze,PEpos,SEEK_SET);
	lseek(file_to_analyze,6,SEEK_CUR);
	_u16 section_count_offset=lseek(file_to_analyze,0,SEEK_CUR);
	read(file_to_analyze,&nos,2);
	printf("%hi\n",nos);
	lseek(file_to_analyze,16,SEEK_CUR);
	lseek(file_to_analyze,0x108,SEEK_CUR);
	_u32 section_offset=lseek(file_to_analyze,0,SEEK_CUR);
	section=(section_*)malloc(sizeof(l_section)*nos);
	read(file_to_analyze,section,sizeof(l_section)*nos);
	_u16 old_nos=nos;
	for (lv1=0;lv1<nos;lv1++)
	{
		if (section[lv1].SIZE==0)
		{
			des(lv1);
			lv1--;
		}
	}
	for (lv1=0;lv1<nos;lv1++)
	{
		l_section=section[lv1];
		printf("name '");
		for (lv2=0;lv2<8;lv2++) if (l_section.name[lv2]!=0) printf("%c",l_section.name[lv2]); else printf("\\x00");
		printf("'\n");
		printf("PHY_ADDR %08X ",l_section.PHY_ADDR);
		printf("VADDR %08X ",l_section.VADDR);
		if (l_section.SIZE==0) printf("[34m"); else printf("[1m");
		printf("SIZE %08X ",l_section.SIZE);
		printf("[0m");
		printf("PRAW %08X ",l_section.PRAW);
		printf("RELO %08X\n",l_section.RELO);
		for (lv2=0;lv2<32;lv2++)
		{
			if (l_section.R&(1<<lv2))
			{
				for (lv3=0;lv3<sizeof(SECTIONFLAGS)/sizeof(SECTIONFLAGS_);lv3++)
				{
					if (SECTIONFLAGS[lv3].idx==lv2)
					{
						printf("%s ",SECTIONFLAGS[lv3].shortname);
						goto found;
					}
				}
				printf("? ");
				found:;
			}
		}
		printf("CHRXTICS %08X ",l_section.CHRXTICS);
		printf("Q %08X ",l_section.Q);
		printf("R %08X\n",l_section.R);
	}
	lseek(file_to_analyze,section_count_offset,SEEK_SET);
	write(file_to_analyze,&nos,2);
	lseek(file_to_analyze,section_offset,SEEK_SET);
	write(file_to_analyze,section,sizeof(l_section)*old_nos);
	close(file_to_analyze);
}
