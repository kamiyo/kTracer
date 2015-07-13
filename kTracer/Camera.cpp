#include "Camera.h"

Ray Camera::generateRay(const double x, const double y) const {
	Vector2d uv(x, y);
	uv = m_dim.cwiseProduct(uv.cwiseQuotient(m_pixel_dim.cast<double>()) - Vector2d(0.5, 0.5));
	Vector4d point; point << uv, -m_focal_distance, 0;

	// deal with lens blur
	//Vector3d tryvec = -m_focal_distance * m_w + uv.x() * m_u + uv.y() * m_v;
	Vector4d dir = m_uvw * point;
	//std::cout << tryvec.transpose() << " || " << dir.transpose() << std::endl;

	return Ray(m_eye, dir, Ray::VIEW);
}