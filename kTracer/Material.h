#pragma once

#include "typedefs.h"
#include "RayBase.h"
#include "Light.h"

class Material
{
public:
	Material() {}
	~Material() {}
	virtual RGB brdf(const Vector3d& view, const Vector3d& light, const Vector3d& normal, const RGB& intensity) const = 0;

	std::string m_name;
};

