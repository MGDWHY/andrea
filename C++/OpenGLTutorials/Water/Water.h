#pragma once
#include <gl\glut.h>
#include "Vec3.h"

#define WATER_RESOLUTION_X 100
#define WATER_RESOLUTION_Z 100

typedef struct {
	GLfloat h;
	GLfloat v;
} HeightMap;

class Water
{
private:
	GLfloat amplitude;
	
	GLfloat stepX, stepZ;
	GLfloat width, length;
	
	HeightMap *heights;
	Vec3 *normals;
	
	GLfloat phase;

	void setHeight(GLuint x, GLuint z, GLfloat hh, GLfloat hv);
	void setNormal(GLuint x, GLuint z, Vec3 v); 
public:
	Water(GLfloat width, GLfloat length, GLfloat amplitude = 0.2f);
	~Water(void);

	
	GLfloat getHeight(GLuint x, GLuint z);
	Vec3* getNormal(GLuint x, GLuint z);

	GLfloat getStepX() { return stepX; }
	GLfloat getStepZ() { return stepZ; }

	void update(GLfloat dt);
};

