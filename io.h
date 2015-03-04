#ifndef IO_H
#define IO_H
#include <vector>
#include "body.h"

#define TEXT_MODE 0 

std::vector<body> readBodies(char * file);
void writeBodies(char * file, int mode,std::vector<body> bodies);


#endif	
