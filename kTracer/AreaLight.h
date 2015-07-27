#pragma once
#include "Light.h"
#include "Surface.h"

class AreaLight :
	public Light
{
public:
	AreaLight() {}
	AreaLight(const Rgba& intensity, const Vector4d& atten) {
		m_intensity = intensity;
		m_attenuation = atten;
		m_type = AREA;
	}

	AreaLight(const Surface* surface, const Rgba& intensity, const Vector4d& atten) : m_surface(surface) {
		m_intensity = intensity;
		m_attenuation = atten;
		m_type = AREA;
		m_area = m_surface->area();
		m_power = m_intensity * m_area * M_PI;
	}
	~AreaLight() {}

	void setSurface(const Surface* surface) {
		m_surface = surface;
		m_area = m_surface->area();
		m_power = m_intensity * m_area * M_PI;
	}

	Rgba L(const Vector4d& normal, const Vector4d& view) const {
		return (normal.dot(view) > 0.0) ? m_intensity : Vector4d::Zero();
	}

	double getFalloff(const Vector4d& point) const {
		return 1;
	/*	double norm = (point - m_position).norm();
		return 1 / m_attenuation.dot(Vector4d(1.0, norm, norm * norm, 0));
	*/}
	
	Vector4d getVector(const Vector4d& point, const Vector2d& uv) const { return (m_surface->sample(uv) - point); }

	const Surface* m_surface;
	double m_area;
};

