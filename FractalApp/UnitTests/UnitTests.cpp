#include "pch.h"
#include "CppUnitTest.h"

#include "ColorPalette.h"
#include "Zoom.h"
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
			Zoom zoom;
			ColorPalette color(20,false);

			color.GenerateRandomColorPalette();

			Assert::IsTrue(color.SavePaletteToFile("test"));

			Assert::IsTrue(color.LoadPaletteFromFile("test"));
		}

		TEST_METHOD(TestCreatePaletterBitmap)
		{
			auto pZoom = new Zoom(0, 0, 1, 500);
			auto pColor = new ColorPalette(20,false);
			auto pNorm = new Normalization(ParameterToNormalize::double1, NormalizationMethod::BasicNormalization);
			pColor->GenerateRandomColorPalette();
			auto pAlgo = Algorithm::CreateAlgorithm(AlgorithmType::ShowColorPalette, pZoom);
			auto pProc = new  CalculationProcessor(pAlgo, pNorm, pColor);
			pProc->CalculatePoints("paletteTest");
			//wait until we finish writing
			while (pProc->writingResults) {
				Sleep(100);
			}
			pProc->LoadResultFromFile("paletteTest");
			pProc->CreatePicture("paletteTestPicture");
		}

		TEST_METHOD(TestLoadSaveLocation)
		{
			Zoom zoom(-0.773774,-.11766,0.001,1500);
			
			Assert::IsTrue(zoom.SaveZoomDataToFile("test"));

			zoom.x_center = 2;
			zoom.y_center = 4;
			zoom.zoom = 2;

			Assert::IsTrue(zoom.LoadZoomDataFromFile("test"));

			Assert::AreEqual((double)1, zoom.x_center);
			Assert::AreEqual((double)1, zoom.y_center);
			Assert::AreEqual((double)1, zoom.zoom);
		}

		TEST_METHOD(TestSerializeResult)
		{
			int x = 222;
			int y = 345;

			int iter = 100;
			double mag = 300;
			double angle = 200;

			Result r;
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

			Result r2;

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

			std::list<Result> outResults;
			std::list<Result> inResults;

			std::ofstream outFile;

			std::string filePath = workingDirectory + "results\\" + "serializeTest" + ".bin";

			outFile.open(filePath, std::ios::out | std::ios::binary);

			if (outFile.is_open()) {

				for (int i = 0; i < xRange; i++) {
					for (int j = 0; j < yRange; j++) {
						Result r;
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
							Result r;
							r.Deserialize(inFile);
							inResults.push_back(r);
						}
					}
				}

				inFile.close();
				
				for (int i = 0; i < xRange * yRange; i++) {
					Result r1 = inResults.front();
					inResults.pop_front();
					Result r2 = outResults.front();
					outResults.pop_front();
					Assert::IsTrue(r1.AreEqual(r2));
				}

			}
		}

		TEST_METHOD(TestGenerateMandelbrotResults)
		{
			auto pZoom = new Zoom(-0.773774, -.11766, 0.001, 1500);
			auto pColor = new ColorPalette();
			auto pNorm = new Normalization(ParameterToNormalize::Int1, NormalizationMethod::SqrtSmoothing);
			pColor->GenerateRandomColorPalette();
			auto pAlgo = Algorithm::CreateAlgorithm(AlgorithmType::Mandelbrot, pZoom);

			auto pProc = new  CalculationProcessor(pAlgo, pNorm, pColor);

			pProc->CalculatePoints("testGenerateMandelbrotResults");
			
		}

		// this assumes we generateda file above 
		TEST_METHOD(TestLoadMandelbrotResultsFromFile)
		{
			auto pZoom = new Zoom();
			auto pColor = new ColorPalette(30);
			pColor->GenerateRandomColorPalette();
			auto pNorm = new Normalization(ParameterToNormalize::Int1, NormalizationMethod::SqrtSmoothing);
			auto pAlgo = Algorithm::CreateAlgorithm(AlgorithmType::Mandelbrot, pZoom);

			auto pProc = new  CalculationProcessor(pAlgo, pNorm, pColor);
			pProc->LoadResultFromFile("testGenerateMandelbrotResults");

			pProc->CreatePicture("testMandelbrotpicture");
		}


		TEST_METHOD(TestGeneratePolynomialResults)
		{
			auto pZoom = new Zoom(0, 0, 7, 3000);
			auto pColor = new ColorPalette();
			auto pNorm = new Normalization(ParameterToNormalize::double1, NormalizationMethod::BasicNormalization);
			pColor->GenerateRandomColorPalette();
			auto pAlgo = Algorithm::CreateAlgorithm(AlgorithmType::Polynomial, pZoom);

			auto pProc = new  CalculationProcessor(pAlgo, pNorm, pColor);

			pProc->CalculatePoints("testGeneratePolyResults");
			while (pProc->writingResults) {
				Sleep(100);
			}
		}

		TEST_METHOD(TestLoadPolyResultsFromFile)
		{
			auto pZoom = new Zoom();
			auto pColor = new ColorPalette(50, false);
			pColor->GenerateRandomColorPalette();
			auto pNorm = new Normalization(ParameterToNormalize::double1, NormalizationMethod::BasicNormalization);
			auto pAlgo = Algorithm::CreateAlgorithm(AlgorithmType::Polynomial, pZoom);

			auto pProc = new  CalculationProcessor(pAlgo, pNorm, pColor);
			pProc->LoadResultFromFile("testGeneratePolyResults");

			pProc->CreatePicture("testPolypicture");
		}
	};
}
