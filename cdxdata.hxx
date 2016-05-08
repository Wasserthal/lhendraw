#ifndef _i32
#define _i32 int
#endif
#define bezierpointmax 128
/* This unit contains the readers for all datatypes that are no xml tags */
extern int getbufferfromstructure(basicmultilist * input,TELESCOPE_buffer * * bufferptr);
extern basic_instance_nested * currentinstance;
#define arcfloat float
struct cdx_Point2D
{
	float x,y;
};
inline void clear_cdx_Point2D(cdx_Point2D & input)
{
	input.x=0;input.y=0;
}
struct cdx_Point3D
{
	float x,y,z;
	char active;
};
inline void clear_cdx_Point3D(cdx_Point3D & input)
{
	input.x=0;input.y=0;input.z=0;
	input.active=0;
}
inline void clear_arcfloat(arcfloat & ifloat)
{
	ifloat=0;
}

struct cdx_Rectangle
{
	float left,top,right,bottom;
};
inline void clear_cdx_Rectangle(cdx_Rectangle & input)
{
	input.left=0;input.top=0;input.right=0;input.bottom=0;
}

struct cdx_String /*can be filled both by Property value and inter-Object-Text.
If it is filled from PCDATA, its name is PCDATA*/
{
	char a[stringlength+1];
};

struct cdx_Buffered_String /*can be filled both by Property value and inter-Object-Text.
If it is filled from PCDATA, its name is PCDATA*/
{
	char * a;
	int count;
};

inline void clear_cdx_Buffered_String(cdx_Buffered_String & input) /*can be filled both by Property value and inter-Object-Text.
If it is filled from PCDATA, its name is PCDATA*/
{
	input.a=NULL;
};

struct cdx_Bezierpoints
{
	cdx_Point2D a[bezierpointmax]; //TODO**** turn this, and strings, into buffer indices
	int count;
};

struct cdx_Pointreferences
{
	int a[bezierpointmax]; //TODO**** turn this, and strings, into buffer indices
	int count;
};

struct cdx_tlcspot
{
	float Rf;
	_u32 color;
	_u32 CurveType;
};

inline void clear_cdx_String(cdx_String & input)
{
	input.a[0]=0;
}
struct cdx_Stylerun
{
	_i16 startpos;
	_i16 font;
	_i16 face;
	_i16 size;
	_i16 color;
};

int __attribute__((sysv_abi))CDXMLREAD_INVALID(char * input,void * output)
{
	error("invalid READ Operation. Break-in attempt assumed");
	exit(1);
}
int __attribute__((sysv_abi))CDXMLWRITE_INVALID(char * input,void * output)
{
	error("invalid WRITE Operation. Break-in attempt assumed");
	exit(1);
}
int __attribute__((sysv_abi))CDXMLWRITE_cdx_String(char * input,void * output)
{
	fprintf((FILE*)output,"%s",input);
	return 0;
}
int __attribute__((sysv_abi))CDXMLREAD_cdx_String(char * input,void * output)
{
	strncpy(&((*((cdx_String*)output)).a[0]),input,stringlength);
	((*((cdx_String*)output)).a[stringlength])=0;
	//HACK: strings can be in one piece only.
	return 0;
}
int __attribute__((sysv_abi))CDXMLREAD_BIN_cdx_String(char * input,void * output)
{
	return CDXMLREAD_cdx_String(input,output);
}
int __attribute__((sysv_abi))CDXMLWRITE_BIN_cdx_String(char * input,void * output)
{
	static int length=strlen(input)+1;
	fwrite(&length,2,1,(FILE*)output);
	fprintf((FILE*)output,"%s",input);
	return strlen(input)+1;
}

