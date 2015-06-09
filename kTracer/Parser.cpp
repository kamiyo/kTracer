#include "Parser.h"
#include <iostream>
#include <fstream>

Parser::Parser(const std::string filename)
{
	try {
		m_scene = YAML::LoadAllFromFile(filename);
	}
	catch (std::exception e) {
		std::cerr << e.what() << std::endl;
	}	
}


Parser::~Parser()
{
}
