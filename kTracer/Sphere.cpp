#include "Sphere.h"

bool Sphere::hit(RayBase& ray, double t0, double t1, HitRecord& record) const {
	const Vector3d ep = ray.e() - m_position;
	const double dep = ray.d().dot(ep);
	const double dd = ray.d().squaredNorm();
	double discriminant = dep * dep - dd * (ep.squaredNorm() - m_radius_2);
	if (discriminant < 0) { return false; } // no intersection
	if (discriminant == 0) { // one intersection
		double t = -dep / dd;
		if (t < t0 || t > t1) { return false; }

		record.t = t;
		if (ray.type() == RayBase::SHADOW) { return true; }

		record.normal = (ray.getPoint(t) - m_position).normalized();
		record.material = m_material;
		return true;
	}

	discriminant = sqrt(discriminant);
	double t = (-dep - discriminant) / dd;
	if (t > t1) { return false; } // sphere is past t1
	if (t < t0) {
		t = (-dep + discriminant) / dd;
		if (t < t0 || t > t1) { return false; } // t0 and t1 are within but not touching sphere
		
		// + intersection
		record.t = t;
		if (ray.type() == RayBase::SHADOW) { return true; }

		record.normal = (ray.getPoint(t) - m_position).normalized();
		record.material = m_material;
		return true;
	}

	// - intersection
	record.t = t;
	if (ray.type() == RayBase::SHADOW) { return true; }

	record.normal = (ray.getPoint(t) - m_position).normalized();
	record.material = m_material;
	return true;
}