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

void LoadLocation(void* proc)
{
	std::string name;
	cout << "Please enter a file name without extension. " << std::endl;
	cin >> name;

	LoadLocationFromFile(proc, name.c_str());
}

void SaveLocation(void* proc)
{
	std::string name;
	cout << "Please enter a file name without extension. " << std::endl;
	cin >> name;

	SaveLocationToFile(proc, name.c_str());
}

void SetZoom(void* proc) {
	double x_center, y_center, zoom;
	int pixels;

	int option = 1;
	while (option != 10) {

		x_center = Get_X_Value(proc);
		y_center = Get_Y_Value(proc);
		zoom = Get_Zoom(proc);
		pixels = Get_Pixels(proc);

		cout << "Current coordinates: x: " << x_center << ", y: " << y_center << ", zoom scale: " << zoom << ", pixels: " << pixels << std::endl;
		cout << "Please enter a color palette option: " << std::endl;
		cout << "1 = Change x, 2 = Change y, 3 = Change zoom scale, 4 = Change pixels, 5 = Move Left 10%, 6 = Move Right 10%, 7 = Move Down 10%, 8 = Move Up 10%, 10 = exit" << std::endl;
		cin >> option;

		switch (option) {

		case 1:
		{
			cout << "Please enter x - coordinate: " << std::endl;
			cin >> x_center;
			Set_X_Value(proc, x_center);
			break;
		}
		case 2:
		{
			cout << "Please enter y - coordinate: " << std::endl;
			cin >> y_center;
			Set_Y_Value(proc, y_center);
			break;
		}
		case 3:
		{
			cout << "Please enter zoom scale: " << std::endl;
			cin >> zoom;
			Set_Zoom(proc, zoom);
			break;
		}
		case 4:
		{
			cout << "Please enter pixels: " << std::endl;
			cin >> pixels;
			Set_Pixels(proc, pixels);
			break;
		}
		case 5:
		{
			Set_X_Value(proc, x_center - (Get_Zoom(proc) * 0.1));
			break;
		}
		case 6:
		{
			Set_X_Value(proc, x_center + (Get_Zoom(proc) * 0.1));
			break;
		}
		case 7:
		{
			Set_Y_Value(proc, y_center - (Get_Zoom(proc) * 0.1));
			break;
		}
		case 8:
		{
			Set_Y_Value(proc, y_center + (Get_Zoom(proc) * 0.1));
			break;
		}
		default:
		{
			Reset_Zoom(proc);
			break;
		}
		}
	}
}



void SetLocation(void * proc)
{
	int option = 1;

	cout << "Please enter a location setting option: " << std::endl;
	cout << "1 = Load From File , 2 = Enter Location, 3 = Save Current Location Info " << std::endl;
	cin >> option;

	switch (option) {
		case 1: LoadLocation(proc); break;
		case 2: SetZoom(proc); break;
		case 3: SaveLocation(proc); break;
		default:;
	}
} 


void RenderImage(void * proc) {

	std::string name = "";

	cout << "Enter a filename or leave blank for default " << std::endl;
	cin >> name;

	if (name.length() == 0)
		name = "fractal";

	int done = 0;

	while (done == 0) {

		int option = 1;

		cout << "Please enter a generation option: " << std::endl;
		cout << "1 = Generate 500x500 Preview , 2 = Write Results to File, 3 = Generate image from File, 4 = Generate full image " << std::endl;
		cin >> option;

		switch (option) {
		case 1: 
		{
			GeneratePreview(proc, name.c_str());
			break;
		}
		case 2: 
		{

			// save all iteration data to a file
			break;
		}
		case 3:
		{
			// load iteration data from file and generate image
			break;
		}
		case 4:
		{
			// generates and and image in one
			break;
		}
		default:;
		}

		int preview = 0;
		cout << "Enter 1 to generate 500X500 preview : " << std::endl;
		cin >> preview;

		if (preview == 1) {
			GeneratePreview(proc, name.c_str());
		}

		cout << "Enter 1 to generate full image, enter 2 to change coordinates: " << std::endl;
		cin >> done;

		if (done == 1) {
			GenerateImage(proc, name.c_str());
		}
		else if (done == 2) {
			SetLocation(proc);
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
			cout << "1 = Set algorithm , 2 = Set Palette, 3 = Set Zoom, 4 = Render Image, 10 = save and exit" << std::endl;
			cout << "=>";
			cin >> option;

			switch (option) {

			case 1: SetAlgorithm(instPtr); continue;
			case 2: SetPalette(instPtr); continue;
			case 3: SetLocation(instPtr); continue;
			case 4: RenderImage(instPtr); continue;

			case 10: {
				CleanUp(&instPtr);
				break;
			}
			default: continue;
			}

		}
	}
}