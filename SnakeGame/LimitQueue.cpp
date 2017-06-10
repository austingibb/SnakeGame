#include "LimitQueue.h"



LimitQueue::LimitQueue(int limit_)
{
	_limit = std::max(limit_, 1);
}

void LimitQueue::enqueue(Direction direction)
{
	if (_queue.size() == _limit)
	{
		_queue.pop();
	}

	_queue.push(direction);
}

Direction LimitQueue::pop()
{ 
	Direction direction = _queue.front();
	_queue.pop();
	return direction;
}

Direction LimitQueue::peek()
{
	return _queue.front();
}

void LimitQueue::clear()
{
	while (_queue.size() > 0) _queue.pop();
}

bool LimitQueue::has_more()
{
	return (_queue.size() > 0);
}

int LimitQueue::get_size()
{
	return _queue.size();
}
