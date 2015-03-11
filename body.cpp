#include "body.h"

vec body::forceFrom(const body& other) const {
    vec pos_diff = pos - other.pos;
    distance sq_dist = pos_diff.sqnorm();
    distance dist = std::sqrt(sq_dist);
    distance magnitude = G * m * other.m / (sq_dist * dist);
    return magnitude * pos_diff;
}
void body::update(body * target, vec acc)
{
	target->m=m;
	target->vel+=acc*=0.5;
	target->pos+=vel;
}
