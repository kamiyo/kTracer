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
		Vector3d dirVec;
		if (camera["dir"]) {
			YAML::Node dir = camera["dir"];
			dirVec = Vector3d(dir["x"].as<double>(), dir["y"].as<double>(), dir["z"].as<double>());
		}
		else if (camera["at"]) {
			YAML::Node at = camera["at"];
			Vector3d atVec(at["x"].as<double>(), at["y"].as<double>(), at["z"].as<double>());
			dirVec = atVec - eyeVec;
		}

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

void Parser::loadMaterials(std::vector<Material *>& m) const {
	try {
		YAML::Node materials = m_scene["materials"];
		for (YAML::Node m : materials) {
			std::string name = m["name"].as<std::string>();
			RGB diffuse, specular, attenuation;
			diffuse = specular = RGB::Zero();
			attenuation = RGB::Ones();
			if (m["diffuse"]) {
				YAML::Node d = m["diffuse"];
				diffuse = RGB(d["r"].as<double>(), d["g"].as<double>(), d["b"].as<double>());
			}
		}



	}
}


//void Parser::loadScene
