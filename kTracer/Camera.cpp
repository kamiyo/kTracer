#include "Camera.h"

Ray Camera::generateRay(const double x, const double y) const {
	Vector2d uv(x, y);
	uv = m_dim.cwiseProduct(uv.cwiseQuotient(m_pixel_dim) - Vector2d(0.5, 0.5));
	Vector3d point; point << uv, -m_focal_distance;

	// deal with lens blur

	Vector3d dir = point.transpose() * m_uvw;

	return Ray(m_eye, dir);
}