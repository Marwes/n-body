#include "body.h"
#include <iostream>

#define EPSILON 0.0001
vec body::forceFrom(const body& other) const
{
    vec pos_diff = pos - other.pos;
    distance sq_dist = pos_diff.sqnorm();
    distance dist = std::sqrt(sq_dist);
    distance magnitude = G * m * other.m / (sq_dist * dist+EPSILON);
    return magnitude * pos_diff;
}
void body::update(vec force,double dt )
{

	vec acc=force/m;
	
	vel+=(acc/0.5)*dt;
	pos+=vel*dt;

}

