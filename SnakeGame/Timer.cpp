#include "Timer.h"

Timer::Timer()
{
	_running = false;
	_paused = false;
}


Timer::~Timer()
{
}

void Timer::start()
{
	if (!_running)
	{
		_running = true;
		_paused = false;
		_start_time = SDL_GetTicks();
	}
}

void Timer::stop()
{
	if (_running)
	{
		_running = false;
	}
}

void Timer::reset()
{
	if (_running)
	{
		_paused = false;
		_start_time = SDL_GetTicks();
	}
}

void Timer::pause()
{
	if (!_paused && _running)
	{
		_paused = true;
		_pause_time = SDL_GetTicks();
	}
}

void Timer::resume()
{
	if (_paused && _running)
	{
		_paused = false;
		_start_time = SDL_GetTicks() - (_pause_time - _start_time);
	}
}

Uint32 Timer::get_runtime()
{
	Uint32 time = 0;

	if (_running)
	{
		if (_paused)
		{
			time = (_pause_time - _start_time);
		}
		else
		{
			time = SDL_GetTicks() - _start_time;
		}
	}

	return time;
}

bool Timer::is_paused()
{
	return _paused;
}

bool Timer::is_running()
{
	return _running;
}
