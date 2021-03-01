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
			auto pZoom = new Location(0, 0, 1, 500, AlgorithmType::ShowColorPalette);
			auto pColor = new ColorPalette(20,false);
			pColor->SetPaletteLenght(5);
			auto pNorm = new Normalization(ParameterToNormalize::double1, NormalizationMethod::BasicNormalization);
			//pColor->GenerateRandomColorPalette();
			auto pAlgo = Algorithm::CreateAlgorithm(AlgorithmType::ShowColorPalette, pZoom);
			auto pProc = new  CalculationProcessor(pAlgo, pNorm, pColor);
			pProc->Initialize();
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
			auto pZoom = new Location(-0.773774 - 0.00099, -.11766, 0.001, 800);
			auto pColor = new ColorPalette();
			pColor->LoadPaletteFromFile("default");
			pColor->SetPaletteLenght(6);
			//auto pNorm = new Normalization(ParameterToNormalize::double3, NormalizationMethod::BasicNormalization);
			auto pNorm = new Normalization(ParameterToNormalize::Int1, NormalizationMethod::SqrtSmoothing);
			pNorm->SetNormalizationCycle(3);
			auto pAlgo = Algorithm::CreateAlgorithm(AlgorithmType::Mandelbrot, pZoom);

			auto pProc = new  CalculationProcessor(pAlgo, pNorm, pColor);
			pProc->Initialize();
			pProc->LoadPointsFromFile("testFromFile2");

			pProc->GenerateImage("testFromFile2");
		}

		TEST_METHOD(TestGenerateImageFromResultsFileAndTranspose)
		{
			auto pZoom = new Location();
			auto pColor = new ColorPalette();
			pColor->LoadPaletteFromFile("default");
			auto pNorm = new Normalization(ParameterToNormalize::Int1, NormalizationMethod::SqrtSmoothing);
			auto pAlgo = Algorithm::CreateAlgorithm(AlgorithmType::Mandelbrot, pZoom);

			auto pProc = new  CalculationProcessor(pAlgo, pNorm, pColor);
			pProc->Initialize();
			pProc->LoadPointsFromFile("testFromFile2");

			pProc->GenerateImage("testFromFile2");

			pZoom->ResetLocation(pZoom->x_center, pZoom->y_center - 0.00099, pZoom->zoom, pZoom->pixels);

			pProc->GenerateImage("testFromFile3");
		}

		TEST_METHOD(TestGenerateBasicMandelbrot)
		{
			//auto pZoom = new Location(-0.8, 0, 1.5, 500, AlgorithmType::Mandelbrot,1000);
			auto pZoom = new Location(-0.773774 - 0.00099, -.11766, 0.001, 500,AlgorithmType::Mandelbrot);
			auto pColor = new ColorPalette(20,true);
			//pColor->LoadPaletteFromFile("default");
			pColor->SetPaletteLenght(20);
			auto pNorm = new Normalization(ParameterToNormalize::double3, NormalizationMethod::BasicNormalization);
			pNorm->SetNormalizationCycle(4);
			auto pAlgo = Algorithm::CreateAlgorithm(AlgorithmType::Mandelbrot, pZoom);

			auto pProc = new  CalculationProcessor(pAlgo, pNorm, pColor);
			pProc->Initialize();
			//pProc->LoadPointsFromFile("testGenerateMandelbrotResults");
			pProc->GenerateImage("testGenerateMandelbrotResults");


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
			auto pZoom = new Location(0, 0, 7, 3000, AlgorithmType::Polynomial);
			auto pColor = new ColorPalette(5,false);
			pColor->SetPaletteLenght(5);
			auto pNorm = new Normalization(ParameterToNormalize::double1, NormalizationMethod::BasicNormalization);
			//pColor->GenerateRandomColorPalette();
			auto pAlgo = Algorithm::CreateAlgorithm(AlgorithmType::Polynomial, pZoom);

			auto pProc = new  CalculationProcessor(pAlgo, pNorm, pColor);
			pProc->Initialize();
			pProc->LoadPointsFromFile("testGeneratePolyResults");
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
	};
}
