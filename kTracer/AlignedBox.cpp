#include "AlignedBox.h"

bool AlignedBox::hit(RayBase& ray, double t0, double t1, HitRecord& record) const {
	double tmin, tmax, tymin, tymax, tzmin, tzmax;
	const Vector3d e = ray.e();
	const Vector3d i = ray.i();
	const Vector3i s = ray.s();

	tmin = (m_box[s[0]][0] - e[0]) * i[0];
	tmax = (m_box[1 - s[0]][0] - e[0]) * i[0];
	tymin = (m_box[s[1]][1] - e[1]) * i[1];
	tymax = (m_box[1 - s[1]][1] - e[1]) * i[1];
	if ((tmin > tymax) || (tymin > tmax)) return false;
	if (tymin > tmin) tmin = tymin;
	if (tymax < tmax) tmax = tymax;
	tzmin = (m_box[s[2]][2] - e[2]) * i[2];
	tzmax = (m_box[1 - s[2]][2] - e[2]) * i[2];
	if ((tmin > tzmax) || (tzmin > tmax)) return false;
	if (tzmin > tmin) tmin = tzmin;
	if (tzmax < tmax) tmax = tzmax;
	return ((tmin < t1) && (tmax > t0));
}

void AlignedBox::transform(const Transform3d& t) {
	Vector3d rmax, rmin;
	rmin = rmax = t.translation();
	double a, b;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			a = t(i, j) * min()(j);
			b = t(i, j) * max()(j);
			if (a < b) {
				rmin[i] += a;
				rmax[i] += b;
			}
			else {
				rmin[i] += b;
				rmax[i] += a;
			}
		}
	}
	min() = rmin;
	max() = rmax;
	m_midpoint = (min() + max()) / 2.0;
}