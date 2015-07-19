#pragma once
#include "Light.h"
class DirectionalLight :
	public Light
{
public:
	DirectionalLight(const Vector4d& direction, const Rgba& intensity, double r)
		: m_direction(direction)
		, m_radius(r)
	{
		m_intensity = intensity;
		m_type = DIRECTIONAL;
	}
	~DirectionalLight(){}

	double getFalloff(const Vector4d& point) const {
		return 1.0;
	}

	Vector4d getVector(const Vector4d& point) const { return -m_direction; }


private:
	Vector4d m_direction;
	double m_radius;
};

