DESTDIR = /
exec_prefix = $(DESTDIR)/usr/
BIN = $(exec_prefix)/bin
CPPFLAGS+= -D_FORTIFY_SOURCE=2
build: lhendraw
build-arch: lhendraw
build-indep: lhendraw
clean:
	rm ./generated/initialization_lhendraw.hxx ; rm ./generated/propertylist_lhendraw.hxx ; rm ./generated/propertydirectory_lhendraw.hxx ; rm ./generated/multilistlistfile.hxx ; true
	rm ./generated/structure.hxx ; rm ./generated/cambridgestructure.hxx ; rm ./generated/configfilestructure.hxx ; true
	rm ./tools/filestructure ; rm ./tools/reflection_enums ; rm ./tools/reflection ; rm ./tools/pullout ; rm ./tools/option ; true
	rm ./generated/pullout.hxx ; rm ./generated/pulloutmatic.hxx ; rm ./generated/pullout_stringfile.hxx ; rm ./generated/pullout_structfile.hxx ; rm ./generated/pullout_reflectfile.hxx ; true
	rm lhendraw ; true
xclib.o:
	echo You must insert xclib.o from the xclip project!
commandline: ./generated/filestructure.hxx ./generated/internalstructure.hxx parsecdxml.cxx lendefs.h xmldata.hxx xmlparse.hxx cdxdata.hxx enums.hxx definitionlist.h lendefs.h janitor.hxx createsvg.hxx makeinf.hxx makeinf.cxx graphics_variables.hxx ./generated/initialization_parsexml.hxx
	g++ -O0 -g -std=c++0x -Wno-invalid-offsetof parsecdxml.cxx -o parsexml -D DEBUG -D LENNARD_HACK -Wno-format -D GFXOUT_SVG
	g++ -g -O0 -std=c++0x -Wno-invalid-offsetof makeinf.cxx -o makeinf -D DEBUG -D LENNARD_HACK -Wno-format -D GFXOUT_SVG
./tools/filestructure: ./tools/filestructure.c
	gcc -O0 -std=c99 -g ./tools/filestructure.c -o ./tools/filestructure
./tools/reflection_enums: ./tools/reflection_enums.c
	gcc -O0 -std=c99 -g ./tools/reflection_enums.c -o ./tools/reflection_enums
./tools/reflection: ./tools/reflection.c
	gcc -O0 -std=c99 -g ./tools/reflection.c -o ./tools/reflection
./tools/option: ./tools/option.c
	gcc -O0 -std=c99 -g ./tools/option.c -o ./tools/option
./generated/structure.hxx ./generated/cambridgestructure.hxx ./generated/initialization_lhendraw.hxx ./generated/configfilestructure.hxx ./generated/multilistlistfile.hxx ./generated/propertydirectory_lhendraw.hxx: ./tools/filestructure internalstructure.draft filestructure.draft configfile.draft bkchem.draft
	mkdir ./generated ; true
	rm ./generated/initialization_lhendraw.hxx ; rm ./generated/propertylist_lhendraw.hxx ; rm ./generated/propertydirectory_lhendraw.hxx ; rm ./generated/multilistlistfile.hxx ; true
	./tools/filestructure -l0 internalstructure.draft ./generated/structure.hxx ./generated/initialization_lhendraw.hxx '' ./generated/propertylist_lhendraw.hxx ./generated/propertydirectory_lhendraw.hxx
	./tools/filestructure -m1 filestructure.draft ./generated/cambridgestructure.hxx ./generated/initialization_lhendraw.hxx CAMBRIDGE
	./tools/filestructure -m1 bkchem.draft ./generated/bkchemstructure.hxx ./generated/initialization_lhendraw.hxx BKCHEM
	./tools/filestructure -m2 configfile.draft ./generated/configfilestructure.hxx ./generated/initialization_lhendraw.hxx CONFIGBRIDGE
./generated/reflection.hxx: ./tools/reflection_enums ./internal_enum.hxx ./tools/reflection edit.hxx sdlctrl.hxx filedlg.hxx pivot.hxx janitor.hxx selection.hxx print.hxx
	./tools/reflection_enums
	./tools/reflection edit.hxx sdlctrl.hxx filedlg.hxx pivot.hxx selection.hxx print.hxx
makeinf_test: makeinf.hxx makeinf_test.cxx cdxdata.hxx lendefs.h
	g++ -g -O0 -std=c++0x -Wno-invalid-offsetof makeinf_test.cxx -o makeinftest -D DEBUG -Wno-format
