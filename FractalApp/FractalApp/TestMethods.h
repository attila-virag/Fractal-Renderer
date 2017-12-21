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

void TestCreateRandomPaletteAndSave() {
	Zoom zoom;
	ColorPalette color;

	color.GenerateRandomColorPalette();

	color.SavePaletteToFile();

	FractalAlgorithm alg(&zoom, &color);
	alg.algoType == AlgorithmType::ShowColorPalette;

	CalculationProcessor proc(&alg, 8);

	proc.CreatePicture();

	color.LoadPaletteFromFile();
}

void TestLoadPaletteFromFile() {
	ColorPalette color;
	color.LoadPaletteFromFile();
}

void TestCreatePaletterBitmap() {

	Zoom zoom;
	ColorPalette color;
	FractalAlgorithm alg(&zoom, &color);

	alg.algoType = AlgorithmType::ShowColorPalette;

	CalculationProcessor proc(&alg, 8);
	
	proc.CreatePicture();

}

void TestCreateMandelBrot() {
	Zoom zoom;

	zoom.ResetZoom(-0.5, 0, 1.5, 1000);

	ColorPalette color;
	FractalAlgorithm alg(&zoom, &color);

	alg.algoType = AlgorithmType::MandelBrot;
	alg.colorScheme = ColorScheme::FinalMagnitude;

	CalculationProcessor proc(&alg, 8);

	proc.CreatePicture();

}
