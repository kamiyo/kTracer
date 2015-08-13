#pragma once

#include "typedefs.h"
#include <map>
#include "Random.h"

std::ostream &operator<<(std::ostream &os, Ref<Samplerd>s);

// TODO make -0.5 to 0.5

class Sampler
{
public:
	//virtual void genPoints() = 0;
	virtual void genPoints1d(Samplerd& samples, int size, int prime = 0, int offset = 0) = 0;
	virtual void genPoints2d(Samplerd& samples, int size, int prime0 = 0, int prime1 = 0, int offset = 0) = 0;

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

	template <typename Derived>
	static inline void shuffle(Eigen::ArrayBase<Derived> const & samples, Random* rng, bool together = false) {
		Eigen::ArrayBase<Derived> & temp = const_cast<Eigen::ArrayBase<Derived> & >(samples);
		if (together) {
			int random;
			for (int i = (int) temp.cols() - 1; i > 0; i--) {
				random = rng->discrete(0, i);
				temp.col(i).swap(temp.col(random));
			}
			return;
		}
		else {
			int dim = (int) temp.rows();
			int random;
			for (int i = (int) temp.cols() - 1; i > 0; i--) {
				for (int d = 0; d < dim; d++) {
					random = rng->discrete(0, i);
					std::swap(temp(d, i), temp(d, random));
				}
			}
			return;
		}
	}

