#include "Scene.h"
#include <ctime>

void Scene::render(MatrixRGBa& output) {
	output.resize(m_camera->m_pixel_dim.y(), m_camera->m_pixel_dim.x());
	clock_t start = clock();
	Vector2i pixels = m_camera->m_pixel_dim;
	for (int h = 0; h < pixels.y(); ++h) {
		clock_t inner_start = clock();
		int startray = Ray::count;

		for (int w = 0; w < pixels.x(); ++w) {
			RGB result(Vector3d::Zero());

			Ray view = m_camera->generateRay((double) w + 0.5, (double) h + 0.5);
		}
	}
}

void Scene::trace(const Ray& ray, double t0, double t1, RGB&) const {





}
