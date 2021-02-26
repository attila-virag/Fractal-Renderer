#pragma once
#include <iostream>
#include <fstream>
// the result struct holds the outcome of the algorithms applied to each pixel
// 
struct Point {

	int version = 1;

	// coordinates are pixels
	int x_pixel = 0;
	int y_pixel = 0;

	// Version 1:
	int int1 = 0;
	int int2 = 0;
	int int3 = 0;
	int int4 = 0;
	double double1 = 0;
	double double2 = 0;
	double double3 = 0;
	double double4 = 0;
	bool active = true;

	bool processed = false;

	Point()
	{

	}

	Point(const Point& r)
	{
		version = r.version;
		x_pixel = r.x_pixel;
		y_pixel = r.y_pixel;
		int1 = r.int1;
		int2 = r.int2;
		int3 = r.int3;
		int4 = r.int4;
		double1 = r.double1;
		double2 = r.double2;
		double3 = r.double3;
		double4 = r.double4;
		active = r.active;
	}

	bool AreEqual(const Point& other)
	{
		if (this->version == 1) {
			return (this->version == other.version && this->x_pixel == other.x_pixel && this->y_pixel == other.y_pixel && this->int1 == other.int1 &&
				this->int2 == other.int2 && this->int3 == other.int3 && this->int4 == other.int4 &&
				this->double1 == other.double1 && this->double2 == other.double2 &&
				this->double3 == other.double3 && this->double4 == other.double4 && this->active == other.active);
		}
		return false;
	}

	void Serialize(std::ofstream& outFile)
	{
		//outFile.write((char*)this, sizeof(this));
		if (version == 1) {
			int boundary = 1001;
			outFile.write(reinterpret_cast<const char*> (&boundary), sizeof(int));
			outFile.write(reinterpret_cast<const char*> (&version), sizeof(int));
			outFile.write(reinterpret_cast<const char*> (&x_pixel), sizeof(int));
			outFile.write(reinterpret_cast<const char*> (&y_pixel), sizeof(int));
			outFile.write(reinterpret_cast<const char*> (&int1), sizeof(int));
			outFile.write(reinterpret_cast<const char*> (&int2), sizeof(int));
			outFile.write(reinterpret_cast<const char*> (&int3), sizeof(int));
			outFile.write(reinterpret_cast<const char*> (&int4), sizeof(int));
			outFile.write(reinterpret_cast<const char*> (&double1), sizeof(double));
			outFile.write(reinterpret_cast<const char*> (&double2), sizeof(double));
			outFile.write(reinterpret_cast<const char*> (&double3), sizeof(double));
			outFile.write(reinterpret_cast<const char*> (&double4), sizeof(double));
			outFile.write(reinterpret_cast<const char*> (&active), sizeof(bool));
			outFile.write(reinterpret_cast<const char*> (&boundary), sizeof(int));
			return;
		}
	}

	void Deserialize(std::ifstream& inFile)
	{
		int boundary;
		inFile.read((char*)&boundary, sizeof(int));
		inFile.read((char*)&version, sizeof(int));
		if (version == 1) {
			inFile.read((char*)&x_pixel, sizeof(int));
			inFile.read((char*)&y_pixel, sizeof(int));
			inFile.read((char*)&int1, sizeof(int));
			inFile.read((char*)&int2, sizeof(int));
			inFile.read((char*)&int3, sizeof(int));
			inFile.read((char*)&int4, sizeof(int));
			inFile.read((char*)&double1, sizeof(double));
			inFile.read((char*)&double2, sizeof(double));
			inFile.read((char*)&double3, sizeof(double));
			inFile.read((char*)&double4, sizeof(double));
			inFile.read((char*)&active, sizeof(bool));
			inFile.read((char*)&boundary, sizeof(int));
			return;
		}
		std::cout << "Error: file version: " << version << " is not supported" << std::endl;
	}

};