	// only 2d samples (dim = samples.rows() = 2), size = samples.cols() ^ 1/2
	template <typename Derived>
	static inline void shuffle_correlated(Eigen::ArrayBase<Derived> const & samples, int size, Random* rng) {
		Eigen::ArrayBase<Derived> & temp = const_cast<Eigen::ArrayBase<Derived> & >(samples);
		int random;
		for (int j = size - 1; j > 0; j--) {
			random = rng->discrete(0, j);
			for (int i = 0; i < size; i++) {
				std::swap(temp(0, size * j + i), temp(0, size * random + i));
			}
		}
		for (int i = size - 1; i > 0; i--) {
			random = rng->discrete(0, i);
			for (int j = 0; j < size; j++) {
				std::swap(temp(1, size * j + i), temp(1, size * j + random));
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

	void genPoints1d(Samplerd& samples, int size, int prime = 0, int offset = 0) {
		for (int i = 0; i < size; i++) {
			samples(offset + i) = m_rng->real(0, 1);
		}
	}

	void genPoints2d(Samplerd& samples, int size, int prime0 = 0, int prime1 = 0, int offset = 0) {
		for (int i = 0; i < size * size; i++) {
			samples.col(offset + i) << m_rng->real(0, 1), m_rng->real(0, 1);
		}
	}
};

class CenteredSampler : public Sampler {
public:
	CenteredSampler(Random* rng) {
		m_rng = rng;
		m_type = enums["center"];
	}

	void genPoints1d(Samplerd& samples, int size, int prime = 0, int offset = 0) {
		for (int i = 0; i < size; i++) {
			samples(offset + i) = (i + 0.5) / (double) size;
		}
	}

	void genPoints2d(Samplerd& samples, int size, int prime0 = 0, int prime1 = 1, int offset = 0) {
		double invSize = 1.0 / size;
		for (int j = 0; j < size; j++) {
			for (int i = 0; i < size; i++) {
				samples.col(offset + j * size + i) << (i + 0.5) * invSize
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

	void genPoints1d(Samplerd& samples, int size, int prime = 0, int offset = 0) {
		double invSize = 1.0 / size;
		for (int i = 0; i < size; i++) {
			samples(offset + i) = (i + m_rng->real(0, 1)) * invSize;
		}
	}

	void genPoints2d(Samplerd& samples, int size, int prime0 = 0, int prime1 = 0, int offset = 0) {
		double invSize = 1.0 / size;
		for (int j = 0; j < size; j++) {
			for (int i = 0; i < size; i++) {
				samples.col(offset + j * size + i) << (i + m_rng->real(0, 1)) * invSize
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

	void genPoints1d(Samplerd& samples, int size, int prime = 0, int offset = 0) {
		double invSize = 1.0 / size;
		for (int i = 0; i < size; i++) {
			samples(offset + i) = (i + m_rng->real(0, 1)) * invSize;
		}
		shuffle(samples.block(0, offset, 1, size), m_rng);
	}

	void genPoints2d(Samplerd& samples, int size, int prime0 = 0, int prime1 = 0, int offset = 0) {
		int total = size * size;
		double invTotal = 1.0 / total;
		for (int i = 0; i < total; i++) {
			samples.col(offset + i) << (i + m_rng->real(0, 1)) * invTotal
									 , (i + m_rng->real(0, 1)) * invTotal;
		}
		shuffle(samples.block(0, offset, 2, total), m_rng);
	}

};

class MultiJitteredSampler : public Sampler {
public:
	MultiJitteredSampler(Random* rng) {
		m_rng = rng;
		m_type = enums["multijittered"];
	}

	void genPoints1d(Samplerd& samples, int size, int prime = 0, int offset = 0) {
		double invSize = 1.0 / size;
		for (int i = 0; i < size; i++) {
			samples(offset + i) = (i + m_rng->real(0, 1)) * invSize;
		}
		shuffle(samples.block(0, offset, 1, size), m_rng);
	}

	void genPoints2d(Samplerd& samples, int size, int prime0 = 0, int prime1 = 0, int offset = 0) {
		double invSize = 1.0 / size;
		for (int j = 0; j < size; j++) {
			for (int i = 0; i < size; i++) {
				samples.col(offset + j * size + i)
					<< (i + (j + m_rng->real(0, 1)) * invSize) * invSize
					, (j + (i + m_rng->real(0, 1)) * invSize) * invSize;
			}
		}
		shuffle_correlated(samples.block(0, offset, 2, size * size), size, m_rng);
	}

};

class HaltonSampler : public Sampler {
public:
	HaltonSampler(Random* rng) {
		m_rng = rng;
		m_type = enums["halton"];
	}

	HaltonSampler(int dim, Random* rng) : m_dims(dim) {
		m_rng = rng;
	}

	void genPoints1d(Samplerd& samples, int size, int prime = 0, int offset = 0) {
		for (int i = 0; i < size; i++) {
			samples(offset + i) = radicalInverse(i, primes[prime]);
		}
	}

	void genPoints2d(Samplerd& samples, int size, int prime0 = 0, int prime1 = 0, int offset = 0) {
		int total = size * size;
		double invTotal = 1.0 / total;
		for (int i = 0; i < total; i++) {
			samples.col(offset + i)
				<< radicalInverse(i, primes[prime0])
				,  (prime1 == 0) ? ((double) i * invTotal) : radicalInverse(i, primes[prime1]);
		}
	}

	void genDimPoints(Samplerd& samples, int sample) {
		int total = (int) samples.size();
		for (int i = 0; i < m_dims; i++) {
			samples(i) = radicalInverse(sample, primes[i]);
		}
	}

private:
	int m_dims;
};

class PermutedHaltonSampler : public Sampler {
public:
	PermutedHaltonSampler(Random* rng) {
		m_rng = rng;
		for (int i = 0; i < 2; i++) {
			generatePermutation(m_permutations[i], primes[i], m_rng);
		}
		m_type = enums["permutedhalton"];
	}

	PermutedHaltonSampler(int dim, Random* rng) : m_dims(dim) {
		m_rng = rng;
		for (int i = 0; i < m_dims; i++) {
			generatePermutation(m_permutations[i], primes[i], m_rng);
		}
		m_type = enums["permutedhalton"];
	}

	void genPoints1d(Samplerd& samples, int size, int prime = 0, int offset = 0) {
		if (m_permutations[prime].size() == 0) {
			generatePermutation(m_permutations[prime], primes[prime], m_rng);
		}
		for (int i = 0; i < size; i++) {
			samples(offset + i) = permutedRadicalInverse(i, primes[prime], m_permutations[prime]);
		}
	}

	void genPoints2d(Samplerd& samples, int size, int prime0 = 0, int prime1 = 0, int offset = 0) {
		if (m_permutations[prime0].size() == 0) {
			generatePermutation(m_permutations[prime0], primes[prime0], m_rng);
		}
		int total = size * size;
		double invTotal = 1.0 / total;
		for (int i = 0; i < total; i++) {
			samples.col(offset + i)
				<< permutedRadicalInverse(i, primes[prime0], m_permutations[prime0])
				, (double) i * invTotal;
		}
	}
	
	void genDimPoints(Samplerd& samples, int sample) {
		int total = (int) samples.size();
		for (int i = 0; i < m_dims; i++) {
			samples(i) = permutedRadicalInverse(sample, primes[i], m_permutations[i]);
		}
	}

private:
	inline void generatePermutation(Eigen::Array<int, -1, -1>& permutation, int base, Random* rng) {
		permutation.resize(base, 1);
		for (int i = 0; i < base; i++) {
			permutation(i) = i;
		}
		shuffle(permutation, rng);
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
	
	void genPoints1d(Samplerd& samples, int size, int prime = 0, int offset = 0) {
		unsigned int scramble = m_rng->udiscrete();
		for (unsigned int i = 0; i < (unsigned int) size; i++) {
			samples(offset + i) = VanDerCorput(i, scramble);
		}
		shuffle(samples.block(0, offset, 1, size), m_rng);
	}

	void genPoints2d(Samplerd& samples, int size, int prime0 = 0, int prime1 = 0, int offset = 0) {
		int total = size * size;
		Vector2ui scramble(m_rng->udiscrete(), m_rng->udiscrete());
		for (unsigned int i = 0; i < (unsigned int) total; i++) {
			samples.col(offset + i) = Sample02(i, scramble);
		}
		shuffle(samples.block(0, offset, 2, total), m_rng);

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