int writefrombuffer(FILE * output,cdx_Buffered_String * input)
{
	char * instring=input->a;
	for (int ilv1=0;(instring[ilv1]!=0)&&(ilv1<input->count);ilv1++)
	{
		for (int ilv2=0;ilv2<sizeof(list_xml)/sizeof(list_bookstavecode);ilv2++)
		{
			if (instring[ilv1]==list_xml[ilv2].unicode[0])
			{
				fprintf(output,"&%s;",list_xml[ilv2].name);
				goto ifertig;
			}
		}
		if (instring[ilv1] & 0x80)
		{
			for (int ilv2=0;ilv2<list_greeklist_size;ilv2++)
			{
				if (strncmp(list_greeklist[ilv2].output,instring+ilv1,strlen(list_greeklist[ilv2].output))==0)
				{
					ilv1+=strlen(list_greeklist[ilv2].output)-1;
					fprintf(output,"%s",list_greeklist[ilv2].input);
					goto ifertig;
				}
			}
		}
		fprintf(output,"%c",instring[ilv1]);
		ifertig:;
	}
	return 0;
}
int copytobuffer(TELESCOPE_buffer * ibuffer,char * input)
{
	int ilv2;
	int maxlength=LHENDRAW_buffersize-2;
	_u32 icount=(*ibuffer).count;
	if (icount>2147483648) icount=2147483648;
	for (_u32 ilv1=0;input[ilv1]!=0;ilv1++)
	{
		if (input[ilv1]!='&')
		{
			if (icount>=maxlength-1)
			{
				(*ibuffer).buffer[icount++]=0;
				(*ibuffer).count=icount;
				return -1;
			}
			(*ibuffer).buffer[icount++]=input[ilv1];
		}
		else
		{
			for (ilv2=ilv1;;ilv2++)
			{
				if (input[ilv2]==0) return -20;
				if (input[ilv2]==';') {input[ilv2]=0;goto ifound;}
			}
			ifound:
			for (int ilv3=0;ilv3<sizeof(list_xml)/sizeof(list_bookstavecode);ilv3++)
			{
				if (strcmp(list_xml[ilv3].name,input+ilv1+1)==0)
				{
					for (int ilv4=0;ilv4<7;ilv4++)
					{
						if (list_xml[ilv3].unicode[ilv4]==0)
						{
							goto ifertig;
						}
						else
						{
							if (icount>=maxlength-1)
							{
								(*ibuffer).buffer[icount++]=0;
								(*ibuffer).count=icount;
								return -1;
							}
							(*ibuffer).buffer[icount++]=list_xml[ilv3].unicode[ilv4];
						}
					}
					goto ifertig;
				}
			}
			ifertig:
			ilv1=ilv2;
		}
	}
	(*ibuffer).buffer[icount++]=0;
	(*ibuffer).count=icount;
	return 0;
}
int copytobuffer_unconverted(TELESCOPE_buffer * ibuffer,char * input)
{
	int ilv2;
	int maxlength=LHENDRAW_buffersize-2;
	_u32 icount=(*ibuffer).count;
	if (icount>2147483648) icount=2147483648;
	for (_u32 ilv1=0;input[ilv1]!=0;ilv1++)
	{
		if (icount>=maxlength-1)
		{
			(*ibuffer).buffer[icount++]=0;
			(*ibuffer).count=icount;
			return -1;
		}
		(*ibuffer).buffer[icount++]=input[ilv1];
	}
	(*ibuffer).buffer[icount++]=0;
	(*ibuffer).count=icount;
	return 0;
}
int __attribute__((sysv_abi))CDXMLWRITE_cdx_Buffered_String(char * input,void * output)
{
	return writefrombuffer((FILE*)output,((cdx_Buffered_String*)input));
}
int __attribute__((sysv_abi))CDXMLREAD_cdx_Buffered_String(char * input,void * output)
{
	if ((*((cdx_Buffered_String*)output)).a!=NULL)
	{
		goto found;
	}
	else
	{
		TELESCOPE_buffer * buffer;
		if (getbufferfromstructure(findmultilist((*currentinstance).getFullName()),&buffer))
		{
			(*((cdx_Buffered_String*)output)).a=(*buffer).buffer+((*buffer).count);
			int tl_backval=copytobuffer(buffer,input);
			if (tl_backval<0)
			{
				(*((cdx_Buffered_String*)output)).count=0;
				(*((cdx_Buffered_String*)output)).a=NULL;
			}
			return tl_backval;
		}
		else
		{
			return -2;
		}
		return 0;
	}
	found:
	TELESCOPE_buffer * buffer;
	if (getbufferfromstructure(findmultilist((*currentinstance).getFullName()),&buffer))
	{
		if ((*((cdx_Buffered_String*)output)).a+strlen((*((cdx_Buffered_String*)output)).a)==(*buffer).buffer+(*buffer).count-1)
		{
			return copytobuffer(buffer,input);
		}
	}
	else
	{
		return -2;
	}
	return 0;
}
int __attribute__((sysv_abi))CDXMLREAD_BIN_cdx_Buffered_String(char * input,void * output)
{
	if ((*((cdx_Buffered_String*)output)).a!=NULL)
	{
		goto found;
	}
	else
	{
		TELESCOPE_buffer * buffer;
		if (getbufferfromstructure(findmultilist((*currentinstance).getFullName()),&buffer))
		{
			(*((cdx_Buffered_String*)output)).a=(*buffer).buffer+((*buffer).count);
			int tl_backval=copytobuffer_unconverted(buffer,input);
			if (tl_backval<0)
			{
				(*((cdx_Buffered_String*)output)).count=0;
				(*((cdx_Buffered_String*)output)).a=NULL;
			}
			return tl_backval;
		}
		else
		{
			return -2;
		}
		return 0;
	}
	found:
	TELESCOPE_buffer * buffer;
	if (getbufferfromstructure(findmultilist((*currentinstance).getFullName()),&buffer))
	{
		if ((*((cdx_Buffered_String*)output)).a+strlen((*((cdx_Buffered_String*)output)).a)==(*buffer).buffer+(*buffer).count-1)
		{
			return copytobuffer_unconverted(buffer,input);
		}
	}
	else
	{
		return -2;
	}
	return 0;
}
int __attribute__((sysv_abi))CDXMLWRITE_BIN_cdx_Buffered_String(char * input,void * output)
{
	static int length=strlen(input)+1;
	fwrite(&length,2,1,(FILE*)output);
	fprintf((FILE*)output,"%s",input);
	static char zero=0;
	fwrite(&zero,1,1,(FILE*)output);
	return strlen(input)+1;
}


