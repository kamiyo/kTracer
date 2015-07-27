#pragma once
#include "Surface.h"
class Group :
	public Surface
{
public:
	Group() {}
	~Group() {}

	virtual bool hit(RayBase& ray, double t0, double t1, HitRecord& record) const;
	Group* push_back(Surface* i) {
		m_objects.push_back(i);
		m_boundingBox.extend(i->m_boundingBox);
		return (Group*)this;
	}

	Surface* back() {
		return m_objects.back();
	}

	virtual Vector4d getNormal(const Vector4d& point) const { return nINF4DDIR; }

	std::vector<Surface *> m_objects;
};

