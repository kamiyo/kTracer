#include "Plane.h"

bool Plane::hit(RayBase& ray, double t0, double t1, HitRecord& record) const {
	const Vector3d dir = ray.d();
	double denominator = m_normal.dot(dir);
	if (denominator == 0) { return false; }
	
	double numerator = m_normal.dot(m_position - ray.e());
	double t = numerator / denominator;
	if (t < t0 || t > t1) { return false; }

	if (ray.type == RayBase::SHADOW) return true;
	
	record.normal = m_normal;
	record.material = m_material;

	return true;
}