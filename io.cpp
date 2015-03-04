#include "io.h"
#include <iostream>
#include <fstream>


std::vector<body> readBodies_text(char * file)
{
	std::vector<body> ret;
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
