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

#pragma omp parallel for schedule(dynamic)
		for (int w = 0; w < width; ++w) {
			Sampler* s = m_options->getAASampler(m_rng);
			s->genPoints();
			int number_samples = m_options->m_samples;
			const Sampler2d iSamples = s->getImageSamples();
			const Sampler2d lSamples = s->getLensSamples();
			Eigen::Matrix<Rgba, Eigen::Dynamic, 1> results(number_samples * number_samples);
			Vector2d px(w, h);
			for (int m = 0; m < number_samples * number_samples; m++) {
				const Vector2d iSample = iSamples(m);
				const Vector2d lSample = lSamples(m);
				Ray view = m_camera->generateRay(px + iSample, lSample - Vector2d::Constant(0.5));
				trace(view, 0.0, INF, results(m));
			}

			Rgba result = Rgba::Zero();
			for (int i = 0; i < (int) results.size(); i++) {
				result += results(i);
			}
			result /= (double) results.size();
			int revHeight = height - h - 1;
			output(revHeight, w) = Imf::Rgba((float)result.x(), (float)result.y(), (float)result.z());
		}
		progress += height;
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

void Scene::trace(Ray& ray, double t0, double t1, Rgba& out) const {
	HitRecord record;
	out = Rgba::Zero();
	if (m_objects->hit(ray, t0, t1, record)) {
		const Vector4d xpoint = ray(record.t);
		std::string name = record.material->m_name;
		for (Light* l : m_lights) {
			const Vector4d light = l->getVector(xpoint);
			Ray shadowRay(xpoint, light, RayBase::SHADOW);
			HitRecord shadowRecord;
			if (!m_objects->hit(shadowRay, shadowRay.epsilon(), 1.0, shadowRecord)) {
				out += record.material->brdf(-ray.d().normalized(), light.normalized(), record.normal, l->i());
			}
		}
	}
}
