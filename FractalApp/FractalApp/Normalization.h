#pragma once
#include "Algorithm.h"

enum class ParameterToNormalize {
	Int1,
	Int2,
	Int3,
	Int4,
	double1,
	double2,
	double3,
	double4,
};

enum class NormalizationMethod {
	BasicNormalization,
	SqrtSmoothing
};

class Normalization {

	ParameterToNormalize param;
	NormalizationMethod method;

	double GetParameter(Point* r);

	double valueMin = 0;
	double valueMax = 0;

	double BasicNormalization(Point* r);
	double SqrtSmoothing(Point* r);

public:

	DLL_EXPORT Normalization(ParameterToNormalize p, NormalizationMethod m) { param = p; method = m; }

	virtual DLL_EXPORT ~Normalization() {};

	double DLL_EXPORT GetNormalization(Point* r);
	void DLL_EXPORT CollectMinMaxData(Point* r);
};