#pragma once

#include "Particle.h"
#include <list>


using namespace std;

class Force;

class ParticleSystem : public Particle
{
private:

	float _numSpawns;
    int _numParticles;
    Vector2f* _vecBuffer;

	void _updateReadyList(void);
	void _spawnParticles(void);

protected:
	
	list<Particle*> *readyParticles, *particles;
	list<Force*> *forces;
	float minLifeTime, maxLifeTime;
	int maxParticles, spawnRate;

public:
	
	ParticleSystem(Vector2f*,int,int,float,float);

	~ParticleSystem(void);

	list<Particle*>* getAliveParticles(void) { return particles; }
	list<Particle*>* getReadyParticles(void) { return readyParticles; }

	void addForce(Force *force) { this->forces->push_back(force);}
	void removeForce(Force *force) { this->forces->remove(force); }

	void setMinLifeTime(float value) { this->minLifeTime = value; }
	void setMaxLifeTime(float value) { this->maxLifeTime = value; }

	void setMaxParticles(int maxParticles) { this->maxParticles = maxParticles; }

	void setSpawnRate(int spawnRate) { this->spawnRate = spawnRate; }

	float getMinLifeTime(void) { return this->minLifeTime; }
	float getMaxLifeTime(void) { return this->maxLifeTime; }

	float randomLifeTime(void) { return ((float)rand() / RAND_MAX) * (this->maxLifeTime - this->minLifeTime) + this->minLifeTime; }

	int getMaxParticles(void) { return this->maxParticles; }

	int getSpawnRate(void) { return this->spawnRate; }

	void moveSystem(float);

	void render() override;

	virtual Particle* spawnParticle(Particle*) = 0;

	virtual void evolveParticle(Particle*,float) = 0;
};
