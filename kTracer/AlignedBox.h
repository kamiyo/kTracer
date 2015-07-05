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

	bool hit(RayBase& ray, double t0, double t1, HitRecord& record) const;

	void transform(const Transform3d& t);

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

