#include "Algorithm.h"
#include "MandelbrotSet.h"
#include "ShowColorPalette.h"

Algorithm* Algorithm::CreateAlgorithm(AlgorithmType type, Zoom* zoom) {

  switch (type) {
  case AlgorithmType::ShowColorPalette: return new ShowColorPalette(zoom);
  case AlgorithmType::Mandelbrot: return new MandelbrotSet(zoom);
  default: return nullptr;
  }


 }