lhendraw: ./generated/structure.hxx ./generated/reflection.hxx lendefs.h xmldata.hxx xmlparse.hxx cdxdata.hxx conv_cambridge_internal.hxx enums.hxx definitionlist.h janitor.hxx draw.hxx sdlgfx.hxx lhendraw.cxx draw_variables.hxx ./generated/pullout_stringfile.hxx ./generated/reflection_headers.hxx edit.hxx sdlctrl.hxx selection.hxx conv_config_internalconfig.hxx conv_internalconfig_config.hxx resources_init.hxx filedlg.hxx ps.hxx elements.hxx variables.hxx ellipeq.h analysis.hxx clipboard.hxx memory.hxx menugfx.hxx popup.hxx pivot.hxx quartic.hxx svg.hxx telescope.hxx undo.hxx glyf.hxx hatch.hxx help.hxx internal_enum.hxx cmdline.hxx conv_internal_cambridge.hxx text.h debug.hxx HQ.h bkchemdata.hxx conv_bkchem_internal.hxx undotree.hxx binaries.hxx fileformat.hxx print.hxx
	g++ $(CPPFLAGS) -Wl,-z,relro -fstack-protector -g -O0 -std=c++0x -m`getconf LONG_BIT` lhendraw.cxx -o lhendraw -I/usr/include/SDL -L/usr/lib64 -L/usr/lib/x86_64-linux-gnu -lSDL -lm -lX11 -lXmu -Wno-invalid-offsetof -D GFXOUT_SDL -D BITMODE`getconf LONG_BIT` -lrt -Wreturn-type $(LHENDRAW_DEBUG) -lcups
lhendraw_SDL2: ./generated/structure.hxx ./generated/reflection.hxx lendefs.h xmldata.hxx xmlparse.hxx cdxdata.hxx conv_cambridge_internal.hxx enums.hxx definitionlist.h janitor.hxx draw.hxx sdlgfx.hxx lhendraw.cxx draw_variables.hxx ./generated/pullout_stringfile.hxx ./generated/reflection_headers.hxx edit.hxx sdlctrl.hxx selection.hxx conv_config_internalconfig.hxx conv_internalconfig_config.hxx resources_init.hxx filedlg.hxx ps.hxx elements.hxx variables.hxx ellipeq.h analysis.hxx clipboard.hxx memory.hxx menugfx.hxx popup.hxx pivot.hxx quartic.hxx svg.hxx telescope.hxx undo.hxx glyf.hxx hatch.hxx help.hxx internal_enum.hxx cmdline.hxx conv_internal_cambridge.hxx text.h debug.hxx HQ.h bkchemdata.hxx conv_bkchem_internal.hxx undotree.hxx binaries.hxx fileformat.hxx print.hxx
	g++ $(CPPFLAGS) -Wl,-z,relro -fstack-protector -g -O0 -std=c++0x -m`getconf LONG_BIT` lhendraw.cxx -o lhendraw -I/usr/include/SDL2 -L/usr/lib64 -L/usr/lib/x86_64-linux-gnu -lSDL2 -lm -lX11 -lXmu -Wno-invalid-offsetof -D GFXOUT_SDL -D BITMODE`getconf LONG_BIT` -lrt -Wreturn-type $(LHENDRAW_DEBUG)
lhendraw_no: ./generated/structure.hxx ./generated/reflection.hxx lendefs.h xmldata.hxx xmlparse.hxx cdxdata.hxx conv_cambridge_internal.hxx enums.hxx definitionlist.h janitor.hxx draw.hxx sdlgfx.hxx lhendraw.cxx draw_variables.hxx ./generated/pullout_stringfile.hxx ./generated/reflection_headers.hxx edit.hxx sdlctrl.hxx selection.hxx conv_config_internalconfig.hxx conv_internalconfig_config.hxx resources_init.hxx filedlg.hxx ps.hxx elements.hxx variables.hxx ellipeq.h analysis.hxx clipboard.hxx memory.hxx menugfx.hxx popup.hxx pivot.hxx quartic.hxx svg.hxx telescope.hxx undo.hxx glyf.hxx hatch.hxx help.hxx internal_enum.hxx cmdline.hxx conv_internal_cambridge.hxx text.h debug.hxx HQ.h bkchemdata.hxx conv_bkchem_internal.hxx undotree.hxx binaries.hxx fileformat.hxx
	g++ -g -O0 -std=c++0x -m`getconf LONG_BIT` lhendraw.cxx -o lhendraw -I/usr/include/SDL -L/usr/lib64 -L/usr/lib/x86_64-linux-gnu -lSDL -lm -lX11 -lXmu -Wno-invalid-offsetof -D GFXOUT_SDL -D BITMODE`getconf LONG_BIT` -lrt -Wreturn-type $(LHENDRAW_DEBUG)
