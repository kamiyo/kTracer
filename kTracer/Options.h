#pragma once

#include <map>
#include <string>
#include "Sampler.h"


class Options
{
public:
	Options() {}
	~Options() {}

	void AAtype(std::string type, int n) {
		m_antialias = Sampler::enums[type];
		m_samples = n;
	}
	Sampler* getAASampler(Random* rng) {
		if (m_antialias == Sampler::enums["center"]) {
			Sampler* s = new CenteredSampler(rng);
			return s;
		}
		if (m_antialias == Sampler::enums["jittered"]) {
			Sampler* s = new StratifiedSampler(rng);
			return s;
		}
		if (m_antialias == Sampler::enums["random"]) {
			Sampler* s = new RandomSampler(rng);
			return s;
		}
		if (m_antialias == Sampler::enums["nrooks"]) {
			Sampler* s = new NRooksSampler(rng);
			return s;
		}
		if (m_antialias == Sampler::enums["halton"]) {
			Sampler* s = new HaltonSampler(rng);
			return s;
		}
		if (m_antialias == Sampler::enums["permutedhalton"]) {
			Sampler* s = new PermutedHaltonSampler(rng);
			return s;
		}
		if (m_antialias == Sampler::enums["lowdiscrepancy"]) {
			Sampler* s = new LowDiscrepancySampler(rng);
			return s;
		}
		if (m_antialias == Sampler::enums["multijittered"]) {
			Sampler* s = new MultiJitteredSampler(rng);
			return s;
		}
		return nullptr;
	}

	void shadow(std::string type, std::string shape) {
		m_shadow_type = enums[type];
		m_light_shape = enums[shape];
	}
	void dof(std::string type) {
		m_dof = enums[type];
	}
	void structure(std::string type) {
		m_scene_structure = enums[type];
	}
	void renderOrder(std::string type) {
		m_render_order = enums[type];
	}

	std::map<std::string, int> enums = std::map<std::string, int>{
		{ "off", 0 },
		{ "hard", 1 }, { "soft", 2 },
		{ "square", 1 }, { "circle", 2 },
		{ "bvh", 1 },
		{ "scanline", 0 }, { "hilbert", 1 }
	};

	static const int OFF = 0;
	
	static const int CENTER = 1, RANDOM = 2, JITTERED = 3, NROOKS = 4, NROOKS_CORRELATED = 5, ADAPTIVE = 6;
	int m_antialias;
	int m_samples;
	
	static const int HARD = 1, SOFT = 2;
	static const int SQUARE = 1, CIRCLE = 2;
	int m_shadow_type;
	int m_light_shape;

	int m_dof;

	int m_recursion_depth, m_refraction_depth;

	static const int BVH = 1;
	int m_scene_structure;

	static const int SCANLINE = 0, HILBERT = 1;
	int m_render_order;
};

