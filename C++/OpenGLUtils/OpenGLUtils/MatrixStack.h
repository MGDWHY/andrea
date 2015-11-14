#pragma once

#include "Logger.h"
#include "vecmath.h"

using namespace Vecmath;

namespace glutils {
	class MatrixStack {
	private:
		mat4 *stack;
		unsigned int currentDepth;

		unsigned int depthLevels;
	public:
		MatrixStack(unsigned int maxDepth);
		~MatrixStack();

		mat4& current();

		void loadIdentity();
		void LoadMatrix(const mat4 &m);

		void translate(float tx, float ty, float tz);
		void rotate(float theta, float x, float y, float z);
		void scale(float sx, float sy, float sz);

		void ortho(float left, float right, float bottom, float top, float zNear, float zFar);
		void perspective(float fovY, float aspect, float zNear, float zFar);
		void Frustum(float left, float right, float top, float bottom, float near, float far);
		void Stereo(float fovY, float aspect, float zNear, float zFar, float eyeShift, float zScreen);
		void lookat(vec3 eye, vec3 target, vec3 up);

		void multiply(const mat4 &m);

		void push();
		void pop();
	};
}