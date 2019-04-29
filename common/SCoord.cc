#include <stdlib.h>
#include <math.h>
#include "SMat.h"

// WGS84 defining parameters
// see https://en.wikipedia.org/wiki/Geodetic_datum

// WGS84 semi-major-axis of the Earth, meters
#define WGS84_A 6378137.0

// WGS84 reciprocal of flattening
#define WGS84_F_INV 298.257223563


static double cubic_root(double x) {
	if (x > 0.0) return exp(log(x)/3.0);
	if (x < 0.0) return -exp(log(-x)/3.0);
	return 0.0;
}

// The application of Ferrari's solution
// see https://en.wikipedia.org/wiki/Geographic_coordinate_conversion
int ECEFToLLH(const SVec& ecef, SVec & llh) {
	double X = ecef.x0, Y = ecef.x1, Z = ecef.x2;
	double Z2 = Z * Z;
	double r2 = X * X + Y * Y;
	double r = sqrt(r2);
	double a = WGS84_A;
	double f = 1.0 / WGS84_F_INV;
	double b = a * (1.0 - f); // semi-minor axis of the Earth
	double a2 = a * a;
	double b2 = b * b;
	double e2 = f * (2.0 - f);
	double e12 = (a2 - b2) / b2;
	if (r < EPS) {
		// special case if r is close to zero (polar axis)
		// not a good place for tractor, but navigation should work
		if (Z >= 0.0) {
			llh.x0 = M_PI_2;
			llh.x1 = 0.0;
			llh.x2 = Z - b;
		} else {
			llh.x0 = -M_PI_2;
			llh.x1 = 0.0;
			llh.x2 = -Z - b;
		}
		return 0;
	}
	
	double F = 54.0 * b2 * Z2;
	double G = r2 + (1.0 - e2) * Z2 - e2 * (a2 - b2);
	double c = e2 * e2 * F * r2 / (G * G * G);
	double s = cubic_root(1.0 + c + sqrt(c * c + 2.0 * c));
	double tmp = s + (1.0 / s) + 1.0;
	double P = F / (3.0 * tmp * tmp * G * G);
	double Q = sqrt(1.0 + 2 * e2 * e2 * P);
	tmp = 0.5 * a2 * (1.0 + 1.0 / Q) - P * (1 - e2) * Z2 / (Q * (1.0 + Q)) - 0.5 * P * r2;
	if (tmp < 0.0) {
		// cannot analyze if this case is possible, so take care
		return -1;
	}
	double r0 = sqrt(tmp);
	r0 -= P * e2 * r / (1.0 + Q);
	tmp = r - e2 * r0;
	tmp *= tmp;
	double U = sqrt(tmp + Z2);
	double V = sqrt(tmp + (1 - e2) * Z2);
	tmp = b2 / (a * V);
	double z0 = tmp * Z;
	llh.x2 = U * (1.0 - tmp);
	llh.x0 = atan((Z + e12 * z0) / r);
	llh.x1 = atan2(Y, X);

	return 0;
}

/* see https://en.wikipedia.org/wiki/Local_tangent_plane_coordinates */
void ECEFToNEDMatrix(const SVec & llh, SMat & RT) {
	double s1 = sin(llh.x0), c1 = cos(llh.x0), s2 = sin(llh.x1), c2 = cos(llh.x1);
	RT = SMat(-s1 * c2, -s1 * s2,  c1,
			-s2, c2, 0.0,
			-c1 * c2, -c1 * s2, -s1);
}

double Degrees(double rad) {
	return rad * (180.0 / M_PI);
}
