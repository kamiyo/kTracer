#pragma once

#include "Material.h"

class HitRecord
{
public:
	HitRecord() {}
	~HitRecord() {}
	double t;
	Vector3d normal;
	Material* material;
};

