#pragma once

#include "typedefs.h"
#include "utils.h"

class Sampler
{
public:
	virtual void genPoints() = 0;
	virtual Vector2d getSample(int x, int y) const = 0;
	virtual Sampler2d getSamples() {
		return m_samples;
	}
	void shuffle(bool together = false) {
		if (together) {
			int random;
			for (int i = (int) m_samples.size() - 1; i > 0; i--) {
				random = genRand_int(0, i);
				std::swap(m_samples(i), m_samples(random));
			}
			return;
		}
		else {
			int randomx, randomy;
			for (int i = (int) m_samples.size() - 1; i > 0; i--) {
				randomx = genRand_int(0, i);
				randomy = genRand_int(0, i);
				std::swap(m_samples(i).x(), m_samples(randomx).x());
				std::swap(m_samples(i).y(), m_samples(randomy).y());
			}
			return;
		}
	}

	void shuffle_correlated() {
		int random;
		for (int j = m_height - 1; j > 0; j--) {
			random = genRand_int(0, j);
			for (int i = m_width - 1; i > 0; i--) {
				std::swap(m_samples(i, j).x(), m_samples(i, random).x());
			}
		}
		for (int i = m_width - 1; i > 0; i--) {
			random = genRand_int(0, i);
			for (int j = m_height - 1; j > 0; j--) {
				std::swap(m_samples(i, j).y(), m_samples(random, j).y());
			}
		}
	}

	Sampler2d m_samples;
	int m_height, m_width;
};

class RandomSampler : public Sampler {
public:
	RandomSampler(int w, int h) {
		m_height = h, m_width = w;
		m_samples.resize(w, h);
	}

	void genPoints() {
		for (int i = 0; i < m_width; i++) {
			for (int j = 0; j < m_height; j++) {
				m_samples(i, j) = Vector2d(i + genRand_real(0, 1), j + genRand_real(0, 1));
			}
		}
	}

	Vector2d getSample(int x, int y) const {
		return m_samples(x, y);
	}

};

class CenteredSampler : public Sampler {
public:
	CenteredSampler(int w, int h) {
		m_height = h, m_width = w;
		m_samples.resize(w, h);
	}

	void genPoints() {
		for (int i = 0; i < m_width; i++) {
			for (int j = 0; j < m_height; j++) {
				m_samples(i, j) = Vector2d((i + 0.5) / (double) m_width, (j + 0.5) / (double) m_height);
			}
		}
	}

	Vector2d getSample(int x, int y) const {
		return m_samples(x, y);
	}
};

class StratifiedSampler : public Sampler {
public:
	StratifiedSampler(int w, int h) {
		m_height = h, m_width = w;
		m_samples.resize(w, h);
	}

	void genPoints() {
		for (int i = 0; i < m_width; i++) {
			for (int j = 0; j < m_height; j++) {
				m_samples(i, j) = Vector2d((i + genRand_real(0, 1)) / (double) m_width, (j + genRand_real(0, 1)) / (double) m_height);
			}
		}
	}

	Vector2d getSample(int x, int y) const {
		return m_samples(x, y);
	}
};


class NRooksSampler : public Sampler {
public:
	NRooksSampler(int w, int h) {
		m_height = h, m_width = w;
		m_samples.resize(w, h);
	}

	void genPoints() {
		int total = m_height * m_width;
		for (int i = 0; i < total; i++) {
			m_samples(i) = Vector2d((i + genRand_real(0, 1)) / (double) total, (i + genRand_real(0, 1)) / (double) total);
		}
		shuffle();
	}

	Vector2d getSample(int x, int y) const {
		return m_samples(x, y);
	}

};

class HaltonSampler : public Sampler {
public:
	HaltonSampler(int w, int h) {
		m_height = h, m_width = w;
		m_samples.resize(w, h);
	}

	void genPoints() {
		int total = m_height * m_width;
		for (int i = 0; i < total; i++) {
			double u = radicalInverse(i, 3);
			double v = radicalInverse(i, 2);
			m_samples(i) = Vector2d(u, v);
		}
	}

	Vector2d getSample(int x, int y) const {
		return m_samples(x, y);
	}

};