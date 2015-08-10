#pragma once

#include "typedefs.h"
#include <map>
#include "Random.h"

std::ostream &operator<<(std::ostream &os, Samplerd &s);

// TODO make -0.5 to 0.5

class Sampler
{
public:
	//virtual void genPoints() = 0;
	virtual void genPoints1d(Ref<Samplerd> samples, int size, int prime = 0, int offset = 0) = 0;
	virtual void genPoints2d(Ref<Samplerd> samples, int size, int prime0 = 0, int prime1 = 1, int offset = 0) = 0;

	//fix twoD size
	/*void getIntegratorSamples(const ArrayXi& oneD, const ArrayXi& twoD, Samplerd& oneOut, Samplerd& twoOut) {
		int oneDsize = oneD.sum();
		int twoDsize = (2 * twoD).sum();
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
	}*/

	static inline void shuffle(Ref<Samplerd> samples, Random* rng, bool together = false) {
		if (together) {
			int random;
			for (int i = (int) samples.rows(); i > 0; i--) {
				random = rng->discrete(0, i);
				std::swap(samples.row(i), samples.row(random));
			}
			return;
		}
		else {
			int dim = (int) samples.cols();
			int random;
			for (int i = (int) samples.rows(); i > 0; i--) {
				for (int d = 0; d < dim; d++) {
					random = rng->discrete(0, i);
					std::swap(samples(i, d), samples(random, d));
				}
			}
			return;
		}
	}

