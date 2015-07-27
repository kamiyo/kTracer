#pragma once
#include "Surface.h"
class Triangle :
	public Surface
{
public:
	Triangle(const Vector4d& p1, const Vector4d& p2, const Vector4d& p3, Material* m, AreaLight* l);
	
	~Triangle() {}
	virtual Vector4d getNormal(const Vector4d& point) const { return m_normal; }

	bool hit(RayBase& ray, double t0, double t1, HitRecord& record) const;
	
private:
	static const double M_EPSILON;
	Vector4d m_p1, m_p2, m_p3;
	Vector4d m_e1, m_e2;
	Vector4d m_normal;
	MatrixXd m_matrix;
	double m_denominator;
	int m_axis_1, m_axis_2;
};

