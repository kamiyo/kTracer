#pragma once

#include "typedefs.h"

class Light
{
public:
	virtual Vector4d getVector(const Vector4d& point, const Vector2d& uv = Vector2d::Zero()) const = 0;
	virtual double getFalloff(const Vector4d& point) const = 0;
	virtual Rgba i() const { return m_intensity; }
	int type() const { return m_type; }
	const static int POINT = 0, DIRECTIONAL = 1, SPOT = 2, AREA = 3;

protected:
	Rgba m_intensity;
	Rgba m_power;
	Vector4d m_attenuation;
	int m_type;
};

