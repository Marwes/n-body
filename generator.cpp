#include "body.h"
#include "io.h"
#include <iostream>
#include <vector>

#include <string>

int main(int argc, char ** argv)
{
	if (argc<2)
	{	
		std::cerr<<"missing args\t args are file , number of bodies\n";
		exit(1);
	}
	char * file =argv[1];	
	int n=atoi(argv[2]);
	std::vector<body> bodies;

	bodies= create_bodies(n);
	writeBodies(file, 0, bodies);

}
