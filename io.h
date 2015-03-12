#ifndef IO_H
#define IO_H
#include <vector>
#include "body.h"

#define TEXT_MODE 0 

std::vector<body> readBodies(const char * file,int mode);
void writeBodies(const char * file, int mode,std::vector<body> bodies);


#endif	
