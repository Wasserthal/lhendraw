#define _u8 unsigned char
#define _u16 unsigned short
#define _u32 unsigned int
_u8*g_idata=NULL;
_u32 g_r_addr=0;
#ifndef CROFTOIDAL
typedef struct
{
	_u32 OriginalFirstThunk;
	_u32 TimeDateStamp;
	_u32 ForwarderChain;
	_u32 Name;
	_u32 FirstThunk;
}IMAGE_IMPORT_DESCRIPTOR;
#endif
IMAGE_IMPORT_DESCRIPTOR*image_import_descriptor;
section_*idata_srch_section()
{
	section_*l_section;
	for (_u32 lv1=0;lv1<(*header1).NumberOfSections;lv1++)
	{
		l_section=(section_*)(original_buffer+PEpos+0x18+SizeOfOptionalHeader+(40*lv1));
		if(strncmp((*l_section).name,".idata",8)==0)
		{
			return l_section;
		}
	}
	return NULL;
}
_u32 idata_srch_entry(section_*l_section,_u8*i_name)
{
	_u8*l_idata=original_buffer+(*l_section).PRAW;
	image_import_descriptor=(IMAGE_IMPORT_DESCRIPTOR*)l_idata;
	while ((*image_import_descriptor).Name!=0)
	{
		_u32*FirstThunk=(_u32*)(l_idata+(*image_import_descriptor).FirstThunk-(*l_section).VADDR);
		while (*FirstThunk)
		{
			if (strcmp(i_name,(l_idata+(*FirstThunk)-(*l_section).VADDR+2))==0)
			{
				return (((_u8*)FirstThunk)-l_idata+(*l_section).VADDR+(*header1).ImageBase);
			}
			FirstThunk++;
		}
		image_import_descriptor++;
	}
	return 0;
}
_u32 idata_srch_dllentry(section_*l_section,_u8*dll_name,_u8*i_name)
{
	_u8*l_idata=original_buffer+(*l_section).PRAW;
	image_import_descriptor=(IMAGE_IMPORT_DESCRIPTOR*)l_idata;
	while ((*image_import_descriptor).Name!=0)
	{
		if(strcmp(dll_name,l_idata+(*image_import_descriptor).Name-(*l_section).VADDR))goto l_end;
		_u32*FirstThunk=(_u32*)(l_idata+(*image_import_descriptor).FirstThunk-(*l_section).VADDR);
		while (*FirstThunk)
		{
			if (strcmp(i_name,(l_idata+(*FirstThunk)-(*l_section).VADDR+2))==0)
			{
				return (((_u8*)FirstThunk)-l_idata+(*l_section).VADDR+(*header1).ImageBase);
			}
			FirstThunk++;
		}
		l_end:;
		image_import_descriptor++;
	}
	printf("ERROR! DLL Entry \"%s\" not found in \"%s\"!\n",i_name,dll_name);
	exit(1);
	return 0;
}
