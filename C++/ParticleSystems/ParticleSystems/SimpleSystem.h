#pragma once

#include "ParticleSystem.h"

class SimpleSystem : public ParticleSystem
{
private:
	Vector2f* vecBuf;
public:
	SimpleSystem(void);
	~SimpleSystem(void);

	Particle* spawnParticle(Particle*) override;
	void evolveParticle(Particle*,float) override;
};

