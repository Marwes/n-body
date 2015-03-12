#include "body.h"
#include <iostream>

vec body::forceFrom(const body& other) const {
    return forceFrom(other.pos, other.m);
}
vec body::forceFrom(const vec& other_pos, double other_mass) const
{
    vec pos_diff = pos - other_pos;
    distance sq_dist = pos_diff.sqnorm();
    distance dist = std::sqrt(sq_dist);
    distance magnitude = G * m * other_mass / (sq_dist * dist);
    return magnitude * pos_diff;
}
void body::update(vec force,double dt )
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
