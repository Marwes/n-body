#ifndef UNITS_H
#define UNITS_H
#include <cstddef>
#include <assert.h>

typedef  double mass;
typedef  double distance;

struct vec
{
	distance a[DIM];

    vec(distance x = 0, distance y = 0, distance z = 0) {
        assert(DIM == 3);
        a[0] = x;
        a[1] = y;
        a[2] = z;
    }

	vec & operator +=(vec rhs)
	{
		for(int i=0;i<DIM;i++)
			a[i]+=rhs.a[i];
	}
	vec & operator -=(vec rhs)
	{
		for(int i=0;i<DIM;i++)
			a[i]-=rhs.a[i];
	}
	vec & operator *=(double rhs)
	{
		for(int i=0;i<DIM;i++)
			a[i]*=rhs;
    }
    distance& operator[](std::size_t idx) {
        return a[idx];
    }

    const distance& operator[](std::size_t idx) const {
        return a[idx];
    }
};
inline vec operator+(vec lhs, const vec& rhs) {
    lhs += rhs;
    return lhs;
}
inline vec operator-(vec lhs, const vec& rhs) {
    lhs -= rhs;
    return lhs;
}
inline vec operator*(double lhs, vec rhs) {
    rhs *= lhs;
    return rhs;
}
#endif
