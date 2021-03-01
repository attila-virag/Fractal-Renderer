#pragma once
#include "Normalization.h"
#include <complex>

double Normalization::BasicNormalization(Point* r)
{
  return (GetParameter(r) - valueMin) / (valueMax - (double)valueMin);
}

double Normalization::SqrtSmoothing(Point* r)
{
  double value = BasicNormalization(r);
  for (int i = 0; i < cycles; i++) {
    value = sqrt(value);
  }
  return value;
}

double Normalization::GetNormalization(Point* r)
{
  switch (method) {
  case NormalizationMethod::BasicNormalization : return BasicNormalization(r);
  case NormalizationMethod::SqrtSmoothing : return SqrtSmoothing(r);
  default: return 0;
  }
}

double Normalization::GetParameter(Point* r)
{
  switch (param) {
  case ParameterToNormalize::Int1: return (double)r->int1;
  case ParameterToNormalize::Int2: return (double)r->int2;
  case ParameterToNormalize::Int3: return (double)r->int3;
  case ParameterToNormalize::Int4: return (double)r->int4;
  case ParameterToNormalize::double1: return (double)r->double1;
  case ParameterToNormalize::double2: return (double)r->double2;
  case ParameterToNormalize::double3: return (double)r->double3;
  case ParameterToNormalize::double4: return (double)r->double4;
  default: return 0;
  }
}

void Normalization::CollectMinMaxData(Point* r)
{
  if (GetParameter(r) < this->valueMin) {
    this->valueMin = GetParameter(r);
  }
  if (GetParameter(r) > this->valueMax) {
    this->valueMax = GetParameter(r);
  }
}