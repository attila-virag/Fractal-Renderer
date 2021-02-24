#include "stdafx.h"
#include "CalculationProcessor.h"
#include <sstream>
#include "BitmapWriter.h"

//#ifdef _WIN32
#include <Windows.h>
//#else

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
	p->x_pixel = x;
	p->y_pixel = y;
	pointQueue.push(std::move(p));
}

bool CalculationProcessor::IsQueueEmpty()
{
	std::lock_guard<mutex> locker(_mu1);
	return pointQueue.empty();
}

int CalculationProcessor::GetQueueSize()
{
	std::lock_guard<mutex> locker(_mu1);
	return pointQueue.size();
}

// this function runs the point calculation algorithm and save the results to outfile
bool CalculationProcessor::CalculatePoints(const std::string& fileName)
{
	// record start time
	startTime = GetTimeNow();

	// this prepares out pixels that we will calculate with Result structs
	for (int h = 0; h < m_algo->GetZoom()->pixels; h++) {
		for (int w = 0; w < m_algo->GetZoom()->pixels; w++) {
			AddPointToQueue(w, h);
		}
	}

	std::thread t(&CalculationProcessor::ReportProgress, this);
	t.detach();

	vector<thread> threadList;

	for (unsigned int t = 0; t < m_concurrency; t++) {
		threadList.push_back(thread(&CalculationProcessor::CalculatePoint, this, t));
	}

	// thread to write results out to file
	std::thread rt(&CalculationProcessor::SaveResults, this, fileName);
	rt.detach();

	for_each(threadList.begin(), threadList.end(), std::mem_fn(&std::thread::join));

	return true;
}

void CalculationProcessor::ReportProgress()
{
	while (!IsQueueEmpty()) {
		long long timeNow = GetTimeNow();
		long long runningTime = timeNow - startTime;

		int queueSize = GetQueueSize();
		double percentComplete = 100*(1.0 - (queueSize / ((double)m_algo->GetZoom()->pixels * m_algo->GetZoom()->pixels)));
		double resultsComplete = 100 * (resultsWritten / ((double)m_algo->GetZoom()->pixels * m_algo->GetZoom()->pixels));
		std::stringstream ss;
		ss << "Calculation Percent Complete: " << percentComplete << " Time elapsed: " << runningTime << std::endl;
		ss << "Results written percent complete: " << resultsComplete << std::endl;
		std::cout << ss.str();	
#ifdef _DEBUG
		OutputDebugStringA(ss.str().c_str());
#endif
		Sleep(20);
	}

	while (!IsResultQueueEmpty()) {
		long long timeNow = GetTimeNow();
		long long runningTime = timeNow - startTime;

		double resultsComplete = 100 * (resultsWritten / ((double)m_algo->GetZoom()->pixels * m_algo->GetZoom()->pixels));
		std::stringstream ss;
		ss << "Results written percent complete: " << resultsComplete << " Time elapsed: " << runningTime << std::endl;
		std::cout << ss.str();
#ifdef _DEBUG
		OutputDebugStringA(ss.str().c_str());
#endif
		Sleep(20);

	}
	// we just return once done
}

void CalculationProcessor::SaveResult(std::mutex* mu, std::ofstream* outFile) 
{
	while (!IsQueueEmpty()) {
		if (!IsResultQueueEmpty()) {
			// write result out to file
			auto p = GetNextResult();
			if (p == nullptr) {
				continue;
			}
			std::lock_guard<mutex> locker(*mu);
			p->Serialize(*outFile);
			resultsWritten++;
		}
		else {
			Sleep(2);
		}
	}

	// once the queue is empty see if there is still some in the result queue left
	while (!IsResultQueueEmpty()) {
		auto p = GetNextResult();
		std::lock_guard<mutex> locker(*mu);
		p->Serialize(*outFile);
		resultsWritten++;
	}
}

