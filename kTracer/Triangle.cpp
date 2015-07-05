#include "Triangle.h"

Triangle::Triangle(Vector3d p1, Vector3d p2, Vector3d p3, Material* m)
	: m_p1(p1)
	, m_p2(p2)
	, m_p3(p3)
	, m_normal((p2 - p1).cross(p3 - p1).normalized())
	, m_NP(m_normal.dot(-p1))
{
	m_material = m;
	m_type = Surface::TRIANGLE;

	Vector3d::Index axis;
	m_normal.cwiseAbs().maxCoeff(&axis);
	m_axis_1 = (axis == 0) ? 1 : 0;
	m_axis_2 = (axis == 2) ? 1 : 2;
	m_u1 = m_p2[m_axis_1] - m_p1[m_axis_1]; m_u2 = m_p3[m_axis_1] - m_p1[m_axis_1];
	m_v1 = m_p2[m_axis_2] - m_p1[m_axis_2]; m_v2 = m_p3[m_axis_2] - m_p1[m_axis_2];
	m_denominator = m_u1 * m_v2 - m_u2 - m_v1;
}

bool Triangle::hit(RayBase& ray, double t0, double t1, HitRecord& record) const {
	double denominator = m_normal.dot(ray.d());
	if (denominator == 0) { return false; }

	double numerator = -(m_NP + m_normal.dot(ray.e()));
	double t = numerator / denominator;
	if (t < t0 || t > t1) { return false; }

	Vector3d point = ray.getPoint(t);

	double u0 = point[m_axis_1] - m_p1[m_axis_1];
	double v0 = point[m_axis_2] - m_p1[m_axis_2];
	double alpha, beta;
	if (m_u1 == 0) {
		beta = u0 / m_u2;
		if (beta < 0 || beta > 1) { return false; }
		alpha = (v0 - beta * m_v2) / m_v1;
		if (alpha < 0) { return false; }
	}
	else {
		beta = (m_u1 * v0 - u0 * m_v1) / m_denominator;
		if (beta < 0 || beta > 1) { return false; }
		alpha = (u0 - beta * m_u2) / m_u1;
		if (alpha < 0) { return false; }
	}
	if (alpha + beta > 1) { return false; }

	record.t = t;
	if (ray.type() == RayBase::SHADOW) { return true; }
	record.normal = m_normal;
	record.material = m_material;
	return true;
}
