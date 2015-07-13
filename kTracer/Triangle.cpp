#include "Triangle.h"

const double Triangle::M_EPSILON = 1e-6;

Triangle::Triangle(Vector3d p1, Vector3d p2, Vector3d p3, Material* m)
	: m_p1(p1)
	, m_p2(p2)
	, m_p3(p3)
	, m_e1(p2 - p1)
	, m_e2(p3 - p1)
	, m_normal((p2 - p1).cross(p3 - p1).normalized())
	//, m_NP(-m_normal.dot(p1))
{
	m_material = m;
	m_type = Surface::TRIANGLE;

	m_matrix << m_e1, m_e2, m_normal;
	m_matrix = m_matrix.transpose().inverse();
	
	m_boundingBox = AlignedBox(p1);
	m_boundingBox.extend(p2).extend(p3);
}

bool Triangle::hit(RayBase& ray, double t0, double t1, HitRecord& record) const {
	double denominator = ray.d().dot(m_matrix.col(2));
	if (denominator == 0) { return false; }
	double numerator = (m_p1 - ray.e()).dot(m_matrix.col(2));
	double t = numerator / denominator;
	if (t < t0 || t > t1) { return false; }

	Vector3d point = ray.getPoint(t) - m_p1;
	double u = point.dot(m_matrix.col(0));
	if (u < 0.0) return false;
	double v = point.dot(m_matrix.col(1));
	if (v < 0.0 || u + v > 1.0) return false;

	record.t = t;
	if (ray.type() == RayBase::SHADOW) { return true; }
	record.normal = m_normal;
	record.material = m_material;
	return true;
}

//bool Triangle::hit(RayBase& ray, double t0, double t1, HitRecord& record) const {
//	Vector3d P = ray.d().cross(m_e2);
//	double denominator = m_e1.dot(P);
//	//if (abs(det) < M_EPSILON) { return false; }
//	if (denominator == 0) { return false; }
//
//	denominator = 1.0 / denominator;
//	Vector3d T = ray.e() - m_p1;
//	double u = T.dot(P) * denominator;
//	if (u < 0.0 || u > 1.0) { return false; }
//
//	Vector3d Q = T.cross(m_e1);
//	double v = Q.dot(ray.d()) * denominator;
//	if (v < 0.0 || u + v > 1.0) { return false; }
//
//	record.t = m_e2.dot(Q) * denominator;
//	if (ray.type() == RayBase::SHADOW) { return true; }
//	record.normal = m_normal;
//	record.material = m_material;
//	return true;
//}