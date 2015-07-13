#pragma once
#include "RayBase.h"
class Ray :
	public RayBase
{
public:
	Ray(Vector4d e, Vector4d d, int type) : RayBase(e, d, type) {
		++count;
	}
	~Ray() {}
	static unsigned long count;
};

