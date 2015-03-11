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
    vec forceFrom(const body& other) const;
	void update(body* put, vec acc);
};

#endif
