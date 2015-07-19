#pragma once

#include "Light.h"

class PointLight :
	public Light
{
public:
	PointLight(const Vector4d& position, const Rgba& intensity, const Vector4d& atten, double r)
		: m_position(position)
		, m_radius(r)
	{
		m_intensity = intensity;
		m_attenuation = atten;
		m_type = POINT;
	}
	~PointLight() {}

	double getFalloff(const Vector4d& point) const {
		double norm = (point - m_position).norm();
		return 1 / m_attenuation.dot(Vector4d(1.0, norm, norm * norm, 0));
	}

	Vector4d getVector(const Vector4d& point) const { return (m_position - point);	}

private:
	Vector4d m_position;
	double m_radius;
};

