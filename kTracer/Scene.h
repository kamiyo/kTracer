#pragma once

#include "Objects.h"
#include "Parser.h"
#include <OpenEXR\ImfRgba.h>
#include <unordered_map>

class Scene
{
public:
	Scene(const std::string filename) {
		Parser p(filename);
		p.loadOptions(m_options);
		p.loadCamera(m_camera);
		p.loadMaterials(m_materials);
		p.loadLights(m_lights);
		p.loadScene(m_objects, m_materials);
	}
	~Scene() {
		delete m_options;
		delete m_camera;
		m_options = nullptr;
		m_camera = nullptr;
	}

	void render(MatrixRGBa& output);
	void trace(const Ray& ray, double t0, double t1, RGB&) const;

private:
	Options* m_options;
	Camera* m_camera;
	Group* m_objects;
	std::vector<Light*> m_lights;
	std::unordered_map<std::string, Material*> m_materials;
};

