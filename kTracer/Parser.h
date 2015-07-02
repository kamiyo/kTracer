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
	void loadMaterials(std::vector<Material *>& m) const;
	~Parser() {}

private:
	YAML::Node m_scene;
};