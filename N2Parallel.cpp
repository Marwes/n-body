
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


void runN2_parallel(std::vector<body>& bodies, const int iterations, const double theta, const int threads, const double dt) 
{
	vec zero;
    int numBodies = bodies.size();
    std::vector<vec> forcematrix(numBodies * threads);
    #pragma omp parallel
    for (int ii = 0; ii < iterations; ++ii) 
	{
		#pragma omp for schedule(dynamic,20)
	    for (int bi = 0; bi < numBodies; ++bi) 
		{
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
		#pragma omp for
	    for (int bi = 0; bi < numBodies; ++bi)
		{
			vec force=zero;
			for(int r=0;r<threads;r++)
				force+=forcematrix[r*numBodies+bi];
	        bodies[bi].update(force,dt);
	    }
    }
}

int main(int argc, char** argv)
{
    NBodyRunner(argc, argv, "output_N2parallel", true, false, runN2_parallel);
    return 0;
}
