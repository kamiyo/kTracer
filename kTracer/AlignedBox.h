#pragma once
#include "Intersectable.h"
class AlignedBox :
	public Intersectable
{
public:
	AlignedBox() { setEmpty(); }
	AlignedBox(const Vector4d& point) {
		min() = max() = point;
		m_midpoint = (min() + max()) / 2.0;
	}
	AlignedBox(const Vector4d& newmin, const Vector4d& newmax) {
		min() = newmin, max() = newmax;
		m_midpoint = (min() + max()) / 2.0;
	}
	~AlignedBox() {}
	
	Vector4d min() const { return m_box[0]; }
	Vector4d max() const { return m_box[1]; }
	inline Vector4d& min() { return m_box[0]; }
	inline Vector4d& max() { return m_box[1]; }
	inline Vector4d mid() { return m_midpoint; }

	inline bool isEmpty() const { return (max().array() < min().array()).any(); }
	inline bool isInfinite() const {
		return (max().cwiseEqual(INF4DPOINT).all()
			&& min().cwiseEqual(nINF4DPOINT).all());
	}

	inline void setEmpty() {
		min() = INF4DPOINT;
		max() = nINF4DPOINT;
	}

	inline void setInfinity() {
		min() = nINF4DPOINT;
		max() = INF4DPOINT;
	}

	inline Vector4d size() const { return max() - min(); }
	inline double volume() const { return size().prod(); }
	AlignedBox& extend(AlignedBox& other) {
		min() = min().cwiseMin(other.min());
		max() = max().cwiseMax(other.max());
		m_midpoint = (min() + max()) / 2.0;
		return *this;
	}

	AlignedBox& extend(const Vector4d& point) {
		min() = min().cwiseMin(point);
		max() = max().cwiseMax(point);
		m_midpoint = (min() + max()) / 2.0;
		return *this;
	}

	inline AlignedBox merged(const AlignedBox& other) const {
		return AlignedBox(min().cwiseMin(other.min()), max().cwiseMax(other.max()));
	}

	inline AlignedBox intersection(const AlignedBox& other) const {
		return AlignedBox(min().cwiseMax(other.min()), max().cwiseMin(other.max()));
	}

	inline bool intersects(const AlignedBox& other) const {
		return intersection(other).isEmpty();
	}

	bool hit(RayBase& ray, double t0, double t1, HitRecord& record) const;

	void transform(const Transform3d& t);

	inline bool operator==(const AlignedBox& rhs) {
		return ((min().cwiseEqual(rhs.min()).all() && max().cwiseEqual(rhs.max()).all()));
	}
	inline bool operator!=(const AlignedBox& rhs) {
		return ((min().cwiseNotEqual(rhs.min()).any() || max().cwiseNotEqual(rhs.max()).any()));
	}

	static inline AlignedBox Infinite() { return AlignedBox(nINF4DPOINT, INF4DPOINT); }
	static inline AlignedBox Empty() { return AlignedBox(INF4DPOINT, nINF4DPOINT); }

private:
	Vector4d m_box[2];
	Vector4d m_midpoint;
};

