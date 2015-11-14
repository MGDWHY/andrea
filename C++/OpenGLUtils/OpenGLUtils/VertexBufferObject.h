#pragma once

#include <gl/glew.h>
#include <gl/glut.h>

namespace glutils {

	class VertexBufferObject {
	private:
		GLuint vboID;
	public:
		VertexBufferObject();
		~VertexBufferObject();
	};
}