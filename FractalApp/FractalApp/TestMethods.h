#pragma once

// Test Functions

#include "ColorPalette.h"

void TestRunColorInterpolate() {

	double mag = .333;

	int red, green, blue;

	ColorPalette color;

	color.GetColor(mag, red, green, blue);

	mag = .7777;

	color.GetColor(mag, red, green, blue);
}
