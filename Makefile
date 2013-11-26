all: filestructure.hxx parsecdxml.cxx lendefs.h xmldata.hxx xmlparse.hxx cxxdata.h enums.hxx definitionlist.h lendefs.h janitor.hxx createsvg.hxx makeinf.hxx makeinf.cxx 
	g++ -O0 -g -std=c++0x -Wno-invalid-offsetof parsecdxml.cxx -o parsexml -D DEBUG -D LENNARD_HACK -Wno-format
	g++ -g -O0 -std=c++0x -Wno-invalid-offsetof makeinf.cxx -o makeinf -D DEBUG -D LENNARD_HACK -Wno-format
filestructure_maker: filestructure.c
	gcc -O0 -std=c99 -g filestructure.c -o filestructure_maker -D BITMODE32
filestructure.hxx: filestructure_maker filestructure.draft
	./filestructure_maker
makeinf_test: makeinf.hxx makeinf_test.cxx cxxdata.h lendefs.h
	g++ -g -O0 -std=c++0x -Wno-invalid-offsetof makeinf_test.cxx -o makeinftest -D DEBUG -Wno-format
sdl: filestructure.hxx parsecdxml.cxx lendefs.h xmldata.hxx xmlparse.hxx cxxdata.h enums.hxx definitionlist.h lendefs.h janitor.hxx createsvg.hxx makeinf.hxx sdlgfx.hxx lhendraw.cxx
	g++ -std=c++0x -m64 lhendraw.cxx -o sdl.out -I/usr/include/SDL -L/usr/lib64 -lSDL -lm -Wno-invalid-offsetof
lennard_infget: makeinf.hxx makeinf_test.cxx cxxdata.h lendefs.h
	g++ -g -O0 -std=c++0x -Wno-invalid-offsetof makeinf_test.cxx -o lennard_infget -D DEBUG -D MACHINE_READABLE -Wno-format
