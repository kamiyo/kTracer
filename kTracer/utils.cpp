#include "utils.h"

#include <array>
#include <random>

namespace {
	std::mt19937_64 mt;
	std::uniform_real_distribution<double> uni_real;
	std::normal_distribution<double> uni_gauss;
	std::uniform_int_distribution<int> uni_int;
}

void seedRand() {
	std::array<unsigned int, 2 * std::mt19937_64::state_size> seed_data;
	std::random_device r;
	std::generate_n(seed_data.begin(), seed_data.size(), std::ref(r));
	std::seed_seq seq(std::begin(seed_data), std::end(seed_data));

	mt = std::mt19937_64(seq);
	uni_real = std::uniform_real_distribution<double>(0., 1.);
	uni_gauss = std::normal_distribution<double>(0., 1.);
}

double genRand_real() {
	return uni_real(mt);
}

double genRand_real(double x, double y) {
	std::uniform_real_distribution<double> randouble(x, y);
	return randouble(mt);
}

double genRand_gauss() {
	return uni_gauss(mt);
}

int genRand_int(int x, int y) {
	std::uniform_int_distribution<int> ranint(x, y);
	return ranint(mt);
}


void writeRgba(const char* file, const Imf::Rgba* pixels, int width, int height) {
	Imf::RgbaOutputFile f(file, width, height, Imf::WRITE_RGBA);
	f.setFrameBuffer(pixels, 1, width);
	f.writePixels(height);
}