ld -r -b binary -o ob1.o gfx/buttons.bmp --oformat pei-i386
ld -r -b binary -o ob2.o hotkeys.xml --oformat pei-i386
ld -r -b binary -o ob3.o LiberationMono-Regular.bin --oformat pei-i386
ld -r -b binary -o ob4.o LiberationMono-Regular.lennardfont --oformat pei-i386
g++  ob1.o ob2.o ob3.o ob4.o -O0 --std=c++0x lhendraw.cxx -lgdi32 -lwinspool -o lhendraw.exe -mwindows -Wno-invalid-offsetof -D CROFTOIDAL -D BITMODE32
