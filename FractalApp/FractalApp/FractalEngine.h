#pragma once
#include "Exports.h"

// This is intended to be the main interface to the engine

extern "C" {

	bool DLL_EXPORT CreateFractalEngine(void ** instPtr);

	bool DLL_EXPORT CleanUp(void ** instPtr);

	bool DLL_EXPORT SetAlgorithm(void* instPtr, int algoNumber);

	int DLL_EXPORT GetAlgorithm(void *instPtr);

	bool DLL_EXPORT SetColorAlgorithm(void* instPtr, int colorAlgoNumber);

	int DLL_EXPORT GetColorAlgorithm(void* instPtr);

	bool DLL_EXPORT Set_Pixels(void *instPtr, int pixels);

	int DLL_EXPORT Get_Pixels(void* instPtr);

	bool DLL_EXPORT Set_X_Value(void *instPtr, double x_value);

	double DLL_EXPORT Get_X_Value(void* instPtr);

	bool DLL_EXPORT Set_Y_Value(void *instPtr, double y_value);

	double DLL_EXPORT Get_Y_Value(void* instPtr);

	bool DLL_EXPORT Set_Zoom(void* instPtr, double zoom);

	double DLL_EXPORT Get_Zoom(void* instPtr);

	void DLL_EXPORT Reset_Zoom(void* instPtr);

	bool DLL_EXPORT LoadLocationFromFile(void* instPtr, const char* filename);

	bool DLL_EXPORT SaveLocationToFile(void* instPtr, const char* filename);

	bool DLL_EXPORT LoadColorPaletteFromFile(void * instPtr, const char* filename);

	bool DLL_EXPORT GenerateRandomColorPalette(void * instPtr);

	bool DLL_EXPORT SaveColorPalette(void * instPtr, const char* filename);

	bool DLL_EXPORT GeneratePreview(void* instPtr, const char* filename);

	bool DLL_EXPORT GenerateImage(void* instPtr, const char* filename);
}