int edit_readcolortablefrombuffer(char * input);
int edit_writecolortabletobuffer(char * input,void * output);
int edit_readfonttablefrombuffer(char * input);
int edit_writefonttabletobuffer(char * input,void * output);
int edit_readrepresentfrombuffer(char * input);
int edit_writerepresenttobuffer(char * input,void * output);
int edit_readsfrombuffer(char * input);
int edit_writestobuffer(char * input,void * output);
int __attribute__((sysv_abi))CDXMLREAD_BIN_represent(char * input,void * output)
{
	return edit_readrepresentfrombuffer(input);
}
int __attribute__((sysv_abi))CDXMLWRITE_BIN_represent(char * input,void * output)
{
	return edit_writerepresenttobuffer(input,output);
}
int __attribute__((sysv_abi))CDXMLREAD_BIN_colortable(char * input,void * output)
{
	return edit_readcolortablefrombuffer(input);
}
int __attribute__((sysv_abi))CDXMLWRITE_BIN_colortable(char * input,void * output)
{
	return edit_writecolortabletobuffer(input,output);
}
int __attribute__((sysv_abi))CDXMLREAD_BIN_fonttable(char * input,void * output)
{
	return edit_readfonttablefrombuffer(input);
}
int __attribute__((sysv_abi))CDXMLWRITE_BIN_fonttable(char * input,void * output)
{
	return edit_writefonttabletobuffer(input,output);
}
int __attribute__((sysv_abi))CDXMLREAD_BIN_s(char * input,void * output)
{
	return edit_readsfrombuffer(input);
}
int __attribute__((sysv_abi))CDXMLWRITE_BIN_s(char * input,void * output)
{
	return edit_writestobuffer(input,output);
}
int __attribute__((sysv_abi))CDXMLWRITE__i32(char * input,void * output)
{
	fprintf((FILE*)output,"%i",*((_i32*)input));
	return 0;
}
int __attribute__((sysv_abi))CDXMLREAD_BIN__i32(char * input,void * output)
{
	*((_i32*)output)=*((_i32*)input);
	return 4;
}
int __attribute__((sysv_abi))CDXMLWRITE_BIN__i32(char * input,void * output)
{
	static int length=4;
	fwrite(&length,2,1,(FILE*)output);
	fwrite(input,4,1,(FILE*)output);
	return 4;
}
int CDXMLWRITE_BIN__i32_headerless(int input,FILE * output)//helps procedures that write multiple int's
{
	fwrite(&input,4,1,(FILE*)output);
	return 4;
}
int __attribute__((sysv_abi))CDXMLWRITE_BIN__i16(char * input,void * output)
{
	static int length=2;
	fwrite(&length,2,1,(FILE*)output);
	fwrite(input,2,1,(FILE*)output);//ENDIAN!!!
	return 4;
}
int __attribute__((sysv_abi))CDXMLWRITE_BIN__x8(char * input,void * output)
{
	fwrite(input,1,1,(FILE*)output);
	return 1;
}
int __attribute__((sysv_abi))CDXMLREAD_BIN__x8(char * input,void * output)
{
	static int length=1;
	fwrite(&length,2,1,(FILE*)output);
	*((_u8*)output)=*((_u8*)input);
	return 1;
}
int __attribute__((sysv_abi))CDXMLREAD__i32(char * input,void * output)
{
	int ilv1;
	_i32 wert=0;
	ilv1=0;
	char negative=0;
	while (spaciatic(input[ilv1]))
	{
		ilv1++;
	}
	if (input[ilv1]=='-')
	{
		ilv1++;
		negative^=1;
	}
	iback:
	if (!((input[ilv1]<48) || (input[ilv1]>=58)))
	{
		wert+=input[ilv1]-48;
	}
	ilv1++;
	if ((input[ilv1]>=48) && (input[ilv1]<58))
	{
		wert*=10;
		goto iback;
	}
	if (negative) wert=-wert;
	*((_i32*)output)=wert;
	return ilv1;
}
int __attribute__((sysv_abi))CDXMLWRITE__i8(char * input,void * output)
{
	fprintf((FILE*)output,"%hhi",*((_i8*)input));
	return 0;
}
int __attribute__((sysv_abi))CDXMLREAD_BIN__i8(char * input,void * output)
{
	*((_i8*)output)=*((_i8*)input);
	return 0;
}
int __attribute__((sysv_abi))CDXMLWRITE_BIN__i8(char * input,void * output)
{
	static int length=1;
	fwrite(&length,2,1,(FILE*)output);
	fwrite(input,1,1,(FILE*)output);
	return 0;
}
int __attribute__((sysv_abi))CDXMLREAD__i8(char * input,void * output)
{
	int ilv1;
	_i8 wert=0;
	ilv1=0;
	char negative;
	while (spaciatic(input[ilv1]))
	{
		ilv1++;
	}
	if (input[ilv1]=='-')
	{
		ilv1++;
		negative^=1;
	}
	iback:
	if (!((input[ilv1]<48) || (input[ilv1]>=58)))
	{
		wert+=input[ilv1]-48;
	}
	ilv1++;
	if ((input[ilv1]>=48) && (input[ilv1]<58))
	{
		wert*=10;
		goto iback;
	}
	if (negative) wert=-wert;
	*((_i8*)output)=wert;
	return ilv1;
}

