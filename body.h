#ifndef BODY_H
#define BODY_H
#include "units.h"
class body
{
	mass m;
	//pos
	cord pos;
	//vel
	cord vel;
	void update(body* put, cord acc);
}

#endif
