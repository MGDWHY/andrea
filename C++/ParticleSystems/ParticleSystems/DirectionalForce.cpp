#include "DirectionalForce.h"

DirectionalForce::DirectionalForce(Vector2f* direction, float value)
{
	this->_magnitude = new Vector2f;
	this->direction = new Vector2f;
	this->direction->set(direction);
	this->_value = value;
}


DirectionalForce::~DirectionalForce(void)
{
	delete this->direction;
	delete this->_magnitude;
}

Vector2f* DirectionalForce::getMagnitude(ParticleSystem* system,Particle* particle) {
	this->_magnitude->set(this->direction)->scale(_value);
	return this->_magnitude;
}
