#pragma once
#include "Exports.h"

// This is intended to be the main interface to the engine

extern "C" {

	bool DLL_EXPORT CreateFractalEngine(void** zoomPtr, void** colorPtr, void** normPtr, void** algoPtr);

	bool DLL_EXPORT CleanUp(void ** instPtr);

	bool DLL_EXPORT SetAlgorithm(void* instPtr, int algoNumber);

	int DLL_EXPORT GetAlgorithm(void *instPtr);

	bool DLL_EXPORT SetColorAlgorithm(void* instPtr, int colorAlgoNumber);

	int DLL_EXPORT GetColorAlgorithm(void* instPtr);

	bool DLL_EXPORT Set_Pixels(void *instPtr, int pixels);

	bool DLL_EXPORT Get_Pixels(void* instPtr, int& pixels);

	bool DLL_EXPORT Set_X_Value(void *instPtr, double x_value);

	bool DLL_EXPORT Get_X_Value(void* instPtr, double &x_value);

	bool DLL_EXPORT Set_Y_Value(void *instPtr, double y_value);

	bool DLL_EXPORT Get_Y_Value(void* instPtr, double& y_value);

	bool DLL_EXPORT Set_Zoom(void* instPtr, double zoom);

	bool DLL_EXPORT Get_Zoom(void* instPtr, double& zoom);

	void DLL_EXPORT Reset_Zoom(void* instPtr);

	bool DLL_EXPORT LoadLocationFromFile(void* instPtr, const char* filename);

	bool DLL_EXPORT SaveLocationToFile(void* instPtr, const char* filename);

	bool DLL_EXPORT LoadColorPaletteFromFile(void * instPtr, const char* filename);

	bool DLL_EXPORT GenerateRandomColorPalette(void * instPtr);

	bool DLL_EXPORT SaveColorPalette(void * instPtr, const char* filename);

	bool DLL_EXPORT GeneratePreview(void* instPtr, const char* filename);

	bool DLL_EXPORT GenerateImage(void* instPtr, const char* filename);

	bool DLL_EXPORT CalculatePoints(void* instPtr, const char* filename);
}
