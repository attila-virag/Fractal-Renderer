#pragma once

// Test Functions - just to exersize code

#include "ColorPalette.h"
#include "Zoom.h"
#include "FractalAlgorithm.h"
#include "CalculationProcessor.h"


void TestRunColorInterpolate() {

	double mag = .333;

	int red, green, blue;

	ColorPalette color;

	color.GetColor(mag, red, green, blue);

	mag = .7777;

	color.GetColor(mag, red, green, blue);
}

void TestCreatePaletterBitmap() {

	Zoom zoom;
	ColorPalette color;
	FractalAlgorithm alg(zoom, color);

	alg.algoType = AlgorithmType::ShowColorPalette;

	CalculationProcessor proc(&alg, 8);
	
	proc.CreatePicture();

}
