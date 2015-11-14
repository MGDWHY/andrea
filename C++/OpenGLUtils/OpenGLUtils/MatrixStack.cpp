#include "MatrixStack.h"

namespace glutils {

	MatrixStack::MatrixStack(unsigned int depthLevels) {
		this->depthLevels = depthLevels;

		stack = new mat4[depthLevels];

		currentDepth = 0;
	}
	MatrixStack::~MatrixStack() {
		delete[] stack;
	}

	mat4& MatrixStack::current() { return stack[currentDepth];}

	void MatrixStack::loadIdentity() { stack[currentDepth].toIdentity();}

	void MatrixStack::translate(float tx, float ty, float tz) {
		mat4 mtranslate;

		mtranslate.toIdentity();

		mtranslate.elements[3] = tx;
		mtranslate.elements[7] = ty;
		mtranslate.elements[11] = tz;

		multiply(mtranslate);
	}
	void MatrixStack::rotate(float theta, float x, float y, float z) {
		float ct = cos(theta);
		float st = sin(theta);

		mat4 mrotate;

		mrotate.elements[0] = ct + x * x * (1.0f - ct);
		mrotate.elements[1] = x * y * (1.0f - ct) - z * st;
		mrotate.elements[2] = x * z * (1.0f - ct) + y * st;
		mrotate.elements[3] = 0.0f;

		mrotate.elements[4] = y * x * (1.0f - ct) + z * st;
		mrotate.elements[5] = ct + y * y * (1.0f - ct);
		mrotate.elements[6] = y * z * (1.0f - ct) - x * st;
		mrotate.elements[7] = 0.0f;

		mrotate.elements[8] = z * x * (1.0f - ct) - y * st;
		mrotate.elements[9] = z * y * (1.0f - ct) + x * st;
		mrotate.elements[10] = ct + z * z * (1.0f - ct);
		mrotate.elements[11] = 0.0f;

		mrotate.elements[12] = 0.0f;
		mrotate.elements[13] = 0.0f;
		mrotate.elements[14] = 0.0f;
		mrotate.elements[15] = 1.0f;

		multiply(mrotate);
	}
	void MatrixStack::scale(float sx, float sy, float sz) {
		mat4 mscale;

		mscale.elements[0] = sx;
		mscale.elements[5] = sy;
		mscale.elements[10] = sz;
		mscale.elements[15] = 1.0f;

		multiply(mscale);
	}

	void MatrixStack::ortho(float left, float right, float bottom, float top, float zNear, float zFar) {
		mat4 mortho;

		float width = right - left;
		float height = top - bottom;
		float length = zFar - zNear;

		mortho.elements[0] = 2.0f / width;
		mortho.elements[1] = 0.0f;
		mortho.elements[2] = 0.0f;
		mortho.elements[3] = -(left + right) / width;

		mortho.elements[4] = 0.0f;
		mortho.elements[5] = 2.0f / height;
		mortho.elements[6] = 0.0f;
		mortho.elements[7] = -(top + bottom) / height;

		mortho.elements[8] = 0.0f;
		mortho.elements[9] = 0.0f;
		mortho.elements[10] = 2.0f / length;
		mortho.elements[11] = (zNear + zFar) / length;

		mortho.elements[12] = 0.0f;
		mortho.elements[13] = 0.0f;
		mortho.elements[14] = 0.0f;
		mortho.elements[15] = 1.0f;

		stack[currentDepth] = mortho;
	}
	void MatrixStack::perspective(float fovY, float aspect, float zNear, float zFar) {
		
		mat4 mperspective;

		float f = 1.0f / tan(fovY/2.0f);
		float diff = zNear - zFar;

		mperspective.elements[0] = f / aspect;
		mperspective.elements[1] = 0.0f;
		mperspective.elements[2] = 0.0f;
		mperspective.elements[3] = 0.0f;

		mperspective.elements[4] = 0.0f;
		mperspective.elements[5] = f;
		mperspective.elements[6] = 0.0f;
		mperspective.elements[7] = 0.0f;

		mperspective.elements[8] = 0.0f;
		mperspective.elements[9] = 0.0f;
		mperspective.elements[10] = (zFar + zNear) / diff;
		mperspective.elements[11] = 2.0f * (zNear * zFar) / diff;

		mperspective.elements[12] = 0.0f;
		mperspective.elements[13] = 0.0f;
		mperspective.elements[14] = -1.0f;
		mperspective.elements[15] = 0.0f;

		multiply(mperspective);
	}

