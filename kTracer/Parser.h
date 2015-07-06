#pragma once

#include "Options.h"
#include "Objects.h"
#include <yaml-cpp/yaml.h>
#include <unordered_map>

class Parser
{
public:
	Parser(const std::string filename);
	Options* loadOptions() const;
	Camera* loadCamera() const;
	void loadLights(std::vector<Light*>& lightVec) const;
	void loadMaterials(std::unordered_map<std::string, Material*>& matVec) const;
	Group* loadScene(const std::unordered_map<std::string, Material*>& matVec) const;
	~Parser() {}

private:
	YAML::Node m_scene;
};