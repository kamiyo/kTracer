#include "Sampler.h"

std::ostream &operator<<(std::ostream &os, Sampler2d &s) {
	for (int j = 0; j < s.rows(); j++) {
		for (int i = 0; i < s.cols(); i++) {
			os << s(j, i)[0] << " " << s(j, i)[1] << std::endl;
		}
	}
	return os;
}

const std::vector<int> PermutedHaltonSampler::primes = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };