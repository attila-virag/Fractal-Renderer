#pragma once

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
	Location zoom;
	ColorPalette color;

	color.GenerateRandomColorPalette();

	color.SavePaletteToFile("test");

	FractalAlgorithm alg(&zoom, &color);
	alg.algoType == AlgorithmType::ShowColorPalette;

	CalculationProcessor proc(&alg, 8);

	proc.CreatePicture("testPicture");

	color.LoadPaletteFromFile("test");
}

void TestLoadPaletteFromFile() {
	ColorPalette color;
	color.LoadPaletteFromFile("test");
}

void TestCreatePaletterBitmap() {

	Location zoom;
	ColorPalette color;
	FractalAlgorithm alg(&zoom, &color);

	alg.algoType = AlgorithmType::ShowColorPalette;

	CalculationProcessor proc(&alg, 8);
	
	proc.CreatePicture("testPicture");

}

void TestCreateMandelBrot() {
	Location zoom;

	//zoom.ResetZoom(-0.5, 0, 1.5, 1000);

	zoom.ResetLocation(-1.7497219297, -0.000029016647, 0.001, 500);

	ColorPalette color;
	color.LoadPaletteFromFile("test");
	FractalAlgorithm alg(&zoom, &color);

	alg.algoType = AlgorithmType::MandelBrot;
	alg.colorScheme = ColorScheme::IterationCount;

	CalculationProcessor proc(&alg, 8);

	proc.CreatePicture("testPicture");

}
