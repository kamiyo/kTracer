#pragma once

#include <yaml-cpp/yaml.h>
#include "Options.h"
#include "Material.h"
#include "Objects.h"

class Parser
{
public:
	Parser(const std::string filename);
	void loadOptions(Options* o) const;
	~Parser() {}

private:
	YAML::Node m_scene;
};