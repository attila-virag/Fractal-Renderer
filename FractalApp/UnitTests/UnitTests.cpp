#include "pch.h"
#include "CppUnitTest.h"

#include "ColorPalette.h"
#include "Zoom.h"
#include "FractalAlgorithm.h"
#include "CalculationProcessor.h"


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
			ColorPalette color;

			color.GenerateRandomColorPalette();

			Assert::IsTrue(color.SavePaletteToFile("test"));

			Assert::IsTrue(color.LoadPaletteFromFile("test"));
		}

		TEST_METHOD(TestCreatePaletterBitmap)
		{
			Zoom zoom;
			ColorPalette color;
			FractalAlgorithm alg(&zoom, &color);

			alg.algoType = AlgorithmType::ShowColorPalette;

			CalculationProcessor proc(&alg, 8);

			proc.CreatePicture("testPicture");
		}

		TEST_METHOD(TestLoadSaveLocation)
		{
			Zoom zoom(1,1,1,500);
			
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

			r.escaped = true;
			r.x_pixel = x;
			r.y_pixel = y;

			r.finalAngle = angle;
			r.finalMagnitude = mag;

			std::ofstream outFile;

			std::string filePath = workingDirectory + "results\\" + "serializeTest" + ".txt";

			outFile.open(filePath);

			if (outFile.is_open()) {

				outFile << "X: " << std::endl;

				r.Serialize(outFile);

			}

			Result r2;

			outFile.close();

			std::ifstream inFile;

			inFile.open(filePath);

			if (inFile.is_open()) {
				r2.Deserialize(inFile);
			}

			Assert::IsTrue(r.AreEqual(r2));
		}

		TEST_METHOD(TestMandelbrot)
		{

		}
	};
}
