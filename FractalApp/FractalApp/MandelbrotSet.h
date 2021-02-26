#pragma once
#include "Algorithm.h"
#include <complex>

class MandelbrotSet : public Algorithm {
private:

	bool CheckIfPointIsInside(double x, double y);
	int DoIterations(std::complex<double>& z, std::complex<double>& c, bool& escaped, int iterationLimit, double power);

	double m_pow;
	const double PI = 3.14159265358979;

public:
	MandelbrotSet(Location* zoom, double pow = 2) : Algorithm(zoom) { m_pow = pow; m_algoType = AlgorithmType::Mandelbrot; }
	virtual DLL_EXPORT ~MandelbrotSet() override {}

	// this method will run in multiple threads
	void CalculatePoint(Point* pt) override;

};