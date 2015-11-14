#pragma once

#include "Force.h"

class DirectionalForce : public Force
{
private:
	float _value;
	Vector2f* _magnitude;
public:
	Vector2f* direction;

	DirectionalForce(Vector2f* direction, float value);
	~DirectionalForce(void);

	float getValue(void) { return _value; }
	void setValue(float value) { _value = value; } 

	Vector2f* getMagnitude(ParticleSystem*,Particle*) override;
};
