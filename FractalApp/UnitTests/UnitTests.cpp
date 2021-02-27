#include "pch.h"
#include "CppUnitTest.h"

#include "ColorPalette.h"
#include "Location.h"
#include "Algorithm.h"
#include "Normalization.h"
#include "CalculationProcessor.h"
#include <list>

#include <Windows.h>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(UnitTests)
	{

		bool CompareAbs(double d1, double d2, double tol)
		{
			double _d1 = abs(d1);
			double _d2 = abs(d2);

			return abs(_d1 - _d2) < tol;
		}

	public:
		
		TEST_METHOD(TestRunColorInterpolate)
		{

			double mag = .333;

			int red, green, blue;

			ColorPalette color;

			color.GetColor(mag, red, green, blue);

			Assert::AreEqual(252, red);
			Assert::AreEqual(219, green);
			Assert::AreEqual(42, blue);

			mag = .7777;

			color.GetColor(mag, red, green, blue);

			Assert::AreEqual(198, red);
			Assert::AreEqual(96, green);
			Assert::AreEqual(20, blue);
		}

		TEST_METHOD(TestCreateRandomPaletteAndSave)
		{
			Location zoom;
			ColorPalette color(20,false);

			color.GenerateRandomColorPalette();

			Assert::IsTrue(color.SavePaletteToFile("test"));

			Assert::IsTrue(color.LoadPaletteFromFile("test"));
		}

		TEST_METHOD(TestCreatePaletterBitmap)
		{
			auto pZoom = new Location(0, 0, 1, 500);
			auto pColor = new ColorPalette(20,false);
			auto pNorm = new Normalization(ParameterToNormalize::double1, NormalizationMethod::BasicNormalization);
			pColor->GenerateRandomColorPalette();
			auto pAlgo = Algorithm::CreateAlgorithm(AlgorithmType::ShowColorPalette, pZoom);
			auto pProc = new  CalculationProcessor(pAlgo, pNorm, pColor);
			pProc->GenerateImage("paletteTest");
			//wait until we finish writing

		}

		TEST_METHOD(TestLoadSaveLocation)
		{
			double x = -0.773774;
			double y = -.11766;
			double z = 0.001;
			int p = 500;

			Location zoom(x,y,z,p);
			
			Assert::IsTrue(zoom.SaveLocationDataToFile("test"));

			zoom.x_center = 2;
			zoom.y_center = 4;
			zoom.zoom = 2;

			Assert::IsTrue(zoom.LoadLocationDataFromFile("test"));

			double tol = 0.000000001;

			Assert::IsTrue(CompareAbs(x, zoom.x_center, tol));
			Assert::IsTrue(CompareAbs(y, zoom.y_center, tol));
			Assert::IsTrue(CompareAbs(z, zoom.zoom, tol));
		}

		TEST_METHOD(TestSerializeResult)
		{
			int x = 222;
			int y = 345;

			int iter = 100;
			double mag = 300;
			double angle = 200;

			Point r;
			r.version = 1;
			r.active = true;
			r.x_pixel = x;
			r.y_pixel = y;

			r.double1 = angle;
			r.double2 = mag;	

			std::string filePath = workingDirectory + "results\\" + "serializeTest" + ".bin";

			std::ofstream outFile;

			outFile.open(filePath, std::ios::out | std::ios::binary);

			if (outFile.is_open()) {

				r.Serialize(outFile);

			}

			Point r2;

			outFile.close();

			std::ifstream inFile;

			inFile.open(filePath, std::ios::in | std::ios::binary);

			if (inFile.is_open()) {
				inFile.seekg(0);
				r2.Deserialize(inFile);
			}

			Assert::IsTrue(r.AreEqual(r2));
		}

		TEST_METHOD(SerializeMultipleResults)
		{
			int xRange = 500;
			int yRange = 500;

			std::list<Point> outResults;
			std::list<Point> inResults;

			std::ofstream outFile;

			std::string filePath = workingDirectory + "results\\" + "serializeTest" + ".bin";

			outFile.open(filePath, std::ios::out | std::ios::binary);

			if (outFile.is_open()) {

				for (int i = 0; i < xRange; i++) {
					for (int j = 0; j < yRange; j++) {
						Point r;
						r.x_pixel = i+20;
						r.y_pixel = j+20;
						r.double1 = (double)i * j;
						r.double2 = (double)i * j;

						r.Serialize(outFile);
						outResults.push_back(r);
					}
				}

				outFile.close();

				std::ifstream inFile;

				inFile.open(filePath, std::ios::in | std::ios::binary);

				if (inFile.is_open()) {
					for (int i = 0; i < xRange; i++) {
						for (int j = 0; j < yRange; j++) {
							Point r;
							r.Deserialize(inFile);
							inResults.push_back(r);
						}
					}
				}

				inFile.close();
				
				for (int i = 0; i < xRange * yRange; i++) {
					Point r1 = inResults.front();
					inResults.pop_front();
					Point r2 = outResults.front();
					outResults.pop_front();
					Assert::IsTrue(r1.AreEqual(r2));
				}

			}
		}

		TEST_METHOD(TestGenerateImageFromResultsFile)
		{
			auto pZoom = new Location(-0.773774 - 0.00099, -.11766, 0.001, 500);
			auto pColor = new ColorPalette();
			pColor->LoadPaletteFromFile("default");
			auto pNorm = new Normalization(ParameterToNormalize::Int1, NormalizationMethod::SqrtSmoothing);
			auto pAlgo = Algorithm::CreateAlgorithm(AlgorithmType::Mandelbrot, pZoom);

			auto pProc = new  CalculationProcessor(pAlgo, pNorm, pColor);
			pProc->Initialize();
			pProc->LoadPointsFromFile("testFromFile");

			pProc->GenerateImage("testFromFile");
		}

		TEST_METHOD(TestGenerateMandelbrotResults)
		{
			auto pZoom = new Location(-0.773774 - 0.00099, -.11766, 0.001, 500);
			pZoom->SaveLocationDataToFile("default");
			auto pColor = new ColorPalette();
			pColor->LoadPaletteFromFile("BW");
			auto pNorm = new Normalization(ParameterToNormalize::Int1, NormalizationMethod::SqrtSmoothing);
			pColor->GenerateRandomColorPalette();
			auto pAlgo = Algorithm::CreateAlgorithm(AlgorithmType::Mandelbrot, pZoom);

			auto pProc = new  CalculationProcessor(pAlgo, pNorm, pColor);
			pProc->Initialize();
			pProc->LoadPointsFromFile("test4");
			pProc->GenerateImage("testGenerateMandelbrotResults");

			// translate position and recalc

			pZoom->x_center = -0.773774 - 0.00099;
			pZoom->y_center = -.11766 + 0.00005;

			pZoom->ResetLocation();
			pZoom->SaveLocationDataToFile("default");

			pProc->GenerateImage("testGenerateMandelbrotResults2");

			pZoom->x_center = -0.773774 - 0.00199;
			pZoom->y_center = -.11766 + 0.00055;

			pZoom->ResetLocation();
			pZoom->SaveLocationDataToFile("default");

			pProc->GenerateImage("testGenerateMandelbrotResults3");
			pZoom->x_center = -0.773774 - 0.00199;
			pZoom->y_center = -.11766 + 0.00155;

			pZoom->ResetLocation();
			pZoom->SaveLocationDataToFile("default");

			pProc->GenerateImage("testGenerateMandelbrotResults4");

		}

		// this assumes we generateda file above 
		TEST_METHOD(TestLoadMandelbrotResultsFromFile)
		{
			auto pZoom = new Location();
			auto pColor = new ColorPalette(30);
			pColor->GenerateRandomColorPalette();
			auto pNorm = new Normalization(ParameterToNormalize::Int1, NormalizationMethod::SqrtSmoothing);
			auto pAlgo = Algorithm::CreateAlgorithm(AlgorithmType::Mandelbrot, pZoom);

			auto pProc = new  CalculationProcessor(pAlgo, pNorm, pColor);
			pProc->LoadPointsFromFile("testGenerateMandelbrotResults");

			pProc->CreatePicture("testMandelbrotpicture");
		}


		TEST_METHOD(TestGeneratePolynomialResults)
		{
			auto pZoom = new Location(0, 0, 7, 3000);
			auto pColor = new ColorPalette();
			auto pNorm = new Normalization(ParameterToNormalize::double1, NormalizationMethod::BasicNormalization);
			pColor->GenerateRandomColorPalette();
			auto pAlgo = Algorithm::CreateAlgorithm(AlgorithmType::Polynomial, pZoom);

			auto pProc = new  CalculationProcessor(pAlgo, pNorm, pColor);

			pProc->GenerateImage("testGeneratePolyResults");
		}

		TEST_METHOD(TestLoadPolyResultsFromFile)
		{
			auto pZoom = new Location();
			auto pColor = new ColorPalette(50, false);
			pColor->GenerateRandomColorPalette();
			auto pNorm = new Normalization(ParameterToNormalize::double1, NormalizationMethod::BasicNormalization);
			auto pAlgo = Algorithm::CreateAlgorithm(AlgorithmType::Polynomial, pZoom);

			auto pProc = new  CalculationProcessor(pAlgo, pNorm, pColor);
			pProc->LoadPointsFromFile("testGeneratePolyResults");

			pProc->CreatePicture("testPolypicture");
		}

		TEST_METHOD(TestTransposeVector)
		{
			const int pixels = 500;

			std::vector<std::vector<Point*>> oldVec;
			std::vector<std::vector<Point*>> newVec;

			std::queue<std::unique_ptr<Point>> queue1;
			std::queue<std::unique_ptr<Point>> queue2;

			for (int x = 0; x < pixels; x++) {
				for (int y = 0; y < pixels; y++) {
					Point* r = new Point();
					r->x_pixel = x;
					r->y_pixel = y;
					r->processed = true;
					std::unique_ptr<Point> p(r);
					queue1.push(std::move(p));

				}
			}

			// prep the old vector
			for (int x = 0; x < pixels; x++) {
				std::vector<Point*> t;
				for (int y = 0; y < pixels; y++) {
					t.push_back(nullptr);
				}
				oldVec.push_back(t);
			}

			// move the data from queue to old vector
			while (!queue1.empty()) {
				unique_ptr<Point> p = std::move(queue1.front());
				oldVec[p->x_pixel][p->y_pixel] = p.get();
				p.release();
				queue1.pop();
			}

			// prep new target
			for (int x = 0; x < pixels; x++) {
				std::vector<Point*> t;
				for (int y = 0; y < pixels; y++) {
					t.push_back(nullptr);
				}
				newVec.push_back(t);
			}

			// move right 10 px and up 10 px
			int dX = 10;
			int dY = 10;

			for (int x = 0; x < pixels; x++) {
				for (int y = 0; y < pixels; y++) {

					if (x - dX < 0 || x - dX > pixels-1) {
						delete oldVec[x][y];
						oldVec[x][y] = nullptr;
						continue;
					}
					if (y - dY < 0 || y - dY > pixels-1) {
						delete oldVec[x][y];
						oldVec[x][y] = nullptr;
						continue;
					}

					// else we keep the result but transpose the pixel

					newVec[x - dX][y - dY] = oldVec[x][y];
					newVec[x - dX][y - dY]->x_pixel = x - dX;
					newVec[x - dX][y - dY]->y_pixel = y - dY;
					oldVec[x][y] = nullptr;
				}
			}

			for (int x = 0; x < pixels; x++) {
				for (int y = 0; y < pixels; y++) {

					if (newVec[x][y] == nullptr) {
						Point* r = new Point();
						r->x_pixel = x;
						r->y_pixel = y;
						r->processed = false;
						newVec[x][y] = r;
					}
				}
			}

			// load new Vec back into queue form
			for (int x = 0; x < pixels; x++) {
				for (int y = 0; y < pixels; y++) {
					std::unique_ptr<Point> p (newVec[x][y]);
					newVec[x][y] = nullptr;
					queue2.push(std::move(p));
				}
			}

			// check how many points we need to recalc
			int recalc = 0;
			while (!queue2.empty()) {
				unique_ptr<Point> p = std::move(queue2.front());
				if (!p->processed) {
					recalc++;
				}
				queue1.push(std::move(p));
				queue2.pop();
			}
			Assert::AreEqual(recalc,9900);


			// now same thing but move back 
		}
	};
}
