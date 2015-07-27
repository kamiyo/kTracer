#pragma once

#include "typedefs.h"
#include "Random.h"

std::ostream &operator<<(std::ostream &os, Sampler2d &s);

// TODO make -0.5 to 0.5

class Sampler
{
public:
	//virtual void genPoints() = 0;
	virtual void genPoints(Ref<Sampler1d>& samples) = 0;
	virtual void genPoints(Ref<Sampler1d>& samples, int size, int offset = 0) = 0;
	virtual void genPoints(Ref<Sampler2d>& samples) = 0;
	virtual void genPoints(Sampler2d& samples, int size, int offset = 0) = 0;

	void getIntegratorSamples(const ArrayXi& oneD, const ArrayXi& twoD, Sampler1d& oneOut, Sampler2d& twoOut) {
		int oneDsize = oneD.sum();
		int twoDsize = (twoD * twoD).sum();
		oneOut.resize(oneDsize, 1);
		twoOut.resize(twoDsize, 1);
		int offset = 0;
		for (int i = 0; i < (int) oneD.size(); i++) {
			int nSamples = oneD(i);
			genPoints(oneOut, offset, nSamples);
			offset += nSamples;
		}
		if (offset != oneDsize) {
			std::cerr << "something went wrong with the sampling" << std::endl;
		}
		offset = 0;
		for (int i = 0; i < (int) twoD.size(); i++) {
			int nSamples = twoD(i);
			genPoints(twoOut, offset, nSamples);
			offset += nSamples * nSamples;
		}
		if (offset != twoDsize) {
			std::cerr << "something went wrong with the sampling" << std::endl;
		}
	}

	void shuffle(Sampler2d& samples, bool together = false) {
		if (together) {
			int random;
			for (int i = (int) samples.size() - 1; i > 0; i--) {
				random = m_rng->discrete(0, i);
				std::swap(samples(i), samples(random));
			}
			return;
		}
		else {
			int randomx, randomy;
			for (int i = (int) samples.size() - 1; i > 0; i--) {
				randomx = m_rng->discrete(0, i);
				randomy = m_rng->discrete(0, i);
				std::swap(samples(i).x(), samples(randomx).x());
				std::swap(samples(i).y(), samples(randomy).y());
			}
			return;
		}
	}

	template <typename T>
	static inline void shuffle(Eigen::Array<T, -1, -1>& samples, Random* rng) {
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
	
	Random* m_rng;
};

class RandomSampler : public Sampler {
public:
	RandomSampler(Random* rng) {
		m_rng = rng;
	}

	void genPoints(Sampler1d& samples) {
		for (int i = 0; i < (int) samples.size(); i++) {
			samples(i) = m_rng->real(0, 1);
		}
	}

	void genPoints(Sampler2d& samples) {
		for (int i = 0; i < (int) samples.size(); i++) {
			samples(i) << m_rng->real(0, 1), m_rng->real(0, 1);
		}
	}

	void genPoints(Sampler1d& samples, int offset, int size) {
		for (int i = 0; i < size; i++) {
			samples(offset + i) = m_rng->real(0, 1);
		}
	}

	void genPoints(Sampler2d& samples, int offset, int size) {
		for (int i = 0; i < size * size; i++) {
			samples(offset + i) << m_rng->real(0, 1), m_rng->real(0, 1);
		}
	}

};

class CenteredSampler : public Sampler {
public:
	CenteredSampler(Random* rng) {
		m_rng = rng;
	}

	void genPoints(Sampler1d& samples) {
		int size = (int)samples.size();
		for (int i = 0; i < size; i++) {
			samples(i) = (i + 0.5) / size;
		}
	}

	void genPoints(Sampler2d& samples) {
		int height = (int) samples.rows(), width = (int) samples.cols();
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				samples(j, i) << (i + 0.5) / width, (j + 0.5) / height;
			}
		}
	}
	
	void genPoints(Sampler1d& samples, int size, int offset = 0) {
		for (int i = 0; i < size; i++) {
			samples(offset + i) = (i + 0.5) / size;
		}
	}

	void genPoints(Sampler2d& samples, int size, int offset = 0) {
		for (int j = 0; j < size; j++) {
			for (int i = 0; i < size; i++) {
				samples(offset + (j * size) + i) << (i + 0.5) / size, (j + 0.5) / size;
			}
		}
	}
};

class StratifiedSampler : public Sampler {
public:
	StratifiedSampler(Random* rng) {
		m_rng = rng;
	}

	void genPoints(Sampler1d& samples) {
		int size = (int) samples.size();
		for (int i = 0; i < (int) samples.size(); i++) {
			samples(i) = (i + m_rng->real(0, 1)) / (double) size;
		}
	}

