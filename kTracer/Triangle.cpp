#include "Triangle.h"

const double Triangle::M_EPSILON = 1e-6;

Triangle::Triangle(const Vector4d& p1, const Vector4d& p2, const Vector4d& p3, Material* m, AreaLight* l)
	: m_p1(p1)
	, m_p2(p2)
	, m_p3(p3)
	, m_e1(p2 - p1)
	, m_e2(p3 - p1)
	, m_normal((p2 - p1).cross3(p3 - p1).normalized())
{
	m_material = m;
	m_light = l;
	m_type = Surface::TRIANGLE;
	m_matrix.resize(4, 3);
	m_matrix << m_e1, m_e2, m_normal;
	m_matrix.block<3, 3>(0, 0) = m_matrix.block<3, 3>(0, 0).transpose().inverse();

	m_boundingBox = AlignedBox(p1);
	m_boundingBox.extend(p2).extend(p3);
	m_ID = counter++;
}

bool Triangle::hit(RayBase& ray, double t0, double t1, HitRecord& record) const {
	double denominator = ray.d().dot(m_matrix.col(2));
	if (denominator == 0) { return false; }
	double numerator = (m_p1 - ray.e()).dot(m_matrix.col(2));
	double t = numerator / denominator;
	if (t < t0 || t > t1) { return false; }

	Vector4d point = ray(t) - m_p1;
	double u = point.dot(m_matrix.col(0));
	if (u < 0.0) return false;
	double v = point.dot(m_matrix.col(1));
	if (v < 0.0 || u + v > 1.0) return false;

	record.t = t;
	if (ray.type() == RayBase::SHADOW) { return true; }
	record.normal = m_normal;
	record.surface = this;
	return true;
}
