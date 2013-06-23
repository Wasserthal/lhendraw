all: filestructure.hxx parsecdxml.cxx
	g++ -g -Wno-invalid-offsetof parsecdxml.cxx -o parsexml -D DEBUG -Wno-format
filestructure_maker: filestructure.c
	gcc -std=c99 -g filestructure.c -o filestructure_maker -D BITMODE32
filestructure.hxx: filestructure_maker filestructure.draft
	./filestructure_maker
