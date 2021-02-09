#pragma once

// class will orchestrate multithreaded point calculations
// use helper struct in a queue

//#include "ComplexCalculator.h"
#include "Algorithm.h"
#include "Normalization.h"
#include "ColorPalette.h"
#include "BitmapWriter.h"

#include <queue>
#include <vector>
#include <memory>
#include <mutex>
#include <chrono>

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

	long long startTime = 0;
	int resultsWritten = 0;

	unsigned int m_concurrency{ 2 };

	// passes a point in queue to thread
	unique_ptr<Result> GetNextPoint();

	unique_ptr<Result> GetNextResult();

	void AddPointToQueue(int x, int y);

	//queue.empty() is not thread safe
	bool IsQueueEmpty();

	int GetQueueSize();

	bool IsResultQueueEmpty();

	// each thread will run this method
	void CalculatePoint(int threadId);

	void ProcessResult();

	void PreparePoints();

	void PrepareRGBVectors();


	void SerializeResult(std::string fileName);

	// single thread
	void WriteImage(std::string fileName);

	long long GetTimeNow() { return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count(); }

	void ReportProgress();

	void SaveResults(std::string fileName);

	void SaveResult(std::mutex* mu, std::ofstream* outFile);

public:

	//FractalFunction m_func = nullptr;
	Algorithm* m_algo = nullptr;
	Normalization* m_norm = nullptr;
	ColorPalette* m_color = nullptr;

	bool writingResults = false;

	DLL_EXPORT CalculationProcessor(Algorithm* algo, Normalization* m_norm, ColorPalette* color, int threads = 0);

	DLL_EXPORT ~CalculationProcessor();

	bool DLL_EXPORT CalculatePoints(const std::string& fileName);

	bool DLL_EXPORT LoadResultFromFile(std::string filename);

	void DLL_EXPORT CreatePicture(std::string fileName);

};