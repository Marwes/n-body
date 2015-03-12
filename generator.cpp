#include "body.h"
#include "io.h"
#include <iostream>
#include <vector>
#include <random>
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

	std::uniform_real_distribution<double> pos_rand(-10000,10000);
	std::uniform_real_distribution<double> vel_rand(0,100);
	std::exponential_distribution<double> mass_rand(1.0/10000000000L);
	std::default_random_engine re;
	for(int i=0;i<n;i++)
	{
		body b;
		b.m=mass_rand(re);
		for(int i=0;i<DIM;i++)
		{
			b.pos.a[i]=pos_rand(re);
		}
		for(int i=0;i<DIM;i++)
		{
			b.vel.a[i]=vel_rand(re);
		}
		bodies.push_back(b);
	}
	writeBodies(file, 0, bodies);

}
