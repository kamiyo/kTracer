#pragma once

#include "Options.h"
#include "Objects.h"
#include <yaml-cpp/yaml.h>
#include <unordered_map>

class Parser
{
public:
	Parser(const std::string filename);
	void loadOptions(Options* o) const;
	void loadCamera(Camera* c) const;
	void loadLights(std::vector<Light*>& lightVec) const;
	void loadMaterials(std::unordered_map<std::string, Material*>& matVec) const;
	void loadScene(std::vector<Surface*>& sVec, const std::unordered_map<std::string, Material*>& matVec) const;
	~Parser() {}

private:
	YAML::Node m_scene;
};