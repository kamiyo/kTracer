#pragma once

#include "HitRecord.h"
#include "RayBase.h"

class Intersectable
{
public:
	virtual bool hit(RayBase& ray, double t0, double t1, HitRecord& record) const = 0;

};

