
#include <vector>
#include "body.h"
#include "io.h"


void runN2_single(std::vector<body>& bodies, const int iterations, const double theta, const int threads, const double dt) {
    vec zero;
    int numBodies = bodies.size();
    std::vector<vec> forces(bodies.size());
    for (int ii = 0; ii < iterations; ++ii) 
	{
		//if((ii%(iterations/PROGRES_BAR_INC +1))==0)	
		//	std::cout<<ii<<"/"<<iterations<<"\n";
        for (int bi = 0; bi < numBodies; ++bi) 
		{
            forces[bi]=zero;
			vec force;
            const body& self = bodies[bi];
            for (int bj = bi+1; bj < numBodies; ++bj) 
			{
				
                force = self.forceFrom(bodies[bj]);
				forces[bi]+=force;
				forces[bj]+=force;
            }
        }
        for (int bi = 0; bi < numBodies; ++bi) {
            bodies[bi].update(forces[bi],dt);
        }
    }
}

int main(int argc, char** argv)
{
	NBodyRunner(argc, argv, "output_N2single", runN2_single);
    return 0;
}
