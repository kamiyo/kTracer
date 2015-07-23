#include "Surface.h"

int Surface::counter = 0;

double Surface::pdf(RayBase& ray) const {
	HitRecord record;
	if (!hit(ray, ray.epsilon(), 1, record)) return 0;
	double result = (ray.e() - ray(record.t)).squaredNorm() 
			  / (record.normal.dot(-ray.d()) * area());
	return result;
}
