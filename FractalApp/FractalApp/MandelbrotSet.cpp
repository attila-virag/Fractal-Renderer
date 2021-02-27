#include "MandelbrotSet.h"
#include <complex>

using namespace std;

void MandelbrotSet::CalculatePoint(Point* pt)
{
	int iterationCountLimit = m_location->recommendedIterations;

	double x_point = m_location->x_min + pt->x_pixel * m_location->x_increment;
	double y_point = m_location->y_min + pt->y_pixel * m_location->y_increment;

	int iteration = 0;
	double normalizedIteration = 0;
	double escapeAngle = 0;
	double magnitude = 0;

  complex<double> z = 0;
	complex<double> c(x_point, y_point);

	// we are actually switched x and y?
	if (CheckIfPointIsInside(x_point, y_point)) {
		pt->active = false;
	}
	else { // run escape algorithm
		iteration = DoIterations(z, c, pt->active, iterationCountLimit, m_pow);
		if (iteration >= iterationCountLimit) {
			pt->active = false;
		}
	}

	// post processing as needed, we wont bother with unescaped points since we color them black
	if (pt->active) {
		magnitude = abs(z);
		escapeAngle = arg(z) / 2 * PI;
		//normalizedIteration = NormalizeIterations(iteration, z, c);
		//normalizedIteration = log10(log2(iteration / iterationCountLimit));
	}

	//we will record iteration count, final angle, final magnitude
	pt->double1 = escapeAngle;
	pt->double2 = magnitude;
	pt->int1 = iteration;
}

// optimization to check if point is within cardioid or bulb
// prevents needing to run the escape algorithm for the point
// only works for mandelbrot set
bool MandelbrotSet::CheckIfPointIsInside(double x, double y) {
	//cardioid check
	double q = std::pow((x - (1 / 4.0)), 2) + std::pow(y, 2);

	double lhs = q * (q + (x - (1 / 4.0)));
	double rhs = (1 / 4) * std::pow(y, 2);

	if (lhs <= rhs) {
		return true;
	}

	// check for p-2 bulb
	lhs = std::pow((x + 1), 2) + std::pow(y, 2);

	if (lhs <= (1 / 16.0)) {
		return true;
	}

	return false;
}

int MandelbrotSet::DoIterations(complex<double>& z, complex<double>& c, bool& escaped, int iterationLimit, double power) {

	int iteration = 0;

	while (iteration < iterationLimit) {
		z = std::pow(z, power) + c;

		double magnitude = abs(z);

		// if larger than 4 the point has escaped
		if (magnitude > 4) {

			escaped = true;

			break;
		}
		iteration++;
	}
	return iteration;
}