lhendraw_portable: ./generated/structure.hxx ./generated/reflection.hxx lendefs.h xmldata.hxx xmlparse.hxx cdxdata.hxx conv_cambridge_internal.hxx enums.hxx definitionlist.h janitor.hxx draw.hxx sdlgfx.hxx lhendraw.cxx draw_variables.hxx ./generated/pullout_stringfile.hxx ./generated/reflection_headers.hxx edit.hxx sdlctrl.hxx selection.hxx conv_config_internalconfig.hxx conv_internalconfig_config.hxx resources_init.hxx filedlg.hxx ps.hxx elements.hxx variables.hxx ellipeq.h analysis.hxx clipboard.hxx memory.hxx menugfx.hxx popup.hxx pivot.hxx quartic.hxx svg.hxx telescope.hxx undo.hxx glyf.hxx hatch.hxx help.hxx internal_enum.hxx cmdline.hxx conv_internal_cambridge.hxx text.h debug.hxx HQ.h bkchemdata.hxx conv_bkchem_internal.hxx undotree.hxx binaries.hxx fileformat.hxx print.hxx ./generated/bininclude1.txt ./generated/bininclude2.txt ./generated/bininclude3.txt ./generated/bininclude4.txt
	g++ $(CPPFLAGS) -Wl,-z,relro -fstack-protector -g -O0 -std=c++0x -m`getconf LONG_BIT` lhendraw.cxx -o lhendraw -I/usr/include/SDL -L/usr/lib64 -L/usr/lib/x86_64-linux-gnu -lSDL -lm -lX11 -lXmu -Wno-invalid-offsetof -D GFXOUT_SDL -D BITMODE`getconf LONG_BIT` -lrt -Wreturn-type $(LHENDRAW_DEBUG) -D PORTABLE
lhendraw_b32: ./generated/structure.hxx ./generated/reflection.hxx lendefs.h xmldata.hxx xmlparse.hxx cdxdata.hxx conv_cambridge_internal.hxx enums.hxx definitionlist.h janitor.hxx draw.hxx sdlgfx.hxx lhendraw.cxx draw_variables.hxx ./generated/pullout_stringfile.hxx ./generated/reflection_headers.hxx edit.hxx sdlctrl.hxx selection.hxx conv_config_internalconfig.hxx conv_internalconfig_config.hxx resources_init.hxx filedlg.hxx ps.hxx elements.hxx variables.hxx ellipeq.h analysis.hxx clipboard.hxx memory.hxx menugfx.hxx popup.hxx pivot.hxx quartic.hxx svg.hxx telescope.hxx undo.hxx glyf.hxx hatch.hxx help.hxx internal_enum.hxx cmdline.hxx conv_internal_cambridge.hxx HQ.h text.h debug.hxx bkchemdata.hxx conv_bkchem_internal.hxx undotree.hxx binaries.hxx fileformat.hxx print.hxx
	g++ -g -O0 -std=c++0x -m32 lhendraw.cxx -o lhendraw -I/usr/include/SDL -L/usr/lib -L/usr/lib/i386-linux-gnu -lSDL -lm -Wno-invalid-offsetof -D GFXOUT_SDL -D BITMODE32 -lrt -Wreturn-type $(LHENDRAW_DEBUG) -D NOCLIPBOARD
lhendraw_pi: ./generated/structure.hxx ./generated/reflection.hxx lendefs.h xmldata.hxx xmlparse.hxx cdxdata.hxx conv_cambridge_internal.hxx enums.hxx definitionlist.h janitor.hxx draw.hxx sdlgfx.hxx lhendraw.cxx draw_variables.hxx ./generated/pullout_stringfile.hxx ./generated/reflection_headers.hxx edit.hxx sdlctrl.hxx selection.hxx conv_config_internalconfig.hxx conv_internalconfig_config.hxx resources_init.hxx filedlg.hxx ps.hxx elements.hxx variables.hxx ellipeq.h analysis.hxx clipboard.hxx memory.hxx menugfx.hxx popup.hxx pivot.hxx quartic.hxx svg.hxx telescope.hxx undo.hxx glyf.hxx hatch.hxx help.hxx internal_enum.hxx cmdline.hxx conv_internal_cambridge.hxx HQ.h text.h debug.hxx bkchemdata.hxx conv_bkchem_internal.hxx undotree.hxx binaries.hxx fileformat.hxx
	g++ -g -O0 -std=c++0x lhendraw.cxx -o lhendraw -I/usr/include/SDL -L/usr/lib -L/usr/lib/i386-linux-gnu -lSDL -lm -Wno-invalid-offsetof -D GFXOUT_SDL -D BITMODE32 -lrt -Wreturn-type $(LHENDRAW_DEBUG) -D NOCLIPBOARD
