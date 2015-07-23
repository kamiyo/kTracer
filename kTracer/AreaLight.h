#pragma once
#include "Light.h"
#include "Surface.h"

class AreaLight :
	public Light
{
public:
	AreaLight() {}
	AreaLight(const Surface* surface, const Rgba& intensity, const Vector4d& atten) : m_surface(surface) {
		m_intensity = intensity;
		m_attenuation = atten;
		m_type = AREA;
		m_area = m_surface->area();
		m_power = m_intensity * m_area * M_PI;
	}
	~AreaLight() {}

	/*double getFalloff(const Vector4d& point) const {
		double norm = (point - m_position).norm();
		return 1 / m_attenuation.dot(Vector4d(1.0, norm, norm * norm, 0));
	}
	*/
	Vector4d getVector(const Vector4d& point) const { return (m_surface->sample() - point); }

	const Surface* m_surface;
	double m_area;
};

