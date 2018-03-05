all: compile run

compile:
	g++ -O3 -std=c++11 `pkg-config --cflags gtk+-3.0` *.cpp -o cg.o `pkg-config --libs gtk+-3.0`
run:
	./cg.o
	
