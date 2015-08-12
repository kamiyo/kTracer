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
	Sampler* s = m_options->getAASampler(m_rng);
	int number_samples = m_options->m_samples;
	Samplerd iSamples, lSamples, uvSamples;
	iSamples.resize(2, number_samples * number_samples);
	lSamples.resize(2, number_samples * number_samples);
	uvSamples.resize(2, number_samples * number_samples);
	for (int h = 0; h < height; ++h) {
		clock_t inner_start = clock();
		int startray = Ray::count;

#pragma omp parallel for schedule(dynamic) firstprivate(s, iSamples, lSamples)
		for (int w = 0; w < width; ++w) {
			
			s->genPoints2d(iSamples, number_samples, 0, 1);
			s->genPoints2d(lSamples, number_samples, 2, 3);
			s->genPoints2d(uvSamples, number_samples, 4, 5);
			s->shuffle(uvSamples, s->m_rng);
			s->shuffle(lSamples, s->m_rng);
			Eigen::Matrix<Rgba, Eigen::Dynamic, 1> results(number_samples * number_samples);
			Vector2d px(w, h);
			for (int m = 0; m < number_samples * number_samples; m++) {
				const Vector2d iSample = iSamples.col(m);
				const Vector2d lSample = lSamples.col(m);
				const Vector2d uvSample = uvSamples.col(m);
				Ray view = m_camera->generateRay(px + iSample, lSample - Vector2d::Constant(0.5));
				trace(view, 0.0, INF, uvSample, results(m));
			}
			// if (needsSuperSample)
			Rgba result = Rgba::Zero();
			for (int i = 0; i < (int) results.size(); i++) {
				result += results(i);
			}
			result /= (double) results.size();
			int revHeight = height - h - 1;
			output(revHeight, w) = Imf::Rgba((float)result.x(), (float)result.y(), (float)result.z());
		}
#pragma omp atomic
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

void Scene::trace(Ray& ray, double t0, double t1, const Vector2d& uv, Rgba& out) const {
	HitRecord record;
	out = Rgba::Zero();
	if (m_objects->hit(ray, t0, t1, record)) {
		const Vector4d xpoint = ray(record.t);
		const Material* m = record.surface->m_material;
		const AreaLight* al = record.surface->m_light;
		//std::string name = m->m_name;
		if (al) {
			out += al->L(xpoint, -ray.d());
		}
		else {
			for (Light* l : m_lights) {
				const Vector4d light = l->getVector(xpoint, uv);
				Ray shadowRay(xpoint, light, RayBase::SHADOW);
				HitRecord shadowRecord;
				if (!m_objects->hit(shadowRay, shadowRay.epsilon(), 1.0, shadowRecord)
					|| ((l->type() == Light::AREA) && ((AreaLight*) l)->m_surface == shadowRecord.surface)
					) {
					const double falloff = l->getFalloff(light);
					if (m) {
						out += m->brdf(-ray.d().normalized(), light.normalized()
							, record.normal
							, falloff * l->L(xpoint + light, -light));
					}
				}
			}
		}
	}
}
