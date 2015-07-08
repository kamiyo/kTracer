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

	std::vector<Surface *> m_objects;
};

