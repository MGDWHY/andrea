#include "Timer.h"

using namespace glutils;

Timer::Timer() {
	prevTime = timeStart = clock();
}


unsigned int Timer::timeMillis() {
	return (int)((clock() - timeStart) / (float)CLOCKS_PER_SEC * 1000);
}

float Timer::timeSecs() {
	return timeMillis() / 1000.0f;
}

unsigned int Timer::dtMillis() {
	clock_t result = clock() - prevTime;
	prevTime = clock();
	return (unsigned int)(result / (float)CLOCKS_PER_SEC * 1000);
}

float Timer::dtSecs() {
	return dtMillis() / 1000.0f;
}

void Timer::restart() {
	prevTime = timeStart = clock();
}