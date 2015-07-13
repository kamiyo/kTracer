#pragma once
#include "Surface.h"
class Triangle :
	public Surface
{
public:
	Triangle(Vector3d p1, Vector3d p2, Vector3d p3, Material* m);
	
	~Triangle() {}

	bool hit(RayBase& ray, double t0, double t1, HitRecord& record) const;
	
private:
	static const double M_EPSILON;
	Vector3d m_p1, m_p2, m_p3;
	Vector3d m_e1, m_e2;
	Vector3d m_normal;
	Matrix3d m_matrix;
	double m_denominator;
	int m_axis_1, m_axis_2;
};

