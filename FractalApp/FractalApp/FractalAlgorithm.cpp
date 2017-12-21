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

			escapeAngle = arg(z) / PI;

			normalizedIteration = sqrt((iteration / (double)m_zoom->recommendedIterations));

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

void FractalAlgorithm::GetNormalization(Result* pt) {

	if (colorScheme == ColorScheme::FinalMagnitude)
	{
		pt->magnitude = sqrt(pt->magnitude / topMagnitude);
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
