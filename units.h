#ifndef UNITS_H
#define UNITS_H
#include <cstddef>
#include <cmath>
#include <assert.h>

typedef  double mass;
typedef  double distance;

const distance G = 6.67384 / 10000000000.0;// 10^-11 m3 kg-1 s-2

struct vec
{
	distance a[DIM];

    vec(distance x = 0, distance y = 0, distance z = 0) {
        assert(DIM == 3);
        a[0] = x;
        a[1] = y;
        a[2] = z;
    }

    //Calculates the squared norm of the vector
    distance sqnorm() const {
        distance result = 0;
		for(int i = 0; i < DIM; i++)
			result += a[i] * a[i];
        return result;
    }

    //Calculates the the norm of the vector
    distance norm() const {
        return std::sqrt(sqnorm());
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
inline vec operator/(vec lhs, double rhs) {
    vec v;
	for(int i=0;i<DIM;i++)
		v[i]=lhs[i]/rhs;
    return v;
}
inline vec operator*(vec lhs, double rhs) {
    vec v;
	for(int i=0;i<DIM;i++)
		v[i]=lhs[i]*rhs;
    return v;
}
#endif
