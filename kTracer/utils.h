#pragma once

/* utility fx */

#include <OpenEXR\ImfRgbaFile.h>
#include "typedefs.h"

void writeRgba(const char* file, const Imf::Rgba* pixels, int width, int height);

Vector4d sampleSphere(double u, double v);

double spherePdf();

void to_unit_disk(double seedx, double seedy, Vector2d& v);