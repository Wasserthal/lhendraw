all: sdl
commandline: ./generated/filestructure.hxx ./generated/internalstructure.hxx parsecdxml.cxx lendefs.h xmldata.hxx xmlparse.hxx cdxdata.hxx enums.hxx definitionlist.h lendefs.h janitor.hxx createsvg.hxx makeinf.hxx makeinf.cxx graphics_variables.hxx ./generated/initialization_parsexml.hxx
	g++ -O0 -g -std=c++0x -Wno-invalid-offsetof parsecdxml.cxx -o parsexml -D DEBUG -D LENNARD_HACK -Wno-format -D GFXOUT_SVG
	g++ -g -O0 -std=c++0x -Wno-invalid-offsetof makeinf.cxx -o makeinf -D DEBUG -D LENNARD_HACK -Wno-format -D GFXOUT_SVG
./tools/filestructure_maker: ./tools/filestructure.c
	gcc -O0 -std=c99 -g ./tools/filestructure.c -o ./tools/filestructure_maker -D BITMODE32
./tools/reflection_enums: ./tools/reflection_enums.c
	gcc -O0 -std=c99 -g ./tools/reflection_enums.c -o ./tools/reflection_enums -D BITMODE32
./tools/reflection: ./tools/reflection.c
	gcc -O0 -std=c99 -g ./tools/reflection.c -o ./tools/reflection -D BITMODE32
./generated/filestructure.hxx ./generated/internalstructure.hxx ./generated/initialization_parsexml.hxx: ./tools/filestructure_maker filestructure.draft internalstructure.draft
	mkdir ./generated ; true
	echo 'void automatic_init() {'> ./generated/initialization_parsexml.hxx
	./tools/filestructure_maker -m filestructure.draft ./generated/filestructure.hxx ./generated/initialization_parsexml.hxx '' 
	./tools/filestructure_maker -l internalstructure.draft ./generated/internalstructure.hxx ./generated/initialization_parsexml.hxx INTERNAL ./generated/propertylist_lhendraw.hxx ./generated/propertydirectory_lhendraw.hxx
	echo '}' >> ./generated/initialization_parsexml.hxx
./generated/structure.hxx ./generated/cambridgestructure.hxx ./generated/initialization_lhendraw.hxx ./generated/configfilestructure.hxx: ./tools/filestructure_maker internalstructure.draft filestructure.draft configfile.draft
	mkdir ./generated ; true
	rm ./generated/initialization_lhendraw.hxx ; rm ./generated/propertylist_lhendraw.hxx ; rm ./generated/propertydirectory_lhendraw.hxx ; rm ./generated/multilistlistfile.hxx ; true
	./tools/filestructure_maker -l0 internalstructure.draft ./generated/structure.hxx ./generated/initialization_lhendraw.hxx '' ./generated/propertylist_lhendraw.hxx ./generated/propertydirectory_lhendraw.hxx
	./tools/filestructure_maker -m1 filestructure.draft ./generated/cambridgestructure.hxx ./generated/initialization_lhendraw.hxx CAMBRIDGE
	./tools/filestructure_maker -m2 configfile.draft ./generated/configfilestructure.hxx ./generated/initialization_lhendraw.hxx CONFIGBRIDGE
./generated/reflection_enums.hxx: ./tools/reflection_enums ./internal_enum.hxx ./tools/reflection edit.hxx sdlctrl.hxx filedlg.hxx pivot.hxx janitor.hxx
	./tools/reflection_enums
	./tools/reflection edit.hxx sdlctrl.hxx filedlg.hxx pivot.hxx
makeinf_test: makeinf.hxx makeinf_test.cxx cdxdata.hxx lendefs.h
	g++ -g -O0 -std=c++0x -Wno-invalid-offsetof makeinf_test.cxx -o makeinftest -D DEBUG -Wno-format
sdl: ./generated/cambridgestructure.hxx ./generated/structure.hxx parsecdxml.cxx lendefs.h xmldata.hxx xmlparse.hxx cdxdata.hxx conv_cambridge_internal.hxx enums.hxx definitionlist.h lendefs.h janitor.hxx draw.hxx sdlgfx.hxx lhendraw.cxx conv_cambridge_internal.hxx draw_variables.hxx ./generated/initialization_lhendraw.hxx ./generated/reflection_enums.hxx ./generated/pullout_stringfile.hxx ./generated/reflection_headers.hxx edit.hxx sdlctrl.hxx selection.hxx conv_config_internalconfig.hxx resources_init.hxx filedlg.hxx
	g++ -g -O0 -std=c++0x -m64 lhendraw.cxx -o lhendraw -I/usr/include/SDL -I/usr/include/freetype2 -L/usr/lib64 -L/usr/lib/x86_64-linux-gnu -lSDL -lm -lfreetype -Wno-invalid-offsetof -D GFXOUT_SDL -D BITMODE64 -lrt
lennard_infget: makeinf.hxx makeinf_test.cxx cdxdata.hxx lendefs.h
	g++ -g -O0 -std=c++0x -Wno-invalid-offsetof makeinf_test.cxx -o lennard_infget -D DEBUG -D MACHINE_READABLE -Wno-format
./generated/pullout_stringfile.hxx: tools/pullout.c *.pullout.hxx
	ls -1 *\.pullout.hxx | cut -d. -f 1 | xargs -I{} ./tools/pulloutmatic.sh {} > ./generated/pulloutmatic.hxx
	cat *.pullout.hxx > ./generated/pullout.hxx
	gcc tools/pullout.c -o tools/pullout
	./tools/pullout
install:
	mkdir -p /usr/share/lhendraw
	cp lhendraw -t /usr/share/lhendraw
	cp hotkeys.xml -t /usr/share/lhendraw
	cp -R gfx -t /usr/share/lhendraw/
	cp *.ttf -t /usr/share/lhendraw
	ln -s -f /usr/share/lhendraw/lhendraw /usr/bin/lhendraw
