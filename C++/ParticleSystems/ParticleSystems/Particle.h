#pragma once

#include "Vector2f.h"


class Particle
{
private:
	float _lifeTime;
public:

	Particle(void);
	~Particle(void);

	Vector2f *position, *velocity;
	bool isAlive(void) { return _lifeTime > 0; }
	
	void setLifeTime(float value) { _lifeTime = value; }
	float getLifeTime(void) { return _lifeTime > 0 ? _lifeTime : 0; }

	void decrementLifeTime(float t) { _lifeTime -= t; }

	virtual void render(void) = 0;
};
