#pragma once
#include <math.h>

class Zoom {

	double x_center{ 0 };
	double y_center{ 0 };

	// scale on either side of center
	double zoom{ 1 };


	// completely arbitrary guesstimate function, can be changed
	int GetRecommendedIterations() {

		double scale = pixels / (2 * zoom);
		return (int)(50 * pow(log(scale), 1.25));
	}

public:

	Zoom() {

		x_min = x_center - zoom;
		x_max = x_center + zoom;
		y_min = y_center - zoom;
		y_max = y_center + zoom;

		recommendedIterations = GetRecommendedIterations();
	}

	Zoom(double x_center, double y_center, double zoom, int pixels):
		x_center(x_center),
		y_center(y_center),
		zoom(zoom),
		pixels(pixels)
	{
		x_min = x_center - zoom;
		x_max = x_center + zoom;
		y_min = y_center - zoom;
		y_max = y_center + zoom;

		recommendedIterations = GetRecommendedIterations();
	}

	//
	void ResetZoom(double x_center, double y_center, double zoom) {
		x_min = x_center - zoom;
		x_max = x_center + zoom;
		y_min = y_center - zoom;
		y_max = y_center + zoom;

		recommendedIterations = GetRecommendedIterations();
	}

	double x_min{ 0 };
	double x_max{ 0 };
	double y_min{ 0 };
	double y_max{ 0 };

	double x_increment{ 0 };
	double y_increment{ 0 };
	// will always create a square picture
	int pixels{ 500 };

	// guesstimate based on zoom level and pizels etc
	int recommendedIterations{ 100 };

};