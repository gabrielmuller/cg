all: compile run

compile:
	g++ `pkg-config --cflags gtk+-3.0` teste.cpp -o teste.o `pkg-config --libs gtk+-3.0`
run:
	./teste.o
	
