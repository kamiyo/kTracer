#pragma once

#include "Light.h"
#include "Surface.h"
#include "Parser.h"

class Scene
{
public:
	Scene();
	~Scene();

private:
	//Options* m_options;
	std::vector<Surface*> m_surfaces;
	std::vector<Light*> m_lights;
	std::vector<Material*> m_materials;
};

