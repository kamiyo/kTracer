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

	template <typename T>
	static inline void shuffle(Eigen::Matrix<T, -1, -1>& samples) {
		int random;
		for (int i = (int) samples.size() - 1; i > 0; i--) {
			random = genRand_int(0, i);
			std::swap(samples(i), samples(random));
		}
		return;
	}

	static inline void shuffle(Sampler2d& samples, bool together = false) {
		if (together) {
			int random;
			for (int i = (int) samples.size() - 1; i > 0; i--) {
				random = genRand_int(0, i);
				std::swap(samples(i), samples(random));
			}
			return;
		}
		else {
			int randomx, randomy;
			for (int i = (int) samples.size() - 1; i > 0; i--) {
				randomx = genRand_int(0, i);
				randomy = genRand_int(0, i);
				std::swap(samples(i).x(), samples(randomx).x());
				std::swap(samples(i).y(), samples(randomy).y());
			}
			return;
		}
	}

	static inline void shuffle_correlated(Sampler2d& samples) {
		int random;
		int height = (int)samples.rows(), width = (int)samples.cols();
		for (int j = height - 1; j > 0; j--) {
			random = genRand_int(0, j);
			for (int i = width - 1; i > 0; i--) {
				std::swap(samples(i, j).x(), samples(i, random).x());
			}
		}
		for (int i = width - 1; i > 0; i--) {
			random = genRand_int(0, i);
			for (int j = height - 1; j > 0; j--) {
				std::swap(samples(i, j).y(), samples(random, j).y());
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
		shuffle(m_samples);
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
			double u = radicalInverse(i, 7);
			double v = radicalInverse(i, 5);
			m_samples(i) = Vector2d(u, v);
		}
	}

	Vector2d getSample(int x, int y) const {
		return m_samples(x, y);
	}

};

class PermutedHaltonSampler : public Sampler {
public:
	PermutedHaltonSampler(int w, int h) {
		m_height = h, m_width = w;
		m_samples.resize(w, h);
		generatePermutation(m_permutationU, 7);
		generatePermutation(m_permutationV, 5);
	}

	inline void generatePermutation(Eigen::Matrix<int, -1, -1>& permutation, int base) {
		permutation.resize(base, 1);
		for (int i = 0; i < base; i++) {
			permutation(i) = i;
		}
		shuffle(permutation);
	}

	void genPoints() {
		int total = m_height * m_width;
		for (int i = 0; i < total; i++) {
			double u = permutedRadicalInverse(i, 7, m_permutationU);
			double v = permutedRadicalInverse(i, 5, m_permutationV);
			m_samples(i) = Vector2d(u, v);
		}
	}

	Vector2d getSample(int x, int y) const {
		return m_samples(x, y);
	}

private:
	Eigen::Matrix<int, -1, -1> m_permutationU, m_permutationV;
	//static const int primes[];

};