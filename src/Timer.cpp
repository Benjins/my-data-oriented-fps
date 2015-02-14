#include "..\header\int\Timer.h"


Timer::Timer(){
	totalTime = 0;
	deltaTime = 0;

#if defined(_WIN32) || defined(_WIN64)
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&prev);
#else
#endif
}


void Timer::NextFrame(){
#if defined(_WIN32) || defined(_WIN64)
	LARGE_INTEGER end;
	QueryPerformanceCounter(&end);

	long long int diff = end.QuadPart - prev.QuadPart;
	double diffDec = (double)diff;

	deltaTime = diffDec/freq.QuadPart;
	totalTime = ((double)end.QuadPart)/freq.QuadPart;

	prev = end;
#else
#endif
}
