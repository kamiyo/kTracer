#pragma once

#include "Material.h"

class HitRecord
{
public:
	HitRecord() {}
	~HitRecord() {}
	double t;
	Vector4d normal;
	Material* material;
};

