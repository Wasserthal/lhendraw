import: import.cxx font.hxx lendefs.h
	g++ -O0 -g -o g import.cxx -o fimport
export: export.cxx font.hxx lendefs.h
	g++ -O0 -g -o g export.cxx -o fexport
	./fexport LiberationMono-Regular.ttf LiberationMono-Regular.lennardfont
all: lendefs.h glyf.cxx font.hxx
	g++ glyf.cxx -I/usr/include/SDL2 `pkg-config sdl2 --cflags --libs` -o g
hex:
	wxHexEditor LiberationMono-Regular.ttf
mac:
	g++ glyf.cxx -I/Library/Frameworks/SDL2.framework/Headers -framework SDL2 -o g
hexmac:
	/Applications/wxHexEditor.app/Contents/MacOS/wxHexEditor *.ttf
