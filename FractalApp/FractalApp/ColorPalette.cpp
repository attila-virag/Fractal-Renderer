#include "stdafx.h"
#include "ColorPalette.h"

void ColorPalette::LinearInterpolate(double magnitude, int & rValue, int & gValue, int & bValue)
{
	// make cyclic colors i.e after .5 everything repeats backwards

	if (magnitude > 1) magnitude = 1;
	if (magnitude < 0) magnitude = 0;

	if (magnitude > 0.5) magnitude = 1 - magnitude; // "reverse" values greater than 5 to make palette cyclic

	double xa = 0;
	double xb = .1;
	double slope;

	double redValue, greenValue, blueValue;

	if (magnitude < 0.1) {
		slope = (magnitude - xa) / (xb - xa);

		redValue = Rvalues[0] + (Rvalues[1] - Rvalues[0])*slope;
		greenValue = Gvalues[0] + (Gvalues[1] - Gvalues[0])*slope;
		blueValue = Bvalues[0] + (Bvalues[1] - Bvalues[0])*slope;
	}
	else if (magnitude < 0.2) {
		xa = 0.1;
		xb = 0.2;
		slope = (magnitude - xa) / (xb - xa);

		redValue = Rvalues[1] + (Rvalues[2] - Rvalues[1])*slope;
		greenValue = Gvalues[1] + (Gvalues[2] - Gvalues[1])*slope;
		blueValue = Bvalues[1] + (Bvalues[2] - Bvalues[1])*slope;
	}
	else if (magnitude < 0.3) {
		xa = 0.2;
		xb = 0.3;
		slope = (magnitude - xa) / (xb - xa);

		redValue = Rvalues[2] + (Rvalues[3] - Rvalues[2])*slope;
		greenValue = Gvalues[2] + (Gvalues[3] - Gvalues[2])*slope;
		blueValue = Bvalues[2] + (Bvalues[3] - Bvalues[2])*slope;
	}
	else if (magnitude < 0.4) {
		xa = 0.3;
		xb = 0.4;
		slope = (magnitude - xa) / (xb - xa);

		redValue = Rvalues[3] + (Rvalues[4] - Rvalues[3])*slope;
		greenValue = Gvalues[3] + (Gvalues[4] - Gvalues[3])*slope;
		blueValue = Bvalues[3] + (Bvalues[4] - Bvalues[3])*slope;
	}
	else if (magnitude <= 0.5) {
		xa = 0.4;
		xb = 0.5;
		slope = (magnitude - xa) / (xb - xa);

		redValue = Rvalues[4] + (Rvalues[0] - Rvalues[4])*slope;
		greenValue = Gvalues[4] + (Gvalues[0] - Gvalues[4])*slope;
		blueValue = Bvalues[4] + (Bvalues[0] - Bvalues[4])*slope;
	}

	rValue = static_cast<int>(redValue);
	gValue = static_cast<int>(greenValue);
	bValue = static_cast<int>(blueValue);

}

ColorPalette::ColorPalette(vector<int> red, vector<int> green, vector<int> blue)
{
	for (int i = 0; i < 5; i++) {
		Rvalues[i] = red[i];
		Gvalues[i] = green[i];
		Bvalues[i] = blue[i];
	}
}
