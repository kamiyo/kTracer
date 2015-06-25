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
		o->AAtype(options["antialias"]["type"].as<std::string>(),
			options["antialias"]["n"].as<int>());
		o->shadow(options["shadows"]["type"].as<std::string>(),
			options["shadows"]["shape"].as<std::string>());
		o->m_recursion_depth = options["recursionDepth"].as<int>();
		o->m_refraction_depth = options["refractionDepth"].as<int>();
		o->dof(options["depthOfField"].as<std::string>());
		o->structure(options["structure"].as<std::string>());
		o->renderOrder(options["order"].as<std::string>());
	}
	catch (std::exception e) {
		std::cerr << e.what() << std::endl;
	}
}

void Parser::loadCamera(Camera* c) const {
	try {
		YAML::Node camera = m_scene["camera"];
		YAML::Node eye = camera["eye"];
		Vector3d eyeVec(eye["x"].as<double>(), eye["y"].as<double>(), eye["z"].as<double>());
		YAML::Node dir = camera["dir"];
		Vector3d dirVec(dir["x"].as<double>(), dir["y"].as<double>(), dir["z"].as<double>());
		YAML::Node up = camera["up"];
		Vector3d upVec(up["x"].as<double>(), up["y"].as<double>(), up["z"].as<double>());

		YAML::Node image = camera["image"];
		double w = image["w"].as<double>();
		double h = image["h"].as<double>();
		
		YAML::Node pixels = camera["pixels"];
		int nx = pixels["w"].as<int>();
		int ny = pixels["h"].as<int>();

		double fstop = (camera["f-stop"]) ? camera["f-stop"].as<double>() : nINF;
		double focal = (camera["focal"]) ? camera["focal"].as<double>() : nINF;
		c = new Camera(eyeVec, dirVec, upVec, w, h, nx, ny, focal, fstop);

	}
	catch (std::exception e) {
		std::cerr << e.what() << std::endl;
	}
}