int __attribute__((sysv_abi))CDXMLREAD_BIN_float(char * input,void * output)
{
	*(float*)output=*(double*)input;
	return 0;
}
int __attribute__((sysv_abi))CDXMLWRITE_BIN_float(char * input,void * output)
{
	static int length=sizeof(float);
	fwrite(&length,2,1,(FILE*)output);
	fwrite(input,sizeof(float),1,(FILE*)output);
	return sizeof(float);
}
int __attribute__((sysv_abi))CDXMLWRITE_float(char * input,void * output)
{
	fprintf((FILE*)output,"%f",*((float*)input));
	return 0;
}
int __attribute__((sysv_abi))CDXMLREAD_float(char * input,void * output)
{
	int ilv1;
	float wert=0;
	char commamode=0;
	float factor=0.1;
	char negative;
	negative=0;
	ilv1=0;
	while (spaciatic(input[ilv1]))
	{
		ilv1++;
	}
	if (input[ilv1]=='-')
	{
		ilv1++;
		negative=1;
	}
	goto ientry;
	iback:
	if (commamode==0)
	{
		wert+=input[ilv1]-48;
	}
	else
	{
		wert+=(input[ilv1]-48)*factor;
		factor=factor*0.1;
	}
	idone:
	ilv1++;
	if (((input[ilv1]>='0') && (input[ilv1]<='9'))||(input[ilv1]=='.'))
	{
		ientry:
		if (input[ilv1]=='.')
		{
			commamode=1;
			factor=0.1;
			goto idone;
		}
		if (!commamode)
		{
			wert*=10;
		}
		goto iback;
	}
	*((float*)output)=negative ? -wert:wert;
	return ilv1;
}
int __attribute__((sysv_abi))CDXMLREAD_arcfloat(char * input,void * output)
{
	return CDXMLREAD_float(input,output);
}
int __attribute__((sysv_abi))CDXMLWRITE_arcfloat(char * input,void * output)
{
	return CDXMLWRITE_float(input,output);
}
int __attribute__((sysv_abi))CDXMLWRITE_BIN_arcfloat(char * input,void * output)
{
	_i16 wert=(*(float*)input)*10.0;
	int length=2;
	fwrite(&length,2,1,(FILE*)output);
	fwrite(&wert,2,1,(FILE*)output);
	return 2;
}
int __attribute__((sysv_abi))CDXMLREAD_BIN_arcfloat(char * input,void * output)
{
	*(float*)output=(*(_i32*)input)/10.0;
	return 0;
}

