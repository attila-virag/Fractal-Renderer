#include "stdafx.h"
#include "FractalAlgorithm.h"
#include <complex>

using std::complex;



void FractalAlgorithm::ShowColorPalette(Result * pt)
{
	pt->magnitude = pt->x_coordinate / (double)m_zoom->pixels;
}

void FractalAlgorithm::MandelBrotSet(Result * pt)
{
	double x_point = m_zoom->x_min + pt->x_coordinate*m_zoom->x_increment;
	double y_point = m_zoom->y_min + pt->y_coordinate*m_zoom->y_increment;

	int iteration = 0;
	double normalizedIteration = 0;
	double escapeAngle = 0;
	double magnitude = 0;

	complex<double> z = 0;
	complex<double> c(x_point, y_point);

	pt->escaped = false;

	while (iteration < m_zoom->recommendedIterations) {
		z = std::pow(z, m_pow) + c;

		 magnitude = abs(z);

		if (magnitude > 4) {

			escapeAngle = arg(z) / 2*PI;

			normalizedIteration = NormalizeIterations(iteration, z, c);

			if (topMagnitude < magnitude)
				topMagnitude = magnitude;

			pt->escaped = true;

			break;
		}
		iteration++;
	}

	switch(colorScheme) {
		case ColorScheme::EscapeAngle:
			pt->magnitude = escapeAngle;
			break;
		case ColorScheme::IterationCount :
			pt->magnitude = normalizedIteration;
			break;
		case ColorScheme::FinalMagnitude:
			pt->magnitude = magnitude;
	}

	
}

double FractalAlgorithm::NormalizeIterations(int iterations, complex<double> Zn, complex<double> C)
{
	int extraIter = 0;

	for (; extraIter < 3; extraIter++) {
		Zn = std::pow(Zn, m_pow) + C;
	}

	auto nd = log(log(abs(Zn))) / log(m_pow); // log((log(abs(Zn)) / log(2))) / log(m_pow);

	auto sn = iterations+extraIter+1 - nd;

	if (sn < min_mag)
		min_mag = sn;
	if (sn > max_mag)
		max_mag = sn;

	return sn;
}

void FractalAlgorithm::GetNormalization(Result* pt) {

	if (colorScheme == ColorScheme::FinalMagnitude)
	{
		pt->magnitude = sqrt(sqrt(pt->magnitude / topMagnitude));
	}
	else if (colorScheme == ColorScheme::IterationCount)
	{
		pt->magnitude = (pt->magnitude - min_mag) / (max_mag - min_mag);
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
