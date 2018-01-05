#pragma once
#include "Zoom.h"
#include "ColorPalette.h"
#include <complex>
#include <memory>

struct Result {
	int x_coordinate;
	int y_coordinate;

	double magnitude; // could mean differnet things depending on the algorithm but should be between 0-1 before passed into the palette

	bool escaped;
};

typedef void(*FractalFunction)(Result *r);

enum class AlgorithmType {

	ShowColorPalette,
	MandelBrot,
	JuliaSet

};

enum class ColorScheme {

	IterationCount,
	HistogramCount,
	EscapeAngle,
	FinalMagnitude
};

class FractalAlgorithm {

	void ShowColorPalette(Result* pt);

	void MandelBrotSet(Result * pt);

	const double PI = 3.14159;

	double topMagnitude = 0;

	double NormalizeIterations(int iterations, std::complex<double> Zn, std::complex<double> C);

	//double GetHistogram

public:

	double m_pow{ 2 };

	std::unique_ptr<Zoom> m_zoom;

	std::unique_ptr<ColorPalette> m_color;

	double max_mag = 0;
	double min_mag = 0;

	AlgorithmType algoType = AlgorithmType::MandelBrot;
	ColorScheme colorScheme = ColorScheme::IterationCount;

	FractalAlgorithm(Zoom *zoom, ColorPalette * palette);

	// this method will run in multiple threads
	void CalculatePoint(Result* pt);

	void GetNormalization(Result * pt);

	
};
