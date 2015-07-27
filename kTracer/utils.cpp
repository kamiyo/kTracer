#include "utils.h"

void writeRgba(const char* file, const Imf::Rgba* pixels, int width, int height) {
	Imf::RgbaOutputFile f(file, width, height, Imf::WRITE_RGBA);
	f.setFrameBuffer(pixels, 1, width);
	f.writePixels(height);
}

Vector4d sampleSphere(double u, double v) {
	double z = 1.0 - 2.0 * u;
	double r = sqrt(std::max(0.0, 1.0 - z * z));
	double phi = 2.0 * M_PI * v;
	double x = r * cos(phi);
	double y = r * sin(phi);
	return Vector4d(x, y, z, 1);
}

double spherePdf() {
	return 1.0 / (4.0 * M_PI);
}

void to_unit_disk(double seedx, double seedy, Vector2d& v)
{
	double phi, r;
	double a = 2 * seedx - 1;   /* (a,b) is now on [-1,1]^2 */
	double b = 2 * seedy - 1;

	if (a > -b) {
		/* region 1 or 2 */
		if (a > b) {
			/* region 1, also |a| > |b| */
			r = a;
			phi = (M_PI / 4) * (b / a);
		}
		else       {
			/* region 2, also |b| > |a| */
			r = b;
			phi = (M_PI / 4) * (2 - (a / b));
		}
	}
	else {
		/* region 3 or 4 */
		if (a < b) {
			/* region 3, also |a| >= |b|, a != 0 */
			r = -a;
			phi = (M_PI / 4) * (4 + (b / a));
		}
		else       {
			/* region 4, |b| >= |a|, but a==0 and b==0 could occur. */
			r = -b;
			if (b != 0)
				phi = (M_PI / 4) * (6 - (a / b));
			else
				phi = 0;
		}
	}
	v << r * cos(phi), r * sin(phi);
}

Vector2d to_unit_disk(const Vector2d& uv)
{
	double phi, r;
	double a = 2 * uv.x() - 1;   /* (a,b) is now on [-1,1]^2 */
	double b = 2 * uv.y() - 1;

	if (a > -b) {
		/* region 1 or 2 */
		if (a > b) {
			/* region 1, also |a| > |b| */
			r = a;
			phi = (M_PI / 4) * (b / a);
		}
		else       {
			/* region 2, also |b| > |a| */
			r = b;
			phi = (M_PI / 4) * (2 - (a / b));
		}
	}
	else {
		/* region 3 or 4 */
		if (a < b) {
			/* region 3, also |a| >= |b|, a != 0 */
			r = -a;
			phi = (M_PI / 4) * (4 + (b / a));
		}
		else       {
			/* region 4, |b| >= |a|, but a==0 and b==0 could occur. */
			r = -b;
			if (b != 0)
				phi = (M_PI / 4) * (6 - (a / b));
			else
				phi = 0;
		}
	}
	return Vector2d(r * cos(phi), r * sin(phi));
}