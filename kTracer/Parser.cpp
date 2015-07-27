#include "Parser.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stack>

Parser::Parser(const std::string filename) {
	try {
		m_scene = YAML::LoadFile(filename);
	}
	catch (std::exception e) {
		std::cerr << e.what() << std::endl;
	}	
}

Options* Parser::loadOptions() const {
	try {
		Options* o = new Options();
		YAML::Node options = m_scene["options"];
		o->AAtype(options["antialias"]["type"].as<std::string>(),
			options["antialias"]["n"].as<int>());
		o->shadow(options["shadows"]["type"].as<std::string>(),
			options["shadows"]["shape"].as<std::string>());
		o->m_recursion_depth = options["recursionDepth"].as<int>();
		o->m_refraction_depth = options["refractionDepth"].as<int>();
		o->dof(options["depthOfField"]["type"].as<std::string>());
		o->structure(options["structure"].as<std::string>());
		o->renderOrder(options["order"].as<std::string>());
		return o;
	}
	catch (std::exception e) {
		std::cerr << "options: " << e.what() << std::endl;
		return nullptr;
	}
}

Camera* Parser::loadCamera() const {
	try {
		YAML::Node camera = m_scene["camera"];
		YAML::Node eye = camera["eye"];
		Vector4d eyeVec(eye["x"].as<double>(), eye["y"].as<double>(), eye["z"].as<double>(), 1);
		Vector4d dirVec;
		if (camera["dir"]) {
			YAML::Node dir = camera["dir"];
			dirVec << dir["x"].as<double>(), dir["y"].as<double>(), dir["z"].as<double>(), 0;
		}
		else if (camera["at"]) {
			YAML::Node at = camera["at"];
			Vector4d atVec(at["x"].as<double>(), at["y"].as<double>(), at["z"].as<double>(), 1);
			dirVec = atVec - eyeVec;
		}

		YAML::Node up = camera["up"];
		Vector4d upVec(up["x"].as<double>(), up["y"].as<double>(), up["z"].as<double>(), 0);

		YAML::Node image = camera["image"];
		double w = image["w"].as<double>();
		double h = image["h"].as<double>();
		
		YAML::Node pixels = camera["pixels"];
		int nx = pixels["w"].as<int>();
		int ny = pixels["h"].as<int>();

		double fstop = (camera["f-stop"]) ? camera["f-stop"].as<double>() : nINF;
		double focal = (camera["focal"]) ? camera["focal"].as<double>() : nINF;
		Camera* c = new Camera(eyeVec, dirVec, upVec, w, h, nx, ny, focal, fstop);
		return c;
	}
	catch (std::exception e) {
		std::cerr << "camera: " << e.what() << std::endl;
		return nullptr;
	}
}

void Parser::loadLights(std::vector<Light*>& lightVec) const {
	try {
		YAML::Node lights = m_scene["lights"];
		for (YAML::Node l : lights) {
			//l = l["light"];
			double size = (l["size"]) ? l["size"].as<double>() : 0.0;
			Vector4d attenuation(1.0, 0.0, 0.0, 0.0);

			YAML::Node i = l["intensity"];
			Rgba intensity(i["r"].as<double>(), i["g"].as<double>(), i["b"].as<double>(), 0);
			std::string type = l["type"].as<std::string>();

			if (l["attenuation"]) {
				YAML::Node a = l["attenuation"];
				attenuation << a["constant"].as<double>(), a["linear"].as<double>(), a["squared"].as<double>(), 0;
			}

			if (type == "point") {
				YAML::Node p = l["position"];
				Vector4d position(p["x"].as<double>(), p["y"].as<double>(), p["z"].as<double>(), 1);
				lightVec.push_back(new PointLight(position, intensity, attenuation, size));
				continue;
			}
			if (type == "directional") {
				YAML::Node d = l["direction"];
				Vector4d direction(d["x"].as<double>(), d["y"].as<double>(), d["z"].as<double>(), 0);
				lightVec.push_back(new DirectionalLight(direction, intensity, size));
				continue;
			}
			if (type == "area") {
				lightVec.push_back(new AreaLight(intensity, attenuation));
				continue;
			}
		}

	}
	catch (std::exception e) {
		std::cerr << "lights: " << e.what() << std::endl;
	}
}

