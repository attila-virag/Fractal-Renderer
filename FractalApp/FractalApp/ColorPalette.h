#pragma once
#include "Exports.h"
#include <array>
#include <vector>
#include <random>
#include <string>

// This class creates a cyclic color palette from preset rgb values
// input range between 0 and 1
// ouput is a set of RGB values between 0 and 255 each
// the palette is build by selecting five RGB values and interpolating in between

using std::array;
using std::vector;

class  ColorPalette {
private:
	array<int, 5> Rvalues;
	array<int, 5> Gvalues;
	array<int, 5> Bvalues;

	std::random_device rd;

	const std::string paletteDir = workingDirectory + "palettes\\";

	void SetDefaultPalette() {

		// palette take from http://www.color-hex.com/color-palette/51323

		Rvalues[0] = 255;
		Rvalues[1] = 251;
		Rvalues[2] = 184;
		Rvalues[3] = 251;
		Rvalues[4] = 255;

		Gvalues[0] = 147;
		Gvalues[1] = 95;
		Gvalues[2] = 55;
		Gvalues[3] = 242;
		Gvalues[4] = 175;

		Bvalues[0] = 0;
		Bvalues[1] = 2;
		Bvalues[2] = 9;
		Bvalues[3] = 61;
		Bvalues[4] = 4;
	}

	void LinearInterpolate(double magnitude, int &redValue, int& greenValue, int& blueValue);

	int GetRandomColor(); // returns value between 0 -255

public:

	DLL_EXPORT ColorPalette() {
		SetDefaultPalette();
	}
	//ColorPalette(ColorPalette &orig){}

	void DLL_EXPORT EnterPalette(vector<int> red, vector<int> green, vector<int> blue);

	void DLL_EXPORT GetColor(double magnitude, int &redValue, int &greenValue, int &blueValue) {
		LinearInterpolate(magnitude, redValue, greenValue, blueValue);
	}

	bool DLL_EXPORT SavePaletteToFile(std::string fileName);

	bool DLL_EXPORT LoadPaletteFromFile(std::string fileName);

	void DLL_EXPORT GenerateRandomColorPalette();

};
