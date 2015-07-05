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
	Vector3d m_p1, m_p2, m_p3, m_normal;
	double m_NP;
	double m_u1, m_u2, m_v1, m_v2;
	double m_denominator;
	int m_axis_1, m_axis_2;
};

