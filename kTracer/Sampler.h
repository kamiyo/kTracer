#pragma once

#include "typedefs.h"
#include "Random.h"

std::ostream &operator<<(std::ostream &os, Sampler2d &s);

// TODO make -0.5 to 0.5

class Sampler
{
public:
	Sampler(Random* rng) : m_rng(rng) {}
	virtual void genPoints() {}
	virtual const Sampler2d& getImageSamples() {
		return m_image;
	}
	/*virtual const Sampler1d& getLightSamples() {
		return m_light;
	}*/
	virtual const Sampler2d& getLensSamples() const {
		return m_lens;
	}

	void getSamples(const ArrayXi& oneD, const ArrayXi& twoD, Sampler1d& oneOut, Sampler2d& twoOut) {


	}

	template <typename T>
	static inline void shuffle(Eigen::Matrix<T, -1, -1>& samples, Random* rng) {
		int random;
		for (int i = (int) samples.size() - 1; i > 0; i--) {
			random = rng->discrete(0, i);
			std::swap(samples(i), samples(random));
		}
		return;
	}

	static inline void shuffle(Sampler2d& samples, Random* rng, bool together = false) {
		if (together) {
			int random;
			for (int i = (int) samples.size() - 1; i > 0; i--) {
				random = rng->discrete(0, i);
				std::swap(samples(i), samples(random));
			}
			return;
		}
		else {
			int randomx, randomy;
			for (int i = (int) samples.size() - 1; i > 0; i--) {
				randomx = rng->discrete(0, i);
				randomy = rng->discrete(0, i);
				std::swap(samples(i).x(), samples(randomx).x());
				std::swap(samples(i).y(), samples(randomy).y());
			}
			return;
		}
	}

	static inline void shuffle_correlated(Sampler2d& samples, Random* rng) {
		int random;
		int height = (int)samples.rows(), width = (int)samples.cols();
		for (int j = height - 1; j > 0; j--) {
			random = rng->discrete(0, j);
			for (int i = 0; i < width; i++) {
				std::swap(samples(j, i).x(), samples(random, i).x());
			}
		}
		for (int i = width - 1; i > 0; i--) {
			random = rng->discrete(0, i);
			for (int j = 0; j < height; j++) {
				std::swap(samples(j, i).y(), samples(j, random).y());
			}
		}
	}
	
	Sampler2d m_image, m_lens;
	Random* m_rng;
	//Sampler1d m_light;
	int m_height, m_width;
};

class RandomSampler : public Sampler {
public:
	RandomSampler(int w, int h, Random* rng) : Sampler(rng) {
		m_height = h, m_width = w;
		m_image.resize(h, w);
		m_lens.resize(h, w);
	}

	void genPoints() {
		for (int i = 0; i < m_width; i++) {
			for (int j = 0; j < m_height; j++) {
				m_image(j, i) << i + m_rng->real(0, 1), j + m_rng->real(0, 1);
				m_lens(j, i) << i + m_rng->real(0, 1), j + m_rng->real(0, 1);
			}
		}
	}


};

class CenteredSampler : public Sampler {
public:
	CenteredSampler(int w, int h, Random* rng) : Sampler(rng) {
		m_height = h, m_width = w;
		m_image.resize(h, w);
		m_lens.resize(h, w);
	}

	void genPoints() {
		for (int i = 0; i < m_width; i++) {
			for (int j = 0; j < m_height; j++) {
				m_image(j, i) << (i + 0.5) / (double) m_width, (j + 0.5) / (double) m_height;
				m_lens(j, i) << (i + 0.5) / (double) m_width, (j + 0.5) / (double) m_height;
			}
		}
		shuffle(m_lens, m_rng);
	}

};

class StratifiedSampler : public Sampler {
public:
	StratifiedSampler(int w, int h, Random* rng) : Sampler(rng) {
		m_height = h, m_width = w;
		m_image.resize(h, w);
		m_lens.resize(h, w);
	}

	void genPoints() {
		for (int i = 0; i < m_width; i++) {
			for (int j = 0; j < m_height; j++) {
				m_image(j, i) = Vector2d((i + m_rng->real(0, 1)) / (double) m_width, (j + m_rng->real(0, 1)) / (double) m_height);
				m_lens(j, i) = Vector2d((i + m_rng->real(0, 1)) / (double) m_width, (j + m_rng->real(0, 1)) / (double) m_height);
			}
		}
		shuffle(m_lens, m_rng);
	}

};


class NRooksSampler : public Sampler {
public:
	NRooksSampler(int w, int h, Random* rng) : Sampler(rng) {
		m_height = h, m_width = w;
		m_image.resize(h, w);
		m_lens.resize(h, w);
	}

	void genPoints() {
		int total = m_height * m_width;
		for (int i = 0; i < total; i++) {
			m_image(i) = Vector2d((i + m_rng->real(0, 1)) / (double) total, (i + m_rng->real(0, 1)) / (double) total);
			m_lens (i) = Vector2d((i + m_rng->real(0, 1)) / (double) total, (i + m_rng->real(0, 1)) / (double) total);
		}
		shuffle(m_image, m_rng);
		shuffle(m_lens, m_rng);
	}

};

class MultiJitteredSampler : public Sampler {
public:
	MultiJitteredSampler(int w, int h, Random* rng) : Sampler(rng) {
		m_height = h, m_width = w;
		m_image.resize(h, w);
		m_lens.resize(h, w);
	}

