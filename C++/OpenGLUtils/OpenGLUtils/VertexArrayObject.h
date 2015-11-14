#pragma once

#include <iostream>
#include <gl/glew.h>

namespace glutils {

	class VertexArrayObject {

	private:

		GLenum usage;

		unsigned int numElements, numBuffers;
		
		unsigned int vaoID;
		unsigned int * vboIDs;
		 

	public:
		VertexArrayObject(unsigned int numBuffers, GLenum usage);
		~VertexArrayObject();

		virtual unsigned int getID();

		unsigned int * getVboIDs();

		void setBufferData(unsigned int bufferIndex, GLsizeiptr size, GLvoid * data);

		void setVertexAttribute(unsigned int bufferIndex, unsigned int attribIndex, int attribSIze, GLenum dataType);
		void setVertexAttribute(unsigned int bufferIndex, unsigned int attribIndex, int attribSIze, GLenum dataType,
			GLboolean normalized, GLsizei stride, GLvoid * pointer);

		void setElementsData(unsigned int dataSize, unsigned int numElements, GLvoid * data);

		void enableVertexAttribute(unsigned int attribIndex);
		void disableVertexAttribute(unsigned int attribIndex);

		unsigned int getElementsCount();

	};
}