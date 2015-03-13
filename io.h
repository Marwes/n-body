#ifndef IO_H
#define IO_H
#include <vector>
#include "body.h"

#define TEXT_MODE 0 

std::vector<body> readBodies(const char * file,int mode);
void writeBodies(const char * file, int mode,std::vector<body> bodies);
std::vector<body> create_bodies(int n);
int init(int argc,char **argv,std::vector<body> & ret_bodies,int & iters,double & dt, double & theta,int & threads);

#endif	
