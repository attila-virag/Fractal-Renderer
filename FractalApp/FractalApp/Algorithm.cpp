#include "Algorithm.h"
#include "MandelbrotSet.h"
#include "ShowColorPalette.h"
#include "PolynomialAlgorithm.h"

Algorithm* Algorithm::CreateAlgorithm(AlgorithmType type, Location* location) {

  switch (type) {
  case AlgorithmType::ShowColorPalette: return new ShowColorPalette(location);
  case AlgorithmType::Mandelbrot: return new MandelbrotSet(location);
  case AlgorithmType::Polynomial: return new PolynomialAlgorithm(location);
  default: return nullptr;
  }


 }