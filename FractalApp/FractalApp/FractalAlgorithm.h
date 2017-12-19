#pragma once
#include "Zoom.h"
#include "ColorPalette.h"

struct Result {
	int x_coordinate;
	int y_coordinate;

	double magnitude; // could mean differnet things depending on the algorithm but should be between 0-1 before passed into the palette
};

typedef void(*FractalFunction)(Result *r);

enum class AlgorithmType {

	ShowColorPalette,
	MandelBrot,
	JuliaSet

};

class FractalAlgorithm {

	void ShowColorPalette(Result* pt);

public:
	Zoom m_zoom;

	ColorPalette m_color;

	AlgorithmType algoType = AlgorithmType::ShowColorPalette;

	FractalAlgorithm(Zoom &zoom, ColorPalette & palette);

	// this method will run in multiple threads
	void CalculatePoint(Result* pt);

	void GetNormalization(Result * pt);

	
};
