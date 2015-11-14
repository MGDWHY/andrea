#include "ParticleSystem.h"
#include <iostream>

ParticleSystem::ParticleSystem(Vector2f* emitter, int maxParticles, int spawnRate, float minLifeTime, float maxLifeTime)
{
	this->position = new Vector2f;
	this->readyParticles = new list<Particle*>;
	this->particles = new list<Particle*>;
	this->forces = new list<Force*>;

	this->position->set(position);

	this->maxParticles = maxParticles;

	this->spawnRate = spawnRate;

	this->minLifeTime = minLifeTime;
	this->maxLifeTime = maxLifeTime;

	this->_numSpawns = 0;
	this->_numParticles = 0;
	this->_vecBuffer = new Vector2f;
}


ParticleSystem::~ParticleSystem(void)
{
	
	delete readyParticles;
	delete particles;
	delete forces;
	delete position;
}

void ParticleSystem::render() {
	for(list<Particle*>::iterator i = this->particles->begin(); i != this->particles->end(); i++)
		(*i)->render();
}

void ParticleSystem::moveSystem(float dt) {
	_updateReadyList();

	if(particles->size() < maxParticles) 
		_numSpawns += dt * spawnRate;
	else
		_numSpawns = 0;

	_spawnParticles();

	list<Particle*>::iterator i;

	for(i = particles->begin(); i != particles->end();) {
		Particle *p = *i;
		if(p->isAlive()) {
			evolveParticle(p,dt);
			i++;
		} else {
			readyParticles->push_back(p);
			i = particles->erase(i);
		}
	}
}

void ParticleSystem::_updateReadyList(void) {
	while(_numParticles < maxParticles) {
		Particle *p = spawnParticle((Particle*)0);
		readyParticles->push_back(p);
		_numParticles++;
	}

	while(_numParticles > maxParticles && readyParticles->size() > 0) {
		readyParticles->pop_front();
		_numParticles--;
	}
}

void ParticleSystem::_spawnParticles(void) {
	while(_numSpawns > 1 && readyParticles->size() > 0 && particles->size() < maxParticles) {
		Particle *p = readyParticles->front();
		readyParticles->pop_front();
		particles->push_back(spawnParticle(p));
		_numSpawns--;
	}
}