	void MatrixStack::Stereo(float fovY, float aspect, float zNear, float zFar, float eyeShift, float zScreen) {
		float top = zNear * atan(fovY / 2.0f);
		float bottom = -top;
		float right = top * aspect;
		float left = bottom * aspect;
		float frustumShift = eyeShift * zNear / zScreen;

		this->Frustum(left - frustumShift, right - frustumShift, top, bottom, zNear, zFar); 
	}

	void MatrixStack::Frustum(float left, float right, float top, float bottom, float near, float far) {
		mat4 mfrustum;

		float a, b, c, d;

		a = (right + left) / (right - left);
		b = (top + bottom) / (top - bottom);
		c = -(far + near) / (far - near);
		d = -2.0f * far * near / (far - near);

		mfrustum.elements[0] = 2 * near / (right - left);
		mfrustum.elements[1] = 0.0f;
		mfrustum.elements[2] = a;
		mfrustum.elements[3] = 0.0f;

		mfrustum.elements[4] = 0.0f;
		mfrustum.elements[5] = 2 * near / (top - bottom);
		mfrustum.elements[6] = b;
		mfrustum.elements[7] = 0.0f;

		mfrustum.elements[8] = 0.0f;
		mfrustum.elements[9] = 0.0f;
		mfrustum.elements[10] = c;
		mfrustum.elements[11] = d;

		mfrustum.elements[12] = 0.0f;
		mfrustum.elements[13] = 0.0f;
		mfrustum.elements[14] = -1.0f;
		mfrustum.elements[15] = 0.0f;

		this->multiply(mfrustum);

	}


	void MatrixStack::lookat(vec3 eye, vec3 target, vec3 up) {
		vec3 w = normalize(sub(eye, target));
		vec3 v = cross(normalize(up), w);
		vec3 u = cross(w, v);

		mat4 rotate;

		rotate.elements[0] = v.x;
		rotate.elements[1] = v.y;
		rotate.elements[2] = v.z;
		rotate.elements[3] = 0.0;

		rotate.elements[4] = u.x;
		rotate.elements[5] = u.y;
		rotate.elements[6] = u.z;
		rotate.elements[7] = 0.0;
	
		rotate.elements[8] = w.x;
		rotate.elements[9] = w.y;
		rotate.elements[10] = w.z;
		rotate.elements[11] = 0.0;

		rotate.elements[12] = 0.0;
		rotate.elements[13] = 0.0;
		rotate.elements[14] = 0.0;
		rotate.elements[15] = 1.0;

		multiply(rotate);
		translate(-eye.x, -eye.y, -eye.z);
	}

	void MatrixStack::LoadMatrix(const mat4 &m) {
		this->stack[this->currentDepth] = m;
	}


	void MatrixStack::multiply(const mat4 &m) { stack[currentDepth] = mult(stack[currentDepth], m); }

	void MatrixStack::push() {
		#ifdef GL_UTILS_LOG_ENABLED
			if(currentDepth == depthLevels - 1)
				Logger::getDefaultLogger()->writeMessage(1, "MatrixStack::push()", "Reached maximum stack depth");
		#endif
		if(currentDepth < depthLevels - 1) {
			currentDepth++;
			stack[currentDepth] = stack[currentDepth-1];
		}
	}
	void MatrixStack::pop() {
		#ifdef GL_UTILS_LOG_ENABLED
			if(currentDepth == 0)
				Logger::getDefaultLogger()->writeMessage(1, "MatrixStack::pop()", "Reached stack depth 0");
		#endif
		if(currentDepth > 0)
			currentDepth--;
	}

}