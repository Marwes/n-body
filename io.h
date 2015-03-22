#ifndef IO_H
#define IO_H
#include <vector>
#include "body.h"

#define TEXT_MODE 0 

std::vector<body> readBodies(const char * file,int mode);
void writeBodies(const char * file, int mode,std::vector<body> bodies);
std::vector<body> create_bodies(int n);
int init(int argc,char **argv,std::vector<body> & ret_bodies,int & iters,double & dt, double & theta,int & threads);

typedef void (*NBodyFunc)(std::vector<body>& bodies, const int iterations, const double theta, const int workers, const double dt);

//Handles the parsing of command line arguments and then runs `func` which is responsible for the logic of the particular
//N-body algorithm
void NBodyRunner(int argc, char** argv, const char* outputfile, bool needThreads, bool needTheta, NBodyFunc func);


#endif	
