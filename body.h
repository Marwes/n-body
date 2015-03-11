#ifndef BODY_H
#define BODY_H
#include "units.h"
struct body
{
public:
	mass m;
	//pos
	vec pos;
	//vel
	vec vel;
	void update(body* put, vec acc);
};

#endif
