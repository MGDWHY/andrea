#pragma once

#include <gl\glut.h>
#include "Particle.h"

typedef struct {
	float r;
	float b;
	float g;
} Color3f;

class SimpleParticle : public Particle{
private:
	static GLuint list;
public:
	Color3f *color;
	SimpleParticle(void);
	~SimpleParticle(void);

	void render() override;
	static void init(void);
};


