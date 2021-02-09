#pragma once
#pragma once
#include "Algorithm.h"
#include <math.h>
//#include "ColorPalette.h"

class PolynomialAlgorithm : public Algorithm {

public:

  PolynomialAlgorithm(Zoom* zoom) : Algorithm(zoom) { m_algoType = AlgorithmType::Polynomial; }
  virtual DLL_EXPORT ~PolynomialAlgorithm() {}

  
  void CalculatePoint(Result* pt) override
  {
    double x_point = m_zoom->x_min + pt->x_pixel * m_zoom->x_increment;
    double y_point = m_zoom->y_min + pt->y_pixel * m_zoom->y_increment;

    // apply two variable function to get z-value
    //double z = pow(x_point,2)*sin(y_point);
    double z = cos(x_point) - sin(y_point);

    pt->active = true;
    pt->double1 = z;

    //double angle
  }

};