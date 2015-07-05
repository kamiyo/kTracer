#pragma once
#include "Intersectable.h"
class Group :
	public Intersectable
{
public:
	Group() {}
	~Group() {}

	virtual bool hit(RayBase& ray, double t0, double t1, HitRecord& record) const;
	void push_back(Intersectable* i);

	std::vector<Intersectable *> m_objects;
};

