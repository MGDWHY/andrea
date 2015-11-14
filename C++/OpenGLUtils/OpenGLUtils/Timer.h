#pragma once

#include <ctime>
#include <gl\glew.h>

namespace glutils {
	class Timer {
	private:
		clock_t timeStart, prevTime;
	public:

		Timer();
		~Timer();

		unsigned int timeMillis();
		float timeSecs();

		unsigned int dtMillis();
		float dtSecs();

		void restart();
	};
}