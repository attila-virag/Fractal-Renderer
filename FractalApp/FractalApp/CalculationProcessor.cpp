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
//unique_ptr<Point> CalculationProcessor::GetNextPoint()
//{
//	std::lock_guard<mutex> locker(_mu1);
//	if (pointQueue.empty()) {
//		return nullptr;
//	}
//	unique_ptr<Point> p = std::move(pointQueue.front());
//	pointQueue.pop();
//
//	return std::move(p);
//}
//
//unique_ptr<Point> CalculationProcessor::GetNextResult()
//{
//	std::lock_guard<mutex> locker(_mu2);
//	if (resultQueue.empty()) {
//		return nullptr;
//	}
//	unique_ptr<Point> p = std::move(resultQueue.front());
//	resultQueue.pop();
//
//	return std::move(p);
//}
//
//// for each pixel that we are interested in
//void CalculationProcessor::AddPointToQueue(int x, int y)
//{
//	std::lock_guard<mutex> locker(_mu1);
//	unique_ptr<Point> p(new Point());
//	p->x_pixel = x;
//	p->y_pixel = y;
//	pointQueue.push(std::move(p));
//}
//
//bool CalculationProcessor::IsQueueEmpty()
//{
//	std::lock_guard<mutex> locker(_mu1);
//	return pointQueue.empty();
//}
//
//int CalculationProcessor::GetQueueSize()
//{
//	std::lock_guard<mutex> locker(_mu1);
//	return pointQueue.size();
//}

void CalculationProcessor::FindChanges() {
	// this function checks if any of the previous calculation data can be reused
	// if we have a transpose with some shared pixels from before, we reuse them
	int pixels = m_algo->GetZoom()->pixels;
	// zoom changes will cause a recalc of everything
	m_algo->GetZoom()->LoadLocationDataFromFile("default");
	if (!m_lastCalculated.IsTransposeOnly(m_algo->GetZoom()->zoom)) {
		// redo all points
		for (int h = 0; h < m_algo->GetZoom()->pixels; h++) {
			for (int w = 0; w < m_algo->GetZoom()->pixels; w++) {
				unique_ptr<Point> p(new Point());
				p->x_pixel = w;
				p->y_pixel = h;
				p->processed = false;
				toBeCalculated.AddPoint(std::move(p));
				pointsToBeCalculated++;
			}
		}
		// dump the previous result queue
		m_lastCalculated.ResetLocation(m_algo->GetZoom()->x_center, m_algo->GetZoom()->y_center, m_algo->GetZoom()->zoom, m_algo->GetZoom()->pixels);
		lastResults.ClearQueue();
		pointsToBeRecorded.store(pixels * pixels);
		return;
	}

	// we only have some transpose, save common pixels data
	std::vector<std::vector<Point*>> oldGrid;
	std::vector<std::vector<Point*>> newGrid;

	// prep the old vector
	for (int x = 0; x < pixels; x++) {
		std::vector<Point*> v;
		for (int y = 0; y < pixels; y++) {
			v.push_back(nullptr);
		}
		oldGrid.push_back(v);
	}

	// move the data from queue to old vector
	while (!lastResults.IsQueueEmpty()) {
		unique_ptr<Point> p = lastResults.GetNextPoint();
		oldGrid[p->x_pixel][p->y_pixel] = p.get();
		p.release();
	}

	// prep new target
	for (int x = 0; x < pixels; x++) {
		std::vector<Point*> v;
		for (int y = 0; y < pixels; y++) {
			v.push_back(nullptr);
		}
		newGrid.push_back(v);
	}

	// get the resulting pixels offsets
	int dX = m_lastCalculated.GetXPixelOffset(m_algo->GetZoom()->x_center);
	int dY = m_lastCalculated.GetYPixelOffset(m_algo->GetZoom()->y_center);

	// apply the transpose where we can
	for (int x = 0; x < pixels; x++) {
		for (int y = 0; y < pixels; y++) {

			// if pixels are off screen now, we delete them
			if (x - dX < 0 || x - dX > pixels - 1) {
				delete oldGrid[x][y];
				oldGrid[x][y] = nullptr;
				continue;
			}
			if (y - dY < 0 || y - dY > pixels - 1) {
				delete oldGrid[x][y];
				oldGrid[x][y] = nullptr;
				continue;
			}

			// else we keep the result but transpose the pixel
			auto pt = oldGrid[x][y];
			if (nullptr != pt) {
				newGrid[x - dX][y - dY] = pt;
				newGrid[x - dX][y - dY]->x_pixel = x - dX;
				newGrid[x - dX][y - dY]->y_pixel = y - dY;
				oldGrid[x][y] = nullptr;
			}
		}
	}

	// fill in the resulting empty slots with new points to be calculated
	for (int x = 0; x < pixels; x++) {
		for (int y = 0; y < pixels; y++) {

			if (newGrid[x][y] == nullptr) {
				Point* r = new Point();
				r->x_pixel = x;
				r->y_pixel = y;
				r->processed = false;
				newGrid[x][y] = r;
			}
		}
	}

	// divide up the points into the appropriate queues
	for (int x = 0; x < pixels; x++) {
		for (int y = 0; y < pixels; y++) {
			std::unique_ptr<Point> p(newGrid[x][y]);
			newGrid[x][y] = nullptr;
			if (p->processed) {
				toBeWritten.AddPoint(std::move(p));
			}
			else {
				toBeCalculated.AddPoint(std::move(p));
				pointsToBeCalculated++;
			}
		}
	}
	pointsToBeRecorded.store(pixels * pixels);
	m_lastCalculated.ResetLocation(m_algo->GetZoom()->x_center, m_algo->GetZoom()->y_center, m_algo->GetZoom()->zoom, m_algo->GetZoom()->pixels);
}

