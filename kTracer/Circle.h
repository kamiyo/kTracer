#pragma once
#include "Surface.h"
#include "utils.h"

class Circle :
	public Surface
{
public:
	Circle(const Vector4d& position, const Vector4d& normal, double radius, Material* material, AreaLight* alight) 
	: m_position(position)
	, m_normal(normal.normalized())
	, m_radius(radius)
	, m_radius_2(radius * radius)
	, m_NP(-m_normal.dot(m_position))
	{
		m_material = material;
		m_light = alight;
		m_type = Surface::CIRCLE;
		Vector4d r(radius, radius, radius, 0);
		m_boundingBox = AlignedBox(position - r, position + r); // lazy bounding
		m_ID = counter++;
		if (m_light) {
			createUV();
		}
	}
	~Circle() {}

	void createUV() {
		Vector4d up(0, 1, 0, 0);
		if (abs(m_normal.dot(up)) == 1.0) { up << 1, 0, 0, 0; }
		m_u = up.cross3(m_normal).normalized() * m_radius;
		m_v = m_normal.cross3(m_u).normalized() * m_radius;
	}

	double area() const { return M_PI * m_radius_2; }
	virtual Vector4d getNormal(const Vector4d& point) const { return m_normal; }

	bool hit(RayBase& ray, double t0, double t1, HitRecord& record) const;

	Vector4d sample(const Vector2d& uv) const {
		Vector2d circ = to_unit_disk(uv);
		Vector4d out = m_position + m_u * circ.x() + m_v * circ.y();
		return out;
	}


private:
	Vector4d m_position, m_normal;
	Vector4d m_u, m_v;
	double m_radius, m_radius_2;
	double m_NP;
};

