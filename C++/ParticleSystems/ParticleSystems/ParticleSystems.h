#ifndef _PARTICLESSYSTEMS_H_
#define _PARTICLESSYSTEMS_H_

#include "Vector2f.h"
#include <list>
#include <random>

using namespace std;

class Particle;
class ParticleSystem;
class Force;
class DirectionalForce;
class PointForce;

class Particle
{
private:
	float _lifeTime;
public:

	Particle(void);
	~Particle(void);

	Vector2f *position, *velocity;
	bool isAlive(void) { return this->_lifeTime > 0 ? true : false; }
	
	void setLifeTime(float value) { this->_lifeTime = value; }
	float getLifeTime(void) { return this->_lifeTime; }

	virtual void render(void) = 0;
};

class ParticleSystem : public Particle
{
private:

	float _numSpawns;
    int _numParticles;
    Vector2f* _vecBuffer;
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

	void addForce(Force* force) { this->forces->push_back(force);}
	void removeForce(Force* force) { this->forces->remove(force); }

	void setMinLifeTime(float value) { this->minLifeTime = value; }
	void setMaxLifeTime(float value) { this->maxLifeTime = value; }

	void setMaxParticles(int maxParticles) { this->maxParticles = maxParticles; }

	void setSpawnRate(int spawnRate) { this->spawnRate = spawnRate; }

	float getMinLifeTime(void) { return this->minLifeTime; }
	float getMaxLifeTime(void) { return this->maxLifeTime; }

	float randomLifeTime(void) { return ((float)rand() / RAND_MAX)  * (this->maxLifeTime - this->minLifeTime) + this->minLifeTime;}

	int getMaxParticles(void) { return this->maxParticles; }
	
	int getSpawnRate(void) { return this->spawnRate; }

	void render() override {}
};

class Force
{
public:
	virtual Vector2f* getMagnitude(Particle*, ParticleSystem*) = 0;
	Force(void);
	~Force(void);
};

class PointForce : public Force
{
private:
	float _value;
	Vector2f* _magnitude;
public:
	Vector2f* position;

	PointForce(Vector2f*,float);
	~PointForce(void);

	void setValue(float value) { this->_value = value; }
	float getValue(void) { return this->_value; }
	
	Vector2f* getMagnitude(ParticleSystem*,Particle*);
};

class DirectionalForce : public Force
{
private:
	float _value;
	Vector2f* _magnitude;
public:
	Vector2f* direction;

	DirectionalForce(Vector2f* direction, float value);
	~DirectionalForce(void);

	float getValue(void) { return this->_value;}
	void setValue(float value) { this->_value = value; }

	virtual Vector2f* getMagnitude(ParticleSystem*,Particle*);
};

#endif