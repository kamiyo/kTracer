#pragma once

#include "Material.h"
class Surface;

class HitRecord
{
public:
	HitRecord() {}
	~HitRecord() {}
	double t;
	Vector4d normal;
	const Surface* surface;
};

