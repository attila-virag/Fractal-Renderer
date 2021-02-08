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

	double GetParameter(Result* r);

	double valueMin = 0;
	double valueMax = 0;

	double BasicNormalization(Result* r);
	double SqrtSmoothing(Result* r);

public:

	Normalization(ParameterToNormalize p, NormalizationMethod m) { param = p; method = m; }

	virtual ~Normalization();

	double DLL_EXPORT GetNormalization(Result* r);
	void DLL_EXPORT CollectMinMaxData(Result* r);
};