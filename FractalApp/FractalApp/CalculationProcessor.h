#pragma once

// class will orchestrate multithreaded point calculations
// use helper struct in a queue

//#include "ComplexCalculator.h"
#include "FractalAlgorithm.h"
#include "BitmapWriter.h"

#include <queue>
#include <vector>
#include <memory>
#include <mutex>

using std::queue;
using std::vector;
using std::unique_ptr;
using std::mutex;

class  CalculationProcessor {
private:

	vector<vector<int>> m_redData;
	vector<vector<int>> m_blueData;
	vector<vector<int>> m_greenData;

	mutex _mu1;
	mutex _mu2;

	queue < unique_ptr<Result>> pointQueue;

	queue <unique_ptr<Result>> resultQueue;


	unsigned int m_concurrency{ 2 };

	// passes a point in queue to thread
	unique_ptr<Result> GetNextPoint();

	unique_ptr<Result> GetNextResult();

	void AddPointToQueue(int x, int y);

	//queue.empty() is not thread safe
	bool IsQueueEmpty();

	bool IsResultQueueEmpty();

	// each thread will run this method
	void CalculatePoint(int threadId);

	void ProcessResult();

	void PreparePoints();

	void SerializeResult(std::string fileName);

	// single thread
	void WriteImage(std::string fileName);

public:

	FractalFunction m_func = nullptr;
	FractalAlgorithm* m_algo = nullptr;

	DLL_EXPORT CalculationProcessor(FractalAlgorithm* algo,  int threads = 0);

	DLL_EXPORT ~CalculationProcessor();

	bool DLL_EXPORT LoadResultFromFile(std::string filename);

	void DLL_EXPORT CreatePicture(std::string fileName);

};