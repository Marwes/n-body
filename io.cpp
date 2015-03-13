#include "io.h"
#include <iostream>
#include <random>
#include <fstream>
#include <omp.h>
#include <string.h>

std::vector<body> create_bodies(int n)
{
	std::vector<body> bodies;
	std::uniform_real_distribution<double> pos_rand(-10000,10000);
	std::uniform_real_distribution<double> vel_rand(-10,10);
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
	return bodies;
}

int init(int argc,char **argv,std::vector<body> & ret_bodies,int & iters,double & dt, double & theta,int & threads)
{
	int n=1;
	//numbodies
	int size=strtol(argv[n],nullptr,10);
	if(size)
	{
		n++;
		ret_bodies=create_bodies(size);
	}
	else
	{
		ret_bodies=readBodies(argv[n], 0);
		n++;
	}
	//iters
	iters=atoi(argv[n]);
	n++;
	//theta
	if (argc==5)
	{
		theta=atof(argv[n]);
		n++;
	}
	//threads
	if(argc>3)
	{
		threads=atoi(argv[n]);
	}
	else 
		threads=1;
	return 0;
}


std::vector<body> readBodies_text(const char * file)
{
	std::vector<body> ret;
	
	std::ifstream in(file);
	int rows;
	in>>rows;
	for (int i=0;i<rows;i++)
	{
		body b;
		in>>b.m;
		for(int i=0;i<DIM;i++)
		{
			in>>b.pos.a[i];
		}
		for(int i=0;i<DIM;i++)
		{
			in>>b.vel.a[i];
		}
		ret.push_back(b);
	}
	return ret;
}


std::vector<body> readBodies(const char * file, int mode)
{
	switch (mode)
	{
		case TEXT_MODE:
			return readBodies_text(file);
			
		default:
			std::cerr<<"error unknown mode"<<std::endl;
			exit(1);
	}

	
}
void writeBodies_text(const char * file,std::vector<body> bodies)
{
	
	std::ofstream out(file);
	out<<bodies.size();
	for(int i=0;i<bodies.size();i++)
	{
		//std::cout <<i<<"\n";
		body b=bodies[i];
		out<<"\n"<<b.m;
		for(int i=0;i<DIM;i++)
		{
			out<<" "<<b.pos.a[i];
		}
		for(int i=0;i<DIM;i++)
		{
			out<<" "<<b.vel.a[i];
		}
	}
	return ;
}

void writeBodies(const char * file, int mode,std::vector<body> bodies)
{
	switch (mode)
	{
		case TEXT_MODE:
			return writeBodies_text(file,bodies);
			
		default:
			std::cerr<<"error unknown mode"<<std::endl;
			exit(1);
	}

	
}


void NBodyRunner(int argc, char** argv, const char* outputfile, NBodyFunc func) {
    std::vector<body> bodies;
    int iterations = 100;
    double dt = 0.1;
    double theta = 0.5;
    int threads = 1;
	init(argc,argv,bodies, iterations,dt, theta,threads);
	omp_set_num_threads(threads);
	double start=omp_get_wtime(); 
    func(bodies, iterations, theta, threads, dt);
	double end=omp_get_wtime(); 
	std::cout<<(end-start)<<"\t";
	writeBodies(outputfile, 0, bodies);
}
