#include "BlinnPhong.h"

void BlinnPhong::calcNormalizationConst() {
	m_diffuse *= M_1_PI;
#ifndef USE_APPROXIMATION
	m_specular *= (m_power + 2) * (m_power + 4) / (8 * M_PI * (std::pow(2, m_power / 2) + m_power));
#else
	m_specular *= (m_power + 8) / (8 * M_PI);
#endif
}

RGB BlinnPhong::brdf(const Vector3d& view, const Vector3d& light, const Vector3d& normal, const RGB& intensity) const {
	double ndotl = normal.dot(light);
	double ndotv = normal.dot(view);
	double ndoth = normal.dot((light + view).normalized());
	return m_diffuse * intensity * std::max(0.0, ndotl) + m_specular * std::pow(ndoth, m_power);	// ndoth is always >= 0 (half vector max is perpendicular)
}
