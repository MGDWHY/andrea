#pragma once

#include <gl\glew.h>
#include "Logger.h"
#include "vecmath.h"

using namespace Vecmath;

namespace glutils {
	class Shader
	{
	private:
		unsigned int vertex, fragment, geometry, tessControl, tessEvaluation, program;
		Shader(unsigned int vertexShader, unsigned int fragmentShader);
		Shader(unsigned int vertexShader, unsigned int geometryShader, unsigned int fragmentShader);
		Shader(unsigned int vertexShader, unsigned int tessControlShader, unsigned int tessEvaluationShader, unsigned int fragmentShader);
		Shader(unsigned int vertexShader, unsigned int tessControlShader, unsigned int tessEvaluationShader, unsigned int geometryShader, unsigned int fragmentShader);

		void init(unsigned int vertexShader, unsigned int tessControlShader, unsigned int tessEvaluationShader, unsigned int geometryShader, unsigned int fragmentShader);

	public:
		~Shader(void);

		void setUniformTexture(const char* name, unsigned int textureUnit, unsigned int texture);
		void setUniformTexture(const char* name, unsigned int textureUnit, unsigned int texture, GLenum target);

		void setUniform(const char* name, int v1);
		void setUniform(const char* name, int v1, int v2);
		void setUniform(const char* name, int v1, int v2, int v3);
		void setUniform(const char* name, int v1, int v2, int v3, int v4);

		void setUniform(const char* name, float v1);
		void setUniform(const char* name, float v1, float v2);
		void setUniform(const char* name, float v1, float v2, float v3);
		void setUniform(const char* name, float v1, float v2, float v3, float v4);

		void setUniform(const char* name, const vec2 &v);
		void setUniform(const char* name, const vec3 &v);
		void setUniform(const char* name, const vec4 &v);

		inline void setUniform(const char* name, const ivec2 &v);
		inline void setUniform(const char* name, const ivec3 &v);
		inline void setUniform(const char* name, const ivec4 &v);

		void setUniformMatrix(const char* name, mat4 &m);

		void enable();
		void disable();

		int getUniformLocation(const char* name);
		int getAttributeLocation(const char* name);

		unsigned int getProgram();

		friend class ShaderManager;
	};
}