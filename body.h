#ifndef BODY_H
#define BODY_H
#include <iosfwd>

#include "units.h"
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

std::ostream& operator << (std::ostream& o, const vec& v);
std::ostream& operator << (std::ostream& o, const body& b);

#endif
