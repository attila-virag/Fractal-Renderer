#pragma once

// This is intended to be the main interface to the engine

extern "C" {

	bool CreateFractalEngine(void ** instPtr);

	bool CleanUp(void ** instPtr);

	bool SetAlgorithm(void* instPtr, int algoNumber);

	int GetAlgorithm(void *instPtr);

	bool SetZoom(void *instPtr, double x_value, double y_value, double center);

	bool Set_X_Value(void *instPtr, double x_value);

	bool Set_Y_Value(void *instPtr, double y_value);

	bool Set_Zoom(void* instPtr, double zoom);


}
