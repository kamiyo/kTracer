#pragma once

#include "typedefs.h"
#include "RayBase.h"
#include "Light.h"

class Material
{
public:
	Material() {}
	~Material() {}
	virtual RGB brdf(const Vector3d& view, const Light& light, const Vector3d& normal) = 0;
};

