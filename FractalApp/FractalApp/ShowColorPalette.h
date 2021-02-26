#pragma once
#include "Algorithm.h"
//#include "ColorPalette.h"

class ShowColorPalette : public Algorithm {

public:

  ShowColorPalette(Location* zoom) : Algorithm(zoom) { m_algoType = AlgorithmType::ShowColorPalette; }
  virtual DLL_EXPORT ~ShowColorPalette() {}

  // this will simply range the magnitude 0 -> 1 -> 0 going left to right
  void CalculatePoint(Point* pt) override
  {
    pt->double1 = pt->x_pixel / (double)m_location->pixels;
  }

};
