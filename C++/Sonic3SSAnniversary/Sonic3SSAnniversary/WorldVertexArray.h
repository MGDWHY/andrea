#pragma once

#include "VertexArrayObject.h"
#include "vecmath.h"

using namespace glutils;
using namespace Vecmath;

class WorldVertexArray: public VertexArrayObject
{
private:

	GLfloat size;
	GLuint tessellation;
	GLuint verticesCount;
	GLuint verticesPerEdge;

	vec3 * positions;
	vec3 * normals;
	vec2 * texCoords;
	GLuint * indices;

public:
	WorldVertexArray(GLuint size, GLuint tessellation);
	~WorldVertexArray(void);
};