mac: ./generated/structure.hxx ./generated/reflection.hxx lendefs.h xmldata.hxx xmlparse.hxx cdxdata.hxx conv_cambridge_internal.hxx enums.hxx definitionlist.h janitor.hxx draw.hxx sdlgfx.hxx lhendraw.cxx draw_variables.hxx ./generated/pullout_stringfile.hxx ./generated/reflection_headers.hxx edit.hxx sdlctrl.hxx selection.hxx conv_config_internalconfig.hxx conv_internalconfig_config.hxx resources_init.hxx filedlg.hxx ps.hxx elements.hxx variables.hxx ellipeq.h analysis.hxx clipboard.hxx memory.hxx menugfx.hxx popup.hxx pivot.hxx quartic.hxx svg.hxx telescope.hxx undo.hxx glyf.hxx hatch.hxx help.hxx internal_enum.hxx cmdline.hxx conv_internal_cambridge.hxx HQ.h text.h debug.hxx bkchemdata.hxx conv_bkchem_internal.hxx undotree.hxx binaries.hxx fileformat.hxx
	g++ -g -O0 -std=c++0x -m64 lhendraw.cxx -o lhendraw -I/Library/Frameworks/SDL2.framework/Headers -lm -Wno-invalid-offsetof -Wno-dynamic-class-memaccess -D GFXOUT_SDL -D BITMODE64 -D NODEBUG -D OSX -ferror-limit=1000 -D SDL2 -framework SDL2 -L/opt/X11/lib/ -D NOCLIPBOARD -D MAC
lennard_infget: makeinf.hxx makeinf_test.cxx cdxdata.hxx lendefs.h
	g++ -g -O0 -std=c++0x -Wno-invalid-offsetof makeinf_test.cxx -o lennard_infget -D DEBUG -D MACHINE_READABLE -Wno-format
./generated/pullout_stringfile.hxx: ./tools/option tools/pullout.c *.pullout.hxx edit.hxx variables.hxx
	./tools/pulloutmatic.sh
	rm ./generated/pullout.hxx ; rm ./generated/pullout_stringfile.hxx ; rm ./generated/pullout_structfile.hxx ; rm ./generated/pullout_reflectfile.hxx ; true;
	cat *.pullout.hxx > ./generated/pullout.hxx
	gcc tools/pullout.c -o tools/pullout
	./tools/pullout
	./tools/option ./edit.hxx ./variables.hxx
test_install:
	mkdir -p /usr/share/lhendraw
	cp lhendraw -t /usr/share/lhendraw
	cp hotkeys.xml -t /usr/share/lhendraw
	cp -R gfx -t /usr/share/lhendraw/
	cp *.ttf -t /usr/share/lhendraw
	cp *.lennardfont -t /usr/share/lhendraw
	ln -s -f /usr/share/lhendraw/lhendraw /usr/bin/lhendraw
install:
	mkdir -p $(DESTDIR)/usr/share/lhendraw
	install -d $(BIN) $(DESTDIR)/usr/share/lhendraw
	install lhendraw $(BIN)
	cp hotkeys.xml -t $(DESTDIR)/usr/share/lhendraw
	cp -R gfx -t $(DESTDIR)/usr/share/lhendraw/
	cp *.lennardfont -t $(DESTDIR)/usr/share/lhendraw
	cp *.bin -t $(DESTDIR)/usr/share/lhendraw
	mkdir -p $(DESTDIR)/usr/share/man/man1
	gzip -c gfx/manpage.1 > $(DESTDIR)/usr/share/man/man1/lhendraw.1.gz
insert64:
	objcopy -I binary -O elf64-little gfx/buttons.bmp dump1.o
	elfedit --output-mach x86-64 dump1.o
insert32:
	ld -r -b binary -o dump1.o gfx/buttons.bmp
./tools/binary: ./tools/binary.c
	gcc -O0 -g ./tools/binary.c -o ./tools/binary
