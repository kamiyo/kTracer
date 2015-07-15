#pragma once

/* utility fx */

#include <OpenEXR\ImfRgbaFile.h>


void seedRand();
double genRand_real();
double genRand_gauss();
double genRand_real(double x, double y);
int genRand_int(int x, int y);

void writeRgba(const char* file, const Imf::Rgba* pixels, int width, int height);

inline double radicalInverse(int n, int base) {
	double result = 0;
	double invBase = 1.0 / base, invBaseIncrement = invBase;
	while (n > 0) {
		int digit = (n % base);
		result += digit * invBaseIncrement;
		n *= invBase;
		invBaseIncrement *= invBase;
	}
	return result;
}