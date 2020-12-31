#pragma once
#include "Zoom.h"
#include "ColorPalette.h"
#include <complex>
#include <memory>
#include <iostream>
#include <unordered_map>

// the result struct holds the outcome of the algorithms applied to each pixel
// 
struct Result {

	int version = 1;

	// coordinates are pixels
	int x_pixel = 0;
	int y_pixel = 0;

	// Version 1:
	int int1 = 0;
	int int2 = 0;
	double double1 = 0;
	double double2 = 0;
	bool escaped = true;

	bool AreEqual(const Result& other)
	{
		if (this->version == 1) {
			return (this->version == other.version && this->x_pixel == other.x_pixel && this->y_pixel == other.y_pixel && this->int1 == other.int1 &&
				this->int2 == other.int2 && this->double1 == other.double1 && this->double2 == other.double2 && this->escaped == other.escaped);
		}
		return false;
	}

	void Serialize(std::ofstream& outFile)
	{
		//outFile.write((char*)this, sizeof(this));
		if (version == 1) {
			int boundary = 1001;
			outFile.write(reinterpret_cast<const char*> (&boundary), sizeof(int));
			outFile.write(reinterpret_cast<const char*> (&version), sizeof(int));
			outFile.write(reinterpret_cast<const char*> (&x_pixel), sizeof(int));
			outFile.write(reinterpret_cast<const char*> (&y_pixel), sizeof(int));
			outFile.write(reinterpret_cast<const char*> (&int1), sizeof(int));
			outFile.write(reinterpret_cast<const char*> (&int2), sizeof(int));
			outFile.write(reinterpret_cast<const char*> (&double1), sizeof(double));
			outFile.write(reinterpret_cast<const char*> (&double2), sizeof(double));
			outFile.write(reinterpret_cast<const char*> (&escaped), sizeof(bool));
			outFile.write(reinterpret_cast<const char*> (&boundary), sizeof(int));
			return;
		}
	}

	void Deserialize(std::ifstream& inFile)
	{
		int boundary;
		inFile.read((char*)&boundary, sizeof(int));
		inFile.read((char*)&version, sizeof(int));
		if (version == 1) {
			inFile.read((char*)&x_pixel, sizeof(int));
			inFile.read((char*)&y_pixel, sizeof(int));
			inFile.read((char*)&int1, sizeof(int));
			inFile.read((char*)&int2, sizeof(int));
			inFile.read((char*)&double1, sizeof(double));
			inFile.read((char*)&double2, sizeof(double));
			inFile.read((char*)&escaped, sizeof(bool));
			inFile.read((char*)&boundary, sizeof(int));
			return;
		}
		std::cout << "Error: file version: " << version << " is not supported" << std::endl;
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
