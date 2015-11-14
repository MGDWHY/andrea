#include "SimpleSystem.h"
#include "SimpleParticle.h"
#include "Randomizer.h"

SimpleSystem::SimpleSystem(void) : ParticleSystem(new Vector2f(0,0), 1000,1000, 1, 1)
{
	vecBuf = new Vector2f;
}


SimpleSystem::~SimpleSystem(void)
{
}

void SimpleSystem::evolveParticle(Particle *particle, float dt) {
	vecBuf->set(particle->velocity)->scale(dt);
	particle->position->add(vecBuf);
	particle->decrementLifeTime(dt);
}

Particle* SimpleSystem::spawnParticle(Particle *particle) {
	if(particle == 0)
		particle = new SimpleParticle;

	float* angle = Randomizer::nextTrigonometricAngle();

	((SimpleParticle*)particle)->color->r = 1;
	particle->position->set(position);
	particle->velocity->set(Randomizer::nextFloat() * 50 * angle[1], Randomizer::nextFloat() * 50 * angle[0]);
	particle->setLifeTime(randomLifeTime());

	return particle;
}
