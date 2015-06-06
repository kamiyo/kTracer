#pragma once

class Scene
{
public:
	Scene();
	~Scene();

private:
	Options* m_options;
	Surface* m_surfaces;
	Lights* m_lights;
	Materials* m_materials;
};

