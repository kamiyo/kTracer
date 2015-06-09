#include "Parser.h"
#include <iostream>
#include <fstream>
#include <string>

Parser::Parser(const std::string filename) {
	try {
		m_scene = YAML::LoadFile(filename);
	}
	catch (std::exception e) {
		std::cerr << e.what() << std::endl;
	}	
}

void Parser::loadOptions(Options* o) const {
	try {
		YAML::Node options = m_scene["options"];
		o->AAtype(options["antialias"]["type"].as<std::string>(), options["antialias"]["n"].as<int>());


	}
	catch (std::exception e) {
		std::cerr << e.what() << std::endl;
	}

}