int __attribute__((sysv_abi))CDXMLWRITE__x8(char * input,void * output)
{
	fprintf((FILE*)output,"%hhX.%hhX",(char)((*((unsigned char *) input))/16),(char)((*((unsigned char *) input))%16));
	return 0;
}
int __attribute__((sysv_abi))CDXMLREAD__x8(char * input,void * output)
{
	int ilv1;
	unsigned char ihv1;
	_x8 wert=0;
	char commamode=0;
	_x8 factor=1;
	char negative;
	negative=0;
	ilv1=0;
	while (spaciatic(input[ilv1]))
	{
		ilv1++;
	}
	if (input[ilv1]=='-')
	{
		ilv1++;
		negative=1;
	}
	goto ientry;
	iback:
	ihv1=(input[ilv1]);
	if (ihv1>='a')
	{
		ihv1+=10-'a';
	}
	if (ihv1>='A')
	{
		ihv1+=10-'A';
	}
	if (ihv1>='0')
	{
		ihv1-=48;
	}
	if (commamode==0)
	{
		wert+=ihv1*16;
	}
	else
	{
		wert+=ihv1*factor;
		factor=factor/16;
	}
	idone:
	ilv1++;
	if ((input[ilv1]!=0) && (!spaciatic(input[ilv1])))
	{
		ientry:
		if (input[ilv1]=='.')
		{
			commamode=1;
			factor=1;
			goto idone;
		}
		if (!commamode)
		{
			wert*=16;
		}
		goto iback;
	}
	*((_x8*)output)=negative ? -wert:wert;
	return ilv1;
}

int __attribute__((sysv_abi))CDXMLWRITE_cdx_Bezierpoints(char * input,void * output)
{
	cdx_Bezierpoints * list=(cdx_Bezierpoints*)input;
	for (int ilv1=0;ilv1<(*list).count;ilv1++)
	{
		fprintf((FILE*)output," %f %f ",(((cdx_Bezierpoints*)input)->a+ilv1)->x,(((cdx_Bezierpoints*)input)->a+ilv1)->y);
	}
	return 0;
}
int __attribute__((sysv_abi))CDXMLREAD_cdx_Bezierpoints(char * input,void * output)
{
	int ilv1;
	cdx_Bezierpoints * list=(cdx_Bezierpoints*)output;
	char ended=0;
	(*list).count=0;
	ilv1=0;
	iback:
	if ((*list).count>bezierpointmax-1) {(*list).count=bezierpointmax-1;return ilv1;}
	ilv1+=CDXMLREAD_float(input+ilv1,&((*list).a[(*list).count].x));
	while(spaciatic(input[ilv1])) ilv1++;
	if (input[ilv1]==0) return ilv1;
	ilv1+=CDXMLREAD_float(input+ilv1,&((*list).a[(*list).count].y));
	while(spaciatic(input[ilv1])) ilv1++;
	if (input[ilv1]==0) ended=1;
	(*list).count++;
	if (!ended) goto iback;
	return ilv1;
}

