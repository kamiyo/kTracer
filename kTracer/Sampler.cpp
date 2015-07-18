#include "Sampler.h"

std::ostream &operator<<(std::ostream &os, Sampler2d &s) {
	for (int i = 0; i < s.size(); i++) {
		os << s(i)[0] << " " << s(i)[1] << " " << std::endl;
	}
	return os;
}
//const int PermutedHaltonSampler::primes[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };