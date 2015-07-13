#include "Scene.h"
#include <OpenEXR\half.h>
#include <ctime>

void Scene::render(MatrixRgba& output) {
	int height = m_camera->m_pixel_dim.y(), width = m_camera->m_pixel_dim.x();
	int progress = 0;
	double total = (double) (width * height);

	output.resize(height, width);
	clock_t start = clock();
	Vector2i pixels = m_camera->m_pixel_dim;

	for (int h = 0; h < height; ++h) {
		clock_t inner_start = clock();
		int startray = Ray::count;

		for (int w = 0; w < width; ++w) {
			Rgb result;

			Ray view = m_camera->generateRay((double) w + 0.5, (double) h + 0.5);
			trace(view, 0.0, INF, result);
			
			int revHeight = height - h - 1;
			output(revHeight, w) = Imf::Rgba((float)result.x(), (float)result.y(), (float)result.z());
			
			progress += 1;

		}
		clock_t inner_stop = clock();
		double runtime = (double) (inner_stop - inner_start) / CLOCKS_PER_SEC;
		double percentage = (double) progress / total;
		int raysince = Ray::count - startray;
		percentage *= 100;

		std::cout << "\rprogress: " << floor(percentage) << "% : " << progress << " " << Ray::count << " rays - " << ((double) raysince / runtime) << " r/s";
		std::cout.flush();

	}
	clock_t stop = clock();
	double master_run = (double) (stop - start) / CLOCKS_PER_SEC;

	std::cout << "\r\rprogress: 100%, " << Ray::count << " rays at " << ((double) Ray::count / master_run) << " r/s\t\t\t" << std::endl;
	std::cout.flush();
	std::cout << "render time: " << master_run << std::endl;
}

void Scene::trace(Ray& ray, double t0, double t1, Rgb& out) const {
	HitRecord record;
	out = Vector3d::Zero();
	if (m_objects->hit(ray, t0, t1, record)) {
		const Vector3d xpoint = ray.getPoint(record.t);
		std::string name = record.material->m_name;
		for (Light* l : m_lights) {
			const Vector3d light = l->getVector(xpoint);
			Ray shadowRay(xpoint, light, RayBase::SHADOW);
			HitRecord shadowRecord;
			if (!m_objects->hit(shadowRay, shadowRay.epsilon(), 1.0, shadowRecord)) {
				out += record.material->brdf(-ray.d().normalized(), light.normalized(), record.normal, l->i());
			}
		}
	}
}
