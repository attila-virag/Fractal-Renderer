#include "stdafx.h"
#include "CalculationProcessor.h"

#include "BitmapWriter.h"

#include <thread>

using std::thread;

unique_ptr<Result> CalculationProcessor::GetNextPoint()
{
	std::lock_guard<mutex> locker(_mu1);
	if (pointQueue.empty()) {
		return nullptr;
	}
	unique_ptr<Result> p = std::move(pointQueue.front());
	pointQueue.pop();

	return p;
}

unique_ptr<Result> CalculationProcessor::GetNextResult()
{
	std::lock_guard<mutex> locker(_mu2);
	if (resultQueue.empty()) {
		return nullptr;
	}
	unique_ptr<Result> p = std::move(resultQueue.front());
	resultQueue.pop();

	return p;
}

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

bool CalculationProcessor::IsResultQueueEmpty() {
	std::lock_guard<mutex> locker(_mu2);
	return resultQueue.empty();
}

void CalculationProcessor::CalculatePoint(int threadId)
{
	while (!IsQueueEmpty()) {

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
		
		m_algo->GetNormalization(p.get());

		int red = 0;
		int green = 0;
		int blue = 0;

		if (p->escaped || m_algo->algoType == AlgorithmType::ShowColorPalette) {
			m_algo->m_color->GetColor(p->magnitude, red, green, blue);
		}
		m_redData[p->x_coordinate][p->y_coordinate] = red;
		m_greenData[p->x_coordinate][p->y_coordinate] = green;
		m_blueData[p->x_coordinate][p->y_coordinate] = blue;

		p.reset(nullptr);

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
	fileName = fileName + ".bmp";

	// order red, blue, green
	BitmapWriter bp(m_redData, m_blueData, m_greenData, m_algo->m_zoom->pixels, m_algo->m_zoom->pixels);

	bp.WriteBitmap(fileName.c_str());

}

CalculationProcessor::CalculationProcessor(FractalAlgorithm* algo, int threads):
	m_algo(algo)
{

	// try to pull ideal concurency level of machine
	// assume cpu bound process
	auto concurrency = std::thread::hardware_concurrency();
	if (concurrency > 0 && threads == 0) {
		m_concurrency = concurrency;
	}
	else {
		m_concurrency = threads;
	}

	m_redData.clear();
	m_blueData.clear();
	m_greenData.clear();
}

void CalculationProcessor::CreatePicture(std::string fileName)
{
	// will create as many threads as derived from the conurency level and work throuhg the point queue

	
	PreparePoints(); // generation could be its own thread

	vector<thread> threadList;

	for (unsigned int t = 0; t < m_concurrency; t++) {
		threadList.push_back(thread(&CalculationProcessor::CalculatePoint, this, t));
	}

	for_each(threadList.begin(), threadList.end(), mem_fn(&std::thread::join));

	//generatorThread.join();
	vector<thread> threadList2; // TODO see about reusing the original thread vector

	for (unsigned int t = 0; t < m_concurrency; t++) {
		threadList2.push_back(thread(&CalculationProcessor::ProcessResult, this));
	}

	for_each(threadList2.begin(), threadList2.end(), mem_fn(&std::thread::join));


	WriteImage(fileName);
}
