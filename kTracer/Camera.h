#pragma once
#include "typedefs.h"

class Camera
{
public:
	Camera(Vector3d eye, Vector3d dir, Vector3d up,
		double width, double height, int pixelw, int pixelh, double focal, double fstop)
		: m_eye(eye)
		, m_dir(dir.normalized())
		, m_width(width), m_height(height)
		, m_pixel_width(pixelw), m_pixel_height(pixelh)
	{
		m_w = -m_dir;
		m_u = (up.cross(m_w)).normalized();
		m_v = (m_w.cross(m_u)).normalized();
		m_focal_distance = (focal == nINF) ? m_dir.norm() : focal;
		m_lens_radius = (fstop == nINF) ? 0.0 : m_focal_distance / fstop;
	}
	~Camera() {}

private:
	Vector3d m_eye, m_dir;
	Vector3d m_u, m_v, m_w;
	//Vector3d m_focal_plane_pos, m_focal_plane_dir;
	double m_width, m_height;
	int m_pixel_width, m_pixel_height;
	double m_focal_distance;
	double m_lens_radius;
};

