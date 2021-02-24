#include "stdafx.h"
#include "FractalEngine.h"

#include "ColorPalette.h"
#include "Zoom.h"
#include "FractalAlgorithm.h"
#include "CalculationProcessor.h"
#include <Windows.h>


extern "C" {

	bool CreateFractalEngine(void** zoomPtr, void ** colorPtr, void ** normPtr, void** algoPtr)
	{

		auto pZoom = new Zoom();
		auto pColor = new ColorPalette();
		auto pNorm = new Normalization(ParameterToNormalize::Int1, NormalizationMethod::SqrtSmoothing);
		auto pAlgo = Algorithm::CreateAlgorithm(AlgorithmType::Mandelbrot, pZoom);

		// load the default files
		pZoom->LoadZoomDataFromFile("default");
		pColor->LoadPaletteFromFile("default");

		*zoomPtr = (void*)pZoom;
		*colorPtr = (void*)pColor;
		*normPtr = (void*)pNorm;
		*algoPtr = (void*)pAlgo;

		return true;
	}

	bool CleanUp(void** instPtr)
	{
		//if (instPtr != nullptr) {

		//	auto pProc = (CalculationProcessor*)instPtr;

		//	// lets save the palletes and coordinates as defaults
		//	pProc->m_algo->m_zoom->SaveZoomDataToFile("default");
		//	pProc->m_algo->m_color->SavePaletteToFile("default");

		//	delete pProc;

		//	pProc = nullptr;

		//}

		return false;
	}

	bool SetAlgorithm(void* instPtr, int algoNumber)
	{
		return true;
		/*if (instPtr != nullptr) {

			switch (algoNumber) {
			case 1: ((CalculationProcessor*)instPtr)->m_algo->algoType = AlgorithmType::MandelBrot; break;
			case 2: ((CalculationProcessor*)instPtr)->m_algo->algoType = AlgorithmType::JuliaSet; break;
			default: ((CalculationProcessor*)instPtr)->m_algo->algoType = AlgorithmType::MandelBrot; break;
			}

			return true;
		}
		else return false;*/
	}

	int GetAlgorithm(void* instPtr)
	{
		return 0;
		/*if (instPtr != nullptr) {
			return static_cast<int>(((CalculationProcessor*)instPtr)->m_algo->algoType);
		}
		else  return -1;*/
	}

	bool SetColorAlgorithm(void* instPtr, int colorAlgoNumber)
	{
		return true;
		/*if (instPtr != nullptr) {

			switch (colorAlgoNumber) {
			case 1: ((CalculationProcessor*)instPtr)->m_algo->colorScheme = ColorScheme::IterationCount; break;
			case 2: ((CalculationProcessor*)instPtr)->m_algo->colorScheme = ColorScheme::HistogramCount; break;
			case 3: ((CalculationProcessor*)instPtr)->m_algo->colorScheme = ColorScheme::EscapeAngle; break;
			case 4: ((CalculationProcessor*)instPtr)->m_algo->colorScheme = ColorScheme::FinalMagnitude; break;
			default: ((CalculationProcessor*)instPtr)->m_algo->colorScheme = ColorScheme::IterationCount; break;
			}

			return true;
		}
		else return false;*/
	}

	int GetColorAlgorithm(void* instPtr)
	{
		return 0;
		/*if (instPtr != nullptr) {
			return static_cast<int>(((CalculationProcessor*)instPtr)->m_algo->colorScheme);
		}
		else  return -1;*/
	}

	bool Set_Pixels(void* instPtr, int pixels)
	{
		return true;
		if (instPtr != nullptr) {

			((Zoom*)instPtr)->pixels = pixels;
			((Zoom*)instPtr)->ResetZoom();

			return true;
		}
		else return false;
	}

	bool Get_Pixels(void* instPtr, int& pixels)
	{
		if (instPtr != nullptr) {
			pixels = ((Zoom*)instPtr)->pixels;
			return true;
		}
		else return false;
	}

	bool Set_X_Value(void* instPtr, double x_value)
	{
		if (instPtr != nullptr) {

			((Zoom*)instPtr)->x_center = x_value;
			((Zoom*)instPtr)->ResetZoom();

			return true;
		}
		else return false;
	}

	bool Get_X_Value(void* instPtr, double& x_value)
	{
		if (instPtr != nullptr) {
			x_value = ((Zoom*)instPtr)->x_center;
			return true;
		}
		else return false;
	}

	bool Set_Y_Value(void* instPtr, double y_value)
	{
		if (instPtr != nullptr) {

			((Zoom*)instPtr)->y_center = y_value;
			((Zoom*)instPtr)->ResetZoom();

			return true;
		}
		else return false;
	}

	bool Get_Y_Value(void* instPtr, double& y_value)
	{
		if (instPtr != nullptr) {
			y_value = ((Zoom*)instPtr)->y_center;
			return true;
		}
		else return false;
	}

	bool Set_Zoom(void* instPtr, double zoom)
	{
		if (instPtr != nullptr) {

			((Zoom*)instPtr)->zoom = zoom;
			((Zoom*)instPtr)->ResetZoom();

			return true;
		}
		else return false;
	}

	bool Get_Zoom(void* instPtr, double& zoom)
	{
		if (instPtr != nullptr) {
			zoom = ((Zoom*)instPtr)->zoom;
			return true;
		}
		else return false;
	}

	void DLL_EXPORT Reset_Zoom(void* instPtr)
	{
		/*((CalculationProcessor*)instPtr)->m_algo->m_zoom->ResetZoom();*/
	}

	bool LoadLocationFromFile(void* instPtr, const char* filename)
	{
		if (instPtr != nullptr) {
			((Zoom*)instPtr)->LoadZoomDataFromFile(filename);
			return true;
		}
		else return false;
	}

	bool SaveLocationToFile(void* instPtr, const char* filename)
	{
		if (instPtr != nullptr) {
			((Zoom*)instPtr)->SaveZoomDataToFile(filename);
			return true;
		}
		else return false;
	}

	bool LoadColorPaletteFromFile(void* instPtr, const char* filename)
	{
		return true;
		/*if (instPtr != nullptr) {

			return ((CalculationProcessor*)instPtr)->m_algo->m_color->LoadPaletteFromFile(filename);
		}
		return false;*/
	}

	bool GenerateRandomColorPalette(void* instPtr)
	{
		//if (instPtr != nullptr) {

		//	auto proc = ((CalculationProcessor*)instPtr);

		//	auto algoType = proc->m_algo->algoType;
		//	proc->m_algo->algoType = AlgorithmType::ShowColorPalette;

		//	double x_center = proc->m_algo->m_zoom->x_center;
		//	double y_center = proc->m_algo->m_zoom->y_center;
		//	double zoom = proc->m_algo->m_zoom->zoom;

		//	proc->m_algo->m_zoom->ResetZoom(.5, .5, .5, proc->m_algo->m_zoom->pixels);

		//	proc->m_algo->m_color->GenerateRandomColorPalette();

		//	// this will spawn the worker threads
		//	proc->CreatePicture("randomColorPalette");

		//	proc->m_algo->algoType = algoType;
		//	proc->m_algo->m_zoom->ResetZoom(x_center, y_center, zoom, proc->m_algo->m_zoom->pixels);
		//	return true;
		//}
		return false;
	}

	bool SaveColorPalette(void* instPtr, const char* filename)
	{
		//if (instPtr != nullptr) {

		//	((CalculationProcessor*)instPtr)->m_algo->m_color->SavePaletteToFile(filename);
		//	return true;
		//}
		return false;
	}

	bool GeneratePreview(void* zoomPtr, void* colorPtr, void* normPtr, void* algoPtr)
	{
		//DLL_EXPORT CalculationProcessor(Algorithm* algo, Normalization* m_norm, ColorPalette* color, int threads = 0);
		CalculationProcessor proc((Algorithm*)algoPtr, (Normalization*)normPtr, (ColorPalette*)colorPtr);
		//auto proc = ((CalculationProcessor*)instPtr);

		//auto pixels = proc->m_algo->m_zoom->pixels;

		//proc->m_algo->m_zoom->pixels = 500;

		//proc->m_algo->m_zoom->ResetZoom();

		proc.CalculatePoints("default");

		// this busy wait to be fixed
		while (proc.writingResults) {
			Sleep(100);
		}

		// this should be not necesary to load from file here
		proc.LoadResultFromFile("default");

		proc.CreatePicture("default");

		//proc->m_algo->m_zoom->pixels = pixels;

		//proc->m_algo->m_zoom->ResetZoom();

		return true;
	}

	bool CalculatePoints(void* instPtr, const char* filename)
	{

		//auto proc = ((CalculationProcessor*)instPtr);

		//proc->CalculatePoints(std::string(filename));

		return true;
	}

	bool GenerateImage(void* instPtr, const char* filename)
	{
		//auto proc = ((CalculationProcessor*)instPtr);

		//auto pixels = proc->m_algo->m_zoom->pixels;

		//proc->CreatePicture(std::string(filename));

		return true;
	}
}
