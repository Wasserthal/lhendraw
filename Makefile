all:
	g++ -g -Wno-invalid-offsetof parsecdxml.cxx -o parsexml -D DEBUG -Wno-format
