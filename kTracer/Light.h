#pragma once

#include "typedefs.h"

class Light
{
public:
	virtual Vector4d getVector(Vector4d point) const = 0;
	virtual double getFalloff(Vector4d point) const = 0;
	Rgba i() const { return m_intensity; }
	int type() const { return m_type; }
	const static int POINT = 0, DIRECTIONAL = 1, SPOT = 2;

protected:
	Rgba m_intensity;
	Vector4d m_attenuation;
	int m_type;
};

