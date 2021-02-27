#pragma once
#include "Location.h"
#include "Exports.h"
#include "Point.h"


// Interface to pixel by pixel calulation algorithm
class Algorithm {

protected:

	Location* m_location;

	AlgorithmType m_algoType = AlgorithmType::ShowColorPalette;

public:

	Algorithm(Location* location) { m_location = location; }

	virtual DLL_EXPORT ~Algorithm() {};

	// this method will run in multiple threads
	virtual void DLL_EXPORT CalculatePoint(Point* pt) = 0;

	Location*  GetLocation() { return m_location; }

	AlgorithmType GetAlgorithmType() { return m_algoType; }

	DLL_EXPORT static Algorithm*  CreateAlgorithm(AlgorithmType type, Location* loation);

};