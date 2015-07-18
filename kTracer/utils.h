#pragma once

/* utility fx */

#include <OpenEXR\ImfRgbaFile.h>
#include "typedefs.h"

void writeRgba(const char* file, const Imf::Rgba* pixels, int width, int height);