bool CalculationProcessor::GenerateImage(std::string fileName)
{
	// record start time
	startTime = GetTimeNow();

	FindChanges();
	PrepareRGBVectors();
	// 
	vector<thread> calcThreadList;
	for (unsigned int t = 0; t < m_concurrency; t++) {
		calcThreadList.push_back(thread(&CalculationProcessor::CalculatePoint, this, t));
	}

	// thread to write results out to file, this will spawn multiple threads
	std::thread rt(&CalculationProcessor::SaveResults, this, "default");
	rt.detach();

	vector<thread> resultThreadList;

	for (unsigned int t = 0; t < m_concurrency; t++) {
		resultThreadList.push_back(thread(&CalculationProcessor::ProcessResult, this));
	}

	for_each(calcThreadList.begin(), calcThreadList.end(), std::mem_fn(&std::thread::join));
	for_each(resultThreadList.begin(), resultThreadList.end(), std::mem_fn(&std::thread::join));

	// at this point all threads should have joined
	WriteImage(fileName);
	return true;
}
//
//// this function runs the point calculation algorithm and save the results to outfile
//bool CalculationProcessor::CalculatePoints(const std::string& fileName)
//{
//	// record start time
//	startTime = GetTimeNow();
//
//	// this prepares out pixels that we will calculate with Result structs
//	for (int h = 0; h < m_algo->GetZoom()->pixels; h++) {
//		for (int w = 0; w < m_algo->GetZoom()->pixels; w++) {
//			AddPointToQueue(w, h);
//		}
//	}
//
//	std::thread t(&CalculationProcessor::ReportProgress, this);
//	t.detach();
//
//	vector<thread> threadList;
//
//	for (unsigned int t = 0; t < m_concurrency; t++) {
//		threadList.push_back(thread(&CalculationProcessor::CalculatePoint, this, t));
//	}
//
//	// thread to write results out to file
//	std::thread rt(&CalculationProcessor::SaveResults, this, fileName);
//	rt.detach();
//
//	for_each(threadList.begin(), threadList.end(), std::mem_fn(&std::thread::join));
//
//	return true;
//}
//
//void CalculationProcessor::ReportProgress()
//{
//	while (!IsQueueEmpty()) {
//		long long timeNow = GetTimeNow();
//		long long runningTime = timeNow - startTime;
//
//		int queueSize = GetQueueSize();
//		double percentComplete = 100*(1.0 - (queueSize / ((double)m_algo->GetZoom()->pixels * m_algo->GetZoom()->pixels)));
//		double resultsComplete = 100 * (resultsWritten / ((double)m_algo->GetZoom()->pixels * m_algo->GetZoom()->pixels));
//		std::stringstream ss;
//		ss << "Calculation Percent Complete: " << percentComplete << " Time elapsed: " << runningTime << std::endl;
//		ss << "Results written percent complete: " << resultsComplete << std::endl;
//		std::cout << ss.str();	
//#ifdef _DEBUG
//		OutputDebugStringA(ss.str().c_str());
//#endif
//		Sleep(20);
//	}
//
//	while (!IsResultQueueEmpty()) {
//		long long timeNow = GetTimeNow();
//		long long runningTime = timeNow - startTime;
//
//		double resultsComplete = 100 * (resultsWritten / ((double)m_algo->GetZoom()->pixels * m_algo->GetZoom()->pixels));
//		std::stringstream ss;
//		ss << "Results written percent complete: " << resultsComplete << " Time elapsed: " << runningTime << std::endl;
//		std::cout << ss.str();
//#ifdef _DEBUG
//		OutputDebugStringA(ss.str().c_str());
//#endif
//		Sleep(20);
//
//	}
//	// we just return once done
//}

