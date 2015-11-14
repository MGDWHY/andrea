#pragma once

#include <gl\glew.h>

namespace glutils {

	class FrameBufferObject {
	private:
		unsigned int bufferID;
		unsigned int width, height;
	public:
		FrameBufferObject(unsigned int width, unsigned int height);
		~FrameBufferObject();

		unsigned int createAttachment(int internalFormat, GLenum format, GLenum dataType);

		void attach(unsigned int slot, unsigned int texture, unsigned int target);
		void attach(unsigned int slot, unsigned int texture);

		unsigned int getWidth();
		unsigned int getHeight();

		unsigned int getFrameBufferID();

		void enable();

		void disable();
	};

}