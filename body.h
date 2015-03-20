#ifndef BODY_H
#define BODY_H
#include <iosfwd>

#include "units.h"

#define EPSILON 0.0001

struct body
{
public:
	mass m;
	//pos
	vec pos;
	//vel
	vec vel;

    vec forceFrom(const vec& other_pos, double other_mass) const;
    vec forceFrom(const body& other) const;
	void update(vec acc,double dt);
};

inline vec calcForce(const vec& pos_diff, double dist, double mass, double other_mass)
{
    distance magnitude = G * mass * other_mass / (dist * dist * dist + EPSILON);
    return magnitude * pos_diff;
}

std::ostream& operator << (std::ostream& o, const vec& v);
std::ostream& operator << (std::ostream& o, const body& b);

#endif