./generated/bininclude1.txt: ./gfx/buttons.bmp hotkeys.xml LiberationMono-Regular.bin LiberationMono-Regular.lennardfont
	./tools/binary ./gfx/buttons.bmp ./generated/bininclude1.txt
	./tools/binary hotkeys.xml ./generated/bininclude2.txt
	./tools/binary LiberationMono-Regular.bin ./generated/bininclude3.txt
	./tools/binary LiberationMono-Regular.lennardfont ./generated/bininclude4.txt
cross_pe32: ./generated/structure.hxx ./generated/reflection.hxx
	ld -m elf_i386 -r -b binary -o ob1.o gfx/buttons.bmp --oformat elf32-i386
	ld -m elf_i386 -r -b binary -o ob2.o hotkeys.xml --oformat elf32-i386
	ld -m elf_i386 -r -b binary -o ob3.o LiberationMono-Regular.bin --oformat elf32-i386
	ld -m elf_i386 -r -b binary -o ob4.o LiberationMono-Regular.lennardfont --oformat elf32-i386
	g++ -v -S -O0 --std=c++0x -Wl,--relocatable  lhendraw.cxx -m32 -o lhendraw.s -mno-red-zone -Wno-invalid-offsetof -D CROFTOIDAL -D BITMODE32 -DFULLCROSS -ffreestanding -fno-pie -fno-exceptions -fno-asynchronous-unwind-tables -fno-use-cxa-atexit -fno-common -mno-tls-direct-seg-refs
	sed -b -i -e 's/\.section	\.text\.\(_Z[NT1].*\),"axG",@progbits,\([^,]*\),comdat/.globl	\1/' lhendraw.s
	sed -b -i -e 's/\.section	\.rodata\.\(_Z[NT1].*\),"aG",@progbits,\([^,]*\),comdat/.globl	\1/' lhendraw.s
	sed -b -i -e 's/^	\.bss\>/	\.data/g' lhendraw.s
	as -v --32 -o lhendraw.o lhendraw.s
	/usr/lib/gcc/x86_64-linux-gnu/4.8/collect2 --sysroot=/ --build-id -m elf_i386 --hash-style=gnu --as-needed -static -z relro -o lhendraw_cross_pe32.o /usr/lib/gcc/x86_64-linux-gnu/4.8/../../../i386-linux-gnu/crt1.o /usr/lib/gcc/x86_64-linux-gnu/4.8/../../../i386-linux-gnu/crti.o /usr/lib/gcc/x86_64-linux-gnu/4.8/32/crtbeginT.o -L/usr/lib/gcc/x86_64-linux-gnu/4.8/32 -L/usr/lib/gcc/x86_64-linux-gnu/4.8/../../../i386-linux-gnu -L/usr/lib/gcc/x86_64-linux-gnu/4.8/../../../../lib32 -L/lib/i386-linux-gnu -L/lib/../lib32 -L/usr/lib/i386-linux-gnu -L/usr/lib/../lib32 -L/usr/lib/gcc/x86_64-linux-gnu/4.8 -L/usr/lib/gcc/x86_64-linux-gnu/4.8/../../../i386-linux-gnu -L/usr/lib/gcc/x86_64-linux-gnu/4.8/../../.. -L/lib/i386-linux-gnu -L/usr/lib/i386-linux-gnu --relocatable lhendraw.o -lstdc++ -lm --start-group -lgcc -lgcc_eh -lc --end-group /usr/lib/gcc/x86_64-linux-gnu/4.8/32/crtend.o /usr/lib/gcc/x86_64-linux-gnu/4.8/../../../i386-linux-gnu/crtn.o
	objcopy lhendraw_cross_pe32.o -R .gnu_debuglink
	objcopy lhendraw_cross_pe32.o -R .comment
	ld -m i386pe windows/imports.def ob1.o ob2.o ob3.o ob4.o lhendraw_cross_pe32.o -o lhendraw.exe --oformat=pei-i386 --subsystem console -e main --ignore-unresolved-symbol _GLOBAL_OFFSET_TABLE_ --ignore-unresolved-symbol 	__preinit_array_start --ignore-unresolved-symbol 	__preinit_array_end --ignore-unresolved-symbol 	__init_array_start --ignore-unresolved-symbol 	__init_array_end --ignore-unresolved-symbol 	__fini_array_start --ignore-unresolved-symbol 	__fini_array_end --ignore-unresolved-symbol ___tls_get_addr
