CC = g++
OPT = -O3
CVER = -std=c++11
CFLAGS = `pkg-config --cflags gtk+-3.0`
DBFLAGS = ''
VGFLAGS = --leak-check=yes --log-file="valgrind.log"
LIBS = `pkg-config --libs gtk+-3.0`
SRC = *.cpp
BIN = cg.o

all: compile run

dbcompile: OPT = -O0
dbcompile: CFLAGS += -g
dbcompile: compile
debug: dbcompile dbrun
vg: dbcompile vgrun

dbrun: 
	gdb $(BIN) $(DBFLAGS)
vgrun: 
	valgrind $(VGFLAGS) ./$(BIN)

compile:
	$(CC) $(OPT) $(CVER) $(CFLAGS) $(SRC) -o $(BIN) $(LIBS)
run:
	./$(BIN)
	
