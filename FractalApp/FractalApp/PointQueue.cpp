#include "PointQueue.h"


void PointQueue::AddPoint(std::unique_ptr<Point> p)
{
	std::lock_guard<std::mutex> locker(_mu);
	_queue.push(std::move(p));
}

std::unique_ptr<Point> PointQueue::GetNextPoint()
{
	std::lock_guard<std::mutex> locker(_mu);
	if (_queue.empty()) {
		return nullptr;
	}
	std::unique_ptr<Point> p = std::move(_queue.front());
	_queue.pop();

	return std::move(p);
}

bool PointQueue::IsQueueEmpty()
{
	std::lock_guard<std::mutex> locker(_mu);
	return _queue.empty();
}

int PointQueue::GetQueueSize()
{
	std::lock_guard<std::mutex> locker(_mu);
	return _queue.size();
}

void PointQueue::ClearQueue()
{
	std::lock_guard<std::mutex> locker(_mu);
	std::queue<std::unique_ptr<Point>> empty;
	std::swap(_queue, empty);
}