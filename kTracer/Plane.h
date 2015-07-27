#pragma once
#include "Surface.h"
class Plane :
	public Surface
{
public:
	Plane(const Vector4d& position, const Vector4d& normal, Material* material)
	: m_position(position)
	, m_normal(normal.normalized())
	, m_NP(-m_normal.dot(m_position))
	{
		m_material = material;
		m_light = nullptr;
		m_normal.normalize();
		m_type = Surface::PLANE;
		m_boundingBox = AlignedBox::Infinite();
		m_ID = counter++;
	}
	~Plane() {}

	bool hit(RayBase& ray, double t0, double t1, HitRecord& record) const;
	virtual Vector4d getNormal(const Vector4d& point) const { return m_normal; }

private:
	Vector4d m_position, m_normal;
	double m_NP;
};

