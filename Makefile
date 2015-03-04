CC=g++ -std=c++11
CFLAGS=-O3
Dim=-DDIM=3
bin=bin/

all: reset comp clean

comp:	N2single N2parallel BH_single BH_parallel

body.o: body.cpp body.h
	$(CC) $(CFLAGS) -c -o $@ body.cpp $(Dim)

N2single: body.o N2Single.cpp
	$(CC) $(CFLAGS) -o $(addprefix $(bin), $@) $^
N2parallel:	body.o
BH_single:	body.o
BH_parallel:	body.o


clean:

reset:
	reset

