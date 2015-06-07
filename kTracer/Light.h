#pragma once

#include "typedefs.h"

class Light
{
public:
	Light();
	~Light();
	virtual Vector3d getVector(Vector3d point) const = 0;
	virtual double getFalloff(Vector3d point) const = 0;

	const static int POINT = 0, DIRECTIONAL = 1, SPOT = 2;
protected:
	RGB m_intensity;
	Vector3d m_attenuation;
	int m_type;
};

