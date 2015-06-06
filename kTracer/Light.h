#pragma once

#include "typedefs.h"

class Light
{
public:
	Light();
	~Light();
	virtual void getVector() const = 0;
	virtual double getFalloff(Vector3d point) const = 0;
protected:
	RGB m_intensity;
	Vector3d m_attenuation;
};

