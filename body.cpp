#include "body.h"

void body::update(body * target,cord acc)
{
	target->m=m;
	target->vel+=acc*=0.5;
	target->pos+=vel;
}
