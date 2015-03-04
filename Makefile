CC=g++ -std=c++11
CFLAGS=-O3 -DDIM=3
bin=bin/

all: reset comp clean

comp:	N2single N2parallel BH_single BH_parallel

IO.o:	io.cpp io.h 
	$(CC) $(CFLAGS) -c -o $@ $(filter %.cpp %.o,$^) 

body.o: body.cpp body.h
	$(CC) $(CFLAGS) -c -o $@ body.cpp 

N2single: body.o N2Single.cpp IO.o
	$(CC) $(CFLAGS) -o $(addprefix $(bin), $@ ) $(filter %.cpp %.o,$^)
N2parallel:	body.o
BH_single:	body.o
BH_parallel:	body.o


clean:

reset:
	#reset

