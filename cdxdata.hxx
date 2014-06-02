#ifndef _i32
#define _i32 int
#endif
#define bezierpointmax 128
/* This unit contains the readers for all datatypes that are no xml tags */
extern int getbufferfromstructure(basicmultilist * input,TELESCOPE_buffer * * bufferptr);
extern basic_instance * currentinstance;
struct cdx_enum
{
	int a;
};

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
	char * a;//TODO: add count, remove NULL-termination. or rather: collapse this entire structure type to a propertylist trail.
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

inline void clear_cdx_String(cdx_String & input)
{
	input.a[0]=0;
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

int writefrombuffer(FILE * output,char * input)
{
	for (int ilv1=0;input[ilv1]!=0;ilv1++)
	{
		for (int ilv2=0;ilv2<sizeof(list_xml)/sizeof(list_bookstavecode);ilv2++)
		{
			if (input[ilv1]==list_xml[ilv2].unicode[0])
			{
				fprintf(output,"&%s;",list_xml[ilv2].name);
				goto ifertig;
			}
		}
		fprintf(output,"%c",input[ilv1]);
		ifertig:;
	}
}
int copytobuffer(TELESCOPE_buffer * ibuffer,char * input)//TODO: what if string is longer than 2147483647 bytes?
{
	int ilv2;
	int maxlength=(*ibuffer).max-2;
	int icount=(*ibuffer).count;
	for (int ilv1=0;input[ilv1]!=0;ilv1++)
	{
		if (input[ilv1]!='&')
		{
			if (icount>=maxlength)
			{
				return -1;
			}
			(*ibuffer).buffer[icount++]=input[ilv1];
		}
		else
		{
			for (ilv2=ilv1;;ilv2++)
			{
				if (input[ilv2]==0) return -20;
				if (input[ilv2]==';') {input[ilv2++]=0;goto ifound;}
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
							if (icount>=maxlength)
							{
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
int __attribute__((sysv_abi))CDXMLWRITE_cdx_Buffered_String(char * input,void * output)
{
	writefrombuffer((FILE*)output,*(char**)input);
	return 0;
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

int __attribute__((sysv_abi))CDXMLWRITE__i32(char * input,void * output)
{
	fprintf((FILE*)output,"%i",*((_i32*)input));
	return 0;
}
int __attribute__((sysv_abi))CDXMLREAD__i32(char * input,void * output)
{
	int ilv1;
	_i32 wert=0;
	ilv1=0;
	while (spaciatic(input[ilv1]))
	{
		ilv1++;
	}
	iback:
	wert+=input[ilv1]-48;
	ilv1++;
	if ((input[ilv1]!=0) && (!spaciatic(input[ilv1])))
	{
		wert*=10;
		goto iback;
	}
	*((_i32*)output)=wert;
	return ilv1;
}
int __attribute__((sysv_abi))CDXMLWRITE__i8(char * input,void * output)
{
	fprintf((FILE*)output,"%hhi",*((_i32*)input));
	return 0;
}
int __attribute__((sysv_abi))CDXMLREAD__i8(char * input,void * output)
{
	int ilv1;
	_i8 wert=0;
	ilv1=0;
	while (spaciatic(input[ilv1]))
	{
		ilv1++;
	}
	iback:
	wert+=input[ilv1]-48;
	ilv1++;
	if ((input[ilv1]!=0) && (!spaciatic(input[ilv1])))
	{
		wert*=10;
		goto iback;
	}
	*((_i8*)output)=wert;
	return ilv1;
}
int __attribute__((sysv_abi))CDXMLWRITE_cdx_enum(char * input,void * output)
{
	fprintf((FILE*)output,"%i",(*((cdx_enum*)input)).a);
	return 0;
}
int __attribute__((sysv_abi))CDXMLREAD_cdx_enum(char * input,void * output)
{
	return CDXMLREAD__i32(input,&((*((cdx_enum*)output)).a));
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
	if ((input[ilv1]!=0) && (!spaciatic(input[ilv1])))
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

int __attribute__((sysv_abi))CDXMLWRITE__x8(char * input,void * output)
{
	fprintf((FILE*)output,"%hhX.%hhX",(*((unsigned char *) input))/16,(*((unsigned char *) input))%16);//TODO: div16
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
		fprintf((FILE*)output," %f ",*(((float*)input)+ilv1));
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
	int ilv1;
	cdx_Pointreferences * list=(cdx_Pointreferences*)output;
	char ended=0;
	(*list).count=0;
	ilv1=0;
	iback:
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
struct bienum
{
	char name[40];
	intl number;
};
struct trienum
{
	char name[40];
	intl number;
	intl size;
};
struct fuenum
{
	int owner;//which unit it came from
	char name[20];
	catalogized_command_functype function;
	int type;//0: single 1: iterated
};
struct structenum
{
	char name[25];
	int count;
	void * pointer;
	int type;//0: MENU pullout
	int size;//the size of the type
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
intl get_bienum(bienum * ibienum,char * input,intl count)
{
	intl ilv1;
	for (ilv1=0;ilv1<count;ilv1++)
	{
		if (strcmp(input,ibienum[ilv1].name)==0)
		{
			return ibienum[ilv1].number;
		}
	}
	fprintf(stderr,"unknown mark %s in context",input);
	return 0;
};
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
intl get_bienum_multi(bienum * ibienum,char * input,intl count)
{
	intl ilv1;
	intl wert=0;
	char again;
	char * currentpos=input;
	char * tlbackvalue;
	iback:
	again=0;
	tlbackvalue=strchr(currentpos,' ');
	if (tlbackvalue!=NULL)
	{
		*tlbackvalue=0;
		again=1;
	}
	if (input[0]==0) return 0;
	for (ilv1=0;ilv1<count;ilv1++)
	{
		if (strcmp(currentpos,ibienum[ilv1].name)==0)
		{
			wert|=ibienum[ilv1].number;
			goto ifound;
		}
	}
	fprintf(stderr,"unknown Element of mark %s in context",input);
	ifound:
	if (again){currentpos=tlbackvalue+1; goto iback;}
	return wert;
};
