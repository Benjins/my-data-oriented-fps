#ifndef TIMER_H
#define TIMER_H

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#else
#endif

struct Timer{
	double totalTime;
	double deltaTime;

#if defined(_WIN32) || defined(_WIN64)
	LARGE_INTEGER freq;
	LARGE_INTEGER prev;
#else
#endif

	Timer();

	void NextFrame();
};


#endif
