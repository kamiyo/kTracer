#pragma once

#include "Light.h"

class PointLight :
	public Light
{
public:
	PointLight(Vector3d position, RGB intensity, Vector3d atten, double r)
		: m_position(position)
		, m_radius(r)
	{
		m_intensity = intensity;
		m_attenuation = atten;
		m_type = POINT;
	}
	~PointLight() {}

	double getFalloff(Vector3d point) const {
		double norm = (point - m_position).norm();
		return 1 / m_attenuation.dot(Vector3d(1.0, norm, norm * norm));
	}

	Vector3d getVector(Vector3d point) const { return (m_position - point);	}

private:
	Vector3d m_position;
	double m_radius;
};

