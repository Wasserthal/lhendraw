/* This unit contains all windows native stuff
It is only used for Win32 compatible systems like MS or ReactOS.
*/
//#include <stdio.h>
//#include <stdlib.h>
#ifndef FULLCROSS
#include <windows.h>
#include <wingdi.h>
#else
typedef void*HANDLE;
typedef void*HGLOBAL;
typedef void*HINSTANCE;
typedef void*HDC;
typedef void*HWND;
typedef void*HBRUSH;
typedef void*HBITMAP;
typedef void*HGDIOBJ;
typedef void*HMENU;
#define LRESULT _u32
#define CALLBACK
typedef _u32 (CALLBACK *WNDPROC)(HWND,_u32,_u32,_uXX);
#define VK_LBUTTON 1
#define VK_RBUTTON 2
#define CREATE_NEW 1
#define CREATE_ALWAYS 2
#define OPEN_EXISTING 3
#define OPEN_ALWAYS 4
#define TRUNCATE_EXISTING 5
#define GENERIC_READ 0x80000000
#define GENERIC_WRITE 0x40000000
#define GENERIC_EXECUTE 0x20000000
#define FILE_ATTRIBUTE_NORMAL 0x00000080
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#define CF_TEXT 1
#define GMEM_MOVEABLE 2
#define HORZRES 8
#define PD_RETURNDC 0x00000100
#define SM_CXSIZEFRAME 32
#define SM_CYCAPTION 4
#define VERTRES 10
#define	POSTSCRIPT_PASSTHROUGH 4115
#define CS_HREDRAW 2
#define CS_VREDRAW 1
#define COLOR_WINDOW 5
#define DIB_RGB_COLORS 0
#define HIWORD(word) ((_u16)(((word)>>16)&0xFFFF))
#define LOWORD(word) ((_u16)((word)&0xFFFF))
#define SIZE_MAXIMIZED 2
#define SIZE_RESTORED 0
#define WM_COMMAND 273
#define WM_MOUSEWHEEL 522
#define WM_PAINT 15
#define WM_SIZE 5
#define WM_CLOSE 16
#define WM_QUIT 18
struct BITMAPINFOHEADER
{
	_u32 biSize;
	_i32 biWidth;
	_i32 biHeight;
	_u16 biPlanes;
	_u16 biBitCount;
	_u32 biCompression;
	_u32 biSizeImage;
	_i32 biXPelsPerMeter;
	_i32 biYPelsPerMeter;
	_u32 biClrUsed;
	_u32 biClrImportant;
};
struct BITMAPINFO{
BITMAPINFOHEADER bmiHeader;
_RGB bmiColors[1];
};
struct RECT
{
	_u32 left;
	_u32 top;
	_u32 right;
	_u32 bottom;
};
struct WNDCLASSEXA
{
	_u32 cbSize;
	_u32 style;
	WNDPROC lpfnWndProc;
	_i32 cbClsExtra;
	_i32 cbWndExtra;
	HINSTANCE hInstance;
	void*hIcon;
	void*hCursor;
	HBRUSH hbrBackground;
	char*lpszMenuName;
	char*lpszClassName;
	void*hIconSm;
};
struct PAINTSTRUCT
{
	HDC hdc;
	_u32 fErase;
	RECT rcPaint;
	_u32 fRestore;
	_u32 fIncUpdate;
	_u8 rgbReserved[32];
};
struct DOCINFOA
{
	_i32 cbSize;
	const char*lpszDocName;
	const _u8*lpszOutput;
	const _u8*lpszDatatype;
	_u32 fwType;
};
struct POINT
{
	_u32 x;
	_u32 y;
};
struct MSG
{
	HWND hwnd;
	_u32 message;
	_u32 wParam;
	_u32 lParam;
	_u32 time;
	POINT pt;
};
struct PRINTDLG
{
	_u32 lStructSize;
	HWND hwndOwner;
	HGLOBAL hDevMode;
	HGLOBAL hDevNames;
	HDC hDC;
	_u32 Flags;
	_u16 nFromPage;
	_u16 nToPage;
	_u16 nMinPage;
	_u16 nMaxPage;
	_u16 nCopies;
	HINSTANCE hInstance;
	_u32 lCustData;
	void*lpfnPrintHook;
	void*lpfnSetupHook;
	const _u8*lpPrintTemplateName;
	const _u8*lpSetupTemplateName;
	HGLOBAL hPrintTemplate;
	HGLOBAL hSetupTemplate;
};
#define alloca __builtin_alloca
extern "C" void*malloc(_u32);
extern "C" void free(void*);
extern "C" double atof(const char*);
extern "C" _i32 atoi(const char*);
extern "C" float strtof(const char*);
extern "C" double strtod(const char*);
extern "C" char*getenv(const char*);
extern "C" _i32 rand(void);
extern "C" void*realloc(void *,_u32 size);
extern "C" _i32 system(const char *command);
extern "C" _i32 abs(_i32);
extern "C" HANDLE __attribute__((__stdcall__))CreateFileA(const char*,_u32,_u32,void*,_u32,_u32,HANDLE);
extern "C" _u32 __attribute__((__stdcall__))DispatchMessageA(const MSG*);
extern "C" _u32 __attribute__((__stdcall__))GetDefaultPrinterA(char*,_u32*);
extern "C" _u32 __attribute__((__stdcall__))GetMessageA(MSG*,HWND,_u32,_u32);
extern "C" HINSTANCE __attribute__((__stdcall__))GetModuleHandleA(const _u8*);
extern "C" _u32 __attribute__((__stdcall__))MapVirtualKeyA(_u32,_u32);
extern "C" _u32 __attribute__((__stdcall__))PeekMessageA(MSG*,HWND,_u32,_u32,_u32);
extern "C" _u32 __attribute__((__stdcall__))PrintDlgA(PRINTDLG*);
extern "C" _u32 __attribute__((__stdcall__))RegisterClipboardFormatA(const char*);
extern "C" _i32 __attribute__((__stdcall__))StartDocA(HDC,const DOCINFOA*);
extern "C" _u32 __attribute__((__stdcall__))MessageBoxA(HWND,const char*,const char*,_u32);
#define WS_OVERLAPPEDWINDOW 0xcf0000
#define INVALID_HANDLE_VALUE (void*)(~(long)0)
extern "C" void __attribute__((__stdcall__))ExitProcess(long);
extern "C" void __attribute__((__stdcall__))Sleep(int);
extern "C" void exit(_u32);
extern "C" HDC __attribute__((__stdcall__))BeginPaint(HWND,PAINTSTRUCT*);
extern "C" _u32 __attribute__((__stdcall__))CloseClipboard(void);
extern "C" _u32 __attribute__((__stdcall__))CloseHandle(HANDLE);
extern "C" HBITMAP __attribute__((__stdcall__))CreateCompatibleBitmap(HDC,_i32,_i32);
extern "C" HDC __attribute__((__stdcall__))CreateCompatibleDC(HDC);
extern "C" _u32 __attribute__((__stdcall__))DeleteObject(HGDIOBJ);
extern "C" _u32 __attribute__((__stdcall__))DestroyWindow(HWND);
extern "C" _u32 __attribute__((__stdcall__))EmptyClipboard(void);
extern "C" _i32 __attribute__((__stdcall__))EndDoc(HDC);
extern "C" _i32 __attribute__((__stdcall__))EndPage(HDC);
extern "C" _u32 __attribute__((__stdcall__))EndPaint(HWND,const PAINTSTRUCT*);
extern "C" _i32 __attribute__((__stdcall__))ExtEscape(HDC,_i32,_i32,const char*,_i32,_u8*);
extern "C" _u32 __attribute__((__stdcall__))GetClientRect(HWND,RECT*);
extern "C" HANDLE __attribute__((__stdcall__))GetClipboardData(_u32);
extern "C" _u32 __attribute__((__stdcall__))GetCursorPos(POINT*);
extern "C" _i32 __attribute__((__stdcall__))GetDeviceCaps(HDC,_i32);
extern "C" _u32 __attribute__((__stdcall__))GetKeyboardState(_u8*);
extern "C" _u16 __attribute__((__stdcall__))GetKeyState(_i32);
extern "C" _i32 __attribute__((__stdcall__))GetSystemMetrics(_i32);
extern "C" _u32 __attribute__((__stdcall__))GetWindowRect(HWND,RECT*);
extern "C" HGLOBAL __attribute__((__stdcall__))GlobalAlloc(_u32,_u32);
extern "C" void*__attribute__((__stdcall__))GlobalLock(HGLOBAL);
extern "C" HGLOBAL __attribute__((__stdcall__))GlobalReAlloc(HGLOBAL,_u32,_u32);
extern "C" _u32 __attribute__((__stdcall__))GlobalUnlock(HGLOBAL);
extern "C" _u32 __attribute__((__stdcall__))InvalidateRect(HWND,const RECT*,_u32);
extern "C" _u32 __attribute__((__stdcall__))OpenClipboard(HWND);
extern "C" _u32 __attribute__((__stdcall__))ReadFile(HANDLE,void*,_u32,_uXX*,void*);
extern "C" HGDIOBJ __attribute__((__stdcall__))SelectObject(HDC,HGDIOBJ);
extern "C" HANDLE __attribute__((__stdcall__))SetClipboardData(_u32,HANDLE);
extern "C" _u32 __attribute__((__stdcall__))SetCursorPos(_i32,_i32);
extern "C" _i32 __attribute__((__stdcall__))SetDIBitsToDevice(HDC,_i32,_i32,_u32,_u32,_i32,_i32,_u32,_u32,const void*,const BITMAPINFO*,_u32);
extern "C" _u32 __attribute__((__stdcall__))SetFilePointer(HANDLE,_u32,_u32*,_u32);
extern "C" _u32 __attribute__((__stdcall__))ShowWindow(HWND,_i32);
extern "C" _i32 __attribute__((__stdcall__))StartPage(HDC);
extern "C" _i32 __attribute__((__stdcall__))ToUnicode(_u32,_u32,const _u8*,_u16*,_i32,_u32);
extern "C" _u32 __attribute__((__stdcall__))TranslateMessage(const MSG*);
extern "C" _u32 __attribute__((__stdcall__))UpdateWindow(HWND);
extern "C" _u32 __attribute__((__stdcall__))WriteFile(HANDLE,const void*,_u32,_uXX*,void*);
extern "C" _u32 __attribute__((__stdcall__))DefWindowProcA(HWND,_u32,_u32,_u32);
extern "C" _u16 __attribute__((__stdcall__))RegisterClassExA(const WNDCLASSEXA*);
extern "C" HWND __attribute__((__stdcall__))CreateWindowExA(_u32,char*,const char*,_u32,_i32,_i32,_i32,_i32,HWND,HMENU,HINSTANCE,void*);
extern "C" void*memcpy(void*,const void*,_u32);
extern "C" int memcmp(const void*,const void*,_u32);
extern "C" char*strcpy(char*,const char*);
extern "C" char*strncpy(char*,const char*,int);
extern "C" _u32 strcmp(const char*,const char*);
extern "C" _u32 strncmp(const char*,const char*,int);
extern "C" _u32 strlen(const char*);
extern "C" char*strstr(const char*,const char*);
extern "C" char*strchr(const char*,_u32);
extern "C" double tan(double);
extern "C" double sin(double);
extern "C" double cos(double);
extern "C" double atan(double);
extern "C" double asin(double);
extern "C" double acos(double);
extern "C" char*strcat(char*,const char*);
extern "C" char*strncat(char*,const char*,_u32);
struct dirent
{
	_u32 d_ino;
	_u16 int d_reclen;
	_u16 d_namelen;
	char*d_name;
};
struct winDIR
{
	_u32 dwFileAttributes;
	_u64 ftCreationTime;
	_u64 ftLastAccessTime;
	_u64 ftLastWriteTime;
	_u32 nFileSizeHigh;
	_u32 nFileSizeLow;
	_u32 dwReserved0;
	_u32 dwReserved1;
	char cFileName[260];
	char cAlternateFileName[14];
	_u32 stuffword1;
	_u32 stuffword2;
	_u32 stuffword3;
	_u16 stuffshort;
};
struct DIR
{
	_u32 windowshandle;
	_u32 fsm;//0: empty 1: one name waiting 2: await next read 3: empty after opening
	winDIR windowsstruct;
	dirent linuxstruct;
}__attribute__((packed));
DIR W32_DIR_instance={0};
extern "C" DIR*opendir(const char*);
extern "C" dirent*readdir(DIR*);
extern "C" void closedir(DIR*);
struct _jmp_buf
{
	_u32 ebp;
	_u32 ebx;
	_u32 edi;
	_u32 esi;
	_u32 esp;
	_u32 eip;
	_u32 reg;
	_u32 attempt;
	_u32 cookie;
	_u32 unwind_func;
	_u32 unwind_data[6];
};
typedef _jmp_buf jmp_buf[1];
extern "C" void longjmp(jmp_buf,_u32 val);
extern "C" double fabs(double);
extern "C" double fmod(double,double);
extern "C" double sqrt(double);
extern "C" double pow(double,double);
extern "C" void*memset(void*,_u32,_u32);
extern "C" double modf(double,double*);
extern "C" int __attribute__((__stdcall__))GetStdHandle(_i32);
extern "C" void __attribute__((__stdcall__))WriteConsoleA(_u32,const char*,_u32,void*,_u32);
extern "C" _u32 __attribute__((__stdcall__))FindFirstFileA(const char*,winDIR*);
extern "C" _u32 __attribute__((__stdcall__))FindNextFileA(_u32,winDIR*);
extern "C" void __attribute__((__stdcall__))FindClose(_u32);
extern "C" _u32 setjmp(jmp_buf env);
extern "C" char*GetCommandLineA();
#define binary_gfx_buttons_bmp_start _binary_gfx_buttons_bmp_start
#define binary_hotkeys_xml_start _binary_hotkeys_xml_start
#define binary_LiberationMono_Regular_bin_start _binary_LiberationMono_Regular_bin_start
#define binary_LiberationMono_Regular_lennardfont_start _binary_LiberationMono_Regular_lennardfont_start
#define binary_gfx_buttons_bmp_end _binary_gfx_buttons_bmp_end
#define binary_hotkeys_xml_end _binary_hotkeys_xml_end
#define binary_LiberationMono_Regular_bin_end _binary_LiberationMono_Regular_bin_end
#define binary_LiberationMono_Regular_lennardfont_end _binary_LiberationMono_Regular_lennardfont_end
double nan(_u8*irrelevant)
{
	char*nanstring="\x00\x00\x00\x00\x00\x00\xf8\x7f";
	return *(double*)(nanstring);
}
float nanf(_u8*irrelevant)
{
	char*nanstring="\x00\x00\xc0\x7f";
	return *(float*)(nanstring);
}
int abs(_i32 in)
{
	if (((_u32)in)==0x80000000) return 0x7FFFFFFF;
	if (in<0) return -in;
	return in;
}
#endif
#include "lennyWinIo.h"
#define SDL_HWSURFACE 0
#define SDL_SWSURFACE 0
#define SDL_RESIZABLE 0
#define SDL_INIT_VIDEO 0
#define SDL_FULLSCREEN 1
HWND W32_window;
int W32_window_set=0;
HINSTANCE W32_hInst;
char szWindowClass[]="WindowMain1";
_u32 schirm[2000000];
int W32_painting=0;
PRINTDLG W32_pd;
_u8 W32_default_printer_name[257];
typedef struct
{
	_u32 * pixels;
	_u32 flags;
}SDL_Surface_;
extern _u32 * screen;
SDL_Surface_ W32_surface={schirm,1};
extern int gfx_screensizex,gfx_screensizey;
#define SDL_Surface SDL_Surface_
int W32_Surfacelock=0;
typedef enum
{
	SDLK_BACKSPACE=0x08,
	SDLK_TAB=0x09,
	SDLK_CLEAR=0x0C,
	SDLK_RETURN=0x0D,
	SDLK_ENTER=0x0D,
	SDLK_CAPSLOCK=0x14,
	SDLK_ESCAPE=0x1B,
	SDLK_SPACE=0x20,
	SDLK_PAGEUP=0x21,
	SDLK_PAGEDOWN=0x22,
	SDLK_END=0x23,
	SDLK_HOME=0x24,
	SDLK_LEFT=0x25,
	SDLK_UP=0x26,
	SDLK_RIGHT=0x27,
	SDLK_DOWN=0x28,
	SDLK_PRINTSCREEN=0x2C,
	SDLK_INSERT=0x2D,
	SDLK_DELETE=0x2E,
	SDLK_0=0x30,
	SDLK_1=0x31,
	SDLK_2=0x32,
	SDLK_3=0x33,
	SDLK_4=0x34,
	SDLK_5=0x35,
	SDLK_6=0x36,
	SDLK_7=0x37,
	SDLK_8=0x38,
	SDLK_9=0x39,
	SDLK_a=0x41,
	SDLK_b=0x42,
	SDLK_c=0x43,
	SDLK_d=0x44,
	SDLK_e=0x45,
	SDLK_f=0x46,
	SDLK_g=0x47,
	SDLK_h=0x48,
	SDLK_i=0x49,
	SDLK_j=0x4A,
	SDLK_k=0x4B,
	SDLK_l=0x4C,
	SDLK_m=0x4D,
	SDLK_n=0x4E,
	SDLK_o=0x4F,
	SDLK_p=0x50,
	SDLK_q=0x51,
	SDLK_r=0x52,
	SDLK_s=0x53,
	SDLK_t=0x54,
	SDLK_u=0x55,
	SDLK_v=0x56,
	SDLK_w=0x57,
	SDLK_x=0x58,
	SDLK_y=0x59,
	SDLK_z=0x5A,
	SDLK_LSUPER=0x5B,
	SDLK_RSUPER=0x5C,
	SDLK_MENU=0x5D,
	SDLK_SLEEP=0x5F,
	SDLK_KP0=0x60,
	SDLK_KP1=0x61,
	SDLK_KP2=0x62,
	SDLK_KP3=0x63,
	SDLK_KP4=0x64,
	SDLK_KP5=0x65,
	SDLK_KP6=0x66,
	SDLK_KP7=0x67,
	SDLK_KP8=0x68,
	SDLK_KP9=0x69,
	SDLK_F1=0x70,
	SDLK_F2=0x71,
	SDLK_F3=0x72,
	SDLK_F4=0x73,
	SDLK_F5=0x74,
	SDLK_F6=0x75,
	SDLK_F7=0x76,
	SDLK_F8=0x77,
	SDLK_F9=0x78,
	SDLK_F10=0x79,
	SDLK_F11=0x7A,
	SDLK_F12=0x7B,
	SDLK_F13=0x7C,
	SDLK_F14=0x7D,
	SDLK_F15=0x7E,
	SDLK_F16=0x7F,
	SDLK_F17=0x80,
	SDLK_F18=0x81,
	SDLK_F19=0x82,
	SDLK_F20=0x83,
	SDLK_F21=0x84,
	SDLK_F22=0x85,
	SDLK_F23=0x86,
	SDLK_F24=0x87,
	SDLK_LSHIFT=0xA0,
	SDLK_RSHIFT=0xA1,
	SDLK_LCTRL=0xA2,
	SDLK_RCTRL=0xA3,
	SDLK_AC_BACK=0xA6,
	SDLK_AC_FORWARD=0xA7,
	SDLK_AC_REFRESH=0xA8,
	SDLK_AC_STOP=0xA9,
	SDLK_AC_SEARCH=0xAA,
	SDLK_AC_BOOKMARKS=0xAB,
	SDLK_AC_HOME=0xAC,
	SDLK_AUDIOMUTE=0xAD,
	SDLK_VOLUMEDOWN=0xAE,
	SDLK_VOLUMEUP=0xAF,
	SDLK_AUTONEXT=0xB0,
	SDLK_AUDIOPREV=0xB1,
	SDLK_AUDIOSTOP=0xB2,
	SDLK_AUDIOPLAY=0xB3,
	SDLK_MAIL=0xB4,
	SDLK_MEDIASELECT=0xB5,
	SDLK_COLON=0xBA,
	SDLK_PLUS=0xBB,
	SDLK_COMMA=0xBC,
	SDLK_MINUS=0xBD,
	SDLK_PERIOD=0xBE,
	SDLK_SLASH=0xBF,
	SDLK_LEFTBRACKET=0xDB,
	SDLK_RIGHTBRACKET=0xDD,
	SDLK_QUOTEDBL=0xDE,
	SDLK_BACKSLASH=0xE2,
	SDLK_AGAIN,
	SDLK_ALTERASE,
	SDLK_QUOTE,
	SDLK_APPLICATION,
	SDLK_AUDIONEXT,
	SDLK_BRIGHTNESSDOWN,
	SDLK_BRIGHTNESSUP,
	SDLK_CALCULATOR,
	SDLK_CANCEL,
	SDLK_COMPUTER,
	SDLK_COPY,
	SDLK_CRSEL,
	SDLK_CURRENCYSUBUNIT,
	SDLK_CURRENCYUNIT,
	SDLK_CUT,
	SDLK_DECIMALSEPARATOR,
	SDLK_DISPLAYSWITCH,
	SDLK_EJECT,
	SDLK_EQUALS,
	SDLK_EXECUTE,
	SDLK_EXSEL,
	SDLK_FIND,
	SDLK_BACKQUOTE,
	SDLK_HELP,
	SDLK_KBDILLUMDOWN,
	SDLK_KBDILLUMTOGGLE,
	SDLK_KBDILLUMUP,
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
	SDLK_LALT,
	SDLK_LGUI,
	SDLK_MODE,
	SDLK_MUTE,
	SDLK_NUMLOCKCLEAR,
	SDLK_OPER,
	SDLK_OUT,
	SDLK_PASTE,
	SDLK_PAUSE,
	SDLK_POWER,
	SDLK_PRIOR,
	SDLK_RALT,
	SDLK_RETURN2,
	SDLK_RGUI,
	SDLK_SCROLLLOCK,
	SDLK_SELECT,
	SDLK_SEMICOLON,
	SDLK_SEPARATOR,
	SDLK_STOP,
	SDLK_SYSREQ,
	SDLK_THOUSANDSSEPARATOR,
	SDLK_UNDO,
	SDLK_UNKNOWN,
	SDLK_WWW,
	SDLK_AMPERSAND,
	SDLK_ASTERISK,
	SDLK_AT,
	SDLK_CARET,
	SDLK_DOLLAR,
	SDLK_EXCLAIM,
	SDLK_GREATER,
	SDLK_HASH,
	SDLK_LESS,
	SDLK_PERCENT,
	SDLK_QUESTION,
	SDLK_UNDERSCORE,
	SDLK_max//No comma to make sure it stays at the end
}SDLKey;
_u8 W32_keystates[SDLK_AGAIN];//Bit0: current Bit1: last
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
typedef enum
{
	SDL_PRESSED=1,
	SDL_RELEASED,
}SDL_KEYSTATE;
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
	_u8 state;
	_u16 x,y;
	_u8 button;
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
_u32 * winscreen=(_u32*)malloc(10000000);
SDL_Surface * SDL_SetVideoMode(int i_screensizex,int i_screensizey,int i_colordepth,_uXX i_flags)
{
	//Do nothing, all is done in SDL_Init
	free(winscreen);
	winscreen=(_u32*)malloc(i_screensizex*i_screensizey*4);
	return &W32_surface;//Return happy nothing, because that stuff is done in situ in the window redraw procedure
}
void SDL_UpdateRect(SDL_Surface * i_surface,int i_left,int i_top,int gfx_screensizex,int gfx_screensizey)
{
	PAINTSTRUCT ps;
	HDC hdc;
	MSG msg;
	while (PeekMessageA(&msg,W32_window,0,0,0)>0)
	{
		if (GetMessageA(&msg,W32_window,0,0)>0)
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
	}
	if (W32_window_set!=0)
	{
		for (int ilv1=0;ilv1<gfx_screensizex;ilv1++)
		{
			for (int ilv2=0;ilv2<gfx_screensizey;ilv2++)
			{
				schirm[(gfx_screensizey-1-ilv2)*gfx_screensizex+ilv1]=screen[ilv2*gfx_screensizex+ilv1];
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
#define W32_Eventfifo_max 100
SDL_Event W32_Eventfifo[W32_Eventfifo_max];
int W32_Eventfifo_in=0;
int W32_Eventfifo_out=0;
int SDL_PollEvent(SDL_Event * i_Event)
{
	if (W32_Eventfifo_out!=W32_Eventfifo_in)
	{
		(*i_Event)=W32_Eventfifo[W32_Eventfifo_out];
		W32_Eventfifo_out=(W32_Eventfifo_out+1)%W32_Eventfifo_max;
		return 1;
	}
	return 0;
}
int W32_RefreshEvents()
{
	SDL_Event * i_Event=W32_Eventfifo+W32_Eventfifo_in;
	_u16 resultstring[10];
	int lbuttonstate;
	POINT lppoint;
	GetCursorPos(&lppoint);
	RECT lprect;
	GetWindowRect(W32_window,&lprect);
	for (int ilv1=0;ilv1<sizeof(W32_keystates)/sizeof(W32_keystates[0]);ilv1++)
	{
		_u8 kb[256];
		W32_keystates[ilv1]=W32_keystates[ilv1]<<1;
		if (GetKeyState(ilv1)&0x8000)
		{
			W32_keystates[ilv1]|=1;
		}
		if ((W32_keystates[ilv1]&0x3)==1)
		{
			GetKeyboardState(kb);
			(*i_Event).type=SDL_KEYDOWN;
			(*i_Event).key.state=SDL_PRESSED;
			(*i_Event).key.keysym.sym=(SDLKey)ilv1;
			(*i_Event).key.keysym.scancode=MapVirtualKeyA(ilv1,0);
			ToUnicode(ilv1,(*i_Event).key.keysym.scancode,kb,resultstring,10,0);
			(*i_Event).key.keysym.unicode=resultstring[0];
			goto eventfinished;
		}
		if ((W32_keystates[ilv1]&0x3)==2)
		{
			GetKeyboardState(kb);
			(*i_Event).type=SDL_KEYUP;
			(*i_Event).key.state=SDL_RELEASED;
			(*i_Event).key.keysym.sym=(SDLKey)ilv1;
			(*i_Event).key.keysym.scancode=MapVirtualKeyA(ilv1,0);
			ToUnicode(ilv1,(*i_Event).key.keysym.scancode,kb,resultstring,10,0);
			(*i_Event).key.keysym.unicode=resultstring[0];
			goto eventfinished;
		}
	}
	{
		int border_thicknessy=GetSystemMetrics(SM_CYCAPTION);
		int border_thicknessx=GetSystemMetrics(SM_CXSIZEFRAME);
		lppoint.x-=lprect.left+border_thicknessx;
		lppoint.y-=lprect.top+border_thicknessy;
	}
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
		goto eventfinished;
	}
	lbuttonstate=((((GetKeyState(VK_LBUTTON)&0x8000)!=0)*(1<<SDL_BUTTON_LEFT)) | (((GetKeyState(VK_RBUTTON)&0x8000)!=0)*(1<<SDL_BUTTON_RIGHT)));
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
			goto eventfinished;
		}
	}
	return 0;
	eventfinished:;
	W32_Eventfifo_in=(W32_Eventfifo_in+1)%W32_Eventfifo_max;
	if (((W32_Eventfifo_in+1)%W32_Eventfifo_max)==W32_Eventfifo_out) return 0;//Buffer full
	return 1;
}
void SDL_WarpMouse(int x,int y)
{
	W32_mousex=x;
	W32_mousey=y;
	RECT lprect;
	GetWindowRect(W32_window,&lprect);
	int border_thicknessx=GetSystemMetrics(SM_CXSIZEFRAME);
	int border_thicknessy=GetSystemMetrics(SM_CYCAPTION);
	SetCursorPos(x+lprect.left+border_thicknessx,y+lprect.top+border_thicknessy);
}
LRESULT CALLBACK W32_WndProc(HWND hWnd,_u32 message,_u32 wParam,_uXX lParam)
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
			return DefWindowProcA(hWnd,message,wParam,lParam);
		}
		break;
		case WM_MOUSEWHEEL:
		{
			if (((W32_Eventfifo_in+1)%W32_Eventfifo_max)==W32_Eventfifo_out) break;
			SDL_Event i_Event;
			i_Event.type=SDL_MOUSEBUTTONDOWN;
			i_Event.button.state=W32_lastbuttonstate;
			i_Event.button.x=W32_mousex;
			i_Event.button.y=W32_mousey;
			if (wParam & 0x80000000)
			{
				i_Event.button.button=SDL_BUTTON_WHEELDOWN;
			}
			else
			{
				i_Event.button.button=SDL_BUTTON_WHEELUP;
			}
			W32_Eventfifo[W32_Eventfifo_in]=i_Event;
			W32_Eventfifo_in=(W32_Eventfifo_in+1)%W32_Eventfifo_max;
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
				iBitmapInfo.bmiHeader.biWidth=gfx_screensizex;
				iBitmapInfo.bmiHeader.biHeight=gfx_screensizey;
				iBitmapInfo.bmiHeader.biCompression=0;
				iBitmapInfo.bmiHeader.biPlanes=1;
				iBitmapInfo.bmiHeader.biSizeImage=gfx_screensizex*gfx_screensizey*4;
				iBitmapInfo.bmiHeader.biClrUsed=0;
				iBitmapInfo.bmiHeader.biClrImportant=0;
				iBitmapInfo.bmiHeader.biXPelsPerMeter=1000;
				iBitmapInfo.bmiHeader.biYPelsPerMeter=1000;
				int iret;
				iret=SetDIBitsToDevice(hdc,0,0,gfx_screensizex,gfx_screensizey,0,0,0,gfx_screensizey,schirm,&iBitmapInfo,DIB_RGB_COLORS);
				EndPaint(hWnd, &ps);
				SelectObject(hdc,hbm_memdc_old);
				DeleteObject(hbm_memdc);
				DeleteObject(hdcmem);
			break;
		}
		case WM_SIZE:
		{
			if ((wParam!=SIZE_RESTORED) && (wParam!=SIZE_MAXIMIZED)) break;
			if (((W32_Eventfifo_in+1)%W32_Eventfifo_max)==W32_Eventfifo_out) break;
			SDL_Event i_Event;
			i_Event.type=SDL_VIDEORESIZE;
			i_Event.resize.w=lParam&0xFFFF;
			i_Event.resize.h=(lParam>>16)&0xFFFF;
			W32_Eventfifo[W32_Eventfifo_in]=i_Event;
			W32_Eventfifo_in=(W32_Eventfifo_in+1)%W32_Eventfifo_max;
			break;
		}
		case WM_QUIT:
		//FALLTHROUGH
		case WM_CLOSE:
		{
			SDL_Event i_Event;
			i_Event.type=SDL_QUIT;
			W32_Eventfifo[W32_Eventfifo_in]=i_Event;
			W32_Eventfifo_in=(W32_Eventfifo_in+1)%W32_Eventfifo_max;
			break;
		}
		default:
		{
			while (W32_RefreshEvents()){}
			return DefWindowProcA(hWnd,message,wParam,lParam);
		}
	}
	return 1;
}
_u16 W32_MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXA wcex;
	wcex.cbSize=sizeof(WNDCLASSEXA);
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
	return RegisterClassExA(&wcex);
}
_u32 W32_InitInstance(HINSTANCE hInstance,int nCmdShow)
{
	HWND hWnd;
	W32_MyRegisterClass(hInstance);
	int border_thicknessy=GetSystemMetrics(SM_CYCAPTION);
	int border_thicknessx=GetSystemMetrics(SM_CXSIZEFRAME);
	hWnd=CreateWindowExA(0,szWindowClass,"lhendraw",WS_OVERLAPPEDWINDOW,0,0,gfx_screensizex+border_thicknessx*2,gfx_screensizey+border_thicknessy+border_thicknessx,NULL,NULL,hInstance,NULL);
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
	for (int ilv1=0;ilv1<sizeof(W32_keystates)/sizeof(W32_keystates[0]);ilv1++)
	{
		W32_keystates[ilv1]=0;
	}
	W32_hInst=GetModuleHandleA(NULL);
	if (!W32_InitInstance(W32_hInst,1))
	{
		printf("Win32-api init failed");
		return 0;
	}
	return 1;
}
