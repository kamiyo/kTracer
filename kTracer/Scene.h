#pragma once

#include "Light.h"
#include "Surface.h"
#include "Parser.h"
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
	}
	~Scene() {
		delete m_options;
		delete m_camera;
		m_options = nullptr;
		m_camera = nullptr;
	}

private:
	Options* m_options;
	Camera* m_camera;
	std::vector<Surface*> m_surfaces;
	std::vector<Light*> m_lights;
	std::unordered_map<std::string, Material*> m_materials;
};

