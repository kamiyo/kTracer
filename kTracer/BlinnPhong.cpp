#include "BlinnPhong.h"

void BlinnPhong::calcNormalizationConst() {
	m_diffuse *= M_1_PI;
#ifndef USE_APPROXIMATION
	m_specular *= (m_power + 2) * (m_power + 4) / (8 * M_PI * (std::pow(2, m_power / 2) + m_power));
#else
	m_specular *= (m_power + 8) / (8 * M_PI);
#endif
}

Rgba BlinnPhong::brdf(const Vector4d& view, const Vector4d& light, const Vector4d& normal, const Rgba& intensity) const {
	double ndotv = normal.dot(view);
	double ndotl = normal.dot(light);
	double ndoth = normal.dot((light + view).normalized());
	if (ndotv < 0) { ndotv = -ndotv; ndotl = -ndotl; ndoth = -ndoth; }
	return intensity * (m_diffuse * std::max(0.0, ndotl) + m_specular * std::pow(ndoth, m_power));	// ndoth is always >= 0 (half vector max is perpendicular)
}
