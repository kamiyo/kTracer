#pragma once
#include "Surface.h"
class Circle :
	public Surface
{
public:
	Circle(const Vector4d& position, const Vector4d& normal, double radius, Material* material) 
	: m_position(position)
	, m_normal(normal)
	, m_radius(radius)
	, m_radius_2(radius * radius)
	, m_NP(-m_normal.dot(m_position))
	{
		m_material = material;
		m_type = Surface::CIRCLE;
		Vector4d r(radius, radius, radius, 0);
		m_boundingBox = AlignedBox(position - r, position + r); // lazy bounding
		m_ID = counter++;
	}
	~Circle() {}

	double area() const { return M_PI * m_radius_2; }

	bool hit(RayBase& ray, double t0, double t1, HitRecord& record) const;

private:
	Vector4d m_position, m_normal;
	double m_radius, m_radius_2;
	double m_NP;
};