	void genPoints(Sampler2d& samples) {
		int height = (int) samples.rows(), width = (int) samples.cols();
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				samples(j, i) << (i + m_rng->real(0, 1)) / (double) width
							   , (j + m_rng->real(0, 1)) / (double) height;
			}
		}
	}

	void genPoints(Sampler1d& samples, int size, int offset = 0) {
		for (int i = 0; i < size; i++) {
			samples(offset + i) = (i + m_rng->real(0, 1)) / (double) size;
		}
	}

	void genPoints(Sampler2d& samples, int size, int offset = 0) {
		for (int j = 0; j < size; j++) {
			for (int i = 0; i < size; i++) {
				samples(offset + (j * size) + i) << (i + m_rng->real(0, 1)) / (double) size
												  , (j + m_rng->real(0, 1)) / (double) size;
			}
		}
	}

};


class NRooksSampler : public Sampler {
public:
	NRooksSampler(Random* rng) {
		m_rng = rng;
	}

	void genPoints(Sampler1d& samples) {
		int size = (int) samples.size();
		for (int i = 0; i < size; i++) {
			samples(i) = (i + m_rng->real(0, 1)) / (double) size;
		}
		shuffle(samples, m_rng);
	}

	void genPoints(Sampler1d& samples, int size, int offset = 0) {
		for (int i = 0; i < size; i++) {
			samples(offset + i) = (i + m_rng->real(0, 1)) / (double) size;
		}
		shuffle(samples, m_rng);
	}

	void genPoints(Sampler2d& samples) {
		int size = (int) samples.size();
		for (int i = 0; i < size; i++) {
			samples(i) << (i + m_rng->real(0, 1)) / (double) size, (i + m_rng->real(0, 1)) / (double) size;
		}
		shuffle(samples, m_rng);
	}

	void genPoints(Sampler2d& samples, int size, int offset = 0) {
		int total = size * size;
		for (int i = 0; i < total; i++) {
			samples(offset + i) << (i + m_rng->real(0, 1)) / (double) total, (i + m_rng->real(0, 1)) / (double) total;
		}
		shuffle(samples, m_rng);
	}

};

class MultiJitteredSampler : public Sampler {
public:
	MultiJitteredSampler(Random* rng) {
		m_rng = rng;
	}

	void genPoints(Sampler1d& samples) {
		int size = (int) samples.size();
		for (int i = 0; i < size; i++) {
			samples(i) = (i + m_rng->real(0, 1)) / (double) size;
		}
		shuffle(samples, m_rng);
	}

	void genPoints(Sampler1d& samples, int size, int offset = 0) {
		for (int i = 0; i < size; i++) {
			samples(offset + i) = (i + m_rng->real(0, 1)) / (double) size;
		}
		shuffle(samples.block(offset, 0, size, 1), m_rng);
	}

	void genPoints(Sampler2d& samples) {
		int height = (int) samples.rows(), width = (int) samples.cols();
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				samples(j, i) << (i + (j + m_rng->real(0, 1)) / (double) height) / (double) width
							   , (j + (i + m_rng->real(0, 1)) / (double) width) / (double) height;
			}
		}
		shuffle_correlated(samples, m_rng);
	}

	void genPoints(Sampler2d& samples, int size, int offset = 0) {
		int height = size, width = size;
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				samples(offset + (j * width) + height) << (i + (j + m_rng->real(0, 1)) / (double) height) / (double) width
														, (j + (i + m_rng->real(0, 1)) / (double) width) / (double) height;
			}
		}
		shuffle_correlated(samples, m_rng);
	}

};

class HaltonSampler : public Sampler {
public:
	HaltonSampler(Random* rng) : m_current_prime(0) {
		m_rng = rng;
	}

	void genPoints(Sampler2d& samples) {
		int total = (int) samples.size();
		for (int i = 0; i < total; i++) {
			double u = radicalInverse(i, primes[(m_current_prime++) % primes.size()]);
			double v = radicalInverse(i, primes[(m_current_prime++) % primes.size()]);
			samples(i) = Vector2d(u, v);
		}
	}

	void genPoints(Sampler1d& samples) {
		int total = (int) samples.size();
		for (int i = 0; i < total; i++) {
			samples(i) = radicalInverse(i, primes[(m_current_prime++) % primes.size()]);
		}
	}

	void genPoints(Sampler2d& samples, int size, int offset = 0) {
		int total = size * size;
		for (int i = 0; i < total; i++) {
			double u = radicalInverse(i, primes[(m_current_prime++) % primes.size()]);
			double v = radicalInverse(i, primes[(m_current_prime++) % primes.size()]);
			samples(offset + i) = Vector2d(u, v);
		}
	}

