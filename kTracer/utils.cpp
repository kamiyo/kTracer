#include "utils.h"

void writeRgba(const char* file, const Imf::Rgba* pixels, int width, int height) {
	Imf::RgbaOutputFile f(file, width, height, Imf::WRITE_RGBA);
	f.setFrameBuffer(pixels, 1, width);
	f.writePixels(height);
}