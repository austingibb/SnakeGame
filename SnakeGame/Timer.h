#pragma once
#include <SDL.h>

class Timer
{
public:
	Timer();
	~Timer();

	void start();
	void stop();
	void reset();
	void pause();
	void resume();

	Uint32 get_runtime();

	bool is_paused();
	bool is_running();
private:
	int _start_time;
	int _pause_time;

	bool _paused;
	bool _running;
};

