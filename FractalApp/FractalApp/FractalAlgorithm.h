#pragma once
#include "Algorithm.h"
#include "ColorPalette.h"
#include <complex>
#include <memory>
#include <iostream>
#include <unordered_map>



	

//typedef void(*FractalFunction)(Result *r);


enum class ColorScheme {

	IterationCount,
	HistogramCount,
	EscapeAngle,
	FinalMagnitude
};

class MandelbrotSet : public Algorithm {

	void ShowColorPalette(Result* pt);

	//void MandelBrotSet(Result * pt);

	

	double topMagnitude = 0; // this is the max of whatever quantity we are tracking in this run, helps to normalize data 

	double NormalizeIterations(int iterations, std::complex<double> Zn, std::complex<double> C);

	//double GetHistogram

	int DoIterations(std::complex<double>& z, std::complex<double>& c, bool& escaped, int iterationLimit, double power);

	bool CheckIfPointIsInside(double x, double y);

	std::unordered_map<int, int> histogramCount; // this map holds the counts for the histogram method, <escape iteration number, number of pixels in buket>

	double m_pow{ 2 };

public:

	//ColorPalette* m_color;

	double max_mag = 0; // the maximum magnitude calculated
	double min_mag = 0; // the minimum magnitude calculated (should be 1 or 0)

	int iterationsMin = 0; // out of the points that escaped
	int iterationsMax = 0; // out of the points that escaped

	AlgorithmType algoType = AlgorithmType::Mandelbrot;
	ColorScheme colorScheme = ColorScheme::IterationCount;

	DLL_EXPORT MandelbrotSet(Zoom* zoom) :  Algorithm(zoom) {};
	virtual ~MandelbrotSet(){}

	// this method will run in multiple threads
	void DLL_EXPORT CalculatePoint(Result* pt);

	// for each point normalizes the magnitude value based on whatever smoothing methodology applied
	//void DLL_EXPORT GetNormalization(Result * pt);

	
};
