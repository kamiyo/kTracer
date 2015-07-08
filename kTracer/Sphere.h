#pragma once
#include "Surface.h"
class Sphere :
	public Surface
{
public:
	Sphere(Vector3d position, double radius, Material* material)
		: m_position(position)
		, m_radius(radius)
		, m_radius_2(radius * radius)
	{
		m_material = material;
		m_type = Surface::SPHERE;
		m_boundingBox = AlignedBox(position - Vector3d::Constant(radius), position + Vector3d::Constant(radius));
	}
	~Sphere() {}

	bool hit(RayBase& ray, double t0, double t1, HitRecord& record) const;
private:
	Vector3d m_position;
	double m_radius, m_radius_2;
};

