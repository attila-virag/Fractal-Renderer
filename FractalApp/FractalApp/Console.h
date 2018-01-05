#pragma once
// The console interface for now

#include <iostream>
#include <string>
#include "ColorPalette.h"
#include "Zoom.h"
#include "FractalAlgorithm.h"
#include "CalculationProcessor.h"

using std::cout;
using std::cin;

void SetAlgorithm(CalculationProcessor * proc) {

	int option = 1;

	cout << "Please enter an algorithm option: " << std::endl;
	cout << "1 = Mandelbrot , 2 = Julia Set " << std::endl;
	cin >> option;

	switch (option) {

	case 1: proc->m_algo->algoType = AlgorithmType::MandelBrot; break;
	case 2: proc->m_algo->algoType = AlgorithmType::JuliaSet; break;
	default: proc->m_algo->algoType = AlgorithmType::MandelBrot; break;
	}


	cout << "Please enter a coloring algorithm option: " << std::endl;
	cout << "1 = Iteration Count , 2 = Histogram Count, 3 = Escape Angle, 4 = Final magnitude " << std::endl;
	cin >> option;

	switch (option) {

	case 1: proc->m_algo->colorScheme = ColorScheme::IterationCount; break;
	case 2: proc->m_algo->colorScheme = ColorScheme::HistogramCount; break;
	case 3: proc->m_algo->colorScheme = ColorScheme::EscapeAngle; break;
	case 4: proc->m_algo->colorScheme = ColorScheme::FinalMagnitude; break;
	default: proc->m_algo->colorScheme = ColorScheme::IterationCount; break;
	}
}

void LoadColorPalette(CalculationProcessor * proc) {

	std::string fileName;
	cout << "Please enter the color palette file Name: " << std::endl;
	cin >> fileName;

	if (!proc->m_algo->m_color->LoadPaletteFromFile(fileName)) {
		cout << "File not found, using default palette. " << std::endl;
	}
	else {
		cout << "Palette Loaded. " << std::endl;
	}
}
void GenerateRandomPalette(CalculationProcessor * proc) {

	int good = 0;

	auto algoType = proc->m_algo->algoType;
	proc->m_algo->algoType = AlgorithmType::ShowColorPalette;

	double x_center = proc->m_algo->m_zoom->x_center;
	double y_center = proc->m_algo->m_zoom->y_center;
	double zoom = proc->m_algo->m_zoom->zoom;

	proc->m_algo->m_zoom->ResetZoom(.5, .5, .5, proc->m_algo->m_zoom->pixels);

	while (good == 0) {

		proc->m_algo->m_color->GenerateRandomColorPalette();

		proc->CreatePicture("randomColorPalette");

		cout << "Please see the file randomColorPalette.bmp and enter 1 to keep palette or enter 0 to generate another random palette. " << std::endl;
		cin >> good;

	}

	proc->m_algo->algoType = algoType;
	proc->m_algo->m_zoom->ResetZoom(x_center, y_center, zoom, proc->m_algo->m_zoom->pixels);

}

void SavePalette(CalculationProcessor * proc) {
	std::string name;
	cout << "Please enter a file name without extension. " << std::endl;
	cin >> name;

	proc->m_algo->m_color->SavePaletteToFile(name);
}

void SetPalette(CalculationProcessor * proc) {

	int option = 1;

	cout << "Please enter a color palette option: " << std::endl;
	cout << "1 = Load From File , 2 = Generate Random, 3 = Save Current Palette " << std::endl;
	cin >> option;

	switch (option) {

	case 1: LoadColorPalette(proc); break;
	case 2: GenerateRandomPalette(proc); break;
	case 3: SavePalette(proc); break;
		default: ;
	}

}
void SetZoom(CalculationProcessor * proc) {
	double x_center, y_center, zoom;
	int pixels;

	cout << "Please enter x - coordinate: " << std::endl;
	cin >> x_center;
	cout << "Please enter y - coordinate: " << std::endl;
	cin >> y_center;
	cout << "Please enter zoom scale: " << std::endl;
	cin >> zoom;
	cout << "Please enter pixels (larger than 2000 will take long to generate): " << std::endl;
	cin >> pixels;

	proc->m_algo->m_zoom->ResetZoom(x_center, y_center, zoom, pixels);

}
void RenderImage(CalculationProcessor * proc) {

	std::string name = "";

	cout << "Enter a filename or leave blank for default " << std::endl;
	cin >> name;

	if (name.length() == 0)
		name = "fractal";

	int done = 0;

	while (done == 0) {

		int preview = 0;
		cout << "Enter 1 to generate 500X500 preview : " << std::endl;
		cin >> preview;

		if (preview == 1) {
			auto pixels = proc->m_algo->m_zoom->pixels;

			proc->m_algo->m_zoom->pixels = 500;

			proc->CreatePicture(name + "_preview");

			proc->m_algo->m_zoom->pixels = pixels;
		}

		cout << "Enter 1 to generate full image, enter 2 to change coordinates: " << std::endl;
		cin >> done;

		if (done == 1) {
			proc->CreatePicture(name);
		}
		else if (done == 2) {
			SetZoom(proc);
		}

		cout << "Enter 0 to continue or enter 1 to return to main menu" << std::endl;
		cin >> done;

	}

}


void Console() {

	Zoom zoom;
	ColorPalette color;
	FractalAlgorithm alg(&zoom, &color);

	CalculationProcessor proc(&alg);

	while (true) {

		int option = 0;

		cout << "Please enter an option: " << std::endl;
		cout << "1 = Set algorithm , 2 = Set Palette, 3 = Set Zoom, 4 = Render Image, 10 = exit" << std::endl;
		cin >> option;

		switch (option) {

			case 10: break;
			case 1: SetAlgorithm(&proc); continue;
			case 2: SetPalette(&proc); continue;
			case 3: SetZoom(&proc); continue;
			case 4: RenderImage(&proc); continue;

			default: continue;

		}

	}
}