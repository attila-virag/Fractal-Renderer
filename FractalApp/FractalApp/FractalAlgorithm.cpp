#include "stdafx.h"
#include "FractalAlgorithm.h"
#include <complex>

using std::complex;


// this will simply range the magnitude 0 -> 1 -> 0 going left to right
void FractalAlgorithm::ShowColorPalette(Result * pt)
{
	pt->finalMagnitude = pt->x_pixel / (double)m_zoom->pixels;
}

// optimization to check if point is within cardioid or bulb
// prevents needing to run the escape algorithm for the point
// only works for mandelbrot set
bool FractalAlgorithm::CheckIfPointIsInside(double x, double y) {
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

int FractalAlgorithm::DoIterations(complex<double>& z, complex<double>& c, bool& escaped, int iterationLimit, double power) {

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

// 
void FractalAlgorithm::MandelBrotSet(Result * pt)
{
	int iterationCountLimit = m_zoom->recommendedIterations;

	double x_point = m_zoom->x_min + pt->x_pixel*m_zoom->x_increment;
	double y_point = m_zoom->y_min + pt->y_pixel*m_zoom->y_increment;

	int iteration = 0;
	double normalizedIteration = 0;
	double escapeAngle = 0;
	double magnitude = 0;

	complex<double> z = 0;
	complex<double> c(x_point, y_point);

	// we are actually switched x and y?
	if (CheckIfPointIsInside(x_point, y_point)) {
		pt->escaped = false;
	}
	else { // run escape algorithm
		iteration = DoIterations(z, c, pt->escaped, iterationCountLimit, m_pow);
	}

	// post processing as needed
	if (pt->escaped) {
		magnitude = abs(z);
		escapeAngle = arg(z) / 2 * PI;
		//normalizedIteration = NormalizeIterations(iteration, z, c);
		normalizedIteration = log10(log2(iteration / iterationCountLimit));
	}

	switch(colorScheme) {
		case ColorScheme::EscapeAngle:
			pt->finalMagnitude = escapeAngle;
			break;
		case ColorScheme::IterationCount :
		{
			if (iterationsMin > iteration) {
				iterationsMin = iteration;
			}
			if (iterationsMax < iteration) {
				iterationsMax = iteration;
			}
			pt->finalMagnitude = iteration;
			break;
		}
		case ColorScheme::FinalMagnitude:
			pt->finalMagnitude = magnitude;
	}

	
}

// this functions to smoothen out he iteration count so not all the changes are bunched up at the edges
double FractalAlgorithm::NormalizeIterations(int iterations, complex<double> Zn, complex<double> C)
{
	//int extraIter = 0;

	//for (; extraIter < 3; extraIter++) {
	//	Zn = std::pow(Zn, m_pow) + C;
	//}

	auto nd = log(log(abs(Zn))) / log(m_pow); // log((log(abs(Zn)) / log(2))) / log(m_pow);

	auto sn = iterations+(double)1 - nd;

	if (sn < min_mag)
		min_mag = sn;
	if (sn > max_mag)
		max_mag = sn;

	return sn;
}

void FractalAlgorithm::GetNormalization(Result* pt) {

	if (colorScheme == ColorScheme::FinalMagnitude)
	{
		pt->finalMagnitude = sqrt(sqrt(pt->finalMagnitude / topMagnitude));
	}
	else if (colorScheme == ColorScheme::IterationCount)
	{
		pt->finalMagnitude = (pt->finalMagnitude - iterationsMin) / (iterationsMax - (double)iterationsMin);

		// amplify some ranges as needed
		// some square roots can push smaller values higher while still keeping the max at boundary 1
		pt->finalMagnitude = sqrt(sqrt(sqrt(pt->finalMagnitude)));
	}

}


FractalAlgorithm::FractalAlgorithm(Zoom * zoom, ColorPalette * palette):
	m_zoom(zoom),
	m_color(palette)
{
}

void FractalAlgorithm::CalculatePoint(Result * pt)
{
	
	switch (algoType) {
			case AlgorithmType::ShowColorPalette : ShowColorPalette(pt);
				break;
			case AlgorithmType::MandelBrot: MandelBrotSet(pt);
				break;
	}

}
