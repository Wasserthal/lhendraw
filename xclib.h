#define clipboard_XCLIB_XCOUT_NONE	0
#define clipboard_XCLIB_XCIN_NONE	0
extern "C" int xcout(
	Display*,
	Window,
	XEvent,
	Atom,
	Atom,
	Atom*,
	unsigned char**,
	unsigned long*,
	unsigned int*
);
extern "C" int xcin(
	Display*,
	Window*,
	XEvent,
	Atom*,
	Atom,
	unsigned char*,
	unsigned long,
	unsigned long*,
	unsigned int*
);
