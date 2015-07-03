#pragma once
#include "Light.h"
class DirectionalLight :
	public Light
{
public:
	DirectionalLight(Vector3d direction, RGB intensity, double r)
		: m_direction(direction)
		, m_radius(r)
	{
		m_intensity = intensity;
		m_type = DIRECTIONAL;
	}
	~DirectionalLight(){}

	double getFalloff(Vector3d point) const {
		return 1.0;
	}

	Vector3d getVector(Vector3d point) const { return -m_direction; }


private:
	Vector3d m_direction;
	double m_radius;
};

