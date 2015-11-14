#pragma once

#include <cmath>
#include <random>

namespace Randomizer {
	void init(void); // init rand lib and trigonometric table
	float* nextTrigonometricAngle(void); // sin and cos of random angle in range [0, 359]
	float nextAngle(void); // random angle in range [0, 2*PI)
	float nextFloat(void); // random float in range [0, 1)
	int nextInt(int max); // radnom int in range [0, max)
	int nextInt(void); // random int int range [0, RAND_MAX ]
}

