#include "VertexBufferObject.h"

namespace glutils {

	VertexBufferObject::VertexBufferObject() {
		glGenBuffers(1, &vboID);
	}

	VertexBufferObject::~VertexBufferObject() {
		glDeleteBuffers(1, &vboID);
	}


}