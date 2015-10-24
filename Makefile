DESTDIR = /
exec_prefix = $(DESTDIR)/usr/
BIN = $(exec_prefix)/bin
build: lhendraw
build-arch: lhendraw
build-indep: lhendraw
clean:
	rm ./generated/initialization_lhendraw.hxx ; rm ./generated/propertylist_lhendraw.hxx ; rm ./generated/propertydirectory_lhendraw.hxx ; rm ./generated/multilistlistfile.hxx ; true
	rm ./generated/structure.hxx ; rm ./generated/cambridgestructure.hxx ; rm ./generated/configfilestructure.hxx ; true
	rm ./tools/filestructure_maker ; rm ./tools/reflection_enums ; rm ./tools/reflection ; rm ./tools/pullout ; rm ./tools/option ; true
	rm ./generated/pullout.hxx ; rm ./generated/pulloutmatic.hxx ; rm ./generated/pullout_stringfile.hxx ; rm ./generated/pullout_structfile.hxx ; rm ./generated/pullout_reflectfile.hxx ; true
	rm lhendraw ; true
xclib.o:
	echo You must insert xclib.o from the xclip project!
commandline: ./generated/filestructure.hxx ./generated/internalstructure.hxx parsecdxml.cxx lendefs.h xmldata.hxx xmlparse.hxx cdxdata.hxx enums.hxx definitionlist.h lendefs.h janitor.hxx createsvg.hxx makeinf.hxx makeinf.cxx graphics_variables.hxx ./generated/initialization_parsexml.hxx
	g++ -O0 -g -std=c++0x -Wno-invalid-offsetof parsecdxml.cxx -o parsexml -D DEBUG -D LENNARD_HACK -Wno-format -D GFXOUT_SVG
	g++ -g -O0 -std=c++0x -Wno-invalid-offsetof makeinf.cxx -o makeinf -D DEBUG -D LENNARD_HACK -Wno-format -D GFXOUT_SVG
./tools/filestructure_maker: ./tools/filestructure.c
	gcc -O0 -std=c99 -g ./tools/filestructure.c -o ./tools/filestructure_maker
./tools/reflection_enums: ./tools/reflection_enums.c
	gcc -O0 -std=c99 -g ./tools/reflection_enums.c -o ./tools/reflection_enums
./tools/reflection: ./tools/reflection.c
	gcc -O0 -std=c99 -g ./tools/reflection.c -o ./tools/reflection
./tools/option: ./tools/option.c
	gcc -O0 -std=c99 -g ./tools/option.c -o ./tools/option
./generated/filestructure.hxx ./generated/internalstructure.hxx ./generated/initialization_parsexml.hxx: ./tools/filestructure_maker filestructure.draft internalstructure.draft
	mkdir ./generated ; true
	echo 'void automatic_init() {'> ./generated/initialization_parsexml.hxx
	./tools/filestructure_maker -m filestructure.draft ./generated/filestructure.hxx ./generated/initialization_parsexml.hxx '' 
	./tools/filestructure_maker -l internalstructure.draft ./generated/internalstructure.hxx ./generated/initialization_parsexml.hxx INTERNAL ./generated/propertylist_lhendraw.hxx ./generated/propertydirectory_lhendraw.hxx
	echo '}' >> ./generated/initialization_parsexml.hxx
./generated/structure.hxx ./generated/cambridgestructure.hxx ./generated/initialization_lhendraw.hxx ./generated/configfilestructure.hxx ./generated/multilistlistfile.hxx ./generated/propertydirectory_lhendraw.hxx: ./tools/filestructure_maker internalstructure.draft filestructure.draft configfile.draft
	mkdir ./generated ; true
	rm ./generated/initialization_lhendraw.hxx ; rm ./generated/propertylist_lhendraw.hxx ; rm ./generated/propertydirectory_lhendraw.hxx ; rm ./generated/multilistlistfile.hxx ; true
	./tools/filestructure_maker -l0 internalstructure.draft ./generated/structure.hxx ./generated/initialization_lhendraw.hxx '' ./generated/propertylist_lhendraw.hxx ./generated/propertydirectory_lhendraw.hxx
	./tools/filestructure_maker -m1 filestructure.draft ./generated/cambridgestructure.hxx ./generated/initialization_lhendraw.hxx CAMBRIDGE
	./tools/filestructure_maker -m1 bkchem.draft ./generated/bkchemstructure.hxx ./generated/initialization_lhendraw.hxx BKCHEM
	./tools/filestructure_maker -m2 configfile.draft ./generated/configfilestructure.hxx ./generated/initialization_lhendraw.hxx CONFIGBRIDGE
./generated/reflection.hxx: ./tools/reflection_enums ./internal_enum.hxx ./tools/reflection edit.hxx sdlctrl.hxx filedlg.hxx pivot.hxx janitor.hxx
	./tools/reflection_enums
	./tools/reflection edit.hxx sdlctrl.hxx filedlg.hxx pivot.hxx
