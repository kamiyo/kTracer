#pragma once

#include "typedefs.h"
#include "Intersectable.h"
#include "AlignedBox.h"
#include "HitRecord.h"
#include "RayBase.h"

class AreaLight;

class Surface : public Intersectable
{
public:
	//Surface() {}
	//~Surface() {}
	//virtual bool preHit(RayBase& ray, double t0, double t1, HitRecord& record) const;
	/*void setTransform(Matrix4d& m) {
	m_isTransformed = true;
	m_inverse = m.inverse();
	m_transpose = m_inverse.transpose();
	m_boundingBox.transform(m);
	}*/

	virtual bool hit(RayBase& ray, double t0, double t1, HitRecord& record) const = 0;
	virtual double area() const { return 0; }
	virtual Vector4d sample(const Vector2d& uv) const { return nINF4DPOINT; }
	virtual Vector4d getNormal(const Vector4d& point) const = 0;
	virtual double pdf() const { return 1.0 / area(); }
	virtual double pdf(RayBase& ray) const;

	enum { PLANE, SPHERE, CYLINDER, CONE, TORUS, TRIANGLE, FACE, CIRCLE, BOX, BOVOH, INSTANCE };
	Material* m_material;
	AreaLight* m_light;

	//Matrix4d m_inverse;
	//Matrix4d m_transpose;

	AlignedBox m_boundingBox;

	//bool m_isTransformed;
	int m_type;
	int m_ID;
	static int counter;
};