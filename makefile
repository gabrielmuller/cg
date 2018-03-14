CC = g++
OPT = -O3
CVER = -std=c++11
CFLAGS = `pkg-config --cflags gtk+-3.0`
LIBS = `pkg-config --libs gtk+-3.0`
SRC = *.cpp
BIN = cg.o

all: compile run

compile:
	$(CC) $(OPT) $(CVER) $(CFLAGS) $(SRC) -o $(BIN) $(LIBS)
run:
	./$(BIN)
	
