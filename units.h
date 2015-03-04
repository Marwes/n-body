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
	cord & operator *=(double rhs)
	{
		for(int i=0;i<DIM;i++)
			a[i]*=rhs;
	}
};
#endif
