#include "body.h"
#include <iostream>


vec body::forceFrom(const body& other) const 
{
    return forceFrom(other.pos, other.m);
}

vec body::forceFrom(const vec& other_pos, double other_mass) const
{
    vec pos_diff = pos - other_pos;
    distance dist = pos_diff.norm();
    return calcForce(pos_diff, dist, m, other_mass);
}

void body::update(vec force, double dt)
{
	vec acc=force/m;	
	vel+=(acc/0.5)*dt;
	pos+=vel*dt;

}

std::ostream& operator << (std::ostream& o, const vec& v)
{
    o << "(";
    o << v[0];
    for (int i = 1; i < DIM; ++i) {
        o << ", " << v[i];
    }
    o << ")";
    return o; 
}

std::ostream& operator << (std::ostream& o, const body& b)
{
    o << b.m << " " << b.pos << " " << b.vel;
    return o; 
}
