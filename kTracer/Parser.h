#pragma once

#include <yaml-cpp/yaml.h>
#include "Options.h"
#include "Objects.h"

class Parser
{
public:
	Parser(const std::string filename);
	void loadOptions(Options* o) const;
	void loadCamera(Camera* c) const;
	void loadLights(std::vector<Light*>& lightVec) const;
	void loadMaterials(std::vector<Material*>& matVec) const;
	void loadScene(std::vector<Surface*>& sVec) const;
	~Parser() {}

private:
	YAML::Node m_scene;
};