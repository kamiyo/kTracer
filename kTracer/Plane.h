#pragma once
#include "Surface.h"
class Plane :
	public Surface
{
public:
	Plane(Vector3d position, Vector3d normal, Material* material)
	: m_position(position)
	, m_normal(normal.normalized())
	, m_NP(-m_normal.dot(m_position))
	{
		m_material = material;
		m_normal.normalize();
		m_type = Surface::PLANE;
		m_boundingBox = AlignedBox::Infinite();
	}
	~Plane() {}

	bool hit(RayBase& ray, double t0, double t1, HitRecord& record) const;

private:
	Vector3d m_position, m_normal;
	double m_NP;
};

