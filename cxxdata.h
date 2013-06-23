#ifndef _i32
#define _i32 int
#endif
/* This unit contains the readers for all datatypes that are no xml tags */
typedef struct cdx_enum
{
	int a;
};

typedef struct cdx_Point2D
{
	float x,y;
};
inline void clear_cdx_Point2D(cdx_Point2D & input)
{
	input.x=0;input.y=0;
}

typedef struct cdx_Rectangle
{
	float left,top,right,bottom;
};
inline void clear_cdx_Rectangle(cdx_Rectangle & input)
{
	input.left=0;input.top=0;input.right=0;input.bottom=0;
}

typedef struct cdx_String /*can be filled both by Property value and inter-Object-Text.
If it is filled from PCDATA, its name is PCDATA*/
{
	char a[stringlength];
};
inline void clear_cdx_String(cdx_String input)
{
	input.a[0]=0;
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
	printf("Jetzt kommt ein Int: %i!",wert);
	return ilv1;
}
int __attribute__((sysv_abi))CDXMLREAD_cdx_enum(char * input,void * output)
{
	CDXMLREAD__i32(input,&((*((cdx_enum*)output)).a));
}

int __attribute__((sysv_abi))CDXMLREAD_float(char * input,void * output)
{
	int ilv1;
	float wert=0;
	char commamode=0;
	float factor=0.1;
	ilv1=0;
	while (spaciatic(input[ilv1]))
	{
		ilv1++;
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
	*((float*)output)=wert;
	return ilv1;
}

int __attribute__((sysv_abi))CDXMLREAD_cdx_Rectangle(char * input,void * output)
{
		
	int ilv1;
	cdx_Rectangle wert;
	wert.left=0;
	ilv1=CDXMLREAD_float(input,&(wert.left));
	ilv1+=CDXMLREAD_float(input+ilv1,&(wert.top));
	ilv1+=CDXMLREAD_float(input+ilv1,&(wert.right));
	ilv1+=CDXMLREAD_float(input+ilv1,&(wert.bottom));
	*((cdx_Rectangle*)output)=wert;
	return ilv1;
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
