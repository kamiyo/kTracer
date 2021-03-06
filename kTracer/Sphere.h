#pragma once
#include "Surface.h"
class Sphere :
	public Surface
{
public:
	Sphere(const Vector4d& position, double radius, Material* material, AreaLight* alight)
		: m_position(position)
		, m_radius(radius)
		, m_radius_2(radius * radius)
	{
		m_material = material;
		m_light = alight;
		m_type = Surface::SPHERE;
		Vector4d r(radius, radius, radius, 0);
		m_boundingBox = AlignedBox(position - r, position + r);
		m_ID = counter++;
	}
	~Sphere() {}

	double area() const { return 4.0 * M_PI * m_radius_2; }
	virtual Vector4d getNormal(const Vector4d& point) const {
		return (point - m_position).normalized();
	}

	bool hit(RayBase& ray, double t0, double t1, HitRecord& record) const;
private:
	Vector4d m_position;
	double m_radius, m_radius_2;
};

