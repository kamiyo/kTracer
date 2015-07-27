#pragma once

#include "Objects.h"
#include "Parser.h"
#include "Sampler.h"
#include <OpenEXR\ImfRgba.h>
#include <unordered_map>

class Scene
{
public:
	Scene(const std::string filename) {
		Parser p(filename);
		m_options = p.loadOptions();
		m_camera = p.loadCamera();
		p.loadMaterials(m_materials);
		p.loadLights(m_lights);
		m_objects = p.loadScene(m_materials, m_lights);
		m_width = m_camera->m_pixel_dim.x();
		m_height = m_camera->m_pixel_dim.y();
		m_rng = new Random();
	}
	~Scene() {
		delete m_options;
		delete m_camera;
		delete m_objects;
		m_options = nullptr;
		m_camera = nullptr;
		m_objects = nullptr;
	}

	void render(MatrixRgba& output);
	void trace(Ray& ray, double t0, double t1, const Vector2d& uv, Rgba& out) const;

	int m_width, m_height;
private:
	Random* m_rng;
	Options* m_options;
	Camera* m_camera;
	Group* m_objects;
	std::vector<Light*> m_lights;
	std::unordered_map<std::string, Material*> m_materials;
};

