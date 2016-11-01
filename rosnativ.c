/* This unit contains all windows native stuff
It is only used for Win32 compatible systems like MS or ReactOS.
*/
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
HINSTANCE W32_hInst;
char szWindowClass[]="WindowMain1";
int schirm[1000000];
LRESULT CALLBACK W32_WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	int wmId,wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	switch (message)
	{
		case WM_COMMAND:
		wmId=LOWORD(wParam);
		wmEvent=HIWORD(wParam);
		switch (wmId)
		{
			case 100:
			{
				DestroyWindow(hWnd);
				break;
			}
			default:
			return DefWindowProc(hWnd,message,wParam,lParam);
		}
		break;
		case WM_PAINT:
		{
			hdc=BeginPaint(hWnd,&ps);
			HDC hdcmem=CreateCompatibleDC(hdc);
			HDC hbcmem=CreateCompatibleDC(hdc);
			RECT rc;
			GetClientRect(hWnd,&rc);
			HBITMAP hbm_memdc=CreateCompatibleBitmap(hdcmem,rc.right,rc.bottom);
			HBITMAP hbm_memdc_old=(HBITMAP)SelectObject(hdc,hbm_memdc);
			SelectObject(hdc,GetStockObject(DC_PEN));
			SelectObject(hdc,GetStockObject(DC_BRUSH));
			SetDCBrushColor(hdc,RGB(255,0,0));
			SetDCPenColor(hdc,RGB(255,255,0));
			Ellipse(hdc,10,10,100,100);
			Rectangle(hdc,-10,-10,10,10);
			for (int ilv1=0;ilv1<200;ilv1++)
			{
				for (int ilv2=0;ilv2<320;ilv2++)
				{
					float lastr=0;
					float lasti=0;
					int wert=1;
					float largest=0;
					for (int ilv3=0;ilv3<10;ilv3++)
					{
						float newi=2*lastr*lasti+(ilv1/160.0);
						float newr=lastr*lastr-lasti*lasti-(ilv2/160.0);
						if (largest<(lastr*lastr+lasti*lasti))
						{
							largest=(lastr*lastr+lasti*lasti);
						}
						lastr=newr;
						lasti=newi;
						if ((newi==0) && (newr==0))
						{
							wert=0;
						}
					}
					schirm[ilv2*320+ilv1]=RGB(255*((largest)>100),255*(wert==0),0);
				}
			}
			BITMAPINFO iBitmapInfo;
			iBitmapInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
			iBitmapInfo.bmiHeader.biBitCount=32;
			iBitmapInfo.bmiHeader.biWidth=320;
			iBitmapInfo.bmiHeader.biHeight=200;
			iBitmapInfo.bmiHeader.biCompression=0;
			iBitmapInfo.bmiHeader.biPlanes=1;
			iBitmapInfo.bmiHeader.biSizeImage=1000000;
			iBitmapInfo.bmiHeader.biClrUsed=0;
			iBitmapInfo.bmiHeader.biClrImportant=0;
			iBitmapInfo.bmiHeader.biXPelsPerMeter=1000;
			iBitmapInfo.bmiHeader.biYPelsPerMeter=1000;
			int iret;
			iret=SetDIBitsToDevice(hdc,0,0,320,200,0,0,0,200,schirm,&iBitmapInfo,DIB_RGB_COLORS);
			//if (iret=GDI_ERROR) {exit(1);}
			printf("%i",iret);
			EndPaint(hWnd, &ps);
			break;
		}
		default:
		{
			return DefWindowProc(hWnd,message,wParam,lParam);
		}
	}
}
ATOM W32_MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize=sizeof(WNDCLASSEX);
	wcex.style=CS_HREDRAW|CS_VREDRAW;
	wcex.lpfnWndProc=W32_WndProc;
	wcex.cbClsExtra=0;
	wcex.cbWndExtra=0;
	wcex.hInstance=hInstance;
	wcex.hIcon=NULL;
	wcex.hCursor=NULL;
	wcex.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName=NULL;
	wcex.lpszClassName=szWindowClass;
	wcex.hIconSm=NULL;
	return RegisterClassEx(&wcex);
}
BOOL W32_InitInstance(HINSTANCE hInstance,int nCmdShow)
{
	HWND hWnd;
	W32_MyRegisterClass(hInstance);
	hWnd=CreateWindow(szWindowClass,"lhendraw",WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,0,CW_USEDEFAULT,0,NULL,NULL,hInstance,NULL);
	if (!hWnd)
	{
		return 0;
	}
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return 1;
}
int main(int argc,char argv)
{
	W32_hInst=GetModuleHandle(NULL);
	if (!W32_InitInstance(W32_hInst,1))
	{
		printf("Windows init failed");
		return 0;
	}
	while(1)
	{
		Sleep(100);
	}
	printf("OK");
	return 1;
}
