#include "stdafx.h"
#include "FractalEngine.h"

#include "ColorPalette.h"
#include "Zoom.h"
#include "FractalAlgorithm.h"
#include "CalculationProcessor.h"

extern "C" {
	bool CreateFractalEngine(void** instPtr)
	{
		auto pZoom = new Zoom();
		auto pColor = new ColorPalette();
		auto pAlgo = new FractalAlgorithm(pZoom, pColor);
		auto pProc = new  CalculationProcessor(pAlgo);

		*instPtr = (void*)pProc;

		if (*instPtr == nullptr)
			return false;
		else return true;
	}

	bool CleanUp(void** instPtr)
	{
		if (instPtr != nullptr) {

			auto pProc = (CalculationProcessor*)instPtr;

			delete pProc;

			pProc = nullptr;

		}

		return false;
	}

	bool SetAlgorithm(void* instPtr, int algoNumber)
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

	int GetAlgorithm(void* instPtr)
	{
		if (instPtr != nullptr) {
			return static_cast<int>(((CalculationProcessor*)instPtr)->m_algo->algoType);
		}
		else  return -1;
	}

	bool SetColorAlgorithm(void* instPtr, int colorAlgoNumber)
	{
		if (instPtr != nullptr) {

			switch (colorAlgoNumber) {
			case 1: ((CalculationProcessor*)instPtr)->m_algo->colorScheme = ColorScheme::IterationCount; break;
			case 2: ((CalculationProcessor*)instPtr)->m_algo->colorScheme = ColorScheme::HistogramCount; break;
			case 3: ((CalculationProcessor*)instPtr)->m_algo->colorScheme = ColorScheme::EscapeAngle; break;
			case 4: ((CalculationProcessor*)instPtr)->m_algo->colorScheme = ColorScheme::FinalMagnitude; break;
			default: ((CalculationProcessor*)instPtr)->m_algo->colorScheme = ColorScheme::IterationCount; break;
			}

			return true;
		}
		else return false;
	}

	int GetColorAlgorithm(void* instPtr)
	{
		if (instPtr != nullptr) {
			return static_cast<int>(((CalculationProcessor*)instPtr)->m_algo->colorScheme);
		}
		else  return -1;
	}

	bool Set_Pixels(void* instPtr, int pixels)
	{
		if (instPtr != nullptr) {

			((CalculationProcessor*)instPtr)->m_algo->m_zoom->pixels = pixels;

			return true;
		}
		else return false;
	}


	bool Set_X_Value(void* instPtr, double x_value)
	{
		if (instPtr != nullptr) {

			((CalculationProcessor*)instPtr)->m_algo->m_zoom->x_center = x_value;

			return true;
		}
		else return false;
	}

	bool Set_Y_Value(void* instPtr, double y_value)
	{
		if (instPtr != nullptr) {

			((CalculationProcessor*)instPtr)->m_algo->m_zoom->y_center = y_value;

			return true;
		}
		else return false;
	}

	bool Set_Zoom(void* instPtr, double zoom)
	{
		if (instPtr != nullptr) {

			((CalculationProcessor*)instPtr)->m_algo->m_zoom->zoom = zoom;

			return true;
		}
		return false;
	}

	bool LoadLocationFromFile(void* instPtr, const char* filename)
	{
		if (instPtr != nullptr) {

			return ((CalculationProcessor*)instPtr)->m_algo->m_zoom->LoadZoomDataFromFile(filename);
		}
		return false;
	}

	bool SaveLocationToFile(void* instPtr, const char* filename)
	{
		if (instPtr != nullptr) {

			return ((CalculationProcessor*)instPtr)->m_algo->m_zoom->SaveZoomDataToFile(filename);
		}
		return false;
	}

	bool LoadColorPaletteFromFile(void* instPtr, const char* filename)
	{
		if (instPtr != nullptr) {

			return ((CalculationProcessor*)instPtr)->m_algo->m_color->LoadPaletteFromFile(filename);
		}
		return false;
	}

	bool GenerateRandomColorPalette(void* instPtr)
	{
		if (instPtr != nullptr) {

			auto proc = ((CalculationProcessor*)instPtr);

			auto algoType = proc->m_algo->algoType;
			proc->m_algo->algoType = AlgorithmType::ShowColorPalette;

			double x_center = proc->m_algo->m_zoom->x_center;
			double y_center = proc->m_algo->m_zoom->y_center;
			double zoom = proc->m_algo->m_zoom->zoom;

			proc->m_algo->m_zoom->ResetZoom(.5, .5, .5, proc->m_algo->m_zoom->pixels);

			proc->m_algo->m_color->GenerateRandomColorPalette();

			// this will spawn the worker threads
			proc->CreatePicture("randomColorPalette");

			proc->m_algo->algoType = algoType;
			proc->m_algo->m_zoom->ResetZoom(x_center, y_center, zoom, proc->m_algo->m_zoom->pixels);
			return true;
		}
		return false;
	}

	bool SaveColorPalette(void* instPtr, const char* filename)
	{
		if (instPtr != nullptr) {

			((CalculationProcessor*)instPtr)->m_algo->m_color->SavePaletteToFile(filename);
			return true;
		}
		return false;
	}

	bool GeneratePreview(void* instPtr, const char* filename)
	{
		auto proc = ((CalculationProcessor*)instPtr);

		auto pixels = proc->m_algo->m_zoom->pixels;

		proc->m_algo->m_zoom->pixels = 500;

		proc->CreatePicture(std::string(filename) + "_preview");

		proc->m_algo->m_zoom->pixels = pixels;

		return true;
	}

	bool GenerateImage(void* instPtr, const char* filename)
	{
		auto proc = ((CalculationProcessor*)instPtr);

		auto pixels = proc->m_algo->m_zoom->pixels;

		proc->CreatePicture(std::string(filename));

		return true;
	}
}
