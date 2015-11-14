#include "PointForce.h"

PointForce::PointForce(Vector2f* position, float value)
{
	this->position = new Vector2f;
	this->_magnitude = new Vector2f;
	this->_value = value;
	this->position->set(position);
}


PointForce::~PointForce(void)
{
	delete this->position;
	delete this->_magnitude;
}


Vector2f* PointForce::getMagnitude(ParticleSystem* system, Particle* particle) {
	this->_magnitude->setSub(this->position, particle->position)->normalise()->scale(_value);
	return this->_magnitude;
}