	void genPoints(Sampler1d& samples, int size, int offset = 0) {
		int total = size;
		for (int i = 0; i < total; i++) {
			samples(offset + i) = radicalInverse(i, primes[(m_current_prime++) % primes.size()]);
		}
	}

	static inline double radicalInverse(int n, int base) {
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

private:
	static const std::vector<int> primes;
	int m_current_prime;
};

class PermutedHaltonSampler : public Sampler {
public:
	PermutedHaltonSampler(Random* rng) : m_current_prime(0) {
		m_rng = rng;
		for (int i = 0; i < 2; i++) {
			generatePermutation(m_permutations[i], primes[i], m_rng);
		}
	}

	PermutedHaltonSampler(int dim, Random* rng) : m_current_prime(0), m_dims(dim) {
		m_rng = rng;
		for (int i = 0; i < m_dims; i++) {
			generatePermutation(m_permutations[i], primes[i], m_rng);
		}
	}

	inline void generatePermutation(Eigen::Array<int, -1, -1>& permutation, int base, Random* rng) {
		permutation.resize(base, 1);
		for (int i = 0; i < base; i++) {
			permutation(i) = i;
		}
		shuffle(permutation, rng);
	}

	void genPoints(Sampler2d& samples) {
		int total = (int)samples.size();
		for (int i = 0; i < total; i++) {
			double u = permutedRadicalInverse(i, primes[0], m_permutations[0]);
			double v = permutedRadicalInverse(i, primes[1], m_permutations[1]);
			samples(i) << u, v;
		}
	}

	void genPoints(Sampler2d& samples, int size, int offset = 0) {
		int total = size * size;
		for (int i = 0; i < total; i++) {
			double u = permutedRadicalInverse(i, primes[0], m_permutations[0]);
			double v = permutedRadicalInverse(i, primes[1], m_permutations[1]);
			samples(offset + i) << u, v;
		}
	}

	void genPoints(Sampler1d& samples) {
		int total = (int) samples.size();
		for (int i = 0; i < total; i++) {
			double u = permutedRadicalInverse(i, primes[0], m_permutations[0]);
			samples(i) = u;
		}
	}

	void genPoints(Sampler1d& samples, int size, int offset = 0) {
		for (int i = 0; i < size; i++) {
			samples(offset + i) = permutedRadicalInverse(i, primes[0], m_permutations[0]);
		}
	}

	void genDimPoints(Sampler1d& samples, int sample) {
		int total = (int) samples.size();
		for (int i = 0; i < m_dims; i++) {
			if (m_permutations[i].size() == 0) {
				generatePermutation(m_permutations[i], primes[i], m_rng);
			}
			samples(i) = permutedRadicalInverse(sample, primes[i], m_permutations[i]);
		}
	}

	inline double permutedRadicalInverse(int n, int base, Eigen::Array<int, -1, -1>& permutation) {
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
	Eigen::Array<int, -1, -1> m_permutations[100];
	static const std::vector<int> primes;
	int m_current_prime;
	int m_dims;


};

class LowDiscrepancySampler : public Sampler {
public:
	LowDiscrepancySampler(Random* rng) {
		m_rng = rng;
	}

	void genPoints(Sampler2d& samples) {
		Vector2ui scramble(m_rng->udiscrete(), m_rng->udiscrete());
		std::cerr << scramble.transpose() << std::endl;
		for (unsigned int i = 0; i < (unsigned int) samples.size(); i++) {
			Sample02(i, scramble, samples(i));
		}
		shuffle(samples, m_rng, true);
	}

	void genPoints(Sampler2d& samples, int size, int offset = 0) {
		Vector2ui scramble(m_rng->udiscrete(), m_rng->udiscrete());
		for (unsigned int i = 0; i < (unsigned int) size; i++) {
			Sample02(i, scramble, samples(offset + i));
		}
		shuffle(samples, m_rng, true);
	}

	void genPoints(Sampler1d& samples) {
		unsigned int scramble = m_rng->udiscrete();
		for (unsigned int i = 0; i < (unsigned int) samples.size(); i++) {
			samples(i) = VanDerCorput(i, scramble);
		}
		shuffle(samples, m_rng);
	}

	void genPoints(Sampler1d& samples, int size, int offset = 0) {
		unsigned int scramble = m_rng->udiscrete();
		for (unsigned int i = 0; i < (unsigned int) size; i++) {
			samples(offset + i) = VanDerCorput(i, scramble);
		}
		shuffle(samples, m_rng);
	}

private:
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
		return (n & 0xffffffff) / double(1ULL << 32);
	}

	static inline double Sobol2(unsigned int n, unsigned int scramble) {
		for (unsigned int v = 1 << 31; n != 0; n >>= 1, v ^= v >> 1) {
			if (n & 0x1) scramble ^= v;
		}
		return (scramble & 0xffffffff) / double(1ULL << 32);
	}
};