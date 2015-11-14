#include "Randomizer.h"

#define PI 3.14159265f

namespace Randomizer {

	float **sinCosTable;

	void init(void)
	{
		sinCosTable = new float*[360];
		for(int i = 0; i < 360; i++) {
			sinCosTable[i] = new float[2];
			sinCosTable[i][0] = (float)sin(PI/180*i);
			sinCosTable[i][1] = (float)cos(PI/180*i);
		}
	}

	int nextInt(void) {
		return rand();
	}

	int nextInt(int max) {
		return rand() % max;	
	}

	float nextFloat(void) {
		return (float)rand() / (RAND_MAX+1);
	}

	float nextAngle(void) {
		return nextFloat() * (2*PI);
	}

	float* nextTrigonometricAngle(void) {
		return sinCosTable[nextInt(360)];
	}
}