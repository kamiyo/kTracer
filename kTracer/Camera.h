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
		Vector3d w = -m_dir.normalized();
		Vector3d u = (up.cross(w)).normalized();
		Vector3d v = (w.cross(u)).normalized();
		m_uvw << u, v, w;
		m_focal_distance = (focal == nINF) ? m_dir.norm() : focal;
		m_lens_radius = (fstop == nINF) ? 0.0 : m_focal_distance / fstop;
	}
	~Camera() {}

	Ray generateRay(const double x, const double y) const;

private:
	Vector3d m_eye, m_dir;
	//Vector3d m_u, m_v, m_w;
	Matrix3d m_uvw;
	//Vector3d m_focal_plane_pos, m_focal_plane_dir;
	Vector2d m_dim;
	Vector2i m_pixel_dim;
	double m_focal_distance;
	double m_lens_radius;
};

