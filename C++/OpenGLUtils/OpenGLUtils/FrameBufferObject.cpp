#include "FrameBufferObject.h"

namespace glutils {
	
	FrameBufferObject::FrameBufferObject(unsigned int width, unsigned int height) {
		this->width = width;
		this->height = height;
		glGenFramebuffers(1, &bufferID);
	}

	FrameBufferObject::~FrameBufferObject() {}

	// deprecated (compatibility only)
	void FrameBufferObject::attach(unsigned int slot, unsigned int texture, unsigned int target) {
		enable();
		//glFramebufferTexture2D(GL_FRAMEBUFFER, slot, target, texture, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, slot, texture, 0);
		disable();
	}


	void FrameBufferObject::attach(unsigned int slot, unsigned int texture) {
		enable();
		glFramebufferTexture(GL_FRAMEBUFFER, slot, texture, 0);
		disable();
	}

	unsigned int FrameBufferObject::getWidth() { return width;}
	unsigned int FrameBufferObject::getHeight() { return height;}

	unsigned int FrameBufferObject::getFrameBufferID() {
		return bufferID;
	}

	void FrameBufferObject::enable() {
		glBindFramebuffer(GL_FRAMEBUFFER, bufferID);
	}

	void FrameBufferObject::disable() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	unsigned int FrameBufferObject::createAttachment(int internalFormat, GLenum format, GLenum dataType) {
		unsigned int attach;

		glPushAttrib(GL_TEXTURE_BIT);

		glGenTextures(1, &attach);
		glBindTexture(GL_TEXTURE_2D, attach);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, dataType, 0);

		glPopAttrib();

		return attach;
	}
}