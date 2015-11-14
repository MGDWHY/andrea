#pragma once

#include <string>
#include "vecmath.h"
#include "ShaderManager.h"
#include "MatrixStack.h"
#include "VertexArrayObject.h"

using namespace std;
using namespace glutils;

class GLFont {

	static const GLfloat DEFAULT_SIZE;

	struct Vertex {
		vec2 position;
		vec4 color;
		GLuint asciiCode;
		GLfloat padding[1];
	};

	GLfloat size;

	vec4 color;

	Shader * fontShader;

	GLuint fontTexture;

	VertexArrayObject * objVao;

public:

	GLFont(GLuint fontTexture);
	~GLFont(void);

	void setSize(GLfloat size);
	GLfloat getSize();

	void setColor(vec4 color);

	void strokeString(string s, mat4 modelViewProjectionMatrix);
};

