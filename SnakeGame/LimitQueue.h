#pragma once
#include <queue>
#include "Direction.h"
class LimitQueue
{
public:
	LimitQueue(int limit_);

	void enqueue(Direction direction);
	Direction pop();
	Direction peek();
	void clear();
	bool has_more();
	int get_size();

private:
	std::queue<Direction> _queue;
	int _limit;
};

