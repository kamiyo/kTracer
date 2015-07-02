#pragma once
#include "RayBase.h"
class Ray :
	public RayBase
{
public:
	Ray(Vector3d e, Vector3d d) : RayBase(e, d, RayBase::VIEW) {
		++count;
	}
	~Ray() {}
	static unsigned long count;
};

