#include "Sphere.h"

bool Sphere::hit(RayBase& ray, double t0, double t1, HitRecord& record) const {
	const Vector4d ep = ray.e() - m_position;
	const double dep = ray.d().dot(ep);
	const double dd = ray.d().squaredNorm();
	const double c = ep.squaredNorm() - m_radius_2;
	double discriminant = dep * dep - dd * c;
	if (discriminant < 0) { return false; } // no intersection
	if (discriminant == 0) { // one intersection
		double t = -dep / dd;
		if (t < t0 || t > t1) { return false; }

		record.t = t;
		if (ray.type() == RayBase::SHADOW) { return true; }

		record.normal = (ray(t) - m_position).normalized();
		record.surface = this;
		return true;
	}

	discriminant = sqrt(discriminant);
	double t;
	if (dep >= 0) {
		t = (-dep - discriminant) / dd;
	}
	else {
		t = c / (-dep + discriminant);
	}
	if (t > t1) { return false; } // sphere is past t1
	if (t < t0) {
		if (dep >= 0) {
			t = c / (-dep - discriminant);
		} else {
			t = (-dep + discriminant) / dd;
		}
		if (t < t0 || t > t1) { return false; } // t0 and t1 are within but not touching sphere
		
		// + intersection
		record.t = t;
		if (ray.type() == RayBase::SHADOW) { return true; }

		record.normal = (ray(t) - m_position).normalized();
		record.surface = this;
		return true;
	}

	// - intersection
	record.t = t;
	if (ray.type() == RayBase::SHADOW) { return true; }

	record.normal = (ray(t) - m_position).normalized();
	record.surface = this;
	return true;
}