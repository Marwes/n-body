#include "io.h"
#include <iostream>
#include <fstream>


std::vector<body> readBodies_text(char * file)
{
	std::vector<body> ret;
	
	std::ifstream in(file);
	while(!in.eof())
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


std::vector<body> readBodies(char * file, int mode)
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
void writeBodies_text(char * file,std::vector<body> bodies)
{
	
	std::ofstream out(file);
	for(int i=0;i<bodies.size();i++)
	{
		std::cout <<i<<"\n";
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

void writeBodies(char * file, int mode,std::vector<body> bodies)
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
