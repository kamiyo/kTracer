#pragma once

#include "typedefs.h"
#include "Intersectable.h"
#include "HitRecord.h"
#include "RayBase.h"

class Surface : public Intersectable
{
public:
	//Surface() {}
	//~Surface() {}
	//virtual bool preHit(RayBase& ray, double t0, double t1, HitRecord& record) const;
	virtual bool hit(RayBase& ray, double t0, double t1, HitRecord& record) const = 0;
	/*void setTransform(Matrix4d& m) {
		m_isTransformed = true;
		m_inverse = m.inverse();
		m_transpose = m_inverse.transpose();
		m_boundingBox.transform(m);
	}*/

	enum { PLANE, SPHERE, CYLINDER, CONE, TORUS, TRIANGLE, FACE, CIRCLE, BOX, BOVOH, INSTANCE };
protected:
	Material* m_material;
	//Matrix4d m_inverse;
	//Matrix4d m_transpose;
	//BoundingBox m_boundingBox;
	//bool m_isTransformed;
	int m_type;
};