void CalculationProcessor::SaveResults(std::string fileName)
{
	resultsWritten = 0;
	writingResults.store(true);

	std::ofstream outFile;

	std::string filePath = workingDirectory + "data\\" + fileName+ ".result";

	outFile.open(filePath, std::ios::out | std::ios::binary);

	if (!outFile.is_open()) {
		// error
		std::cout << "Unable to open file" << std::endl;
		return;
	}

	m_algo->GetZoom()->Serialize(outFile);

	mutex mu;

	vector<thread> threadList;
	int threads = 4;

	for (unsigned int t = 0; t < threads; t++) {
		threadList.push_back(thread(&CalculationProcessor::SaveResult, this, &mu, &outFile));
	}

	for_each(threadList.begin(), threadList.end(), std::mem_fn(&std::thread::join));

	outFile.close();
	writingResults.store(false);
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

void CalculationProcessor::SerializeResult(std::string fileName)
{
	std::ofstream outFile;

	std::string filePath = workingDirectory + "results\\" + fileName + ".txt";

	outFile.open(filePath);

	if (outFile.is_open()) {
		//this->
		//r.Serialize(outFile);

	}

	outFile.close();
}

void CalculationProcessor::ProcessResult() {

	while (!IsResultQueueEmpty()) {

		auto p = GetNextResult();
		if (p == nullptr) break;
		
		double value = m_norm->GetNormalization(p.get());

		int red = 0;
		int green = 0;
		int blue = 0;

		if (value == 0) {
			red = 0;
			green = 0;
			blue = 0;
		}

		if (p->active) {
			m_color->GetColor(value, red, green, blue);
		}
		m_redData[p->x_pixel][p->y_pixel] = red;
		m_greenData[p->x_pixel][p->y_pixel ] = green;
		m_blueData[p->x_pixel][p->y_pixel] = blue;

	}
}


void CalculationProcessor::PreparePoints()
{
	for (int h = 0; h < m_algo->GetZoom()->pixels; h++) {

		for (int w = 0; w < m_algo->GetZoom()->pixels; w++) {

			AddPointToQueue(w, h);
		}
	}
}


void CalculationProcessor::PrepareRGBVectors() 
{

	for (int h = 0; h < m_algo->GetZoom()->pixels; h++) {

		vector<int> tempRed;
		vector<int> tempGreen;
		vector<int> tempBlue;

		for (int w = 0; w < m_algo->GetZoom()->pixels; w++) {
			tempRed.push_back(0);
			tempGreen.push_back(0);
			tempBlue.push_back(0);
		}

		m_redData.push_back(tempRed);
		m_greenData.push_back(tempGreen);
		m_blueData.push_back(tempBlue);
	}

}

void CalculationProcessor::WriteImage(std::string fileName)
{
	// order red, blue, green
	BitmapWriter bp(m_redData, m_blueData, m_greenData, m_algo->GetZoom()->pixels, m_algo->GetZoom()->pixels);

	bp.WriteBitmap(fileName.c_str());

}

CalculationProcessor::CalculationProcessor(Algorithm* algo, Normalization* norm, ColorPalette* color, int threads):
	m_algo(algo),
	m_norm(norm),
	m_color(color)
{

	// try to pull ideal concurency level of machine
	// assume cpu bound process
#ifdef SINGLE_THREAD
	m_concurrency = 1;
#else
	auto concurrency = std::thread::hardware_concurrency()*2;
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

//call this function when we have a result list
void CalculationProcessor::CreatePicture(std::string fileName)
{
	vector<thread> threadList;

	for (unsigned int t = 0; t < m_concurrency; t++) {
		threadList.push_back(thread(&CalculationProcessor::ProcessResult, this));
	}

	for_each(threadList.begin(), threadList.end(), std::mem_fn(&std::thread::join));

	WriteImage(fileName);
}

bool CalculationProcessor::LoadResultFromFile(std::string filename)
{
	std::string filePath = workingDirectory + "data\\" + filename + ".result";
	std::ifstream inFile;

	inFile.open(filePath, std::ios::in | std::ios::binary);

	if (inFile.is_open()) {
		m_algo->GetZoom()->Deserialize(inFile);
		PrepareRGBVectors();
		for (int i = 0; i < m_algo->GetZoom()->pixels; i++) {
			for (int j = 0; j < m_algo->GetZoom()->pixels; j++) {
				std::unique_ptr<Result> r(new Result());
				r->Deserialize(inFile);

				// do some data processing here - find max, min of data values
				m_norm->CollectMinMaxData(r.get());

				resultQueue.push(std::move(r));
			}
		}
	}
	else {
		return false;
	}

}

