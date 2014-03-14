all: ./generated/filestructure.hxx ./generated/internalstructure.hxx parsecdxml.cxx lendefs.h xmldata.hxx xmlparse.hxx cxxdata.h enums.hxx definitionlist.h lendefs.h janitor.hxx createsvg.hxx makeinf.hxx makeinf.cxx graphics_variables.hxx ./generated/initialization_parsexml.hxx
	g++ -O0 -g -std=c++0x -Wno-invalid-offsetof parsecdxml.cxx -o parsexml -D DEBUG -D LENNARD_HACK -Wno-format -D GFXOUT_SVG
	g++ -g -O0 -std=c++0x -Wno-invalid-offsetof makeinf.cxx -o makeinf -D DEBUG -D LENNARD_HACK -Wno-format -D GFXOUT_SVG
./tools/filestructure_maker: ./tools/filestructure.c
	gcc -O0 -std=c99 -g ./tools/filestructure.c -o ./tools/filestructure_maker -D BITMODE32
./tools/reflection_enums: ./tools/reflection_enums.c
	gcc -O0 -std=c99 -g ./tools/reflection_enums.c -o ./tools/reflection_enums -D BITMODE32
./generated/filestructure.hxx ./generated/internalstructure.hxx ./generated/initialization_parsexml.hxx: ./tools/filestructure_maker filestructure.draft internalstructure.draft
	echo 'void automatic_init() {'> ./generated/initialization_parsexml.hxx
	./tools/filestructure_maker -m filestructure.draft ./generated/filestructure.hxx ./generated/initialization_parsexml.hxx '' 
	./tools/filestructure_maker -l internalstructure.draft ./generated/internalstructure.hxx ./generated/initialization_parsexml.hxx INTERNAL ./generated/propertylist_lhendraw.hxx ./generated/propertydirectory_lhendraw.hxx
	echo '}' >> ./generated/initialization_parsexml.hxx
./generated/structure.hxx ./generated/cambridgestructure.hxx ./generated/initialization_lhendraw.hxx: ./tools/filestructure_maker internalstructure.draft filestructure.draft
	rm ./generated/initialization_lhendraw.hxx || rm ./generated/propertylist_lhendraw.hxx || rm ./generated/propertydirectory_lhendraw || true
	./tools/filestructure_maker -l internalstructure.draft ./generated/structure.hxx ./generated/initialization_lhendraw.hxx '' ./generated/propertylist_lhendraw.hxx ./generated/propertydirectory_lhendraw.hxx
	./tools/filestructure_maker -m filestructure.draft ./generated/cambridgestructure.hxx ./generated/initialization_lhendraw.hxx CAMBRIDGE
./generated/reflection_enums.hxx: ./tools/reflection_enums ./internal_enum.hxx
	./tools/reflection_enums
makeinf_test: makeinf.hxx makeinf_test.cxx cxxdata.h lendefs.h
	g++ -g -O0 -std=c++0x -Wno-invalid-offsetof makeinf_test.cxx -o makeinftest -D DEBUG -Wno-format
sdl: ./generated/cambridgestructure.hxx ./generated/structure.hxx parsecdxml.cxx lendefs.h xmldata.hxx xmlparse.hxx cxxdata.h conv_cambridge_internal.hxx enums.hxx definitionlist.h lendefs.h janitor.hxx draw.hxx sdlgfx.hxx lhendraw.cxx conv_cambridge_internal.hxx draw_variables.hxx lhendraw_files.hxx ./generated/initialization_lhendraw.hxx ./generated/reflection_enums.hxx ./pullout_stringfile.hxx
	g++ -g -O0 -std=c++0x -m64 lhendraw.cxx -o lhendraw -I/usr/include/SDL -I/usr/include/freetype2 -L/usr/lib64 -L/usr/lib/x86_64-linux-gnu -lSDL -lm -lfreetype -Wno-invalid-offsetof -D GFXOUT_SDL -lrt
lennard_infget: makeinf.hxx makeinf_test.cxx cxxdata.h lendefs.h
	g++ -g -O0 -std=c++0x -Wno-invalid-offsetof makeinf_test.cxx -o lennard_infget -D DEBUG -D MACHINE_READABLE -Wno-format
pullout_stringfile.hxx: tools/pullout.c toolbox.pullout.hxx
	gcc tools/pullout.c -o tools/pullout
	./tools/pullout
install:
	cp lhendraw -t /usr/bin
