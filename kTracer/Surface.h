#pragma once

#include "typedefs.h"

class Surface
{
public:
	Surface();
	~Surface();
	virtual bool preHit(RayBase& ray, double t0, double t1, hitRecord& record) const;
	virtual bool hit(RayBase& ray, double t0, double t1, hitRecord& record) const = 0;
	/*void setTransform(Matrix4d& m) {
		m_isTransformed = true;
		m_inverse = m.inverse();
		m_transpose = m_inverse.transpose();
		m_boundingBox.transform(m);
	}*/

	enum { PLANE, SPHERE, CYLINDER, CONE, TORUS, TRIANGLE, FACE, CIRCLE, BOX, BOVOH, INSTANCE };
protected:
	//std::shared_ptr<Material> m_material;
	//Matrix4d m_inverse;
	//Matrix4d m_transpose;
	//BoundingBox m_boundingBox;
	//bool m_isTransformed;
	int m_type;
};

