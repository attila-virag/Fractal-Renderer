#include "stdafx.h"
#include "FractalEngine.h"

#include "ColorPalette.h"
#include "Zoom.h"
#include "FractalAlgorithm.h"
#include "CalculationProcessor.h"

bool CreateFractalEngine(void ** instPtr)
{
	auto pZoom = new Zoom();
	auto pColor = new ColorPalette();
	auto pAlgo = new FractalAlgorithm(pZoom, pColor);
	auto pProc = new  CalculationProcessor(pAlgo);

	*instPtr = (void*)pProc;

	return false;
}

bool CleanUp(void ** instPtr)
{
	if (instPtr != nullptr) {

		auto pProc = (CalculationProcessor*)instPtr;

		delete pProc;

		pProc = nullptr;

	}

	return false;
}

bool SetAlgorithm(void * instPtr, int algoNumber)
{
	if (instPtr != nullptr) {

		switch (algoNumber) {
			case 1: ((CalculationProcessor*)instPtr)->m_algo->algoType = AlgorithmType::MandelBrot; break;
			case 2: ((CalculationProcessor*)instPtr)->m_algo->algoType = AlgorithmType::JuliaSet; break;
			default: ((CalculationProcessor*)instPtr)->m_algo->algoType = AlgorithmType::MandelBrot; break;
		}

		return true;
	}
	else return false;	
}

int GetAlgorithm(void * instPtr)
{
	if (instPtr != nullptr) {
		return static_cast<int>(((CalculationProcessor*)instPtr)->m_algo->algoType);
	}
	else  return -1;
}

bool SetZoom(void * instPtr, double x_value, double y_value, double zoom)
{
	if (!Set_X_Value(instPtr, x_value))
		return false;
	if (!Set_Y_Value(instPtr, y_value))
		return false;
	if (!Set_Zoom(instPtr, zoom))
		return false;
	else return true;
}

bool Set_X_Value(void * instPtr, double x_value)
{
	if (instPtr != nullptr) {

		((CalculationProcessor*)instPtr)->m_algo->m_zoom->x_center = x_value;

		return true;
	}
	else return false;
}

bool Set_Y_Value(void * instPtr, double y_value)
{
	if (instPtr != nullptr) {

		((CalculationProcessor*)instPtr)->m_algo->m_zoom->y_center = y_value;

		return true;
	}
	else return false;
}

bool Set_Zoom(void * instPtr, double zoom)
{
	if (instPtr != nullptr) {

		((CalculationProcessor*)instPtr)->m_algo->m_zoom->zoom = zoom;

		return true;
	}
	return false;
}
