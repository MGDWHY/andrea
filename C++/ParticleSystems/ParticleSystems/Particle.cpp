#include "Particle.h"

Particle::Particle(void)
{
	this->position = new Vector2f(1,1);
	this->velocity = new Vector2f(1,1);
	this->_lifeTime = 0;
}


Particle::~Particle(void)
{
	delete position;
	delete velocity;
}