#pragma once

#include "Light.h"
#include "Surface.h"
#include "Parser.h"

class Scene
{
public:
	Scene(const std::string filename) {
		Parser p(filename);
		p.loadOptions(m_options);
		p.loadCamera(m_camera);
	}
	~Scene() {}

private:
	Options* m_options;
	Camera* m_camera;
	std::vector<Surface*> m_surfaces;
	std::vector<Light*> m_lights;
	std::vector<Material*> m_materials;
};

