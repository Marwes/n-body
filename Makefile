CC=g++ std=c++11
CFLAGS=-O3
Dim=-DDIM=3

all: reset comp clean

comp:	N2single N2parallel BH_single BH_parallel

body.o: body.cpp body.h
	$(CC) $(CFLAGS) -c $@ body.cpp

N2single:
	$(CC) $(CFLAGS) -c $@ 
N2parallel:
BH_single:
BH_parallel:


clean:

reset:
	reset

