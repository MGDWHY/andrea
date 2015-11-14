#pragma once

#include "vecmath.h"
#include "VertexArrayObject.h"
#include "Global.h"

using namespace glutils;
using namespace Vecmath;
using namespace Global;

class SkyVertexArray : public VertexArrayObject
{
private:

	GLuint * elements;
	vec3 * positions;
	vec3 * colors;
	GLfloat * sizes;

	int numStars;
	GLfloat bigPercentage;
	vec2 heightBounds, sizeBounds;
public:
	
	int GetNumStars();
	
	void InitStarColors(vec3 color1, vec3 color2);

	vec2 GetHeightBounds();
	vec2 GetSizeBounds();
	GLfloat GetBigPercentage();

	SkyVertexArray(int numStars, vec2 heightBounds, vec2 sizeBounds, GLfloat bigPercentage);

	~SkyVertexArray(void);
};

