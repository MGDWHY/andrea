#pragma once

#include "Force.h"

class PointForce : public Force
{
private:
	float _value;
	Vector2f* _magnitude;
public:
	Vector2f* position;

	PointForce(Vector2f*,float);
	~PointForce(void);

	void setValue(float value) { _value = value; }
	float getValue(void) { return _value; }
	
	Vector2f* getMagnitude(ParticleSystem*,Particle*) override;
};

