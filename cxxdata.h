#ifndef _i32
#define _i32 int
#endif

typedef struct cdx_color
{
	int R,G,B;
};

typedef struct cdx_Point2D
{
	_i32 x,y;
};

typedef struct cdx_Rectangle
{
	_i32 left,top,right,bottom;
};

typedef struct cdx_String /*can be filled both by Property value and inter-Object-Text.
If it is filled from PCDATA, its name is PCDATA*/
{
	char a[stringlength];
};

void __attribute__((sysv_abi))CDXMLREAD_cdx_color(char * input,void * output)
{
	return;
}
void __attribute__((sysv_abi))CDXMLREAD__i32(char * input,void * output)
{
	int ilv1;
	_i32 wert=0;
	ilv1=0;
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
	return;
}

void __attribute__((sysv_abi))CDXMLREAD_float(char * input,void * output)
{
	int ilv1;
	float wert=0;
	char commamode=0;
	float factor=0.1;
	ilv1=0;
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
	return;
}

void __attribute__((sysv_abi))CDXMLREAD_Rectangle(char * input,void * output)
{
		
	int ilv1;
	cdx_Rectangle wert;
	wert.left=0;
	ilv1=0;
	iback_left:
	wert.left+=input[ilv1]-48;
	ilv1++;
	if ((input[ilv1]!=0) && (!spaciatic(input[ilv1])))
	{
		wert.left*=10;
		goto iback_left;
	}
	if (input[ilv1]==0){printf("SCHEIẞE!");exit(1);}

	wert.top=0;
	ilv1=0;
	iback_top:
	wert.top+=input[ilv1]-48;
	ilv1++;
	if ((input[ilv1]!=0) && (!spaciatic(input[ilv1])))
	{
		wert.top*=10;
		goto iback_top;
	}
	if (input[ilv1]==0){printf("SCHEIẞE!");exit(1);}

	wert.right=0;
	ilv1=0;
	iback_right:
	wert.right+=input[ilv1]-48;
	ilv1++;
	if ((input[ilv1]!=0) && (!spaciatic(input[ilv1])))
	{
		wert.right*=10;
		goto iback_right;
	}
	if (input[ilv1]==0){printf("SCHEIẞE!");exit(1);}

	wert.bottom=0;
	ilv1=0;
	iback_bottom:
	wert.bottom+=input[ilv1]-48;
	ilv1++;
	if ((input[ilv1]!=0) && (!spaciatic(input[ilv1])))
	{
		wert.bottom*=10;
		goto iback_bottom;
	}

	*((cdx_Rectangle*)output)=wert;
	return;
}
