#pragma once

#include "Vector2f.h"
#include "ParticleSystem.h"


class Force
{
public:
	virtual Vector2f* getMagnitude(ParticleSystem*,Particle*) = 0;
	Force(void);
	~Force(void);
};

