#pragma once

#include "Material.h"
#define USE_APPROXIMATION

class BlinnPhong :
	public Material
{
public:
	BlinnPhong(RGB diffuse, RGB specular, double power, bool normalized = false)
		: m_diffuse(diffuse)
		, m_specular(specular)
		, m_power(power)
	{
		if (normalized) calcNormalizationConst();
	}
	
	~BlinnPhong() {}
	RGB brdf(const Vector3d& view, const Vector3d& light, const Vector3d& normal, const RGB& intensity) const;

private:
	void calcNormalizationConst();

	RGB m_diffuse, m_specular;
	double m_power;
};

