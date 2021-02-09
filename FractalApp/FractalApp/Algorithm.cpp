#include "Algorithm.h"
#include "MandelbrotSet.h"
#include "ShowColorPalette.h"
#include "PolynomialAlgorithm.h"

Algorithm* Algorithm::CreateAlgorithm(AlgorithmType type, Zoom* zoom) {

  switch (type) {
  case AlgorithmType::ShowColorPalette: return new ShowColorPalette(zoom);
  case AlgorithmType::Mandelbrot: return new MandelbrotSet(zoom);
  case AlgorithmType::Polynomial: return new PolynomialAlgorithm(zoom);
  default: return nullptr;
  }


 }