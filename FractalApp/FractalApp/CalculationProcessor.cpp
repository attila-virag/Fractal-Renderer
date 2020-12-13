#include "stdafx.h"
#include "CalculationProcessor.h"

#include "BitmapWriter.h"

#include <thread>
#include <functional>

using std::thread;

//#define SINGLE_THREAD

// grabs a point to be calculated
unique_ptr<Result> CalculationProcessor::GetNextPoint()
{
	std::lock_guard<mutex> locker(_mu1);
	if (pointQueue.empty()) {
		return nullptr;
	}
	unique_ptr<Result> p = std::move(pointQueue.front());
	pointQueue.pop();

	return std::move(p);
}

unique_ptr<Result> CalculationProcessor::GetNextResult()
{
	std::lock_guard<mutex> locker(_mu2);
	if (resultQueue.empty()) {
		return nullptr;
	}
	unique_ptr<Result> p = std::move(resultQueue.front());
	resultQueue.pop();

	return std::move(p);
}

// for each pixel that we are interested in
void CalculationProcessor::AddPointToQueue(int x, int y)
{
	std::lock_guard<mutex> locker(_mu1);
	unique_ptr<Result> p(new Result());
	p->x_coordinate = x;
	p->y_coordinate = y;
	pointQueue.push(std::move(p));
}

bool CalculationProcessor::IsQueueEmpty()
{
	std::lock_guard<mutex> locker(_mu1);
	return pointQueue.empty();
}

// the result queue can be processed as soon as there 
bool CalculationProcessor::IsResultQueueEmpty() {
	std::lock_guard<mutex> locker(_mu2);
	return resultQueue.empty();
}

// Running one per thread
void CalculationProcessor::CalculatePoint(int threadId)
{
	// while there are points to be processed
	while (!IsQueueEmpty()) {

		// get the next available point to calculate
		auto p = GetNextPoint();
		if (p == nullptr) break;

		// process point via passed in method
		m_algo->CalculatePoint(p.get());

		// add p to result queue
		std::unique_lock<mutex> locker(_mu2);
		resultQueue.push(std::move(p));
		locker.unlock();
	}
}

void CalculationProcessor::ProcessResult() {

	while (!IsResultQueueEmpty()) {

		auto p = GetNextResult();
		if (p == nullptr) break;
		
		if (m_algo->algoType != AlgorithmType::ShowColorPalette) {
			m_algo->GetNormalization(p.get());
		}

		int red = 0;
		int green = 0;
		int blue = 0;

		if (p->escaped || m_algo->algoType == AlgorithmType::ShowColorPalette) {
			m_algo->m_color->GetColor(p->magnitude, red, green, blue);
		}
		m_redData[p->x_coordinate][p->y_coordinate] = red;
		m_greenData[p->x_coordinate][p->y_coordinate] = green;
		m_blueData[p->x_coordinate][p->y_coordinate] = blue;

	}
}


void CalculationProcessor::PreparePoints()
{
	for (int h = 0; h < m_algo->m_zoom->pixels; h++) {

		vector<int> tempRed;
		vector<int> tempGreen;
		vector<int> tempBlue;

		for (int w = 0; w < m_algo->m_zoom->pixels; w++) {
			tempRed.push_back(0);
			tempGreen.push_back(0);
			tempBlue.push_back(0);

			AddPointToQueue(w, h);
		}

		m_redData.push_back(tempRed);
		m_greenData.push_back(tempGreen);
		m_blueData.push_back(tempBlue);
	}

}

void CalculationProcessor::WriteImage(std::string fileName)
{
	// order red, blue, green
	BitmapWriter bp(m_redData, m_blueData, m_greenData, m_algo->m_zoom->pixels, m_algo->m_zoom->pixels);

	bp.WriteBitmap(fileName.c_str());

}

CalculationProcessor::CalculationProcessor(FractalAlgorithm* algo, int threads):
	m_algo(algo)
{

	// try to pull ideal concurency level of machine
	// assume cpu bound process
#ifdef SINGLE_THREAD
	m_concurrency = 1;
#else
	auto concurrency = std::thread::hardware_concurrency();
	if (concurrency > 0 && threads == 0) {
		m_concurrency = concurrency;
	}
	else {
		m_concurrency = threads;
	}
#endif
	m_redData.clear();
	m_blueData.clear();
	m_greenData.clear();
}

CalculationProcessor::~CalculationProcessor()
{

}

void CalculationProcessor::CreatePicture(std::string fileName)
{
	// will create as many threads as derived from the conurency level and work throuhg the point queue
	m_redData.clear();
	m_blueData.clear();
	m_greenData.clear();
	
	PreparePoints(); // generation could be its own thread

	vector<thread> threadList;

	// does all calculations and then all image generation, could be optimized

	for (unsigned int t = 0; t < m_concurrency; t++) {
		threadList.push_back(thread(&CalculationProcessor::CalculatePoint, this, t));
	}

	for_each(threadList.begin(), threadList.end(), std::mem_fn(&std::thread::join));

	//generatorThread.join();
	vector<thread> threadList2; // TODO see about reusing the original thread vector

	for (unsigned int t = 0; t < m_concurrency; t++) {
		threadList2.push_back(thread(&CalculationProcessor::ProcessResult, this));
	}

	for_each(threadList2.begin(), threadList2.end(), std::mem_fn(&std::thread::join));


	WriteImage(fileName);
}
