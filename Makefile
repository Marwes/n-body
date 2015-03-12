CC=g++ -std=c++11
CFLAGS=-O3 -DDIM=3 -DROUNDS=1000 -DDELTA=0.1 
bin=bin/

all: reset comp

debug: CFLAGS += -g
debug: all
	


comp:	N2single N2parallel BH_single BH_parallel generator

generator: generator.cpp body.o IO.o 
	$(CC) $(CFLAGS) -o $(addprefix $(bin), $@ ) $(filter %.cpp %.o,$^)

IO.o:	io.cpp io.h 
	$(CC) $(CFLAGS) -c -o $@ $(filter %.cpp %.o,$^) 


OctTree.o: OctTree.cpp
	$(CC) $(CFLAGS) -c -o $@ OctTree.cpp

body.o: body.cpp body.h
	$(CC) $(CFLAGS) -c -o $@ body.cpp

N2single: body.o N2Single.cpp IO.o
	$(CC) $(CFLAGS) -o $(addprefix $(bin), $@ ) $(filter %.cpp %.o,$^)

N2parallel:	body.o

BH_single:	body.o OctTree.o BH_single.cpp
	$(CC) $(CFLAGS) -o $(addprefix $(bin), $@) $^

BH_parallel:	body.o


clean:
	rm *.o

reset:
	#reset


