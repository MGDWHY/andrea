#include "WorldVertexArray.h"


WorldVertexArray::WorldVertexArray(GLuint size, GLuint tessellation) : VertexArrayObject(3, GL_STATIC_DRAW)
{
	this->size = size;
	this->tessellation = tessellation;

	this->verticesPerEdge = size * tessellation;
	this->verticesCount = this->verticesPerEdge * this->verticesPerEdge;

	GLuint numElements = 6 * (this->verticesPerEdge - 1) * (this->verticesPerEdge - 1);
	GLfloat step = 1.0f / this->tessellation;

	this->positions = new vec3[this->verticesCount];
	this->normals = new vec3[this->verticesCount];
	this->texCoords = new vec2[this->verticesCount];
	this->indices = new GLuint[numElements];

	for(int j = 0; j < this->verticesPerEdge; j++) {
		for(int i = 0; i < this->verticesPerEdge; i++) {
			this->positions[j * this->verticesPerEdge + i] = vec3( i * step - size / 2.0f, 0.0f, j * step - size / 2.0f);
			this->normals[j * this->verticesPerEdge + i] = vec3(0.0f, 1.0f, 0.0f);
			this->texCoords[j * this->verticesPerEdge + i] = vec2( i * step / 2.0f, j * step / 2.0f);
		}
	}

	for(int j = 0; j < this->verticesPerEdge - 1; j++) {
		for(int i = 0; i < this->verticesPerEdge - 1; i++) {
			int base = 6 * j * (this->verticesPerEdge - 1) + i * 6;
			indices[base] = j * this->verticesPerEdge + i;
			indices[base + 1] = (j + 1) * this->verticesPerEdge + i;
			indices[base + 2] = j * this->verticesPerEdge + i + 1;
			indices[base + 3] = j * this->verticesPerEdge + i + 1;
			indices[base + 4] = (j + 1) * this->verticesPerEdge + i;
			indices[base + 5] = (j + 1) * this->verticesPerEdge + i + 1;
		}
	}

	this->setBufferData(0, sizeof(vec3) * this->verticesCount, this->positions);
	this->setBufferData(1, sizeof(vec3) * this->verticesCount, this->normals);
	this->setBufferData(2, sizeof(vec2) * this->verticesCount, this->texCoords);

	this->setElementsData(sizeof(GLuint), numElements, indices);

	this->setVertexAttribute(0, 0, 3, GL_FLOAT);
	this->setVertexAttribute(1, 1, 3, GL_FLOAT);
	this->setVertexAttribute(2, 2, 2, GL_FLOAT);

	this->enableVertexAttribute(0);
	this->enableVertexAttribute(1);
	this->enableVertexAttribute(2);
}


WorldVertexArray::~WorldVertexArray(void)
{
	delete this->positions;
	delete this->texCoords;
	delete this->indices;
}
