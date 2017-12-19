#pragma once
#include <future>
#include <vector>

using std::future;
using std::vector;
using std::async;

struct Result {
	int x_coordinate;
	int y_coorsdinate;

	double maxMagnitude;
	double finalAngle;

	int r_value;
	int g_value;
	int b_value;
};

typedef Result(*FractalFunction)(int, int);

void RunAsyncLoop(int xMax, int yMax, FractalFunction func) {

	vector<future<Result>> futures;
	auto start = std::chrono::steady_clock::now();

	for (int i = 0; i < yMax; i++) {
		for (int j = 0; j < xMax; j++) {
			futures.push_back(async(std::launch::async, func, i, j));
		}
	}

	for (auto &e : futures) {
		// get results
	}


}
