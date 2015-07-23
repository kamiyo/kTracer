#include "Circle.h"

bool Circle::hit(RayBase& ray, double t0, double t1, HitRecord& record) const {
	double denominator = m_normal.dot(ray.d());
	if (denominator == 0) { return false; }

	double numerator = -(m_NP + m_normal.dot(ray.e()));
	double t = numerator / denominator;
	if (t < t0 || t > t1) { return false; }

	double distance2 = (ray(t) - m_position).squaredNorm();
	if (distance2 > m_radius_2) { return false; }

	record.t = t;
	if (ray.type() == RayBase::SHADOW) { return true; }

	record.normal = m_normal;
	record.surface = this;

	return true;
}
