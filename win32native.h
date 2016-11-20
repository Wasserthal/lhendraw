/* This unit contains all windows native stuff
It is only used for Win32 compatible systems like MS or ReactOS.
*/
//#include <stdio.h>
//#include <stdlib.h>
#include <windows.h>
#define SDL_HWSURFACE 0
#define SDL_SWSURFACE 0
#define SDL_RESIZABLE 0
#define SDL_INIT_VIDEO 0
#define SDL_FULLSCREEN 1
HWND W32_window;
int W32_window_set=0;
HINSTANCE W32_hInst;
char szWindowClass[]="WindowMain1";
_u32 schirm[1400000];
int W32_painting=0;
typedef struct
{
	_u32 * pixels;
	_u32 flags;
}SDL_Surface_;
extern _u32 * screen;
SDL_Surface_ W32_surface={schirm,1};
extern int gfx_screensizex;
#define SDL_Surface SDL_Surface_
int W32_Surfacelock=0;
typedef enum
{
	SDLK_0=0,
	SDLK_1=1,
	SDLK_2,
	SDLK_3,
	SDLK_4,
	SDLK_5,
	SDLK_6,
	SDLK_7,
	SDLK_8,
	SDLK_9,
	SDLK_a,
	SDLK_AC_BACK,
	SDLK_AC_BOOKMARKS,
	SDLK_AC_FORWARD,
	SDLK_AC_HOME,
	SDLK_AC_REFRESH,
	SDLK_AC_SEARCH,
	SDLK_AC_STOP,
	SDLK_AGAIN,
	SDLK_ALTERASE,
	SDLK_QUOTE,
	SDLK_APPLICATION,
	SDLK_AUDIOMUTE,
	SDLK_AUDIONEXT,
	SDLK_AUDIOPLAY,
	SDLK_AUDIOPREV,
	SDLK_AUDIOSTOP,
	SDLK_b,
	SDLK_BACKSLASH,
	SDLK_BACKSPACE,
	SDLK_BRIGHTNESSDOWN,
	SDLK_BRIGHTNESSUP,
	SDLK_c,
	SDLK_CALCULATOR,
	SDLK_CANCEL,
	SDLK_CAPSLOCK,
	SDLK_CLEAR,
	SDLK_CLEARAGAIN,
	SDLK_COMMA,
	SDLK_COMPUTER,
	SDLK_COPY,
	SDLK_CRSEL,
	SDLK_CURRENCYSUBUNIT,
	SDLK_CURRENCYUNIT,
	SDLK_CUT,
	SDLK_d,
	SDLK_DECIMALSEPARATOR,
	SDLK_DELETE,
	SDLK_DISPLAYSWITCH,
	SDLK_DOWN,
	SDLK_e,
	SDLK_EJECT,
	SDLK_END,
	SDLK_EQUALS,
	SDLK_ESCAPE,
	SDLK_EXECUTE,
	SDLK_EXSEL,
	SDLK_f,
	SDLK_F1,
	SDLK_F10,
	SDLK_F11,
	SDLK_F12,
	SDLK_F13,
	SDLK_F14,
	SDLK_F15,
	SDLK_F16,
	SDLK_F17,
	SDLK_F18,
	SDLK_F19,
	SDLK_F2,
	SDLK_F20,
	SDLK_F21,
	SDLK_F22,
	SDLK_F23,
	SDLK_F24,
	SDLK_F3,
	SDLK_F4,
	SDLK_F5,
	SDLK_F6,
	SDLK_F7,
	SDLK_F8,
	SDLK_F9,
	SDLK_FIND,
	SDLK_g,
	SDLK_BACKQUOTE,
	SDLK_h,
	SDLK_HELP,
	SDLK_HOME,
	SDLK_i,
	SDLK_INSERT,
	SDLK_j,
	SDLK_k,
	SDLK_KBDILLUMDOWN,
	SDLK_KBDILLUMTOGGLE,
	SDLK_KBDILLUMUP,
	SDLK_KP0,
	SDLK_KP00,
	SDLK_KP000,
	SDLK_KP1,
	SDLK_KP2,
	SDLK_KP3,
	SDLK_KP4,
	SDLK_KP5,
	SDLK_KP6,
	SDLK_KP7,
	SDLK_KP8,
	SDLK_KP9,
	SDLK_KPA,
	SDLK_KP_AMPERSAND,
	SDLK_KP_AT,
	SDLK_KP_B,
	SDLK_KP_BACKSPACE,
	SDLK_KP_BINARY,
	SDLK_KP_C,
	SDLK_KP_CLEAR,
	SDLK_KP_CLEARENTRY,
	SDLK_KP_COLON,
	SDLK_KP_COMMA,
	SDLK_KP_D,
	SDLK_KP_DBLAMPERSAND,
	SDLK_KP_DBLVERTICALBAR,
	SDLK_KP_DECIMAL,
	SDLK_KP_DIVIDE,
	SDLK_KP_E,
	SDLK_KP_ENTER,
	SDLK_KP_EQUALS,
	SDLK_KP_EQUALSAS400,
	SDLK_KP_EXCLAM,
	SDLK_KP_F,
	SDLK_KP_GREATER,
	SDLK_KP_HASH,
	SDLK_KP_HEXADECIMAL,
	SDLK_KP_LEFTBRACE,
	SDLK_KP_LEFTPAREN,
	SDLK_KP_LESS,
	SDLK_KP_MEMADD,
	SDLK_KP_MEMCLEAR,
	SDLK_KP_MEMDIVIDE,
	SDLK_KP_MEMMULTIPLY,
	SDLK_KP_MEMRECALL,
	SDLK_KP_MEMSTORE,
	SDLK_KP_MEMSUBTRACT,
	SDLK_KP_MINUS,
	SDLK_KP_MULTIPLY,
	SDLK_KP_OCTAL,
	SDLK_KP_PERCENT,
	SDLK_KP_PERIOD,
	SDLK_KP_PLUS,
	SDLK_KP_PLUSMINUS,
	SDLK_KP_POWER,
	SDLK_KP_RIGHTBRACE,
	SDLK_KP_RIGHTPAREN,
	SDLK_KP_SPACE,
	SDLK_KP_TAB,
	SDLK_KP_VERTICALBAR,
	SDLK_KP_XOR,
	SDLK_l,
	SDLK_LALT,
	SDLK_LCTRL,
	SDLK_LEFT,
	SDLK_LEFTBRACKET,
	SDLK_LGUI,
	SDLK_LSHIFT,
	SDLK_m,
	SDLK_MAIL,
	SDLK_MEDIASELECT,
	SDLK_MENU,
	SDLK_MINUS,
	SDLK_MODE,
	SDLK_MUTE,
	SDLK_n,
	SDLK_NUMLOCKCLEAR,
	SDLK_o,
	SDLK_OPER,
	SDLK_OUT,
	SDLK_p,
	SDLK_PAGEDOWN,
	SDLK_PAGEUP,
	SDLK_PASTE,
	SDLK_PAUSE,
	SDLK_PERIOD,
	SDLK_POWER,
	SDLK_PRINTSCREEN,
	SDLK_PRIOR,
	SDLK_q,
	SDLK_r,
	SDLK_RALT,
	SDLK_RCTRL,
	SDLK_RETURN,
	SDLK_RETURN2,
	SDLK_RGUI,
	SDLK_RIGHT,
	SDLK_RIGHTBRACKET,
	SDLK_RSHIFT,
	SDLK_s,
	SDLK_SCROLLLOCK,
	SDLK_SELECT,
	SDLK_SEMICOLON,
	SDLK_SEPARATOR,
	SDLK_SLASH,
	SDLK_SLEEP,
	SDLK_SPACE,
	SDLK_STOP,
	SDLK_SYSREQ,
	SDLK_t,
	SDLK_TAB,
	SDLK_THOUSANDSSEPARATOR,
	SDLK_u,
	SDLK_UNDO,
	SDLK_UNKNOWN,
	SDLK_UP,
	SDLK_v,
	SDLK_VOLUMEDOWN,
	SDLK_VOLUMEUP,
	SDLK_w,
	SDLK_WWW,
	SDLK_x,
	SDLK_y,
	SDLK_z,
	SDLK_AMPERSAND,
	SDLK_ASTERISK,
	SDLK_AT,
	SDLK_CARET,
	SDLK_COLON,
	SDLK_DOLLAR,
	SDLK_EXCLAIM,
	SDLK_GREATER,
	SDLK_HASH,
	SDLK_LEFTPAREN,
	SDLK_LESS,
	SDLK_PERCENT,
	SDLK_PLUS,
	SDLK_QUESTION,
	SDLK_QUOTEDBL,
	SDLK_RIGHTPAREN,
	SDLK_UNDERSCORE,
	SDLK_LSUPER,
	SDLK_RSUPER,
}SDLKey;
typedef enum
{
KMOD_NONE=0,
KMOD_LSHIFT,
KMOD_RSHIFT,
KMOD_LCTRL,
KMOD_RCTRL,
KMOD_LALT,
KMOD_RALT,
KMOD_LMETA,
KMOD_RMETA,
KMOD_NUM,
KMOD_CAPS,
KMOD_MODE,
KMOD_CTRL,
KMOD_SHIFT,
KMOD_ALT,
KMOD_META,
}SDLMod;
typedef enum 
{
SDL_BUTTON_LEFT=1,
SDL_BUTTON_RIGHT,
SDL_BUTTON_WHEELUP,
SDL_BUTTON_WHEELDOWN,
}SDL_enum_BUTTONS;
typedef struct{
	_u8 scancode;
	SDLKey sym;
	SDLMod mod;
	_u16 unicode;
}SDL_keysym;
typedef struct
{
	_u8 type;
	_u8 state;
	SDL_keysym keysym;
}SDL_KeyboardEvent;
typedef struct
{
	_u8 type;
	_u8 state;
	_u16 x,y;
	_u16 xrel,yrel;
}SDL_MouseMotionEvent;
typedef struct
{
	_u8 type;
	_u8 button;
	_u8 state;
	_u8 x,y;
}SDL_MouseButtonEvent;
typedef struct
{
	_u8 type;
	_i16 w,h;
}SDL_ResizeEvent;
typedef union
{
	_u8 type;
	SDL_KeyboardEvent key;
	SDL_MouseMotionEvent motion;
	SDL_MouseButtonEvent button;
	SDL_ResizeEvent resize;
}SDL_Event;
#define SDL_DEFAULT_REPEAT_DELAY 100
#define SDL_DEFAULT_REPEAT_INTERVAL 10
void SDL_EnableKeyRepeat(int delay,int interval)
{
	//TODO
}
void SDL_WM_ToggleFullScreen(SDL_Surface * i_surface)
{
	return;
}
void SDL_EnableUNICODE(int input)
{
}
typedef enum
{
	SDL_MOUSEMOTION=1,
	SDL_KEYUP,
	SDL_KEYDOWN,
	SDL_MOUSEBUTTONUP,
	SDL_MOUSEBUTTONDOWN,
	SDL_VIDEORESIZE,
	SDL_QUIT=100,
}SDL_EventTypes;
void SDL_WM_SetCaption(const char * input1,const char * input2)
{
	//TODO
}
void SDL_WM_SetIcon()
{
	//TODO
}
SDL_Surface * SDL_SetVideoMode(int i_screensizex,int i_screensizey,int i_colordepth,_uXX i_flags)
{
	//Do nothing, all is done in SDL_Init
	return &W32_surface;//Return happy nothing, because that stuff is done in situ in the window redraw procedure
}
_u32 * winscreen=(_u32*)malloc(10000000);
void SDL_UpdateRect(SDL_Surface * i_surface,int i_left,int i_top,int gfx_screensizex,int gfx_screensizey)
{
	PAINTSTRUCT ps;
	HDC hdc;
	MSG msg;
	while (PeekMessage(&msg,W32_window,0,0,0)>0)
	{
		if (GetMessage(&msg,W32_window,0,0)>0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	if (W32_window_set!=0)
	{
		for (int ilv1=0;ilv1<480;ilv1++)
		{
			for (int ilv2=0;ilv2<480;ilv2++)
			{
				schirm[(479-ilv2)*640+ilv1]=screen[ilv2*gfx_screensizex+ilv1];
			}
		}
	}
	InvalidateRect(W32_window,NULL,0);
	UpdateWindow(W32_window);
	return;
}
const char * SDL_GetError()
{
	return "GDI32 error";
}
int SDL_MUSTLOCK(SDL_Surface * i_Surface)
{
	return 1;
}
int SDL_LockSurface(SDL_Surface * i_Surface)
{
	i_Surface->pixels=winscreen;
	W32_Surfacelock=1;
	return 1;
}
int SDL_UnlockSurface(SDL_Surface * i_Surface)
{
	W32_Surfacelock=0;
	return 1;
}
int W32_mousex=0;
int W32_mousey=0;
int W32_lastbuttonstate=0;
int SDL_PollEvent(SDL_Event * i_Event)
{
	POINT lppoint;
	GetCursorPos(&lppoint);
	RECT lprect;
	GetWindowRect(W32_window,&lprect);
	int border_thicknessy=GetSystemMetrics(SM_CYCAPTION);
	int border_thicknessx=GetSystemMetrics(SM_CXSIZEFRAME);
	lppoint.x-=lprect.left+border_thicknessx;
	lppoint.y-=lprect.top+border_thicknessy;
	if ((lppoint.x!=W32_mousex) || (lppoint.y!=W32_mousey))
	{
		(*i_Event).type=SDL_MOUSEMOTION;
		(*i_Event).motion.type=SDL_MOUSEMOTION;
		(*i_Event).motion.state=W32_lastbuttonstate;
		(*i_Event).motion.xrel=lppoint.x-W32_mousex;
		(*i_Event).motion.yrel=lppoint.y-W32_mousey;
		(*i_Event).motion.x=lppoint.x;
		(*i_Event).motion.y=lppoint.y;
		W32_mousex=lppoint.x;
		W32_mousey=lppoint.y;
		return 1;
	}
	int lbuttonstate=(((GetAsyncKeyState(VK_LBUTTON)!=0)*(1<<SDL_BUTTON_LEFT)) | ((GetAsyncKeyState(VK_RBUTTON)!=0)*(1<<SDL_BUTTON_RIGHT)));
	for (int ilv1=1;ilv1<3;ilv1++)
	{
		if ((W32_lastbuttonstate&(1<<ilv1))!=(lbuttonstate&(1<<ilv1)))
		{
			(*i_Event).type=((lbuttonstate&(1<<ilv1))!=0)?SDL_MOUSEBUTTONDOWN:SDL_MOUSEBUTTONUP;
			(*i_Event).button.state=lbuttonstate;
			(*i_Event).button.button=ilv1;
			(*i_Event).button.x=W32_mousex;
			(*i_Event).button.y=W32_mousey;
			W32_lastbuttonstate&=~(1<<ilv1);
			W32_lastbuttonstate|=(lbuttonstate&(1<<ilv1));
			return 1;
		}
	}
	return 0;
}
void SDL_WarpMouse(int x,int y)
{
	//TODO
}
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
				W32_window=hWnd;
				W32_window_set=1;
				hdc=BeginPaint(hWnd,&ps);
				HDC hdcmem=CreateCompatibleDC(hdc);
				RECT rc;
				GetClientRect(hWnd,&rc);
				HBITMAP hbm_memdc=CreateCompatibleBitmap(hdcmem,rc.right,rc.bottom);
				HBITMAP hbm_memdc_old=(HBITMAP)SelectObject(hdc,hbm_memdc);
				BITMAPINFO iBitmapInfo;
				iBitmapInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
				iBitmapInfo.bmiHeader.biBitCount=32;
				iBitmapInfo.bmiHeader.biWidth=640;
				iBitmapInfo.bmiHeader.biHeight=480;
				iBitmapInfo.bmiHeader.biCompression=0;
				iBitmapInfo.bmiHeader.biPlanes=1;
				iBitmapInfo.bmiHeader.biSizeImage=1400000;
				iBitmapInfo.bmiHeader.biClrUsed=0;
				iBitmapInfo.bmiHeader.biClrImportant=0;
				iBitmapInfo.bmiHeader.biXPelsPerMeter=1000;
				iBitmapInfo.bmiHeader.biYPelsPerMeter=1000;
				int iret;
				iret=SetDIBitsToDevice(hdc,0,0,640,480,0,0,0,480,schirm,&iBitmapInfo,DIB_RGB_COLORS);
				EndPaint(hWnd, &ps);
			break;
		}
		default:
		{
			return DefWindowProc(hWnd,message,wParam,lParam);
		}
	}
	return 1;
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
	wcex.hbrBackground=(HBRUSH)COLOR_WINDOW;
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
int SDL_Init(_uXX i_flags)
{
	W32_hInst=GetModuleHandle(NULL);
	if (!W32_InitInstance(W32_hInst,1))
	{
		printf("Win32-api init failed");
		return 0;
	}
	printf("OK");
	return 1;
}
