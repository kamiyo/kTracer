#pragma once
#include "Intersectable.h"
class AlignedBox :
	public Intersectable
{
public:
	AlignedBox() { min() = Vector3d::Constant(INF), max() = Vector3d::Constant(nINF); }
	AlignedBox(Vector3d newmin, Vector3d newmax) { 
		min() = newmin, max() = newmax;
		m_midpoint = (min() + max()) / 2.0;
	}
	~AlignedBox() {}
	
	inline Vector3d& min() { return m_box[0]; }
	inline Vector3d& max() { return m_box[1]; }
	inline Vector3d mid() { return m_midpoint; }
	inline bool isEmpty() { return (max().array() < min().array()).any(); }

	void extend(AlignedBox& other) {
		min() = min().cwiseMin(other.min());
		max() = max().cwiseMax(other.max());
		m_midpoint = (min() + max()) / 2.0;
	}

	AlignedBox merged(AlignedBox& other) {
		return AlignedBox(min().cwiseMin(other.min()), max().cwiseMax(other.max()));
	}

	bool hit(RayBase& ray, double t0, double t1, HitRecord& record) const {
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

	void transform(const Transform3d& t) {
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

	inline bool operator==(AlignedBox& rhs) {
		return ((min().cwiseEqual(rhs.min()).all() && max().cwiseEqual(rhs.max()).all()));
	}
	inline bool operator!=(AlignedBox& rhs) {
		return ((min().cwiseNotEqual(rhs.min()).any() || max().cwiseNotEqual(rhs.max()).any()));
	}

private:
	Vector3d m_box[2];
	Vector3d m_midpoint;
};

