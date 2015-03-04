#include "body.h"
#include "io.h"

#include <vector>
#include <random>

int main()
{
	int n=100;
	std::vector<body> bodies;

	std::uniform_real_distribution<double> pos_rand(0,100);
	std::exponential_distribution<double> mass_rand(1);
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
			b.vel.a[i]=pos_rand(re);
		}
		bodies.push_back(b);
	}
	writeBodies("data/test_1", 0, bodies);

}
