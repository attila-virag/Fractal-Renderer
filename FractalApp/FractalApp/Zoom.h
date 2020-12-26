#pragma once
#include "Exports.h"
#include <math.h>
#include <iostream>
#include <fstream>

// this describes our x and y coordinates and how much we are "zoomed in"
// also how many pixels in the image
class DLL_EXPORT Zoom {

	// completely arbitrary guesstimate function, can be changed
	int GetRecommendedIterations() {
		// Where did this formula come from?
		double scale = pixels / (2 * zoom);
		return (int)(200 * pow(log(scale), 1.25));
	}

public:

	double x_center{ -1 };
	double y_center{ 0 };

	// scale on either side of center
	double zoom{ 1.5 };

	Zoom() {

		x_min = x_center - zoom;
		x_max = x_center + zoom;
		y_min = y_center - zoom;
		y_max = y_center + zoom;
		x_increment = (x_max - x_min) / (double)pixels;
		y_increment = (y_max - y_min) / (double)pixels;

		recommendedIterations = GetRecommendedIterations();
	}

	Zoom(double x_center, double y_center, double zoom, int pixels):
		x_center(x_center),
		y_center(y_center),
		zoom(zoom),
		pixels(pixels)
	{
		x_min = x_center - zoom;
		x_max = x_center + zoom;
		y_min = y_center - zoom;
		y_max = y_center + zoom;

		recommendedIterations = GetRecommendedIterations();
		x_increment = (x_max - x_min) / (double)pixels;
		y_increment = (y_max - y_min) / (double)pixels;
	}

	void ResetZoom()
	{
		ResetZoom(x_center, y_center, zoom, pixels);
	}

	// call this whenever any of the parameters change
	void ResetZoom(double xCenter, double yCenter, double zoom, int pls) {

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
	int recommendedIterations{ 100 };

	bool LoadZoomDataFromFile(std::string fileName)
	{
		std::ifstream inFile;

		std::string filePath = workingDirectory + "locations\\"+ fileName + ".txt";

		inFile.open(filePath);

		std::string line;

		double xCenter = 0;
		double yCenter = 0;
		double zoom = 0;
		int pixels = 0;

		if (inFile.is_open()) {

			Deserialize(inFile);
		}
		else return false;
	}

	bool Serialize(std::ofstream& outFile)
	{
		if (outFile.is_open())
		{
			outFile << "X Center: " << std::endl;

			outFile << x_center << std::endl;

			outFile << "Y Center: " << std::endl;

			outFile << y_center << std::endl;

			outFile << "Zoom: " << std::endl;

			outFile << zoom << std::endl;

			outFile << "Pixels: " << std::endl;

			outFile << pixels << std::endl;
			return true;
		}
		return false;
	}

	bool Deserialize(std::ifstream& inFile)
	{
		if (inFile.is_open()) {

			std::string line;

			double xCenter = 0;
			double yCenter = 0;
			double zoom = 0;
			int pixels = 0;

			getline(inFile, line);

			inFile >> xCenter;

			getline(inFile, line);
			getline(inFile, line);

			inFile >> yCenter;

			getline(inFile, line);
			getline(inFile, line);

			inFile >> zoom;

			getline(inFile, line);
			getline(inFile, line);

			inFile >> pixels;

			inFile.close();

			ResetZoom(xCenter, yCenter, zoom, pixels);

			return true;
		}
		else return false;
	}

	bool SaveZoomDataToFile(std::string fileName)
	{
		std::ofstream outFile;

		std::string filePath = workingDirectory + "locations\\" + fileName + ".txt";

		outFile.open(filePath);

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