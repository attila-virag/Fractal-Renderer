#pragma once
// The console interface for now

#include <iostream>
#include <string>

#include "FractalEngine.h"

using std::cout;
using std::cin;

void SetAlgorithm(void * instPtr) {

	int option = 1;

	cout << "Please enter an algorithm option: " << std::endl;
	cout << "1 = Mandelbrot , 2 = Julia Set, 10 = return to main menu " << std::endl;
	cout << "=>";
	cin >> option;

	if (option == 10) return;

	SetAlgorithm(instPtr, option);

	cout << "Algorithm was set to option: " << GetAlgorithm(instPtr) << std::endl;

	cout << "Please enter a coloring algorithm option: " << std::endl;
	cout << "1 = Iteration Count , 2 = Histogram Count, 3 = Escape Angle, 4 = Final magnitude, 10 = return to main menu  " << std::endl;
	cout << "=>";
	cin >> option;

	if (option == 10) return;

	SetColorAlgorithm(instPtr, option);

	cout << "Color algorithm was set to option: " << GetColorAlgorithm(instPtr) << std::endl;
}

void LoadColorPalette(void * instPtr) {

	std::string fileName;
	cout << "Please enter the color palette file Name: " << std::endl;
	cin >> fileName;

	if (!LoadColorPaletteFromFile(instPtr, fileName.c_str())) {
		cout << "File not found, using default palette. " << std::endl;
	}
	else {
		cout << "Palette Loaded. " << std::endl;
	}
}
void GenerateRandomPalette(void* instPtr) {

	int good = 0;

	while (good == 0) {

		GenerateRandomColorPalette(instPtr);

		cout << "Please see the file randomColorPalette.bmp and enter 1 to keep palette or enter 0 to generate another random palette. " << std::endl;
		cin >> good;

	}
}

void SavePalette(void * instPtr) {
	std::string name;
	cout << "Please enter a file name without extension. " << std::endl;
	cin >> name;

	SaveColorPalette(instPtr, name.c_str());
}

void SetPalette(void * proc) {

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
void SetZoom(void * proc) {
	double x_center, y_center, zoom;
	int pixels;

	// has to be fixed to take each value separately

	//cout << "Please enter x - coordinate: " << std::endl;
	//cin >> x_center;
	//cout << "Please enter y - coordinate: " << std::endl;
	//cin >> y_center;
	//cout << "Please enter zoom scale: " << std::endl;
	//cin >> zoom;
	//cout << "Please enter pixels (larger than 2000 will take long to generate): " << std::endl;
	//cin >> pixels;

	//proc->m_algo->m_zoom->ResetZoom(x_center, y_center, zoom, pixels);

}
void RenderImage(void * proc) {

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
			//auto pixels = proc->m_algo->m_zoom->pixels;

			//proc->m_algo->m_zoom->pixels = 500;

			//proc->CreatePicture(name + "_preview");

			//proc->m_algo->m_zoom->pixels = pixels;
		}

		cout << "Enter 1 to generate full image, enter 2 to change coordinates: " << std::endl;
		cin >> done;

		if (done == 1) {
			//proc->CreatePicture(name);
		}
		else if (done == 2) {
			//SetZoom(proc);
		}

		cout << "Enter 0 to continue or enter 1 to return to main menu" << std::endl;
		cin >> done;

	}

}


void Console() {

	void* instPtr = nullptr;

	if (CreateFractalEngine(&instPtr)) {

		while (true) {

			int option = 0;

			cout << "Please enter an option: " << std::endl;
			cout << "1 = Set algorithm , 2 = Set Palette, 3 = Set Zoom, 4 = Render Image, 10 = exit" << std::endl;
			cout << "=>";
			cin >> option;

			switch (option) {

			case 10: break;
			case 1: SetAlgorithm(instPtr); continue;
			case 2: SetPalette(instPtr); continue;
			case 3: SetZoom(instPtr); continue;
			case 4: RenderImage(instPtr); continue;

			default: continue;

			}

		}
	}

	CleanUp(&instPtr);
}