int __attribute__((sysv_abi))CDXMLREAD_BIN_cdx_Pointreferences(char * input,void * output)
{
	//TODO: Buffered!
	cdx_Pointreferences * list=(cdx_Pointreferences*)output;
	(*list).count=paramvaluestring_length/4;//Well this is just as hacky as the file format itself...
	//TODO: Buffered_overflow_check!
	if ((*list).count>=bezierpointmax) {(*list).count=0;return -1;}
	for (int ilv1=0;ilv1<(*list).count;ilv1++)
	{
		list->a[ilv1]=((_i32*)input)[ilv1];
	}
	return 0;
}
int __attribute__((sysv_abi))CDXMLWRITE_BIN_cdx_Pointreferences(char * input,void * output)
{
	//TODO: Buffered!
	cdx_Pointreferences * list=(cdx_Pointreferences*)input;
	(*list).count=paramvaluestring_length/4;//Well this is just as hacky as the file format itself...
	int length=(*list).count*4;
	fwrite(&length,2,1,(FILE*)output);
	for (int ilv1=0;ilv1<(*list).count;ilv1++)
	{
		fwrite(&(list->a[ilv1]),4,1,(FILE*)output);
	}
	return length;
}
int __attribute__((sysv_abi))CDXMLWRITE_cdx_Pointreferences(char * input,void * output)
{
	cdx_Pointreferences * list=(cdx_Pointreferences*)input;
	for (int ilv1=0;ilv1<(*list).count;ilv1++)
	{
		fprintf((FILE*)output," %i ",*(((*list).a)+ilv1));
	}
	return 0;
}
int __attribute__((sysv_abi))CDXMLREAD_cdx_Pointreferences(char * input,void * output)
{
	//TODO: Buffered!
	int ilv1;
	cdx_Pointreferences * list=(cdx_Pointreferences*)output;
	char ended=0;
	(*list).count=0;
	ilv1=0;
	iback:
	if (ilv1>=bezierpointmax-1) {(*list).count=ilv1;return ilv1;}
	ilv1+=CDXMLREAD__i32(input+ilv1,&((*list).a[(*list).count]));
	while(spaciatic(input[ilv1])) ilv1++;
	if (input[ilv1]==0) ended=1;
	(*list).count++;
	if (!ended) goto iback;
	return ilv1;
}