	void genPoints() {
		for (int j = 0; j < m_height; j++) {
			for (int i = 0; i < m_width; i++) {
				m_image(j, i) = Vector2d((i + (j + m_rng->real(0, 1)) / (double) m_height) / (double) m_width, (j + (i + m_rng->real(0, 1)) / (double) m_width) / (double) m_height);
				m_lens(j, i) = Vector2d((i + (j + m_rng->real(0, 1)) / (double) m_height) / (double) m_width, (j + (i + m_rng->real(0, 1)) / (double) m_width) / (double) m_height);
			}
		}
		shuffle_correlated(m_image, m_rng);
		shuffle_correlated(m_lens, m_rng);
		shuffle(m_lens, m_rng, true);
	}

};

class HaltonSampler : public Sampler {
public:
	HaltonSampler(int w, int h, Random* rng) : Sampler(rng) {
		m_height = h, m_width = w;
		m_image.resize(h, w);
		m_lens.resize(h, w);
	}

	void genPoints() {
		int total = m_height * m_width;
		for (int i = 0; i < total; i++) {
			double u = radicalInverse(i, 7);
			double v = radicalInverse(i, 5);
			m_image(i) = Vector2d(u, v);
			u = radicalInverse(i, 11);
			v = radicalInverse(i, 13);
			m_lens(i) = Vector2d(u, v);
		}
	}

	inline double radicalInverse(int n, int base) {
		double result = 0;
		double invBase = 1.0 / base, invBaseIncrement = invBase;
		while (n > 0) {
			int digit = (n % base);
			result += digit * invBaseIncrement;
			n = (int) (n * invBase);
			invBaseIncrement *= invBase;
		}
		return result;
	}

};

class PermutedHaltonSampler : public Sampler {
public:
	PermutedHaltonSampler(int w, int h, Random* rng) : Sampler(rng) {
		m_height = h, m_width = w;
		m_image.resize(h, w);
		m_lens.resize(h, w);
		generatePermutation(m_permImageU, 5, rng);
		generatePermutation(m_permImageV, 7, rng);
		generatePermutation(m_permLensU, 11, rng);
		generatePermutation(m_permLensV, 13, rng);
	}

	inline void generatePermutation(Eigen::Matrix<int, -1, -1>& permutation, int base, Random* rng) {
		permutation.resize(base, 1);
		for (int i = 0; i < base; i++) {
			permutation(i) = i;
		}
		shuffle(permutation, rng);
	}

	void genPoints() {
		
		int total = m_height * m_width;
		for (int i = 0; i < total; i++) {
			double u = permutedRadicalInverse(i, 7, m_permImageU);
			double v = permutedRadicalInverse(i, 5, m_permImageV);
			m_image(i) << u, v;
			u = permutedRadicalInverse(i, 11, m_permLensU);
			v = permutedRadicalInverse(i, 13, m_permLensV);
			m_lens(i) << u, v;
		}
	}

	inline double permutedRadicalInverse(int n, int base, Eigen::Matrix<int, -1, -1>& permutation) {
		double result = 0;
		double invBase = 1.0 / base, invBaseIncrement = invBase;
		while (n > 0) {
			int digit = permutation(n % base);
			result += digit * invBaseIncrement;
			n = (int) (n * invBase);
			invBaseIncrement *= invBase;
		}
		return result;
	}

private:
	Eigen::Matrix<int, -1, -1> m_permImageU, m_permImageV, m_permLensU, m_permLensV;
	//static const int primes[];

};

class LowDiscrepancySampler : public Sampler {
public:
	LowDiscrepancySampler(int w, int h, Random* rng) : Sampler(rng) {
		m_height = h, m_width = w;
		m_image.resize(h, w);
		m_lens.resize(h, w);
	}

	void genPoints() {
		LDShuffle(m_image, m_rng);
		LDShuffle(m_lens, m_rng);
	}

private:
	static inline void LDShuffle(Sampler2d& samples, Random* rng) {
		Vector2ui scramble(rng->udiscrete(), rng->udiscrete());
		for (unsigned int i = 0; i < (unsigned int) samples.size(); i++) {
			Sample02(i, scramble, samples(i));
		}
		shuffle(samples, rng);
	}

	static inline void Sample02(unsigned int n, const Vector2ui& scramble, Vector2d& sample) {
		sample.x() = VanDerCorput(n, scramble[0]);
		sample.y() = Sobol2(n, scramble[1]);
	}

	static inline double VanDerCorput(unsigned int n, unsigned int scramble) {
		n = (n << 16) | (n >> 16);
		n = ((n & 0x00ff00ff) << 8) | ((n & 0xff00ff00) >> 8);
		n = ((n & 0x0f0f0f0f) << 4) | ((n & 0xf0f0f0f0) >> 4);
		n = ((n & 0x33333333) << 2) | ((n & 0xcccccccc) >> 2);
		n = ((n & 0x55555555) << 1) | ((n & 0xaaaaaaaa) >> 1);
		n ^= scramble;
		return ((n >> 8) & 0xffffff) / double(1 << 24);
	}

	static inline double Sobol2(unsigned int n, unsigned int scramble) {
		for (unsigned int v = 1 << 31; n != 0; n >>= 1, v ^= v >> 1) {
			if (n & 0x1) scramble ^= v;
		}
		return ((scramble >> 8) & 0xffffff) / double(1 << 24);
	}
};