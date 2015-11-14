#include <iostream>
#include <cmath>
#include <random>

#include "Water.h"

#define PI 3.141592f
#define DEG 0.017453f

Water::Water(GLfloat width, GLfloat length, GLfloat amplitude)
{
	this->width = width;
	this->length = length;
	this->amplitude = amplitude;

	stepX = width / (float) WATER_RESOLUTION_X;
	stepZ = length / (float) WATER_RESOLUTION_Z;

	phase = 0;

	//init vertices and normals matrices
	heights = new HeightMap[WATER_RESOLUTION_X * WATER_RESOLUTION_Z];
	normals = new Vec3[WATER_RESOLUTION_X * WATER_RESOLUTION_Z];

	update(0);
}


Water::~Water(void)
{
}

void Water::setHeight(GLuint x, GLuint z,  GLfloat hh, GLfloat hv) {
	heights[x*WATER_RESOLUTION_X + z].h = hh;
	heights[x*WATER_RESOLUTION_X + z].v = hv;
}

void Water::setNormal(GLuint x, GLuint z, Vec3 v) {
	normals[x*WATER_RESOLUTION_X + z].set(&v);
}

GLfloat Water::getHeight(GLuint x, GLuint z) {
	return heights[x*WATER_RESOLUTION_X + z].h + heights[x*WATER_RESOLUTION_X + z].v;
}

Vec3* Water::getNormal(GLuint x, GLuint z) {
	return &normals[x * WATER_RESOLUTION_X + z];
}

void Water::update(GLfloat dt) {
	
	static Vec3 *v1 = new Vec3();
	static Vec3 *v2 = new Vec3();
	static Vec3 *v3 = new Vec3();

	phase += PI * dt;
	
	if(phase > PI*2) {
		phase = 0;
	}
	for(int x = 0; x < WATER_RESOLUTION_X; x++)
		for(int z = 0; z < WATER_RESOLUTION_Z; z++) {
			// calculate height at x, z
			setHeight(x, z, sin((float)x*0.2 + phase) * amplitude, sin((float)z*0.3 + phase) * amplitude);
			
			// calculate normal at x, z
			v1->set(1, cos((float)x*0.2 + phase) * amplitude, 0);
			v1 = v1->normalise();
			v2->set(0, cos((float)z*0.3 + phase) * amplitude, 1);
			v2 = v2->normalise();
			v3 = v2->cross(v1);
			setNormal(x, z, *v3);
			
		}
}