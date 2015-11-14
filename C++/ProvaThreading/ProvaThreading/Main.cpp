#include <Windows.h>
#include <process.h>
#include <ctime>
#include "vecmath.h"


#define NUM_THREADS 8
#define NUM_PARTICLES 10000000

using namespace Vecmath;

struct Particle {
	vec2 position;
	vec2 velocity;
};

Particle particles[NUM_PARTICLES];
unsigned int threads[NUM_THREADS];

struct UpdateParticlesParam {
	int StartIndex;
	int EndIndex;
};

float Time();
float Random();
unsigned __stdcall UpdateParticles(void * param);

int main(int argc, char ** argv) {
	for(int i = 0; i < NUM_PARTICLES; i++) {
		particles[i].position = vec2(Random(), Random());
		particles[i].velocity = vec2(Random(), Random());
	}

	for(int i = 0; i < NUM_THREADS; i++) {
		UpdateParticlesParam * param = new UpdateParticlesParam();

		param->StartIndex = i * (NUM_PARTICLES / NUM_THREADS);
		param->EndIndex = max(NUM_PARTICLES - 1, NUM_PARTICLES / NUM_THREADS * (i + 1) - 1);

		_beginthreadex(NULL, 0, UpdateParticles, param, 0, &(threads[i]));
	}

	while(1) {
		Sleep(1000);
	}
}

unsigned __stdcall UpdateParticles(void * param) {
	static float prevTime = Time();
	static float dt;

	while(1) {
		dt = Time() - prevTime;
		prevTime += dt;

		UpdateParticlesParam * p = (UpdateParticlesParam*) param;

		for(int i = p->StartIndex; i <= p->EndIndex; i++)
			particles[i].position = particles[i].position + particles[i].velocity * dt;

		Sleep(1);
	}
}

float Random() {
	return rand() / (float) RAND_MAX;
}

float Time() {
	return clock() / (float) CLOCKS_PER_SEC;
}


