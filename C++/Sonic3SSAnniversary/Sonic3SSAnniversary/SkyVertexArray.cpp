#include "SkyVertexArray.h"


SkyVertexArray::SkyVertexArray(int numStars, vec2 heightBounds, vec2 sizeBounds, GLfloat bigPercentage) : VertexArrayObject(3, GL_STATIC_DRAW)
{
	this->numStars = numStars;
	this->heightBounds = heightBounds;
	this->sizeBounds = sizeBounds;
	this->bigPercentage = bigPercentage;

	this->positions = new vec3[this->numStars];
	this->colors = new vec3[this->numStars];
	this->sizes = new GLfloat[this->numStars];
	this->elements = new GLuint[this->numStars];

	for(int i = 0; i < this->numStars; i++) {
		
		bool big = Global::Random(100.0f) <= this->bigPercentage ? true : false;
		vec3 normal = vec3(Global::Random(-1.0f, 1.0f),Global::Random(-1.0f, 1.0f),Global::Random(-1.0f, 1.0f));

		this->positions[i] = normalize(normal) * Global::Random(this->heightBounds.x, this->heightBounds.y);
		
		this->sizes[i] = Global::Random(this->sizeBounds.x, this->sizeBounds.y);

		if(big)
			this->sizes[i] *= 5.0f;
		
		this->elements[i] = i;
	}

	this->setBufferData(0, sizeof(vec3) * this->numStars, positions);
	this->setBufferData(1, sizeof(vec3) * this->numStars, colors);
	this->setBufferData(2, sizeof(GLfloat) * this->numStars, sizes);

	this->setElementsData(sizeof(GLuint), this->numStars, elements);

	this->setVertexAttribute(0, 0, 3, GL_FLOAT);
	this->setVertexAttribute(1, 1, 3, GL_FLOAT);
	this->setVertexAttribute(2, 2, 1, GL_FLOAT);

	this->enableVertexAttribute(0);
	this->enableVertexAttribute(1);
	this->enableVertexAttribute(2);

}


SkyVertexArray::~SkyVertexArray(void)
{
}

void SkyVertexArray::InitStarColors(vec3 color1, vec3 color2) {
	for(int i = 0; i < this->numStars; i++) {
		GLfloat t = Global::Random(1.0f);
		this->colors[i] = lerp(color1, color2, t);
	}

	this->setBufferData(1, sizeof(vec3) * this->numStars, colors);
}

	
int SkyVertexArray::GetNumStars() { return this->numStars; }
vec2 SkyVertexArray::GetHeightBounds() { return this->heightBounds; }
vec2 SkyVertexArray::GetSizeBounds() { return this->sizeBounds; }