makeinf_test: makeinf.hxx makeinf_test.cxx cdxdata.hxx lendefs.h
	g++ -g -O0 -std=c++0x -Wno-invalid-offsetof makeinf_test.cxx -o makeinftest -D DEBUG -Wno-format
lhendraw: ./generated/structure.hxx ./generated/reflection.hxx lendefs.h xmldata.hxx xmlparse.hxx cdxdata.hxx conv_cambridge_internal.hxx enums.hxx definitionlist.h janitor.hxx draw.hxx sdlgfx.hxx lhendraw.cxx draw_variables.hxx ./generated/pullout_stringfile.hxx ./generated/reflection_headers.hxx edit.hxx sdlctrl.hxx selection.hxx conv_config_internalconfig.hxx resources_init.hxx filedlg.hxx ps.hxx elements.hxx parsecdxml_variables.hxx ellipeq.h analysis.hxx clipboard.hxx memory.hxx menugfx.hxx pivot.hxx quartic.hxx svg.hxx telescopic_list.hxx text_freetype.h undo.hxx glyf.hxx hatch.hxx help.hxx internal_enum.hxx cmdline.hxx conv_internal_cambridge.hxx
	g++ -g -O0 -std=c++0x -m64 lhendraw.cxx -o lhendraw -I/usr/include/SDL -I/usr/include/freetype2 -L/usr/lib64 -L/usr/lib/x86_64-linux-gnu -lSDL -lm -lfreetype -lX11 -lXmu -Wno-invalid-offsetof -D GFXOUT_SDL -D BITMODE64 -lrt -Wreturn-type -D NODEBUG
mac: ./generated/structure.hxx ./generated/reflection.hxx lendefs.h xmldata.hxx xmlparse.hxx cdxdata.hxx conv_cambridge_internal.hxx enums.hxx definitionlist.h janitor.hxx draw.hxx sdlgfx.hxx lhendraw.cxx draw_variables.hxx ./generated/pullout_stringfile.hxx ./generated/reflection_headers.hxx edit.hxx sdlctrl.hxx selection.hxx conv_config_internalconfig.hxx resources_init.hxx filedlg.hxx ps.hxx elements.hxx parsecdxml_variables.hxx ellipeq.h analysis.hxx clipboard.hxx memory.hxx menugfx.hxx pivot.hxx quartic.hxx svg.hxx telescopic_list.hxx text_freetype.h undo.hxx glyf.hxx hatch.hxx help.hxx internal_enum.hxx cmdline.hxx conv_internal_cambridge.hxx
	g++ -g -O0 -std=c++0x -m64 lhendraw.cxx -o lhendraw -I/Library/Frameworks/SDL2.framework/Headers -lm -Wno-invalid-offsetof -Wno-dynamic-class-memaccess -D GFXOUT_SDL -D BITMODE64 -D NODEBUG -D OSX -ferror-limit=1000 -D SDL2 -framework SDL2 -L/opt/X11/lib/ -I/opt/x11/share/include/freetype -lfreetype -D NOCLIPBOARD
lennard_infget: makeinf.hxx makeinf_test.cxx cdxdata.hxx lendefs.h
	g++ -g -O0 -std=c++0x -Wno-invalid-offsetof makeinf_test.cxx -o lennard_infget -D DEBUG -D MACHINE_READABLE -Wno-format
./generated/pullout_stringfile.hxx: ./tools/option tools/pullout.c *.pullout.hxx edit.hxx
	ls -1 *\.pullout.hxx | cut -d. -f 1 | xargs -I{} ./tools/pulloutmatic.sh {} > ./generated/pulloutmatic.hxx
	cat *.pullout.hxx > ./generated/pullout.hxx
	gcc tools/pullout.c -o tools/pullout
	./tools/pullout
	./tools/option ./edit.hxx
test_install:
	mkdir -p /usr/share/lhendraw
	cp lhendraw -t /usr/share/lhendraw
	cp hotkeys.xml -t /usr/share/lhendraw
	cp -R gfx -t /usr/share/lhendraw/
	cp *.ttf -t /usr/share/lhendraw
	cp *.lennardfont -t /usr/share/lhendraw
	ln -s -f /usr/share/lhendraw/lhendraw /usr/bin/lhendraw
install:
	install -d $(BIN) $(DESTDIR)/usr/share/lhendraw
	install lhendraw $(BIN)
	cp hotkeys.xml -t $(DESTDIR)/usr/share/lhendraw
	cp -R gfx -t $(DESTDIR)/usr/share/lhendraw/
	cp *.ttf -t $(DESTDIR)/usr/share/lhendraw
	cp *.lennardfont -t $(DESTDIR)/usr/share/lhendraw
