#include "Group.h"

bool Group::hit(RayBase& ray, double t0, double t1, HitRecord& record) const {
	std::cout << m_boundingBox.min().transpose() << " " << m_boundingBox.max().transpose() << std::endl;
	bool hit = false;
	if (!m_boundingBox.hit(ray, t0, t1, record)) { return false; }
	std::cout << "bounding hit!" << std::endl;
	for (auto object : m_objects) {
		HitRecord temp;
		if (object->hit(ray, t0, t1, temp))  {
			hit = true;
			record = temp;
			t1 = record.t;
		}
	}
	return hit;
}
