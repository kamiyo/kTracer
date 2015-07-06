#pragma once
#include "RayBase.h"
class Ray :
	public RayBase
{
public:
	Ray(Vector3d e, Vector3d d, int type) : RayBase(e, d, type) {
		++count;
	}
	~Ray() {}
	static unsigned long count;
};

