#pragma once

#include "typedefs.h"
#include "RayBase.h"
#include "Light.h"

class Material
{
public:
	Material() {}
	~Material() {}
	/* view, normal, and light vectors have to be away from intersection point (not towards) and normalized */
	virtual RGB brdf(const Vector3d& view, const Vector3d& light, const Vector3d& normal, const RGB& intensity) const = 0;

	std::string m_name;
};

