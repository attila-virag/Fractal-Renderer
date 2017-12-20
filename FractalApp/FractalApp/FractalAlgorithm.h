#pragma once
#include "Zoom.h"
#include "ColorPalette.h"

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
	EscapeAngle,
	FinalMagnitude
};

class FractalAlgorithm {

	void ShowColorPalette(Result* pt);

	void MandelBrotSet(Result * pt);

	const double PI = 3.14159;

	double topMagnitude = 0;

public:

	double m_pow{ 2 };

	Zoom m_zoom;

	ColorPalette m_color;

	AlgorithmType algoType = AlgorithmType::ShowColorPalette;
	ColorScheme colorScheme = ColorScheme::EscapeAngle;

	FractalAlgorithm(Zoom &zoom, ColorPalette & palette);

	// this method will run in multiple threads
	void CalculatePoint(Result* pt);

	void GetNormalization(Result * pt);

	
};
