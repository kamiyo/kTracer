#pragma once

#include "AreaLight.h"

class PointLight :
	public AreaLight
{
public:
	PointLight(const Vector4d& position, const Rgba& intensity, const Vector4d& atten, double r, Surface* surface = nullptr)
		: m_position(position)
		, m_radius(r)
	{
		m_surface = surface;
		m_intensity = intensity;
		m_power = 4.0 * M_PI * m_intensity;
		m_attenuation = atten;
		m_type = POINT;
		m_area = (m_surface) ? m_surface->area() : 0.0;
	}
	~PointLight() {}
	Rgba i() const { return m_intensity; }

	double getFalloff(const Vector4d& point) const {
		double norm = (point - m_position).norm();
		return 1 / m_attenuation.dot(Vector4d(1.0, norm, norm * norm, 0));
	}

	Vector4d getVector(const Vector4d& point) const { return (m_surface) ? (m_surface->sample() - point) : (m_position - point);	}

private:
	Vector4d m_position;
	double m_radius;

};

