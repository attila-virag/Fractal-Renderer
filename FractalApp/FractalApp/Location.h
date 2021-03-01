#pragma once
#include "Exports.h"
#include <math.h>
#include <iostream>
#include <fstream>

enum class DLL_EXPORT AlgorithmType {

	ShowColorPalette,
	Polynomial,
	Mandelbrot,
	JuliaSet

};


// this describes our x and y coordinates and how much we are "zoomed in"
// also how many pixels in the image
class DLL_EXPORT Location {

	// completely arbitrary guesstimate function, can be changed
	int GetRecommendedIterations() {
		// Where did this formula come from?
		if (recommendedIterations == 0) {
			double scale = pixels / (2 * zoom);
			return (int)(200 * pow(log(scale), 1.25));
		}
		else {
			return recommendedIterations;
		}
	}

	int GetAlgoType() {
		int algo = 0;
		switch (algoType) {
		case AlgorithmType::ShowColorPalette: algo = 0;
			break;
		case AlgorithmType::Polynomial: algo = 1;
			break;
		case AlgorithmType::Mandelbrot: algo = 2;
			break;
		case AlgorithmType::JuliaSet: algo = 3;
			break;

		default:
			break;
		}
		return algo;
	}

	void SetAlgoType(int algo) {
		switch (algo) {
		case 0: algoType = AlgorithmType::ShowColorPalette;
			break;
		case 1: algoType = AlgorithmType::Polynomial;
			break;
		case 2: algoType = AlgorithmType::Mandelbrot;
			break;
		case 3: algoType = AlgorithmType::JuliaSet;
			break;

		default:
			break;
		}
	}

public:

	double x_center{ -1 };
	double y_center{ 0 };

	// scale on either side of center
	double zoom{ 1.5 };

	double x_min{ 0 };
	double x_max{ 0 };
	double y_min{ 0 };
	double y_max{ 0 };

	double x_increment{ 0 };
	double y_increment{ 0 };
	// will always create a square picture
	int pixels{ 500 };

	// guesstimate based on zoom level and pizels etc
	// iterations may need to increase the more zoomed in to generate accurate edges
	int recommendedIterations{ 0 };


	AlgorithmType algoType = AlgorithmType::Mandelbrot;

	bool IsTransposeOnly(double newZoom) {
		double prev = abs(zoom);
		double n = abs(newZoom);
		double diff = abs(n - prev);
		return diff < 0.00001;
	}

	int GetXPixelOffset(double new_X_center) {
		double offset = new_X_center - x_center;
		// now convert it to how many pixels
		int dX = (int)(offset / x_increment);
		return dX;
	}

	int GetYPixelOffset(double new_Y_center) {
		double offset = new_Y_center - y_center;
		// now convert it to how many pixels
		int dY = (int)(offset / y_increment);
		return dY;
	}

	Location() {

		x_min = x_center - zoom;
		x_max = x_center + zoom;
		y_min = y_center - zoom;
		y_max = y_center + zoom;
		x_increment = (x_max - x_min) / (double)pixels;
		y_increment = (y_max - y_min) / (double)pixels;

		recommendedIterations = GetRecommendedIterations();
	}

	Location(double x, double y, double zoom, int pixels, AlgorithmType type = AlgorithmType::Mandelbrot, int iterations = 0):
		x_center(x),
		y_center(y),
		zoom(zoom),
		pixels(pixels)
	{
		x_min = x_center - zoom;
		x_max = x_center + zoom;
		y_min = y_center - zoom;
		y_max = y_center + zoom;

		recommendedIterations = iterations;

		recommendedIterations = GetRecommendedIterations();
		x_increment = (x_max - x_min) / (double)pixels;
		y_increment = (y_max - y_min) / (double)pixels;

		algoType = type;
	}

	void ResetLocation()
	{
		ResetLocation(x_center, y_center, zoom, pixels);
	}

	// call this whenever any of the parameters change
	void ResetLocation(double xCenter, double yCenter, double zoom, int pls) {

		x_center = xCenter;
		y_center = yCenter;
		this->zoom = zoom;

		x_min = x_center - zoom;
		x_max = x_center + zoom;
		y_min = y_center - zoom;
		y_max = y_center + zoom;

		pixels = pls;

		x_increment = (x_max - x_min) / (double)pixels;
		y_increment = (y_max - y_min) / (double)pixels;

		recommendedIterations = GetRecommendedIterations();
	}

	bool LoadLocationDataFromFile(std::string fileName)
	{
		std::ifstream inFile;

		std::string filePath = workingDirectory + "locations\\"+ fileName + ".bin";

		inFile.open(filePath, std::ios::in | std::ios::binary);

		if (inFile.is_open()) {

			Deserialize(inFile);
			inFile.close();
			return true;
		}
		return false;
	}

	bool Serialize(std::ofstream& outFile)
	{
		if (outFile.is_open())
		{
			int algo = GetAlgoType();

			outFile.write((char*)&x_center, sizeof(double));
			outFile.write((char*)&y_center, sizeof(double));
			outFile.write((char*)&zoom, sizeof(double));
			outFile.write((char*)&pixels, sizeof(int));
			outFile.write((char*)&algo, sizeof(int));
			return true;
		}
		return false;
	}

	bool Deserialize(std::ifstream& inFile)
	{
		if (inFile.is_open()) {
			double xCenter = 0;
			double yCenter = 0;
			double zoom = 0;
			int pixels = 0;
			int algo = 0;

			inFile.read((char*)&xCenter, sizeof(double));
			inFile.read((char*)&yCenter, sizeof(double));
			inFile.read((char*)&zoom, sizeof(double));
			inFile.read((char*)&pixels, sizeof(int));
			inFile.read((char*)&algo, sizeof(int));
			SetAlgoType(algo);
			ResetLocation(xCenter, yCenter, zoom, pixels);

			return true;
		}
		else return false;
	}

	bool SaveLocationDataToFile(std::string fileName)
	{
		std::ofstream outFile;

		std::string filePath = workingDirectory + "locations\\" + fileName + ".bin";

		outFile.open(filePath, std::ios::out | std::ios::binary);

		if (outFile.is_open()) {

			Serialize(outFile);

			outFile.close();
		}
		else
		{
			return false;
		}
		return true;
	}
};