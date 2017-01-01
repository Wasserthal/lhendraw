ld -r -b binary -o ob1.o gfx/buttons.bmp --oformat elf32-i386
ld ob1.o -o obj1.o --oformat=pei-i386 --defsym=_binary_gfx_buttons_bmp_start=0
g++ -O0 ob1.o --std=c++0x lhendraw.cxx -lgdi32 -o lhendraw.exe -Wno-invalid-offsetof -D CROFTOIDAL -D BITMODE32