void Parser::loadMaterials(std::unordered_map<std::string, Material *>& matVec) const {
	try {
		YAML::Node materials = m_scene["materials"];
		for (YAML::Node m : materials) {
			std::string name = m["name"].as<std::string>();
			Rgba diffuse, specular, attenuation;
			diffuse = specular = Rgba::Zero();
			attenuation << 1, 1, 1, 0;
			if (m["diffuse"]) {
				YAML::Node d = m["diffuse"];
				diffuse << d["r"].as<double>(), d["g"].as<double>(), d["b"].as<double>(), 0;
			}
			if (m["specular"]) {
				YAML::Node s = m["specular"];
				specular <<  s["r"].as<double>(), s["g"].as<double>(), s["b"].as<double>(), 0;
			}
			if (m["attenuation"]) {
				YAML::Node a = m["attenuation"];
				attenuation << a["r"].as<double>(), a["g"].as<double>(), a["b"].as<double>(), 0;
			}
			double index = (m["index"]) ? m["index"].as<double>() : 1.0;
			unsigned int power = (m["power"]) ? m["power"].as<unsigned int>() : 1u;
			bool normalized = (m["normalized"]) ? m["normalized"].as<bool>() : false;
			std::string type = m["type"].as<std::string>();
			if (type == "blinn-phong") {
				matVec[name] = new BlinnPhong(name, diffuse, specular, power, normalized);
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
		std::cerr << "materials: " << e.what() << std::endl;
	}
}

// still need to add transform parsing, and a bunch of other objects!
Group* Parser::loadScene(const std::unordered_map<std::string, Material*>& mVec, std::vector<Light*>& lightVec) const {
	try {
		for (auto things : mVec) {
			std::cout << things.first << std::endl;
		}
		Group* objVec = new Group();
		Group* planeVec = new Group();
		std::stack<Transform3d> transform, inverse;
		YAML::Node scene = m_scene["scene"];
		for (YAML::Node s : scene) {
			Material* material = nullptr;
			AreaLight* alight = nullptr;
			std::string matName = s["material"].as<std::string>();
			int lightNum = -1;
			if (matName == "light") {
				lightNum = s["light"].as<int>();
				if (lightVec[lightNum]->type() == Light::AREA) {
					alight = (AreaLight*) lightVec[lightNum];
				}
				else {
					std::cerr << "Light " << lightNum << " is not an area light!" << std::endl;
				}
			}
			else {
				auto m = mVec.find(matName);
				if (m != mVec.end()) {
					material = m->second;
				}
				else {
					throw std::exception("material not found!");
				}
			}
			std::string type = s["type"].as<std::string>();
			if (type == "plane") {
				YAML::Node p = s["position"], n = s["normal"];
				Vector4d position(p["x"].as<double>(), p["y"].as<double>(), p["z"].as<double>(), 1)
					   , normal(n["x"].as<double>(), n["y"].as<double>(), n["z"].as<double>(), 0);
				planeVec->push_back(new Plane(position, normal, material));
				continue;
			}
			if (type == "sphere") {
				YAML::Node p = s["position"];
				Vector4d position(p["x"].as<double>(), p["y"].as<double>(), p["z"].as<double>(), 1);
				double radius = s["radius"].as<double>();
				objVec->push_back(new Sphere(position, radius, material, alight));
				if (alight) ((AreaLight*) lightVec[lightNum])->setSurface(objVec->back());
				continue;
			}
			if (type == "triangle") {
				YAML::Node verts = s["vertices"];
				auto v1 = verts[0];
				auto v2 = verts[1];
				auto v3 = verts[2];
				Vector4d vert1(v1["x"].as<double>(), v1["y"].as<double>(), v1["z"].as<double>(), 1)
					, vert2(v2["x"].as<double>(), v2["y"].as<double>(), v2["z"].as<double>(), 1)
					, vert3(v3["x"].as<double>(), v3["y"].as<double>(), v3["z"].as<double>(), 1);
				objVec->push_back(new Triangle(vert1, vert2, vert3, material, alight));
				if (alight) ((AreaLight*) lightVec[lightNum])->setSurface(objVec->back());
				continue;
			}
			if (type == "circle") {
				YAML::Node p = s["position"], n = s["normal"];
				Vector4d position(p["x"].as<double>(), p["y"].as<double>(), p["z"].as<double>(), 1)
					   , normal(n["x"].as<double>(), n["y"].as<double>(), n["z"].as<double>(), 0);
				double radius = s["radius"].as<double>();
				objVec->push_back(new Circle(position, normal, radius, material, alight));
				if (alight) ((AreaLight*) lightVec[lightNum])->setSurface(objVec->back());
			}
		}
		Group* masterVec = new Group();
		masterVec->push_back(objVec)->push_back(planeVec);
		return masterVec;
	}
	catch (std::exception e) {
		std::cerr << "scene: " << e.what() << std::endl;
		return nullptr;
	}
}
