#include "Camera.h"

Ray Camera::generateRay(Vector2d imageUV, Vector2d lensUV) const {
	imageUV = m_dim.cwiseProduct(imageUV.cwiseQuotient(m_pixel_dim.cast<double>()) - Vector2d(0.5, 0.5));
	Vector4d point; point << imageUV, -m_focal_distance, 0;

	// deal with lens blur
	//Vector3d tryvec = -m_focal_distance * m_w + uv.x() * m_u + uv.y() * m_v;
	Vector4d dir = m_uvw * point;
	//std::cout << tryvec.transpose() << " || " << dir.transpose() << std::endl;
	Vector4d at = m_eye + dir;
	Vector4d luv; luv << lensUV, 0, 0;
	Vector4d eye = m_eye + m_lens_radius * (m_uvw * luv);
	dir = at - eye;
	return Ray(eye, dir, Ray::VIEW);
}