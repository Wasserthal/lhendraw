all: ./generated/filestructure.hxx ./generated/internalstructure.hxx parsecdxml.cxx lendefs.h xmldata.hxx xmlparse.hxx cxxdata.h enums.hxx definitionlist.h lendefs.h janitor.hxx createsvg.hxx makeinf.hxx makeinf.cxx graphics_variables.hxx
	g++ -O0 -g -std=c++0x -Wno-invalid-offsetof parsecdxml.cxx -o parsexml -D DEBUG -D LENNARD_HACK -Wno-format -D GFXOUT_SVG
	g++ -g -O0 -std=c++0x -Wno-invalid-offsetof makeinf.cxx -o makeinf -D DEBUG -D LENNARD_HACK -Wno-format -D GFXOUT_SVG
./tools/filestructure_maker: ./tools/filestructure.c
	gcc -O0 -std=c99 -g ./tools/filestructure.c -o ./tools/filestructure_maker -D BITMODE32
./generated/filestructure.hxx: ./tools/filestructure_maker filestructure.draft
	./tools/filestructure_maker -m filestructure.draft ./generated/filestructure.hxx
./generated/internalstructure.hxx: ./tools/filestructure_maker internalstructure.draft
	./tools/filestructure_maker -l internalstructure.draft ./generated/internalstructure.hxx INTERNAL
./generated/structure.hxx: ./tools/filestructure_maker internalstructure.draft
	./tools/filestructure_maker -l internalstructure.draft ./generated/structure.hxx
./generated/cambridgestructure.hxx: ./tools/filestructure_maker filestructure.draft
	./tools/filestructure_maker -m filestructure.draft ./generated/cambridgestructure.hxx CAMBRIDGE
makeinf_test: makeinf.hxx makeinf_test.cxx cxxdata.h lendefs.h
	g++ -g -O0 -std=c++0x -Wno-invalid-offsetof makeinf_test.cxx -o makeinftest -D DEBUG -Wno-format
sdl: ./generated/cambridgestructure.hxx ./generated/structure.hxx parsecdxml.cxx lendefs.h xmldata.hxx xmlparse.hxx cxxdata.h conv_cambridge_internal.hxx enums.hxx definitionlist.h lendefs.h janitor.hxx draw.hxx sdlgfx.hxx lhendraw.cxx conv_cambridge_internal.hxx draw_variables.hxx lhendraw_files.hxx
	g++ -g -O0 -std=c++0x -m64 lhendraw.cxx -o lhendraw -I/usr/include/SDL -I/usr/include/freetype2 -L/usr/lib64 -lSDL -lm -lfreetype -Wno-invalid-offsetof -D GFXOUT_SDL -lrt
lennard_infget: makeinf.hxx makeinf_test.cxx cxxdata.h lendefs.h
	g++ -g -O0 -std=c++0x -Wno-invalid-offsetof makeinf_test.cxx -o lennard_infget -D DEBUG -D MACHINE_READABLE -Wno-format
