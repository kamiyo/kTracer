#include "Camera.h"

Ray Camera::generateRay(const Vector2d& imageUV, const Vector2d& lensUV) const {
	Vector2d iuv = m_dim.cwiseProduct(imageUV.cwiseQuotient(m_pixel_dim.cast<double>()) - Vector2d(0.5, 0.5));
	Vector4d point; point << iuv, -m_focal_distance, 0;

	Vector4d dir = m_uvw * point;
	Vector4d at = m_eye + dir;
	Vector4d luv; luv << lensUV, 0, 0;
	Vector4d eye = m_eye + m_lens_radius * (m_uvw * luv);
	dir = at - eye;
	return Ray(eye, dir, Ray::VIEW);
}