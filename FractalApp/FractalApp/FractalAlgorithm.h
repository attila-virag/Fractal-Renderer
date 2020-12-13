#pragma once
#include "Zoom.h"
#include "ColorPalette.h"
#include <complex>
#include <memory>
#include <unordered_map>

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

class DLL_EXPORT FractalAlgorithm {

	void ShowColorPalette(Result* pt);

	void MandelBrotSet(Result * pt);

	const double PI = 3.14159;

	double topMagnitude = 0; // this is the max of whatever quantity we are tracking in this run, helps to normalize data 

	double NormalizeIterations(int iterations, std::complex<double> Zn, std::complex<double> C);

	//double GetHistogram

	int DoIterations(std::complex<double>& z, std::complex<double>& c, bool& escaped, int iterationLimit, double power);

	bool CheckIfPointIsInside(double x, double y);

public:

	double m_pow{ 2 };

	Zoom* m_zoom;

	ColorPalette* m_color;

	std::unordered_map<int, int> histogramCount; // this map holds the counts for the histogram method, <escape iteration number, number of pixels in buket>

	double max_mag = 0; // the maximum magnitude calculated
	double min_mag = 0; // the minimum magnitude calculated (should be 1 or 0)

	int iterationsMin = 0; // out of the points that escaped
	int iterationsMax = 0; // out of the points that escaped

	AlgorithmType algoType = AlgorithmType::MandelBrot;
	ColorScheme colorScheme = ColorScheme::IterationCount;

	FractalAlgorithm(Zoom *zoom, ColorPalette * palette);

	// this method will run in multiple threads
	void CalculatePoint(Result* pt);

	// for each point normalizes the magnitude value based on whatever smoothing methodology applied
	void GetNormalization(Result * pt);

	
};