// running one per thread, mutex to lock outfile so only one thread can write at a time
void CalculationProcessor::SaveResult(std::mutex* mu, std::ofstream* outFile) 
{
	while (pointsToBeRecorded.load() > 0 ) {
		if (!toBeWritten.IsQueueEmpty()) {
			// write result out to file
			auto p = toBeWritten.GetNextPoint();
			if (p == nullptr) {
				continue;
			}
			// only let one thread write to the file at a time
			std::lock_guard<mutex> locker(*mu);
			p->Serialize(*outFile);
			// move to write pixel queue
			// do some data processing here - find max, min of data values
			// also run this when loading data set
			m_norm->CollectMinMaxData(p.get());
			getRGBValues.AddPoint(std::move(p));
			pointsToBeRecorded--;
		}
		else {
			Sleep(2);
		}
	}
}

void CalculationProcessor::SaveResults(std::string fileName)
{
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
	int threads = m_concurrency/2; // i am assuming since these threads are io bound we dont need as many threads as for calculations

	for (unsigned int t = 0; t < threads; t++) {
		threadList.push_back(thread(&CalculationProcessor::SaveResult, this, &mu, &outFile));
	}

	for_each(threadList.begin(), threadList.end(), std::mem_fn(&std::thread::join));

	outFile.close();
}


// Running one per thread
void CalculationProcessor::CalculatePoint(int threadId)
{
	// while there are points to be processed
	while (!toBeCalculated.IsQueueEmpty()) {

		// get the next available point to calculate
		auto p = toBeCalculated.GetNextPoint();
		if (p == nullptr) {
			if (pointsToBeCalculated.load() == 0) {
				return;
			}
			continue; // we should really hit the above return in this case
		}

		// process point via passed in method
		m_algo->CalculatePoint(p.get());
		p->processed = true;
		pointsToBeCalculated--;

		// move the point to the write data queue
		toBeWritten.AddPoint(std::move(p));
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

// this function calculates each RGB pixel value
void CalculationProcessor::ProcessResult() {

	while (pixelsWritten.load() < m_algo->GetZoom()->pixels* m_algo->GetZoom()->pixels) {

		auto p = getRGBValues.GetNextPoint();
		if (p == nullptr) {
			continue;
		}
		
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

		//move on to the ready queue
		lastResults.AddPoint(std::move(p));
		pixelsWritten++;

	}
}


void CalculationProcessor::PreparePoints()
{
	for (int h = 0; h < m_algo->GetZoom()->pixels; h++) {

		for (int w = 0; w < m_algo->GetZoom()->pixels; w++) {

			//AddPointToQueue(w, h);
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

//bool CalculationProcessor::LoadDefaultFromFile()
//{
//	std::string filename = "default";
//	std::string filePath = workingDirectory + "data\\" + filename + ".result";
//	std::ifstream inFile;
//
//	return LoadResultFromFile(filePath);
//}

bool CalculationProcessor::LoadPointsFromFile(std::string filename)
{
	std::string filePath = workingDirectory + "data\\" + filename + ".result";
	std::ifstream inFile;

	inFile.open(filePath, std::ios::in | std::ios::binary);

	toBeCalculated.ClearQueue();
	toBeWritten.ClearQueue();
	getRGBValues.ClearQueue();
	lastResults.ClearQueue();

	if (inFile.is_open()) {
		m_algo->GetZoom()->Deserialize(inFile);
		m_lastCalculated.ResetLocation(m_algo->GetZoom()->x_center, m_algo->GetZoom()->y_center, m_algo->GetZoom()->zoom, m_algo->GetZoom()->pixels);
		for (int i = 0; i < m_algo->GetZoom()->pixels; i++) {
			for (int j = 0; j < m_algo->GetZoom()->pixels; j++) {
				std::unique_ptr<Point> r(new Point());
				r->Deserialize(inFile);
				r->processed = true;
				// do some data processing here - find max, min of data values
				m_norm->CollectMinMaxData(r.get());

				lastResults.AddPoint(std::move(r));
			}
		}
	}
	else {
		return false;
	}
	return true;
}

