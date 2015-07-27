#include "Plane.h"

bool Plane::hit(RayBase& ray, double t0, double t1, HitRecord& record) const {
	double denominator = m_normal.dot(ray.d());
	if (denominator == 0) { return false; }
	
	double numerator = -(m_NP + m_normal.dot(ray.e()));
	double t = numerator / denominator;
	if (t < t0 || t > t1) { return false; }

	record.t = t;
	record.surface = this;
	record.normal = m_normal;

	return true;
}