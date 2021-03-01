#pragma once

// class will orchestrate multithreaded point calculations
// use helper struct in a queue

//#include "ComplexCalculator.h"
#include "Algorithm.h"
#include "Normalization.h"
#include "ColorPalette.h"
#include "BitmapWriter.h"

#include "PointQueue.h"
#include <vector>
#include <memory>
#include <mutex>
#include <chrono>
#include <condition_variable>

using std::vector;
using std::unique_ptr;
using std::mutex;


class  CalculationProcessor {
private:

	vector<vector<int>> m_redData;
	vector<vector<int>> m_blueData;
	vector<vector<int>> m_greenData;

	//PointQueue pointQueue;
	//queue <unique_ptr<Point>> resultQueue;

	Location m_lastCalculated;

	// we pass points between the queues
	PointQueue toBeCalculated; // to be calculated
	PointQueue toBeWritten; // writes the data set to file
	PointQueue getRGBValues; // this queue is used to generate RGB values based on the results
	PointQueue lastResults; // we hold on to these after we are done with a generation sequence

	long long startTime = 0;
	int resultsWritten = 0;

	unsigned int m_concurrency{ 2 };

	vector<std::thread> workerThreadList;

	std::mutex m_mu;
	std::condition_variable m_startCond;
	std::condition_variable m_endCond;

	std::mutex outfile_mu;
	std::ofstream outFile;

	// each thread will run this method
	void CalculatePoint();

	void ProcessResult();

	void PreparePoints();

	void PrepareRGBVectors();


	void SerializeResult(std::string fileName);

	// single thread
	void WriteImage(std::string fileName);

	long long GetTimeNow() { return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count(); }

	void ReportProgress();

	void SaveResults(std::string fileName);

	void SaveResult();

	void FindChanges();

	void PreparePasses();

	void WorkerThreadFunction();

	bool JobActive() {
		return jobActive.load();
	}

	bool JobFinished() {
		return !jobActive.load();
	}

public:

	//FractalFunction m_func = nullptr;
	Algorithm* m_algo = nullptr;
	Normalization* m_norm = nullptr;
	ColorPalette* m_color = nullptr;

	std::atomic_bool jobActive = false;
	std::atomic_int workerThreadsActive = 0;
	std::atomic_int pointsToBeCalculated;
	std::atomic_int pointsToBeRecorded;
	std::atomic_int pixelsWritten;

	std::atomic_int currentPass = 0;
	const int totalPasses = 10;

	void DLL_EXPORT Initialize(); // starts worker threads

	DLL_EXPORT CalculationProcessor(Algorithm* algo, Normalization* m_norm, ColorPalette* color, int threads = 0);

	DLL_EXPORT ~CalculationProcessor();

	bool DLL_EXPORT GenerateImage(std::string fileName);

	//bool DLL_EXPORT CalculatePoints(const std::string& fileName);

	bool DLL_EXPORT LoadPointsFromFile(std::string filename);

	//bool DLL_EXPORT LoadDefaultFromFile();

	void DLL_EXPORT CreatePicture(std::string fileName);



};