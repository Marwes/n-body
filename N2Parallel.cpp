
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
		//std::cout<<n<<"\t"<<argv[n]<<"\n";
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
	double start,end;
	vec zero;
	for(int a=0;a<DIM;a++)
		zero[a]=0;
    int iterations=100000 ;
    std::vector<body> bodies;
	
	setup(argc,argv,bodies,iterations,dt);
	//std::cout<<"number of bodies = "<<bodies.size()<<std::endl;
	int numBodies=bodies.size();
    std::vector<vec> forces(bodies.size());
	//std::cout<<"iterations "<<iterations<<"\tdt = "<<dt<<"\n";
    
	vec * forcematrix=new vec[numBodies*threads];
	start=omp_get_wtime(); 
    for (int ii = 0; ii < iterations; ++ii) 
	{
		//if((ii%(iterations/PROGRES_BAR_INC +1))==0)	
		//	std::cout<<ii<<"/"<<iterations<<"\n";

		
		#pragma omp parallel for schedule(dynamic,20)
	    for (int bi = 0; bi < numBodies; ++bi) 
		{
	        forces[bi]=zero;
			vec force=zero;
	        const body& self = bodies[bi];
			int rank= omp_get_thread_num();
			//std::cout<<rank<<" -> "<<bi<<std::endl;
	        for (int bj = bi+1; bj < numBodies; ++bj) 
			{
				force= self.forceFrom(bodies[bj]);
	            forcematrix[rank*numBodies+bj]+=force;
	            forcematrix[rank*numBodies+bi]+=force;
	        }
	    }
		#pragma omp parallel for
	    for (int bi = 0; bi < numBodies; ++bi)
		{
			vec force=zero;
			for(int r=0;r<threads;r++)
				force+=forcematrix[r*numBodies+bi];
	        bodies[bi].update(force,dt);
	    }
		
    }
	end=omp_get_wtime(); 
	std::cout<<(end-start)<<"\t";
	writeBodies("output_N2parallel", 0,bodies);
    return 0;
}
