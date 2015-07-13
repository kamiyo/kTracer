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
	virtual Rgba brdf(const Vector4d& view, const Vector4d& light, const Vector4d& normal, const Rgba& intensity) const = 0;

	std::string m_name;
};

