#include "VertexArrayObject.h"

namespace glutils {

	VertexArrayObject::VertexArrayObject(unsigned int numBuffers, GLenum usage) {
		this->numBuffers = numBuffers;
		this->usage = usage;

		glGenVertexArrays(1, &vaoID);

		this->vboIDs = new unsigned int[numBuffers + 1];

		glGenBuffers(numBuffers + 1, &vboIDs[0]);
	}

	VertexArrayObject::~VertexArrayObject() {
		glDeleteBuffers(this->numBuffers + 1, this->vboIDs);
		glDeleteVertexArrays(1, &(this->vaoID));
		delete[] this->vboIDs;
	}

	unsigned int VertexArrayObject::getID() { return this->vaoID; }
	unsigned int * VertexArrayObject::getVboIDs() { return this->vboIDs; }

	void VertexArrayObject::setBufferData(unsigned int bufferIndex, int size, GLvoid * data) {
		glBindBuffer(GL_ARRAY_BUFFER, this->vboIDs[bufferIndex]);
		glBufferData(GL_ARRAY_BUFFER, size, data, this->usage);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexArrayObject::setVertexAttribute(unsigned int bufferIndex, unsigned int attribIndex, int attribSize, GLenum dataType) {
		this->setVertexAttribute(bufferIndex, attribIndex, attribSize, dataType, GL_FALSE, 0, (GLvoid*)0);
	}

	void VertexArrayObject::setVertexAttribute(unsigned int bufferIndex, unsigned int attribIndex, int attribSize, GLenum dataType,
		GLboolean normalized, GLsizei stride, GLvoid * pointer) {
			
			glBindVertexArray(this->vaoID);
			glBindBuffer(GL_ARRAY_BUFFER, this->vboIDs[bufferIndex]);
			
			glVertexAttribPointer(attribIndex, attribSize, dataType, normalized, stride, pointer);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
	}

	void VertexArrayObject::setElementsData(unsigned int dataSize, unsigned int numElements, GLvoid * data) {
		glBindVertexArray(this->vaoID);

		this->numElements = numElements;

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vboIDs[this->numBuffers]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize * this->numElements, data, this->usage);

		glBindVertexArray(0);
	}

	void VertexArrayObject::enableVertexAttribute(unsigned int attribIndex) {
		glBindVertexArray(this->vaoID);
		glEnableVertexAttribArray(attribIndex);
		glBindVertexArray(0);
	}

	void VertexArrayObject::disableVertexAttribute(unsigned int attribIndex) {
		glBindVertexArray(this->vaoID);
		glEnableVertexAttribArray(attribIndex);
		glBindVertexArray(0);
	}

	unsigned int VertexArrayObject::getElementsCount() { return this->numElements; }

}