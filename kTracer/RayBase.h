#pragma once

#include "typedefs.h"

class RayBase
{
public:
	~RayBase() {}
	RayBase(Vector3d eye, Vector3d dir, int type)
	: m_eye(eye)
	, m_dir(dir)
	, m_type(type)
	{ init(); }

	void init() {
		m_inv = m_dir.cwiseInverse();
		m_epsilon = EPSILON / m_dir.norm();
		m_sign = (m_inv.array() < 0).cast<int>();
	}

	Vector3d getPoint(double t) const {	return m_eye + t * m_dir; }
	Vector3d e() const { return m_eye; }
	Vector3d d() const { return m_dir; }
	
	RayBase& e(Vector3d eye) { m_eye = eye; return *this; }
	RayBase& d(Vector3d dir) { m_dir = dir;	init();	return *this; }

	int type() const { return m_type; }

protected:
	Vector3d m_eye, m_dir, m_inv;
	Vector3i m_sign;
	double m_epsilon;
	int m_type;
	static const int VIEW = 0, SHADOW = 1, PHOTON = 2;
};

