#pragma once

#include "typedefs.h"

class Hilbert {
public:
	Hilbert(int x, int y);
	~Hilbert() {}
	Sampler2i getPoints() const { return m_points; }
	int size() const {	return m_size; }
private:
	Sampler2i m_points;
	int m_size;
	Vector2i m_dim;
	static const int N = 0, E = 1, S = 2, W = 3;
	int nX, nY;
	void move(int d);
	void hilbert(int i, int front, int right, int behind, int left);
};

