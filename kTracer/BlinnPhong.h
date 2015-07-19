#pragma once

#include "Material.h"
#define USE_APPROXIMATION

class BlinnPhong :
	public Material
{
public:
	BlinnPhong(std::string name, const Rgba& diffuse, const Rgba& specular, double power, bool normalized = false)
		: m_diffuse(diffuse)
		, m_specular(specular)
		, m_power(power)
	{
		m_name = name;
		if (normalized) calcNormalizationConst();
	}
	
	~BlinnPhong() {}
	Rgba brdf(const Vector4d& view, const Vector4d& light, const Vector4d& normal, const Rgba& intensity) const;

private:
	void calcNormalizationConst();

	Rgba m_diffuse, m_specular;
	double m_power;
};