	// only 2d samples (dim = samples.cols() = 2), size = samples.rows() ^ 1/2
	static inline void shuffle_correlated(Ref<Samplerd> samples, int size, Random* rng) {
		int random;
		for (int j = size - 1; j > 0; j--) {
			random = rng->discrete(0, j);
			for (int i = 0; i < size; i++) {
				std::swap(samples(size * j + i, 0), samples(size * random + i, 0));
			}
		}
		for (int i = size - 1; i > 0; i--) {
			random = rng->discrete(0, i);
			for (int j = 0; j < size; j++) {
				std::swap(samples(size * j + i, 1), samples(size * j + random, 1));
			}
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
	
	static std::map<std::string, int> enums;

	Random* m_rng;
	int m_type;
	static const std::vector<int> primes;
};

class RandomSampler : public Sampler {
public:
	RandomSampler(Random* rng) {
		m_rng = rng;
		m_type = enums["random"];
	}

	void genPoints1d(Ref<Samplerd> samples, int size, int prime = 0, int offset = 0) {
		for (int i = 0; i < size; i++) {
			samples(offset + i) = m_rng->real(0, 1);
		}
	}

	void genPoints2d(Ref<Samplerd> samples, int size, int prime0 = 0, int prime1 = 1, int offset = 0) {
		for (int i = 0; i < size * size; i++) {
			samples.row(offset + i) << m_rng->real(0, 1), m_rng->real(0, 1);
		}
	}
};

class CenteredSampler : public Sampler {
public:
	CenteredSampler(Random* rng) {
		m_rng = rng;
		m_type = enums["center"];
	}

	void genPoints1d(Ref<Samplerd> samples, int size, int prime = 0, int offset = 0) {
		for (int i = 0; i < size; i++) {
			samples(offset + i) = (i + 0.5) / (double) size;
		}
	}

	void genPoints2d(Ref<Samplerd> samples, int size, int prime0 = 0, int prime1 = 1, int offset = 0) {
		double invSize = 1.0 / size;
		for (int j = 0; j < size; j++) {
			for (int i = 0; i < size; i++) {
				samples.row(offset + j * size + i) << (i + 0.5) * invSize
													, (j + 0.5) * invSize;
			}
		}
	}

};

class StratifiedSampler : public Sampler {
public:
	StratifiedSampler(Random* rng) {
		m_rng = rng;
		m_type = enums["jittered"];
	}

	void genPoints1d(Ref<Samplerd> samples, int size, int prime = 0, int offset = 0) {
		double invSize = 1.0 / size;
		for (int i = 0; i < size; i++) {
			samples(offset + i) = (i + m_rng->real(0, 1)) * invSize;
		}
	}

	void genPoints2d(Ref<Samplerd> samples, int size, int prime0 = 0, int prime1 = 1, int offset = 0) {
		double invSize = 1.0 / size;
		for (int j = 0; j < size; j++) {
			for (int i = 0; i < size; i++) {
				samples.row(offset + j * size + i) << (i + m_rng->real(0, 1)) * invSize
													, (j + m_rng->real(0, 1)) * invSize;
			}
		}
	}
};


class NRooksSampler : public Sampler {
public:
	NRooksSampler(Random* rng) {
		m_rng = rng;
		m_type = enums["nrooks"];
	}

	void genPoints1d(Ref<Samplerd> samples, int size, int prime = 0, int offset = 0) {
		for (int i = 0; i < size; i++) {
			samples(offset + i) = (i + m_rng->real(0, 1)) / (double) size;
		}
		shuffle(samples.block(offset, 0, size, 1), m_rng);
	}

	void genPoints2d(Ref<Samplerd> samples, int size, int prime0 = 0, int prime1 = 1, int offset = 0) {
		int total = size * size;
		double invTotal = 1.0 / total;
		for (int i = 0; i < total; i++) {
			samples.row(offset + i) << (i + m_rng->real(0, 1)) * invTotal
									 , (i + m_rng->real(0, 1)) * invTotal;
		}
		shuffle(samples.block(offset, 0, total, 2), m_rng);
	}

};

class MultiJitteredSampler : public Sampler {
public:
	MultiJitteredSampler(Random* rng) {
		m_rng = rng;
		m_type = enums["multijittered"];
	}

	void genPoints1d(Ref<Samplerd> samples, int size, int prime = 0, int offset = 0) {
		for (int i = 0; i < size; i++) {
			samples(offset + i) = (i + m_rng->real(0, 1)) / size;
		}
		shuffle(samples.block(offset, 0, size, 1), m_rng);
	}

	void genPoints2d(Ref<Samplerd> samples, int size, int prime0 = 0, int prime1 = 1, int offset = 0) {
		for (int j = 0; j < size; j++) {
			for (int i = 0; i < size; i++) {
				samples.row(offset + j * size + i)
					<< (i + (j + m_rng->real(0, 1)) / (double) size) / (double) size
					 , (j + (i + m_rng->real(0, 1)) / (double) size) / (double) size;
			}
		}
		shuffle_correlated(samples.block(offset, 0, size * size, 1), size, m_rng);
	}

};

class HaltonSampler : public Sampler {
public:
	HaltonSampler(Random* rng) : m_current_prime(0) {
		m_rng = rng;
		m_type = enums["halton"];
	}

	HaltonSampler(int dim, Random* rng) : m_current_prime(0), m_dims(dim) {
		m_rng = rng;
	}

	void genPoints1d(Ref<Samplerd> samples, int size, int prime = 0, int offset = 0) {
		for (int i = 0; i < size; i++) {
			samples(offset + i) = radicalInverse(i, primes[(m_current_prime++) % primes.size()]);
		}
	}

	void genPoints2d(Ref<Samplerd> samples, int size, int prime0 = 0, int prime1 = 1, int offset = 0) {
		int total = size * size;
		for (int i = 0; i < total; i++) {
			samples.row(offset + i)
				<< radicalInverse(i, primes[prime0])
				, (double) i / total;
		}
	}

	void genDimPoints(Ref<Samplerd> samples, int sample) {
		int total = (int) samples.size();
		for (int i = 0; i < m_dims; i++) {
			samples(i) = radicalInverse(sample, primes[i]);
		}
	}

private:
	int m_current_prime;
	int m_dims;
};

class PermutedHaltonSampler : public Sampler {
public:
	PermutedHaltonSampler(Random* rng) : m_current_prime(0) {
		m_rng = rng;
		for (int i = 0; i < 2; i++) {
			generatePermutation(m_permutations[i], primes[i], m_rng);
		}
		m_type = enums["permutedhalton"];
	}

	PermutedHaltonSampler(int dim, Random* rng) : m_current_prime(0), m_dims(dim) {
		m_rng = rng;
		for (int i = 0; i < m_dims; i++) {
			generatePermutation(m_permutations[i], primes[i], m_rng);
		}
		m_type = enums["permutedhalton"];
	}

	inline void generatePermutation(Eigen::Array<int, -1, -1>& permutation, int base, Random* rng) {
		permutation.resize(base, 1);
		for (int i = 0; i < base; i++) {
			permutation(i) = i;
		}
		shuffle(permutation, rng);
	}

	void genPoints1d(Ref<Samplerd> samples, int size, int prime = 0, int offset = 0) {
		if (m_permutations[prime].size() == 0) {
			generatePermutation(m_permutations[prime], primes[prime], m_rng);
		}
		for (int i = 0; i < size; i++) {
			samples(offset + i) = permutedRadicalInverse(i, primes[prime], m_permutations[prime]);
		}
	}

	void genPoints2d(Ref<Samplerd> samples, int size, int prime0 = 0, int prime1 = 1, int offset = 0) {
		if (m_permutations[prime0].size() == 0) {
			generatePermutation(m_permutations[prime0], primes[prime0], m_rng);
		}
		int total = size * size;
		for (int i = 0; i < total; i++) {
			samples.row(offset + i)
				<< permutedRadicalInverse(i, primes[prime0], m_permutations[prime0])
				, (double) i / total;
		}
	}
	
	void genDimPoints(Ref<Samplerd> samples, int sample) {
		int total = (int) samples.size();
		for (int i = 0; i < m_dims; i++) {
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
	int m_current_prime;
	int m_dims;


};

class LowDiscrepancySampler : public Sampler {
public:
	LowDiscrepancySampler(Random* rng) {
		m_rng = rng;
		m_type = enums["lowdiscrepancy"];
	}
	
	void genPoints1d(Ref<Samplerd> samples, int size, int prime = 0, int offset = 0) {
		unsigned int scramble = m_rng->udiscrete();
		for (unsigned int i = 0; i < (unsigned int) size; i++) {
			samples(offset + i) = VanDerCorput(i, scramble);
		}
		shuffle(samples.block(offset, 0, size, 1), m_rng);
	}

	void genPoints2d(Ref<Samplerd> samples, int size, int prime0 = 0, int prime1 = 1, int offset = 0) {
		int total = size * size;
		Vector2ui scramble(m_rng->udiscrete(), m_rng->udiscrete());
		for (unsigned int i = 0; i < (unsigned int) total; i++) {
			samples.row(offset + i) = Sample02(i, scramble);
		}
		shuffle(samples.block(offset, 0, total, 2), m_rng, true);
	}

private:
	static inline Vector2d Sample02(unsigned int n, const Vector2ui& scramble) {
		return Vector2d(VanDerCorput(n, scramble[0]), Sobol2(n, scramble[1]));
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