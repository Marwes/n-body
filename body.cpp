#include "body.h"

void body::update(body * target, vec acc)
{
	target->m=m;
	target->vel+=acc*=0.5;
	target->pos+=vel;
}
