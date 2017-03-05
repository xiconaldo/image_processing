all:
	g++ `pkg-config --cflags opencv` main.cpp pdi.cpp `pkg-config --libs opencv` -o main

debug:
	g++ -g3 `pkg-config --cflags opencv` main.cpp pdi.cpp `pkg-config --libs opencv` -o main
	gdb main
