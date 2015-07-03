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

void Parser::loadLights(std::vector<Light*>& lightVec) const {
	try {
		YAML::Node lights = m_scene["lights"];
		for (YAML::Node l : lights) {
			double size = (l["size"]) ? l["size"].as<double>() : 0.0;
			Vector3d attenuation(1.0, 0.0, 0.0);
			YAML::Node i = l["intensity"];
			RGB intensity = RGB(i["r"].as<double>(), i["g"].as<double>(), i["b"].as<double>());
			std::string type = l["type"].as<std::string>();

			if (type == "point") {
				YAML::Node p = l["position"];
				Vector3d position = Vector3d(p["x"].as<double>(), p["y"].as<double>(), p["z"].as<double>());
				lightVec.push_back(new PointLight(position, intensity, attenuation, size));
				continue;
			}
			if (type == "directional") {
				YAML::Node d = l["direction"];
				Vector3d direction = Vector3d(d["x"].as<double>(), d["y"].as<double>(), d["z"].as<double>());
				lightVec.push_back(new DirectionalLight(direction, intensity, size));
				continue;
			}
		}

	}
	catch (std::exception e) {
		std::cerr << e.what() << std::endl;
	}
}

void Parser::loadMaterials(std::vector<Material *>& matVec) const {
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
			if (m["specular"]) {
				YAML::Node s = m["specular"];
				specular = RGB(s["r"].as<double>(), s["g"].as<double>(), s["b"].as<double>());
			}
			if (m["attenuation"]) {
				YAML::Node a = m["attenuation"];
				attenuation = RGB(a["r"].as<double>(), a["g"].as<double>(), a["b"].as<double>());
			}
			double index = (m["index"]) ? m["index"].as<double>() : 1.0;
			unsigned int power = (m["power"]) ? m["power"].as<unsigned int>() : 1u;
			bool normalized = (m["normalized"]) ? m["normalized"].as<bool>() : false;
			std::string type = m["type"].as<std::string>();
			if (type == "blinn-phong") {
				matVec.push_back(new BlinnPhong(name, diffuse, specular, power, normalized));
				continue;
			}
			if (type == "refractive") {
				continue;
				//matVec.push_back(new Refractive)
			}
			if (type == "reflective") {
				// reflective
				continue;
			}
		}
	}
	catch (std::exception e) {
		std::cerr << e.what() << std::endl;
	}
}


//void Parser::loadScene
