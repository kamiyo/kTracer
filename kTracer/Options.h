#pragma once
class Options
{
public:
	Options();
	~Options();

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

