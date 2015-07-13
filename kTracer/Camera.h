#pragma once
#include "typedefs.h"
#include "Ray.h"

class Camera
{
public:
	Camera(Vector3d eye, Vector3d dir, Vector3d up,
		double width, double height, int pixelw, int pixelh, double focal, double fstop)
		: m_eye(eye)
		, m_dir(dir.normalized())
		, m_dim(Vector2d(width, height))
		, m_pixel_dim(Vector2i(pixelw, pixelh))
	{
		m_focal_distance = (focal == nINF) ? dir.norm() : focal;
		m_w = -m_dir;
		m_u = (up.cross(m_w)).normalized();
		m_v = (m_w.cross(m_u)).normalized();
		m_uvw << m_u, m_v, m_w;
		m_uvw.transposeInPlace();
		m_lens_radius = (fstop == nINF) ? 0.0 : m_focal_distance / fstop;
	}
	~Camera() {}

	Ray generateRay(const double x, const double y) const;
	
	Vector2i m_pixel_dim;

private:
	Vector3d m_eye, m_dir;
	Vector3d m_u, m_v, m_w;
	Matrix3d m_uvw;
	//Vector3d m_focal_plane_pos, m_focal_plane_dir;
	Vector2d m_dim;
	double m_focal_distance;
	double m_lens_radius;
};

