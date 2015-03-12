
#include <vector>
#include "body.h"
#include "io.h"
#include <functional> 
#include <iostream>

double dt;

void setup(int argc, char** argv,std::vector<body> & v_b,int & iters,double & dt)
{
	v_b=readBodies(argv[1],TEXT_MODE);
	int n=1;
	std::hash<std::string> str_hash;
	while(argc>n)
	{
		n++;
		int hash=str_hash(argv[n]);
		switch (hash)
		{
			
			default:
				std::cerr<<"Error incorrect args\n";
				exit(1);
		}
	}

}
int main(int argc, char** argv)
{

    int iterations=100 ;
    std::vector<body> bodies;
	
	setup(argc,argv,bodies,iterations,dt);
	int numBodies=bodies.size();
    std::vector<vec> forces(bodies.size());
    for (int ii = 0; ii < iterations; ++ii) 
	{
        for (int bi = 0; bi < numBodies; ++bi) 
		{
            vec force;
            const body& self = bodies[bi];
            for (int bj = 0; bj < numBodies; ++bj) 
			{
                force += self.forceFrom(bodies[bj]);
            }
            forces[bi] = force;
        }
        for (int bi = 0; bi < numBodies; ++bi) {
            bodies[bi].update(forces[bi],dt);
        }
    }
    return 0;
}
