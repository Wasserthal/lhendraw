//any graphics output which is not data of the document
void text_output_bitmap(int * posx,int * posy,fontpixinf_ * ifontpixinf)
{
	int ilv1,ilv2;
	int scanx,scany;
	int maxx,maxy;
	int iscreenskip=gfx_screensizex;
	unsigned int * iscreen=screen;
	int skip=(*ifontpixinf).sizex;
	char * mempos=(*ifontpixinf).memstart;
	scanx=(*posx)+(*ifontpixinf).pivotx;
	scany=(*posy)+(*ifontpixinf).pivoty+12;
	maxx=(*ifontpixinf).sizex+scanx;
	maxy=(*ifontpixinf).sizey+scany;
	if (scanx<0) {mempos-=scanx;scanx=0;}
	if (scany<0) {mempos-=scany*skip;scany=0;}
	if (maxx>=gfx_screensizex) maxx=gfx_screensizex-1;
	if (maxy>=gfx_screensizey) maxy=gfx_screensizey-1;
	skip-=(*ifontpixinf).sizex;
	(*posx)+=8;
	(*posy)+=(*ifontpixinf).deltay;
	if ((scanx>=maxx) || (scany>=maxy))
	{
		return;
	}
	iscreen+=iscreenskip*scany+scanx;
	iscreenskip-=(*ifontpixinf).sizex;
	for (ilv1=scany;ilv1<maxy;ilv1++)
	{
		for (ilv2=scanx;ilv2<maxx;ilv2++)
		{
			if (*(mempos) & 0x80)
			{
				*(iscreen)=SDL_color;
			}
			iscreen++;
			mempos++;
		}
		mempos+=skip;
		iscreen+=iscreenskip;
	}
}
int draw_drawmarkpoint(int x,int y,int gfxno,int number,char noclip)
{
	_u32 * ibutton=resources_bitmap_buttons[LHENDRAW_maxbuttons-gfxno-1][0];
	ibutton+=(number%4)*8+(number/4)*256;
	for (int ilv1=0;ilv1<8;ilv1++)
	{
		for (int ilv2=0;ilv2<8;ilv2++)
		{
			ARGB pixin;
			if ((((y+ilv1)>=gfx_canvasminy) && ((y+ilv1)<gfx_canvasmaxy) && ((x+ilv2)>=gfx_canvasminx) && ((x+ilv2)<gfx_canvasmaxx)) || (noclip))
			{
				pixin.A=screen[(y+ilv1)*gfx_screensizex+x+ilv2];
				ARGB bitsin;
				bitsin.A=ibutton[ilv1*32+ilv2];
				int alpha=(_u8)bitsin.c[3];//no ENDIAN problem. bit order still from file!
				for (int ilv3=0;ilv3<4;ilv3++)
				{
					pixin.c[ilv3]=(((256-alpha)*(_u8)(pixin.c[ilv3]))>>8)+(((alpha)*(_u8)(bitsin.c[ilv3]))>>8);
				}
				screen[(y+ilv1)*gfx_screensizex+x+ilv2]=pixin.A;
			}
		}
	}
	return 1;
}
void sdl_buttondraw(int posx,int posy,int number)
{
	for (int ilv2=0;ilv2<32;ilv2++)
	{
		for (int ilv3=0;ilv3<32;ilv3++)
		{
			_u32 tlgfx=resources_bitmap_buttons[LHENDRAW_maxbuttons-number][ilv2][ilv3];
			if ((tlgfx>>24)>0x00)
			screen[(posy+ilv2)*gfx_screensizex+posx+ilv3]=tlgfx;
		}
	}
	return;
}
void printmenutext(int posx,int posy,const char * iinput,const char * parms,int imode,int start,int end,char symbolmode)
{
	int ilv4=start;
	char linebreak;
	int iposx=posx;
	int iposy=posy;
	thatwasatemporaryskip:
	linebreak=0;
	for (;ilv4<end;ilv4++)
	{
		if ((iinput[ilv4]=='\e') && (symbolmode))
		{
			int wert=0;
			ilv4++;
			while (iinput[ilv4]!=' ')
			{
				wert*=10;
				wert+=iinput[ilv4]-'0';
				ilv4++;
			}
			ilv4++;
			sdl_buttondraw(iposx,iposy-16,wert+1);
			iposx+=32;
			continue;
		}
		if (iinput[ilv4]==10)
		{
			ilv4++;
			linebreak=1;
			goto skipfornow;
		}
		text_output_bitmap(&iposx,&iposy,&fontpixinf[indexfromunicode(((unsigned char)iinput[ilv4]))]);
	}
	skipfornow:
	if (linebreak) {if (ilv4<end) {if (symbolmode) {iposx=posx;iposy+=16;}goto thatwasatemporaryskip;}}//a line break;
}
int sdl_toolboxitemdraw(int posx,int posy,int gfxno,_u32 state)
{
	_u32 * ibutton=resources_bitmap_buttons[0][0];
	_u32 * ihgd=resources_bitmap_buttons[1][0];
	int idelta=(state & 0xD)>>1;
	for (int ilv2=0;ilv2<32-idelta;ilv2++)
	{
		for (int ilv3=0;ilv3<32-idelta;ilv3++)
		{
			_u32 tlgfx=resources_bitmap_buttons[LHENDRAW_maxbuttons-gfxno-1][ilv2][ilv3];
			_u8 tlgfr=(tlgfx>>16) & 0xFF;
			_u8 tlgfg=(tlgfx>>8) & 0xFF;
			_u8 tlgfb=tlgfx & 0xFF;
			if ((state & 1)==1)
			{
				tlgfr=((1+(((tlgfx>>24) & 0xFF)))*tlgfr+((state & 0xFF000000)>>24)*(0xFF-((tlgfx>>24) & 0xFF)))>>8;
				tlgfg=((1+(((tlgfx>>24) & 0xFF)))*tlgfg+((state & 0xFF0000)>>16)*(0xFF-((tlgfx>>24) & 0xFF)))>>8;
				tlgfb=((1+(((tlgfx>>24) & 0xFF)))*tlgfb+((state & 0xFF00)>>8)*(0xFF-((tlgfx>>24) & 0xFF)))>>8;
			}
			screen[gfx_screensizex*(posy+ilv2+idelta)+posx+ilv3+idelta]=((tlgfx & 0xFF000000)==0x00000000) ? resources_bitmap_buttons[LHENDRAW_maxbuttons-17][ilv2][ilv3]:((tlgfr<<16)+(tlgfg<<8)+(tlgfb));
		}
	}
	if (idelta>0)
	{
		for (int ilv1=0;ilv1<idelta;ilv1++)
		{
			for (int ilv2=0;ilv2<32;ilv2++)
			{
				screen[gfx_screensizex*(posy+ilv1)+posx+ilv2]=resources_bitmap_buttons[LHENDRAW_maxbuttons-16][ilv1][ilv2];
				screen[gfx_screensizex*(posy+ilv2)+posx+ilv1]=resources_bitmap_buttons[LHENDRAW_maxbuttons-16][ilv2][ilv1];
			}
		}
	}
	return 1;
}
int sdl_sliderdraw(int posx,int posy,int gfxno,int shift,int color)
{
	_u32 * ibutton=resources_bitmap_buttons[LHENDRAW_maxbuttons-gfxno-1][0];
	int xco,yco;
	_u32 islider[8];
	if (shift>=0)
	{
		shift=shift%8;
	}
	else
	{
		shift=8-((-shift)%8);
	}
	for (int ilv1=0;ilv1<8;ilv1++)
	{
		islider[ilv1]=((_u64)*(ibutton+(ilv1+shift)%8));
		if (color>=8)
		{
			islider[ilv1]=((islider[ilv1]<<8) & 0xFFFFFF)|(islider[ilv1]>>16);
		}
		if (color>=16)
		{
			islider[ilv1]=((islider[ilv1]<<8) & 0xFFFFFF)|(islider[ilv1]>>16);
		}
	}
	for (int ilv1=0;ilv1<32;ilv1++)
	{
		if (ilv1<8)
		{
			yco=ilv1+8;
			if (yco>=12) yco=11;
		}
		else if (ilv1>=24)
		{
			yco=ilv1-16;
			if (yco<12) yco=12;
		}
		else
		{
			for (int ilv2=0;ilv2<gfx_canvassizex;ilv2+=8)
			{
				_u32 * tl_screen=screen+(gfx_screensizex*(posy+ilv1)+posx+ilv2);
				for (int ilv3=0;ilv3<8;ilv3++)
				{
					tl_screen[ilv3]=islider[ilv3];
				}
			}
			goto ifertig;
		}
		for (int ilv2=0;ilv2<gfx_canvassizex;ilv2++)
		{
			if (ilv2<8)
			{
				xco=ilv2;
				if (xco>=4) xco=3;
			}
			else if (ilv2>=gfx_canvassizex-4)
			{
				xco=ilv2-(gfx_canvassizex-8);
				if (xco<4) xco=4;
			}
			screen[gfx_screensizex*(posy+ilv1)+posx+ilv2]=*(ibutton+yco*32+xco);
		}
		ifertig:;
	}
	for (int ilv1=-180;ilv1<=180;ilv1+=15)
	{
		int tl_no=4;
		if (ilv1%30==0) tl_no=1;
		if (ilv1%45==0) tl_no=2;
		if (ilv1%90==0) tl_no=2;
		if (ilv1%180==0) tl_no=5;
		if (ilv1==0) tl_no=3;
		draw_drawmarkpoint(posx+(((ilv1+180)*gfx_canvassizex)/360)-4,posy+24,47,tl_no,1);
	}
	return 1;
}
int quersum(_u32 input,int max=32)
{
	char wert=0;
	_u32 iinput=input;
	for (int ilv1=0;ilv1<max;ilv1++)
	{
		wert+=((iinput & 1)==1);
		iinput=iinput>>1;
	}
	return wert;
}
int sdl_textmenudraw(AUTOSTRUCT_PULLOUTLISTING_ * ilisting,int count,int xpos=0,int ypos=0)
{
	for (int ilv1=0;ilv1<count;ilv1++)
	{
		switch (ilisting[ilv1].lmbmode)
		{
			case 5:
			{
				_u32 * iscreen=screen+(ypos+ilisting[ilv1].y*16)*gfx_screensizex+xpos+ilisting[ilv1].x*192;
				if ((*((_u32*)ilisting[ilv1].variable)) & (1<<ilv1))
				{
					SDL_color=0xFFFFFF;
					for (int ilv2=0;ilv2<16;ilv2++)
					{
						for (int ilv3=0;ilv3<192;ilv3++)
						{
							*(iscreen)=0;
							iscreen++;
						}
						iscreen+=gfx_screensizex-192;
					}
				}
				else
				{
					SDL_color=0x000000;
					for (int ilv2=0;ilv2<16;ilv2++)
					{
						for (int ilv3=0;ilv3<192;ilv3++)
						{
							*(iscreen)=0xFFFFFF;
							iscreen++;
						}
						iscreen+=gfx_screensizex-192;
					}
				}
			}
			break;
			default:
			SDL_color=0x000000;
		}
		printmenutext(ilisting[ilv1].x*192+xpos,ilisting[ilv1].y*16+ypos,ilisting[ilv1].name,NULL,0,0,strlen(ilisting[ilv1].name));
	}
	return 1;
}
int sdl_textbuttonmenudraw(AUTOSTRUCT_PULLOUTLISTING_ * ilisting,int count,int xpos=0,int ypos=0)
{
	SDL_color=0;
	for (int ilv1=0;ilv1<count;ilv1++)
	{
		int left,top,right,bottom;
		left=xpos+ilisting[ilv1].x;
		top=ypos+ilisting[ilv1].y;
		right=xpos+ilisting[ilv1].maxx;
		bottom=ypos+ilisting[ilv1].maxy;
		_u32 * ibutton=resources_bitmap_buttons[LHENDRAW_maxbuttons-34][0];
		int xco,yco;
		for (int ilv1=0;ilv1<32;ilv1++)
		{
			if (ilv1<8)
			{
				yco=ilv1+8;
				if (yco>=12) yco=11;
			}
			else if (ilv1>=24)
			{
				yco=ilv1-16;
				if (yco<12) yco=12;
			}
			else
			{
				yco=11;
			}
			for (int ilv2=0;ilv2<right-left;ilv2++)
			{
				if (ilv2<8)
				{
					xco=ilv2;
					if (xco>=4) xco=3;
				}
				else if (ilv2>=gfx_canvassizex-4)
				{
					xco=ilv2-(gfx_canvassizex-8);
					if (xco<4) xco=4;
				}
				screen[gfx_screensizex*(top+ilv1)+left+ilv2]=*(ibutton+yco*32+xco);
			}
		}
		printmenutext(left,top,ilisting[ilv1].name,NULL,0,0,strlen(ilisting[ilv1].name));
	}
	return 1;
}
int sdl_listmenudraw(AUTOSTRUCT_PULLOUTLISTING_ * ilisting,int count,int xpos=0,int ypos=0,int item=-1)
{
	int ilv2;
	structenum istructenum;
	int bgcolor=0;
	for (int ilv1=0;ilv1<count;ilv1++)
	{
		switch (ilisting[ilv1].lmbmode & 0xFF00)
		{
			case 0x200:
			{
				istructenum=(*(structenum*)(ilisting[ilv1].variable));
				for (ilv2=0;ilv2*16+ypos<ilisting[ilv1].maxy-ilisting[ilv1].y;ilv2++)
				{
					int tl_number=ilv2+istructenum.scroll;
					if (tl_number==istructenum.number)
					{
						bgcolor=0xFF;
					}
					else
					{
						bgcolor=0;
					}
					_u32 * iscreen=screen+((ypos+ilisting[ilv1].y+ilv2*16))*gfx_screensizex+xpos+ilisting[ilv1].x;
					SDL_color=ilisting[ilv1].bgcolor;
					for (int ilv3=0;ilv3<16;ilv3++)
					{
						for (int ilv4=ilisting[ilv1].x;ilv4<ilisting[ilv1].maxx;ilv4++)
						{
							*(iscreen)=bgcolor;
							iscreen++;
						}
						iscreen+=gfx_screensizex-ilisting[ilv1].maxx+ilisting[ilv1].x;
					}
					if (tl_number<istructenum.count)
					{
						char * tl_pointer=((char*)(istructenum.pointer))+istructenum.size*tl_number;
						printmenutext(ilisting[ilv1].x+xpos,(ilisting[ilv1].y+ilv2*16)+ypos,tl_pointer,NULL,0,0,max(strlen(tl_pointer),(ilisting[ilv1].maxx-ilisting[ilv1].x)/8));
					}
				}
				break;
			}
			case 0x300:
			{
				char * tl_pointer=(char*)(ilisting[ilv1].variable);
				int tl_counter;
				_u32 * iscreen=screen+(ypos+ilisting[ilv1].y)*gfx_screensizex+xpos+ilisting[ilv1].x;
				SDL_color=ilisting[ilv1].bgcolor;
				for (int ilv3=0;ilv3<16;ilv3++)
				{
					for (int ilv4=ilisting[ilv1].x,tl_counter=0;ilv4<ilisting[ilv1].maxx;ilv4+=8,tl_counter++)
					{
						if ((control_menutexteditcursor-control_menutextedithorziscroll*(item==ilv1)-tl_counter!=0) || (item!=ilv1))
						{
							*(iscreen++)=0x5F5F5F;
							*(iscreen++)=0x5F5F5F;
							*(iscreen++)=0x5F5F5F;
							*(iscreen++)=0x5F5F5F;
							*(iscreen++)=0x5F5F5F;
							*(iscreen++)=0x5F5F5F;
							*(iscreen++)=0x5F5F5F;
							*(iscreen++)=0x5F5F5F;
						}
						else
						{
							*(iscreen++)=0x7F7FFF;
							*(iscreen++)=0x7F7FFF;
							*(iscreen++)=0x7F7FFF;
							*(iscreen++)=0x7F7FFF;
							*(iscreen++)=0x7F7FFF;
							*(iscreen++)=0x7F7FFF;
							*(iscreen++)=0x7F7FFF;
							*(iscreen++)=0x7F7FFF;
						}
					}
					iscreen+=gfx_screensizex-ilisting[ilv1].maxx+ilisting[ilv1].x;
				}
				tl_pointer=(char*)(ilisting[ilv1].variable);
				printmenutext(ilisting[ilv1].x+xpos,(ilisting[ilv1].y)+ypos,tl_pointer,NULL,0,0,min(strlen(tl_pointer),(ilisting[ilv1].maxx-ilisting[ilv1].x)/8));
				break;
			}
		}
	}
	return 1;
}
void sdl_colorpaldraw(int posx,int posy)
{
	_u32 icolor;
	for (int ilv1=0;ilv1<256;ilv1++)
	{
		for (int ilv2=0;ilv2<256;ilv2++)
		{
			if (control_usingmousebutton==SDL_BUTTON_LEFT)
			{
				icolor=control_drawproperties.color&(0xFFFF0000);
				icolor+=ilv2 & 0xFF;
				icolor+=(ilv1 & 0xFF)<<8;
			}
			else
			{
				icolor=control_drawproperties.color&(0xFFFF);
				icolor+=(ilv2 & 0xFF)<<16;
				icolor+=(ilv1 & 0xFF)<<24;
			}
			screen[posx+ilv2+(posy+ilv1)*gfx_screensizex]=icolor;
		}
	}
}
int sdl_buttonmenudraw(AUTOSTRUCT_PULLOUTLISTING_ * ilisting,int count,int xpos=0,int ypos=0)
{
	for (int ilv1=0;ilv1<count;ilv1++)
	{
		_u32 state=ilisting[ilv1].bgcolor;
		state=(*ilisting).bgcolor<<8;
		int tlvar=ilisting[ilv1].lmbmode;
		iagain:
		switch (tlvar)
		{
			case 1: if (control_tool==ilisting[ilv1].toolnr) state|=9;break;
			case 2: state|=(*((char*)(ilisting[ilv1].variable)) & 1); break;
			case 3: tlvar=ilisting[ilv1].rmbmode;if (tlvar!=3) goto iagain; break;
			case 0x111: state=(((*(int*)(ilisting[ilv1]).variable))<<8)|0x1;if (((ilisting+ilv1)==control_menuitem) && (control_mousestate & 0x20)) sdl_colorpaldraw(ilisting[ilv1].x*32+xpos+32,ilisting[ilv1].y*32+ypos+64);break;
			case 0x103: goto sliderdraw;break;
			case 4: 
			{
				int tlval=quersum(*(_u32*)(ilisting[ilv1].variable),STRUCTURE_OBJECTTYPE_ListSize);
				state=(((ilisting[ilv1].bgcolor & 0xFF)*tlval*256/STRUCTURE_OBJECTTYPE_ListSize)&0xFF00)+(((0xFF00)*(6-abs(tlval-4))*256/STRUCTURE_OBJECTTYPE_ListSize)&0xFF0000)+(((0xFF0000)*(8-tlval)*256/STRUCTURE_OBJECTTYPE_ListSize)&0xFF000000);
				state|=1*(tlval>0);state|=((((tlval*8)-1)/STRUCTURE_OBJECTTYPE_ListSize)&7)<<1; break;
			}
			case 6: state|=((*((_i32*)(ilisting[ilv1].variable)))==ilisting[ilv1].toolnr)?1:0;break;
		}
		sdl_toolboxitemdraw(ilisting[ilv1].x*32+xpos,ilisting[ilv1].y*32+ypos,ilisting[ilv1].picno,state);
		continue;
		sliderdraw:
		sdl_sliderdraw(ilisting[ilv1].x*32+xpos,ilisting[ilv1].y*32+ypos,ilisting[ilv1].picno,(ilisting+ilv1==control_menuitem)?(-control_menudragint):0,ilisting[ilv1].bgcolor);
		continue;
	}
		
	return 1;
}
void sdl_canvasframedraw()
{
	for (int ilv1=gfx_canvasminx;ilv1<gfx_canvasmaxx;ilv1++)
	{
		screen[(gfx_screensizex*gfx_canvasminy-1)+ilv1]=0;
	}
	for (int ilv1=gfx_canvasminx;ilv1<gfx_canvasmaxx;ilv1++)
	{
		screen[(gfx_screensizex*gfx_canvasmaxy)+ilv1]=0;
	}
	for (int ilv1=gfx_canvasminy;ilv1<gfx_canvasmaxy;ilv1++)
	{
		screen[(gfx_screensizex*ilv1-1)+gfx_canvasminx]=0;
	}
	for (int ilv1=gfx_canvasminy;ilv1<gfx_canvasmaxy;ilv1++)
	{
		screen[(gfx_screensizex*ilv1)+gfx_canvasmaxx]=0;
	}
}
structenum * searchreflectedstruct(const char * input);
int addmenu(const char * name,int type,int alignx=0,int aligny=0)
{
	structenum * tlstructenum;
	tlstructenum=searchreflectedstruct(name);
	if (tlstructenum)
	{
		menu_list[menu_list_count].type=type;
		menu_list[menu_list_count].what=*tlstructenum;
		menu_list[menu_list_count].alignx=alignx;
		menu_list[menu_list_count].aligny=aligny;
		menu_list_count++;
		return 1;
	}
	return 0;
}
extern int control_tool;
void sdl_filemenucommon()
{
	menu_list_count=0;
	if (control_filemenu_mode==0) addmenu("filedlg_buttons_load",4);
	if (control_filemenu_mode==1) addmenu("filedlg_buttons_save",4);
	addmenu("filedlg_lists",3);
}
int menu_itembyname(const char * name,int * menu,int * index)
{
	int inumber=0;
	for (int i_c_menu=0;i_c_menu<menu_list_count;i_c_menu++)
	{
		for (int i_c_index=0;i_c_index<menu_list[i_c_menu].what.count;i_c_index++)
		{
			if (strcmp((((AUTOSTRUCT_PULLOUTLISTING_*)(menu_list[i_c_menu].what.pointer))+i_c_index)->name,name)==0)
			{
				if (menu!=NULL) (*menu)=i_c_menu;
				if (index!=NULL) (*index)=i_c_index;
				return inumber;
			}
			inumber++;
		}
	}
	return -1;
}
void menu_itemwadethrough(int * inumber,int * menu,int * index,char direction)
{
	char endmode=direction;
	int i_c_number=0;int i_c_index=0;
	if (direction==0) {(*menu)=0;(*index)=0;} else (*inumber)=0;
	if ((*inumber)==-1) endmode=2;
	structenum * tl_structenum;
	for (int i_c_menu=0;i_c_menu<menu_list_count;i_c_menu++)
	{
		if ((direction==1) && (i_c_menu==(*menu))) {(*inumber)=i_c_number+(*index);return;}
		i_c_number+=menu_list[i_c_menu].what.count;
		if ((direction==0) && (i_c_number>(*inumber))) {i_c_index=(*inumber)-i_c_number+menu_list[i_c_menu].what.count;}
		if ((i_c_index>=(*index)) && (i_c_menu>=(*menu)) && (i_c_number>(*inumber)) && (endmode!=2))
		{
			(*menu)=i_c_menu;(*index)=i_c_index;
			return;
		}
	}
	if (endmode==0) (*inumber)=0;
	if (endmode==2) (*inumber)=i_c_number-1;
	return;
}
void sdl_commonmenucommon()
{
	char tlstring[60];
	char tlstring2[60];
	menu_list_count=0;
	addmenu("toolbox",0);
	for (int ilv1=0;ilv1<AUTOSTRUCT_PULLOUTLISTING_toolbox_Size;ilv1++)
	{
		if (AUTOSTRUCT_PULLOUTLISTING_toolbox[ilv1].lmbmode==1)
		{
			int length=strlen(AUTOSTRUCT_PULLOUTLISTING_toolbox[ilv1].name);
			int cut=strchr(AUTOSTRUCT_PULLOUTLISTING_toolbox[ilv1].name,' ')-AUTOSTRUCT_PULLOUTLISTING_toolbox[ilv1].name; 
			if (cut<length)length=cut;
			snprintf(tlstring,60,"toolspecific_%.*s",length,AUTOSTRUCT_PULLOUTLISTING_toolbox[ilv1].name);
			tlstring[59]=0;
			if (AUTOSTRUCT_PULLOUTLISTING_toolbox[ilv1].toolnr==control_tool)
			{
				addmenu(tlstring,0,gfx_screensizex-160,128);
				goto iknowwhichsubmenu;
			}
			else
			{
				structenum * tl_structenum=searchreflectedstruct(tlstring);
				if (tl_structenum)
				{
					AUTOSTRUCT_PULLOUTLISTING_ * tl_pulloutlisting=(AUTOSTRUCT_PULLOUTLISTING_*)(tl_structenum->pointer);
					int tl_count=tl_structenum->count;
					if (tl_structenum)
					{
						for (int ilv1=0;ilv1<tl_count;ilv1++)
						{
							if (tl_pulloutlisting[ilv1].lmbmode==1)
							{
								if (tl_pulloutlisting[ilv1].toolnr==control_tool)
								{
									int length2=strlen(tl_pulloutlisting[ilv1].name);
									int cut2=strchr(tl_pulloutlisting[ilv1].name,' ')-tl_pulloutlisting[ilv1].name; 
									if (cut2<length2) length2=cut2;
									snprintf(tlstring2,60,"toolspecific_%.*s",length2,tl_pulloutlisting[ilv1].name);
									tlstring2[59]=0;
									goto iknowwhichsubmenu;
								}
							}
						}
					}
				}
			}
		}
	}
	goto submenunotfound;
	iknowwhichsubmenu:;
	addmenu(tlstring,0,gfx_screensizex-160,128);
	submenunotfound:;
	if (control_mousestate & 8)
	{
		sprintf(tlstring,"submenu_%s",menu_matrixsubmenuvariable);
		addmenu(tlstring,1,256,96);
	}
	if (control_mousestate & 0x10)
	{
		//TODO: insert ordinary popups here and tell between PSE
		menu_list[menu_list_count].type=2;
		menu_list[menu_list_count].alignx=160;
		menu_list[menu_list_count].aligny=160;
		menu_list_count++;
	}
}
void sdl_psedraw(int istartx,int istarty)
{
	for (int ilv1=0;ilv1<sizeof(element)/sizeof(element_);ilv1++)
	{
		int borderx=istartx+element[ilv1].PSEX*32;
		int bordery=istarty+element[ilv1].PSEY*48;
		int bordercolor=0;
		if (ilv1==control_drawproperties.Element)
		{
			bordercolor=0xFF9F00;
		}
		for (int ilv2=bordery;ilv2<bordery+48;ilv2++)
		{
			for (int ilv3=borderx;ilv3<borderx+32;ilv3++)
			{
				screen[ilv2*gfx_screensizex+ilv3]=0xFFFFFF;
			}
			screen[ilv2*gfx_screensizex+borderx+31]=bordercolor;
			screen[ilv2*gfx_screensizex+borderx]=bordercolor;
		}
		for (int ilv2=borderx;ilv2<borderx+32;ilv2++)
		{
			screen[bordery*gfx_screensizex+ilv2]=bordercolor;
			screen[(bordery+47)*gfx_screensizex+ilv2]=bordercolor;
		}
		printmenutext(istartx+element[ilv1].PSEX*32+10,istarty+element[ilv1].PSEY*48+24,element[ilv1].name,NULL,0,0,strlen(element[ilv1].name));
	}
}
int sdl_menudraw()
{
	int i_menu;int i_index;
	menu_itemwadethrough(&menu_selectedmenuelement,&i_menu,&i_index,0);
	for (int ilv1=0;ilv1<menu_list_count;ilv1++)
	{
		int tl_index=(i_menu==ilv1)?i_index:-1;
		if (menu_list[ilv1].type==0)
		{
			sdl_buttonmenudraw((AUTOSTRUCT_PULLOUTLISTING_*)menu_list[ilv1].what.pointer,menu_list[ilv1].what.count,menu_list[ilv1].alignx,menu_list[ilv1].aligny);
		}
		if (menu_list[ilv1].type==1)
		{
			sdl_textmenudraw((AUTOSTRUCT_PULLOUTLISTING_*)menu_list[ilv1].what.pointer,menu_list[ilv1].what.count,menu_list[ilv1].alignx,menu_list[ilv1].aligny);
		}
		if (menu_list[ilv1].type==2)
		{
			sdl_psedraw(menu_list[ilv1].alignx,menu_list[ilv1].aligny);
		}
		if (menu_list[ilv1].type==3)
		{
			sdl_listmenudraw((AUTOSTRUCT_PULLOUTLISTING_*)menu_list[ilv1].what.pointer,menu_list[ilv1].what.count,menu_list[ilv1].alignx,menu_list[ilv1].aligny,tl_index);
		}
		if (menu_list[ilv1].type==4)
		{
			sdl_textbuttonmenudraw((AUTOSTRUCT_PULLOUTLISTING_*)menu_list[ilv1].what.pointer,menu_list[ilv1].what.count,menu_list[ilv1].alignx,menu_list[ilv1].aligny);
		}
	}
	return 0;
}
void menuframeline(int x,int y,int maxx,int maxy)
{
	for (int ilv1=y;ilv1<maxy-1;ilv1++)
	{
		*((char*)&(screen[gfx_screensizex*ilv1+x]))=0x00;
		*(((char*)&(screen[gfx_screensizex*ilv1+x]))+2)=0xFF;
		*((char*)&(screen[gfx_screensizex*ilv1+(maxx-1)]))=0x00;
		*(((char*)&(screen[gfx_screensizex*ilv1+(maxx-1)]))+2)=0xFF;
	}
	for (int ilv2=x;ilv2<maxx-1;ilv2++)
	{
		*((char*)&(screen[gfx_screensizex*y+ilv2]))=0x00;
		*(((char*)&(screen[gfx_screensizex*y+ilv2]))+2)=0xFF;
		*((char*)&(screen[gfx_screensizex*(maxy-1)+ilv2]))=0x00;
		*(((char*)&(screen[gfx_screensizex*(maxy-1)+ilv2]))+2)=0xFF;
	}
}
void sdl_menuframe()
{
	int menunr;int index;
	menu_itemwadethrough(&menu_selectedmenuelement,&menunr,&index,0);
	AUTOSTRUCT_PULLOUTLISTING_ * ilisting=(((AUTOSTRUCT_PULLOUTLISTING_*)(menu_list[menunr].what.pointer))+index);
	menuframeline((*ilisting).x,(*ilisting).y,(*ilisting).maxx,(*ilisting).maxy);
	return;
}
void sdl_commonmenudraw()
{
	sdl_commonmenucommon();
	sdl_menudraw();
}
void sdl_filemenudraw()
{
	sdl_filemenucommon();
	sdl_menudraw();
}
extern int control_hotatom;
void draw_reticle()
{
	if (control_reticle)
	{
		if ((control_mousey<gfx_screensizey) && (control_mousey>=0))
		{
			for (int ilv1=0;ilv1<gfx_screensizex;ilv1++)
			{
				screen[gfx_screensizex*(control_mousey)+ilv1]^=0xFFFFFF;
			}
		}
		if ((control_mousex<gfx_screensizex) && (control_mousex>=0))
		{
			for (int ilv1=0;ilv1<gfx_screensizey;ilv1++)
			{
				screen[gfx_screensizex*(ilv1)+control_mousex]^=0xFFFFFF;
			}
		}
	}
}
int sdl_selectiondraw()
{
	int internalpointcount;
	_u32 icompare;
	int isize;
	char * ibufferpos;
	for (int ilv0=0;ilv0<5;ilv0++)
	{
		for (int ilv1=0;ilv1<100;ilv1++)
		{
			if (control_doubleclickenergy>ilv1)
			{
				screen[160+ilv1+((96+ilv0)*gfx_screensizex)]=0xFF0000;
			}
			else
			{
				screen[160+ilv1+((96+ilv0)*gfx_screensizex)]=0x3F0000;
			}
		}
	}
	if (control_mousestate==1)
	{
		if ((control_tool==2) || (control_tool==5))
		{
			gfx_expressline(selection_frame.startx,selection_frame.starty,selection_frame.endx,selection_frame.starty);
			gfx_expressline(selection_frame.startx,selection_frame.starty,selection_frame.startx,selection_frame.endy);
			gfx_expressline(selection_frame.startx,selection_frame.endy,selection_frame.endx,selection_frame.endy);
			gfx_expressline(selection_frame.endx,selection_frame.starty,selection_frame.endx,selection_frame.endy);
		}
		if (control_tool==3)
		{
			for (int ilv1=0;ilv1<gfx_canvassizey;ilv1++)
			{
				for (int ilv2=0;ilv2<gfx_canvassizex;ilv2++)
				{
					if (selection_lassobuffer[ilv1*gfx_canvassizex+ilv2]) screen[(ilv1+gfx_canvasminy)*gfx_screensizex+ilv2+gfx_canvasminx]=0x000000;
				}
			}
		}
	}
	for (int ilv1=0;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
	{
		int follower3=0;
		icompare=1<<ilv1;
		internalpointcount=retrieveprops_basic(1,ilv1);
		int isize= STRUCTURE_OBJECTTYPE_List[ilv1].size;
		basicmultilist * tlmultilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
		if (tlmultilist==NULL) goto i_fertig;
		CDXMLREAD_functype tldummy;
		ibufferpos=(char*)((*tlmultilist).pointer);
		float tlpx,tlpy;
		SDL_color=0x00FF00;
		for (int ilv2=0;ilv2<(*tlmultilist).filllevel;ilv2++)
		{
			if ((*((basic_instance*)(ibufferpos+isize*ilv2))).exist)
			{
				int ilv3=0;
				iback:
				if (retrievepoints_basic(((basic_instance*)(ibufferpos+isize*ilv2)),&tlpx,&tlpy,NULL,ilv3,ilv1)>0)
				{
					if (((selection_clickabilitymatrix.types2[2] & (1<<ilv1)) && (ilv3!=0)) || ((selection_clickabilitymatrix.types2[1] & (1<<ilv1)) && (ilv3==0)))
					{
						if ((((selection_currentselection[ilv2]) & icompare)>0) ^ (ilv3!=0))
						{
							draw_drawmarkpoint((tlpx-SDL_scrollx)*SDL_zoomx-3+gfx_canvasminx,(tlpy-SDL_scrolly)*SDL_zoomy-3+gfx_canvasminy,47,(ilv3!=0),0);
						}
					}
					if (ilv3>0)
					{
						if (internalpointcount>0)
						{
							follower3=ilv2*internalpointcount+ilv3-1;
						}
						if ((selection_currentselection[follower3]) & (1<<(ilv1+STRUCTURE_OBJECTTYPE_ListSize)))
						{
							draw_drawmarkpoint((tlpx-SDL_scrollx)*SDL_zoomx-3+gfx_canvasminx,(tlpy-SDL_scrolly)*SDL_zoomy-3+gfx_canvasminy,47,5,0);
						}
						follower3++;
					}
					ilv3++;
					goto iback;
				}
			}
		}
		i_fertig:;
	}
		float tlpx,tlpy;
		SDL_color=0x3F3F00;
					if (control_hot[STRUCTURE_OBJECTTYPE_n]!=-1)
					{
					int ilv3=0;
					iback2:
						if (retrievepoints_basic(((basic_instance*)(((char*)(*glob_n_multilist).pointer)+sizeof(n_instance)*control_hot[STRUCTURE_OBJECTTYPE_n])),&tlpx,&tlpy,NULL,ilv3,STRUCTURE_OBJECTTYPE_n)>0)
						{
							draw_drawmarkpoint((tlpx-SDL_scrollx)*SDL_zoomx-3+gfx_canvasminx,(tlpy-SDL_scrolly)*SDL_zoomy-3+gfx_canvasminy,47,4,0);
							ilv3++;
							goto iback2;
						}
					}
	return 0;
}
