#pragma once
#include "Zoom.h"
#include "ColorPalette.h"
#include <complex>
#include <memory>
#include <iostream>
#include <unordered_map>

struct Result {

	int version = 1;

	// coordinates are pixels
	int x_pixel = 0;
	int y_pixel = 0;

	int finalIteration = 0;

	double finalAngle = 0;

	double finalMagnitude = 0;

	bool escaped = true;

	bool AreEqual(const Result& other)
	{
		return (this->version == other.version && this->x_pixel == other.x_pixel && this->y_pixel == other.y_pixel && this->finalIteration == other.finalIteration &&
			this->finalAngle == other.finalAngle && this->finalMagnitude == other.finalMagnitude);
	}

	void Serialize(std::ofstream& outFile)
	{
		outFile << "X: " << std::endl;

		outFile << x_pixel << std::endl;

		outFile << "Y: " << std::endl;

		outFile << y_pixel << std::endl;

		outFile << "finalIteration: " << std::endl;

		outFile << finalIteration << std::endl;

		outFile << "finalAngle: " << std::endl;

		outFile << finalAngle << std::endl;
		
		outFile << "finalMagnitude: " << std::endl;

		outFile << finalMagnitude << std::endl;

		outFile << "escaped: " << std::endl;

		outFile << escaped << std::endl;

	}

	void Deserialize(std::ifstream& inFile)
	{
		std::string line;
		std::getline(inFile, line);

		inFile >> x_pixel;

		std::getline(inFile, line);
		std::getline(inFile, line);

		inFile >> y_pixel;

		std::getline(inFile, line);
		std::getline(inFile, line);

		inFile >> finalIteration;

		std::getline(inFile, line);
		std::getline(inFile, line);

		inFile >> finalAngle;

		std::getline(inFile, line);
		std::getline(inFile, line);

		inFile >> finalMagnitude;

		std::getline(inFile, line);
		std::getline(inFile, line);

		inFile >> escaped;

		std::getline(inFile, line);
	}

};

typedef void(*FractalFunction)(Result *r);

enum class AlgorithmType {

	ShowColorPalette,
	MandelBrot,
	JuliaSet

};

enum class ColorScheme {

	IterationCount,
	HistogramCount,
	EscapeAngle,
	FinalMagnitude
};

class FractalAlgorithm {

	void ShowColorPalette(Result* pt);

	void MandelBrotSet(Result * pt);

	const double PI = 3.14159;

	double topMagnitude = 0; // this is the max of whatever quantity we are tracking in this run, helps to normalize data 

	double NormalizeIterations(int iterations, std::complex<double> Zn, std::complex<double> C);

	//double GetHistogram

	int DoIterations(std::complex<double>& z, std::complex<double>& c, bool& escaped, int iterationLimit, double power);

	bool CheckIfPointIsInside(double x, double y);

	std::unordered_map<int, int> histogramCount; // this map holds the counts for the histogram method, <escape iteration number, number of pixels in buket>

public:

	double m_pow{ 2 };

	Zoom* m_zoom;

	ColorPalette* m_color;

	double max_mag = 0; // the maximum magnitude calculated
	double min_mag = 0; // the minimum magnitude calculated (should be 1 or 0)

	int iterationsMin = 0; // out of the points that escaped
	int iterationsMax = 0; // out of the points that escaped

	AlgorithmType algoType = AlgorithmType::MandelBrot;
	ColorScheme colorScheme = ColorScheme::IterationCount;

	DLL_EXPORT FractalAlgorithm(Zoom *zoom, ColorPalette * palette);

	// this method will run in multiple threads
	void DLL_EXPORT CalculatePoint(Result* pt);

	// for each point normalizes the magnitude value based on whatever smoothing methodology applied
	void DLL_EXPORT GetNormalization(Result * pt);

	
};