int __attribute__((sysv_abi))CDXMLWRITE_cdx_Rectangle(char * input,void * output)
{
	cdx_Rectangle * wert=(cdx_Rectangle*)input;
	fprintf((FILE*)output," %f %f %f %f ",(*wert).left,(*wert).top,(*wert).right,(*wert).bottom);
	return 0;
}
int __attribute__((sysv_abi))CDXMLREAD_BIN_cdx_Rectangle(char * input,void * output)
{
		
	int ilv1;
	cdx_Rectangle wert;
	_i32 ileft,itop,iright,ibottom;
	ilv1=CDXMLREAD_BIN__i32(input,&itop);
	ilv1+=CDXMLREAD_BIN__i32(input+ilv1,&ileft);
	ilv1+=CDXMLREAD_BIN__i32(input+ilv1,&ibottom);
	ilv1+=CDXMLREAD_BIN__i32(input+ilv1,&iright);
	wert.left=ileft/65536.0;
	wert.top=itop/65536.0;
	wert.right=iright/65536.0;
	wert.bottom=ibottom/65536.0;
	*((cdx_Rectangle*)output)=wert;
	return ilv1;
}
int __attribute__((sysv_abi))CDXMLWRITE_BIN_cdx_Rectangle(char * input,void * output)
{
		
	int ilv1;
	cdx_Rectangle wert;
	_i32 ileft,itop,iright,ibottom;
	static int length=16;
	fwrite(&length,2,1,(FILE*)output);
	wert=*((cdx_Rectangle*)input);
	ileft=wert.left*65536.0;
	iright=wert.right*65536.0;
	itop=wert.top*65536.0;
	ibottom=wert.bottom*65536.0;
	ilv1=CDXMLWRITE_BIN__i32_headerless(itop,(FILE*)output);
	ilv1+=CDXMLWRITE_BIN__i32_headerless(ileft,(FILE*)output);
	ilv1+=CDXMLWRITE_BIN__i32_headerless(ibottom,(FILE*)output);
	ilv1+=CDXMLWRITE_BIN__i32_headerless(iright,(FILE*)output);
	return ilv1;
}
int __attribute__((sysv_abi))CDXMLREAD_cdx_Rectangle(char * input,void * output)
{
		
	int ilv1;
	cdx_Rectangle wert;
	wert.left=0;
	wert.right=0;
	wert.top=0;
	wert.bottom=0;
	ilv1=CDXMLREAD_float(input,&(wert.left));
	ilv1+=CDXMLREAD_float(input+ilv1,&(wert.top));
	ilv1+=CDXMLREAD_float(input+ilv1,&(wert.right));
	ilv1+=CDXMLREAD_float(input+ilv1,&(wert.bottom));
	*((cdx_Rectangle*)output)=wert;
	return ilv1;
}
int __attribute__((sysv_abi))CDXMLWRITE_cdx_Point2D(char * input,void * output)
{
	cdx_Point2D * wert=(cdx_Point2D*)input;
	fprintf((FILE*)output," %f %f ",(*wert).x,(*wert).y);
	return 0;
}
int __attribute__((sysv_abi))CDXMLREAD_cdx_Point2D(char * input,void * output)
{
		
	int ilv1;
	cdx_Point2D wert;
	ilv1=CDXMLREAD_float(input,&(wert.x));
	ilv1+=CDXMLREAD_float(input+ilv1,&(wert.y));
	*((cdx_Point2D*)output)=wert;
	return ilv1;
}
int __attribute__((sysv_abi))CDXMLWRITE_cdx_Point3D(char * input,void * output)
{
	cdx_Point3D * wert=(cdx_Point3D*)input;
	fprintf((FILE*)output," %f %f %f ",(*wert).x,(*wert).y,(*wert).z);
	return 0;
}
int __attribute__((sysv_abi))CDXMLREAD_BIN_cdx_Point2D(char * input,void * output)
{
	int ilv1;
	_i32 ix,iy;
	cdx_Point2D wert;
	ilv1=CDXMLREAD_BIN__i32(input,&(iy));
	ilv1+=CDXMLREAD_BIN__i32(input+ilv1,&(ix));
	wert.y=iy/65536.0;
	wert.x=ix/65536.0;
	*((cdx_Point2D*)output)=wert;
	return ilv1;
}
int __attribute__((sysv_abi))CDXMLWRITE_BIN_cdx_Point2D_headerless(char * input,void * output)
{
	int ilv1;
	_i32 ix,iy,iz;
	cdx_Point2D wert=*((cdx_Point2D*)input);
	ix=wert.x*65536.0;
	iy=wert.y*65536.0;
	ilv1=CDXMLWRITE_BIN__i32_headerless((iy),(FILE*)output);
	ilv1+=CDXMLWRITE_BIN__i32_headerless((ix),(FILE*)output);
	return ilv1;
}
int __attribute__((sysv_abi))CDXMLWRITE_BIN_cdx_Point2D(char * input,void * output)
{
	int ilv1;
	_i32 ix,iy,iz;
	static int length=8;
	fwrite(&length,2,1,(FILE*)output);
	cdx_Point2D wert=*((cdx_Point2D*)input);
	ix=wert.x*65536.0;
	iy=wert.y*65536.0;
	ilv1=CDXMLWRITE_BIN__i32_headerless((iy),(FILE*)output);
	ilv1+=CDXMLWRITE_BIN__i32_headerless((ix),(FILE*)output);
	return ilv1;
}
int __attribute__((sysv_abi))CDXMLREAD_BIN_cdx_Point3D(char * input,void * output)
{
	int ilv1;
	_i32 ix,iy,iz;
	cdx_Point3D wert;
	ilv1=CDXMLREAD_BIN__i32(input,&(ix));
	ilv1+=CDXMLREAD_BIN__i32(input+ilv1,&(iy));
	ilv1+=CDXMLREAD_BIN__i32(input+ilv1,&(iz));
	wert.x=ix/65536.0;
	wert.y=iy/65536.0;
	wert.z=iz/65536.0;
	wert.active=1;
	*((cdx_Point3D*)output)=wert;
	return ilv1;
}
int __attribute__((sysv_abi))CDXMLWRITE_BIN_cdx_Point3D(char * input,void * output)
{
	int ilv1;
	_i32 ix,iy,iz;
	static int length=12;
	fwrite(&length,2,1,(FILE*)output);
	cdx_Point3D wert=*((cdx_Point3D*)input);
	iz=wert.z*65536.0;
	ix=wert.x*65536.0;
	iy=wert.y*65536.0;
	ilv1=CDXMLWRITE_BIN__i32_headerless((ix),(FILE*)output);
	ilv1+=CDXMLWRITE_BIN__i32_headerless((iy),(FILE*)output);
	ilv1+=CDXMLWRITE_BIN__i32_headerless((iz),(FILE*)output);
	return ilv1;
}
int __attribute__((sysv_abi))CDXMLREAD_cdx_Point3D(char * input,void * output)
{
		
	int ilv1;
	cdx_Point3D wert;
	ilv1=CDXMLREAD_float(input,&(wert.x));
	ilv1+=CDXMLREAD_float(input+ilv1,&(wert.y));
	ilv1+=CDXMLREAD_float(input+ilv1,&(wert.z));
	wert.active=1;
	*((cdx_Point3D*)output)=wert;
	return ilv1;
}
int __attribute__((sysv_abi))CDXMLREAD_BIN_cdx_Bezierpoints(char * input,void * output)
{
	int count=*(_u16*)input;
	(*((cdx_Bezierpoints*)output)).count=count;
	if (count>=bezierpointmax) {(*((cdx_Bezierpoints*)output)).count=0;return -1;}
	int intermediate=0;
	int ilv1=2;
	for (int ilv2=0;ilv2<count;ilv2++)
	{
		ilv1+=CDXMLREAD_BIN_cdx_Point2D(input+ilv1,((cdx_Bezierpoints*)output)->a+ilv2);
	}
	return ilv1;
}
int __attribute__((sysv_abi))CDXMLWRITE_BIN_cdx_Bezierpoints(char * input,void * output)
{
	int count=*(_u16*)input;
	int intermediate=0;
	int ilv1=2;
	cdx_Bezierpoints * list=(cdx_Bezierpoints*)input;
	static int length=8*list->count;
	fwrite(&length,2,1,(FILE*)output);
	for (int ilv2=0;ilv2<count;ilv2++)
	{
		ilv1+=CDXMLWRITE_BIN_cdx_Point2D(input+ilv1,((cdx_Bezierpoints*)output)->a+ilv2);
	}
	return ilv1;
}
struct trienum
{
	char name[40];
	intl number;
	intl size;
};
struct fuenum
{
	int owner;//which unit it came from
	char name[33];
	catalogized_command_functype function;
	int type;//0: single 1: iterated
};
struct structenum
{
	char name[33];
	int count;//actual count
	int max;//max numbers. only valid if list is intended to be variable.
	void * pointer;
	int type;//0: MENU pullout 1: stringlist, size contains symbol count
	int size;//the size of the type
	int number;//the currently selected element, if any, otherwise -1
	int scroll;//the current scroll-position in the list.
};
int set_bienum(bienum * ibienum,FILE * output,intl imax,intl value)
{
	for (int ilv1=0;ilv1<imax;ilv1++)
	{
		if (ibienum[ilv1].number==value)
		{
			fprintf(output,"%s",ibienum[ilv1].name);
		}
	}
	return 0;
};
intl get_bienum(bienum * ibienum,const char * input,intl count)
{
	intl ilv1;
	for (ilv1=0;ilv1<count;ilv1++)
	{
		if (strcmp(input,ibienum[ilv1].name)==0)
		{
			return ibienum[ilv1].number;
		}
	}
	return 0;
};
char * lookup_bienum(bienum * ibienum,intl imax,intl value)
{
	for (int ilv1=0;ilv1<imax;ilv1++)
	{
		if (ibienum[ilv1].number==(_u16)value)
		{
			return ibienum[ilv1].name;
		}
	}
	return NULL;
}
int set_bienum_multi(bienum * ibienum,FILE * output,intl imax,intl value)
{
	for (int ilv1=0;ilv1<imax;ilv1++)
	{
		if ((ibienum[ilv1].number & value)==ibienum[ilv1].number)
		{
			fprintf(output,"%s ",ibienum[ilv1].name);
		}
	}
	return 0;
};
intl get_bienum_multi(bienum * ibienum,const char * input,intl count)
{
	intl ilv1;
	intl length;
	intl wert=0;
	char again;
	char * currentpos=(char*)input;
	char * tlbackvalue;
	iback:
	again=0;
	length=strlen(currentpos);
	tlbackvalue=strchr(currentpos,' ');
	if (tlbackvalue!=NULL)
	{
		length=tlbackvalue-currentpos;
		again=1;
	}
	if (currentpos[0]==0) return wert;
	for (ilv1=0;ilv1<count;ilv1++)
	{
		if (strncmp(currentpos,ibienum[ilv1].name,length)==0)
		{
			wert|=ibienum[ilv1].number;
			goto ifound;
		}
	}
	fprintf(stderr,"unknown Element of mark %s in context",currentpos);
	ifound:
	if (again){currentpos=tlbackvalue+1; goto iback;}
	return wert;
};
