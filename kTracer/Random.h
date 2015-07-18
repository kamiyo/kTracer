#pragma once

#include <array>
#include <random>

class Random
{
public:
	Random() {
		seedRand();
	}
	~Random() {}

	double real() {
		return uni_real(mt);
	}

	double real(double x, double y) {
		std::uniform_real_distribution<double> randouble(x, y);
		return randouble(mt);
	}

	double gauss() {
		return uni_gauss(mt);
	}

	int discrete(int x, int y) {
		std::uniform_int_distribution<int> ranint(x, y);
		return ranint(mt);
	}

	int discrete() {
		std::uniform_int_distribution<int> ranint(0, std::numeric_limits<int>::max());
		return ranint(mt);
	}

	unsigned int udiscrete() {
		std::uniform_int_distribution<unsigned int> ranuint(0, std::numeric_limits<unsigned int>::max());
		return ranuint(mt);
	}

private:

	inline void seedRand() {
		std::array<unsigned int, 2 * std::mt19937_64::state_size> seed_data;
		std::random_device r;
		std::generate_n(seed_data.begin(), seed_data.size(), std::ref(r));
		std::seed_seq seq(std::begin(seed_data), std::end(seed_data));

		mt = std::mt19937_64(seq);
		uni_real = std::uniform_real_distribution<double>(0., 1.);
		uni_gauss = std::normal_distribution<double>(0., 1.);
	}

	std::mt19937_64 mt;
	std::uniform_real_distribution<double> uni_real;
	std::normal_distribution<double> uni_gauss;
	std::uniform_int_distribution<int> uni_int;
};

