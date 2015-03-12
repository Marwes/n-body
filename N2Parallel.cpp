
#include <vector>
#include "body.h"
#include "io.h"
#include <functional> 
#include <iostream>
#include <string>
#include <cstdlib>
#include <omp.h>
#include <cstring>
#define PROGRES_BAR_INC 10

double dt=DELTA;
int threads=1;

void setup(int argc, char** argv,std::vector<body> & v_b,int & iters,double & dt)
{
	char * file="data/test_1";
	dt=1;
	for (int n=1;n<argc;n++)
	{
		std::cout<<n<<"\t"<<argv[n]<<"\n";
		if(!strcmp(argv[n],"-dt"))
		{
			n++;
			dt=atof(argv[n]);
			continue;
		}
		if(!strcmp(argv[n],"-i"))
		{
			n++;
			iters=atoi(argv[n]);
			continue;
		}
		if(!strcmp(argv[n],"-f"))
		{
			n++;
			file=argv[n];
			continue;
		}
		if(!strcmp(argv[n],"-n"))
		{
			n++;
			threads=atoi(argv[n]);
			continue;
		}
		std::cerr<<"Error argument dont match\n";
		exit(1);
	}
	v_b=readBodies(file,TEXT_MODE);
	omp_set_num_threads(threads);
}
int main(int argc, char** argv)
{
	vec zero;
	for(int a=0;a<DIM;a++)
		zero[a]=0;
    int iterations=100000 ;
    std::vector<body> bodies;
	
	setup(argc,argv,bodies,iterations,dt);
	std::cout<<"number of bodies = "<<bodies.size()<<std::endl;
	int numBodies=bodies.size();
    std::vector<vec> forces(bodies.size());
	std::cout<<"iterations "<<iterations<<"\tdt = "<<dt<<"\n";
    
    for (int ii = 0; ii < iterations; ++ii) 
	{
		if((ii%(iterations/PROGRES_BAR_INC +1))==0)	
			std::cout<<ii<<"/"<<iterations<<"\n";

		{
			#pragma omp parallel for
		    for (int bi = 0; bi < numBodies; ++bi) 
			{
		        forces[bi]=zero;
				vec force=zero;
		        const body& self = bodies[bi];
		        for (int bj = 0; bj < numBodies; ++bj) 
				{
		            force += self.forceFrom(bodies[bj]);
		        }
				forces[bi]=force;
		    }
			#pragma omp parallel for
		    for (int bi = 0; bi < numBodies; ++bi) {
		        bodies[bi].update(forces[bi],dt);
		    }
		}
    }
	writeBodies("output_N2single", 0,bodies);
    return 0;
}
