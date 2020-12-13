#pragma once
#include "Exports.h"
#include <vector>
#include <string>


class BitmapWriter {

private:

	std::vector<std::vector<int>> m_redData;
	std::vector<std::vector<int>> m_blueData;
	std::vector<std::vector<int>> m_greenData;

	int m_height{ 0 };
	int m_width{ 0 };

	std::string imageDirectory = workingDirectory + "images\\";

	void GetTestData();

public:

	BitmapWriter(int height, int width);

	BitmapWriter(std::vector<std::vector<int>> redData, std::vector<std::vector<int>> blueData, std::vector<std::vector<int>> greenData, int width, int height);

	bool WriteBitmap(const char* filename);

	bool WriteTestBitmap();


};
