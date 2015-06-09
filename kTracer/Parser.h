#pragma once

#include <yaml-cpp/yaml.h>
#include "Options.h"
#include "Material.h"
#include "Objects.h"

class Parser
{
public:
	Parser(const std::string filename);
	void 
	~Parser();

private:
	std::vector<YAML::Node> m_scene;
};