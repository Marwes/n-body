#ifndef UNITS_H
#define UNITS_H

typedef  double mass;
typedef  double distance;

struct cord
{
	distance a[DIM];

	cord & operator +=(cord rhs)
	{
		for(int i=0;i<DIM;i++)
			a[i]+=rhs.a[i];
	}
	cord & operator -=(cord rhs)
	{
		for(int i=0;i<DIM;i++)
			a[i]-=rhs.a[i];
	}
	cord & operator *=(double rhs)
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
inline cord operator+(cord lhs, const cord& rhs) {
    lhs += rhs;
    return lhs;
}
inline cord operator-(cord lhs, const cord& rhs) {
    lhs -= rhs;
    return lhs;
}
inline cord operator*(double lhs, cord rhs) {
    rhs *= lhs;
    return rhs;
}
#endif
