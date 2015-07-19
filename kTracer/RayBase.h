#pragma once

#include "typedefs.h"

class RayBase
{
public:
	~RayBase() {}
	RayBase(const Vector4d& eye, const Vector4d& dir, int type)
	: m_eye(eye)
	, m_dir(dir)
	, m_type(type)
	{ init(); }

	void init() {
		m_inv = m_dir.cwiseInverse();
		m_epsilon = EPSILON / m_dir.norm();
		m_sign = (m_inv.array() < 0).cast<int>();
	}

	Vector4d operator()(double t) const {	return m_eye + t * m_dir; }
	Vector4d e() const { return m_eye; }
	Vector4d d() const { return m_dir; }
	Vector4d i() const { return m_inv; }
	Array4i s() const { return m_sign; }
	
	RayBase& e(const Vector4d& eye) { m_eye = eye; return *this; }
	RayBase& d(const Vector4d& dir) { m_dir = dir;	init();	return *this; }

	double epsilon() const { return m_epsilon; }
	int type() const { return m_type; }
	static const int VIEW = 0, SHADOW = 1, PHOTON = 2;

protected:
	Vector4d m_eye, m_dir, m_inv;
	Array4i m_sign;
	double m_epsilon;
	int m_type;
};

