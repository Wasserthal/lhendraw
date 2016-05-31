//defines and processes internal variables
char LHENDRAW_clipboardmode=0;//0: no clipboardmode 1: lhendraw is providing a X11 clipboard content
char LHENDRAW_wassaved=1;//0 if the file was modified after the last save
int error_code=0;
char LHENDRAW_filedlgmode=0;//Requires reset
char LHENDRAW_warndlgmode=0;
int LHENDRAW_userdecision=0;
char LHENDRAW_leave=0;
int edit_fileoperationrefersonlytopartofdocument=0;//Hidden parameter
int edit_file_always_overwrite=0;//Hidden parameter
const char * LHENDRAW_warning=NULL;
int userwarning(const char*warning);
_i32 janitor_maxZ=0;
_small LHENDRAW_maxbuttons=74;
typedef _u32 selection_datatype;
#define constants_buttonsize 32
#define constants_bondlength 30
#define constants_angular_distance 2.094395102
#define constants_clickradius 225
#define constants_Element_loneradical 0
#define constants_Element_lonepair 1
#define constants_Element_hydrogen 2
#define constants_Element_implicitcarbon 9
#define constants_VALUEREPRESENTATION_BUFFERSIZE 4096
//It is assumed that the actual carbon is constants_Element_implicitcarbon+1
float glob_clickradius=225;
float glob_subpoint_clickradius=50;
#define constants_maxbonds 10
#define constants_undostep_max 40
#ifndef CROFTOIDAL
#define constants_Directoryslash '/'
#else
#define constants_Directoryslash '\\'
#endif
_u32 selection_max=LHENDRAW_buffersize/sizeof(selection_datatype);
typedef selection_datatype * selection_;
selection_datatype * selection_currentselection;//The ordinary selection, more precisely the current state of it.
_u32 selection_currentselection_found;
selection_datatype * selection_clickselection;//All elements caught with the current click, or another likewise action of the mouse.
_u32 selection_clickselection_found;
selection_datatype * selection_fragmentselection;//A fragment
_u32 selection_fragmentselection_found;
selection_datatype * selection_oldselection;
selection_datatype * selection_objectselection;
selection_datatype * selection_ringselection;
selection_datatype * selection_growselection;
#ifdef SDL2
#define SDLK_LSUPER SDLK_LGUI
#define SDLK_RSUPER SDLK_RGUI
#endif
#define constants_maxlistentries 2047
char constants_cdxstring[]=".cdx";
typedef _u32 resources_button_[32][32];
resources_button_ * resources_bitmap_buttons;
int control_menutextedithorziscroll=0;
int control_menutexteditcursor=0;
char menugfx_menudescription[stringlength+1]={0};
struct control_export_
{
	int bmp_compression;//0: normal 3:ARGB,32bpp only
	int hires;//0: not 1: use HQ_filter
	int transparent;
};
control_export_ control_export={0,1,0};
typedef struct MODIFIER_KEYS_
{
	char CTRL;
	char ALT;
	char LALT,RALT;
	char SHIFT;
	char SUPER;
	char LEFT;
	char RIGHT;
	char UP;
	char DOWN;
}MODIFIER_KEYS_;
MODIFIER_KEYS_ MODIFIER_KEYS={0,0,0,0,0,0,0,0};
#ifdef SDL2
_u16 getunicode(SDL_Event * input)
{
	char ihv1=SDL_GetKeyName(input->key.keysym.sym)[0];
	if (strlen(SDL_GetKeyName(input->key.keysym.sym))==1)
	{
		if ((ihv1>='A') && (ihv1<='Z'))
		{
			if (MODIFIER_KEYS.SHIFT)
			{
				return ihv1;
			}
			else
			{
				return ihv1+0x20;
			}
		}
	}
	return 0;
}
#define SDL_BUTTON_WHEELUP 4
#define SDL_BUTTON_WHEELDOWN 5
#else
_u16 getunicode(SDL_Event * input)
{
	return input->key.keysym.unicode;
}
#endif
char control_filememory_buffer[constants_maxlistentries][256];
char control_devicememory_buffer[constants_maxlistentries][64];
_u32 control_filememory_attribs[constants_maxlistentries];
char control_filememory_port_buffer[constants_maxlistentries][256];
_u32 control_filememory_port_attribs[constants_maxlistentries];
char control_filememory_search_buffer[constants_maxlistentries][256];
_u32 control_filememory_search_attribs[constants_maxlistentries];
//TODO: how are the attribs connected?
int menu_selectedmenuelement;
char control_filemenu_mode=0;
int undo_undodirty=1;//set when undo was stored just to become immediately modified in the same command, like the storeundo at issueclick
structenum control_devicememory=
{
	{"control_devicememory"},
	0,255,&control_devicememory_buffer,1,sizeof(control_devicememory_buffer[0]),-1,0,
};
structenum control_filememory=
{
	{"control_filememory"},
	0,255,&control_filememory_buffer,1,sizeof(control_filememory_buffer[0]),-1,0,
};
structenum control_filememory_port=
{
	{"control_filememory_port"},
	0,255,&control_filememory_port_buffer,1,sizeof(control_filememory_port_buffer[0]),-1,0,
};
structenum control_filememory_search=
{
	{"control_filememory_search"},
	0,255,&control_filememory_search_buffer,1,sizeof(control_filememory_search_buffer[0]),-1,0,
};
char control_currentdirectory[1024]="";
char control_filenamehead[256]="";
char control_currentdirectory_port[1024]="";
char control_filenamehead_port[256]="";
char control_currentdirectory_search[1024]="";
char control_filenamehead_search[256]="";
int control_setfilename(const char * ifilename)
{
	int istrpos;
	for (int ilv1=strlen(ifilename)-1;ilv1>=0;ilv1--)
	{
		if (ifilename[ilv1]==constants_Directoryslash)
		{
			istrpos=ilv1;
			goto ifound;
		}
	}
	if (strlen(ifilename)!=0)
	{
		control_currentdirectory[0]='.';
		control_currentdirectory[1]=0;
		strcpy(control_filenamehead,ifilename);
		goto correct;
	}
	return 0;
	ifound:;
	strncpy(control_currentdirectory,ifilename,istrpos+1);
	control_currentdirectory[istrpos]=0;
	strcpy(control_filenamehead,ifilename+istrpos+1);
	correct:;
	#ifndef SDL2
	SDL_WM_SetCaption(control_filenamehead,control_filenamehead);
	#endif
	if (strcmp(control_filenamehead,"")==0) return 0;
	return 1;
}
int undosteps_count=0;
int currentundostep=-1;//The latest undo-step that is actually saved, so NOT the current one
TELESCOPE_buffer * undo_retrievecontentbuffer(intl start,intl list);
int getbufferfromstructure(basicmultilist * input,TELESCOPE_buffer * * bufferptr)
{
	if (input==CAMBRIDGEPREFIX2(glob,s_multilist))
	{
		(*bufferptr)=&filestructure_text_buffer;
		return 1;
	}
	if (input==CAMBRIDGEPREFIX2(glob,curve_multilist))
	{
		(*bufferptr)=&filestructure_curve_buffer;
		return 1;
	}
/*	if (input==glob_t_multilist)
	{
		(*bufferptr)=&internalstructure_text_buffer;
		return 1;
	}
	if (input==glob_n_multilist)
	{
		(*bufferptr)=&internalstructure_n_buffer;
		return 1;
	}
	if (input==glob_arrow_multilist)
	{
		(*bufferptr)=&internalstructure_arrow_buffer;
		return 1;
	}*/
	char * name=NULL;
	for (int ilv1=0;ilv1<multilist_count;ilv1++)
	{
		if (multilistlist[ilv1].instance==input)
		{
			name=multilistlist[ilv1].name;
		}
	}
	if (name!=NULL)
	{
		for (int ilv1=1;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
		{
			if (strcmp(name,STRUCTURE_OBJECTTYPE_List[ilv1].name)==0)
			{
				(*bufferptr)=glob_contentbuffer+ilv1;
				return 1;
			}
		}
	}
	if (undo_getbufferfromstructure(input,bufferptr)) return 1;
	(*bufferptr)=NULL;
	return 0;
}
void error_reset()
{
	LHENDRAW_filedlgmode=0;
	LHENDRAW_warndlgmode=0;
	LHENDRAW_userdecision=0;
	LHENDRAW_leave=0;
	edit_fileoperationrefersonlytopartofdocument=0;
	edit_file_always_overwrite=0;
	fprintf(stderr,"Error code %+i detected...resetting",error_code);
}
extern char * gfx_linewisebuffer;
extern char * selection_lassobuffer;
int initmemory()
{
	memory_alloc(&(filestructure_text_buffer.buffer),1);
	filestructure_text_buffer.count=0;
	memory_alloc(&(filestructure_curve_buffer.buffer),1);
	filestructure_curve_buffer.count=0;
	memory_alloc(&(gfx_linewisebuffer),7);
	memory_alloc(&(selection_lassobuffer),7);
	memory_alloc((char**)&(selection_currentselection),6);
	memory_alloc((char**)&(selection_clickselection),6);
	memory_alloc((char**)&(selection_fragmentselection),6);
	memory_alloc((char**)&(selection_oldselection),6);
	if ((filestructure_text_buffer.buffer!=NULL) && (filestructure_curve_buffer.buffer!=NULL)) return 1;
	return -1;
}
