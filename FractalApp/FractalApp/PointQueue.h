#pragma once
#include <queue>
#include <memory>
#include <mutex>
#include "Point.h"


class PointQueue {
private:

  std::mutex _mu;

  std::queue<std::unique_ptr<Point>> _queue;

public:

  std::unique_ptr<Point> GetNextPoint();

  void AddPoint(std::unique_ptr<Point> p);

  bool IsQueueEmpty();

  int GetQueueSize();

  void ClearQueue();

};