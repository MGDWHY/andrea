#include "GLFont.h"

const GLfloat GLFont::DEFAULT_SIZE = 20;

GLFont::GLFont(GLuint fontTexture)
{
	this->fontTexture = fontTexture;
	this->size = DEFAULT_SIZE;
	fontShader = ShaderManager::getDefaultManager()->createShader("shaders/font.vert", "shaders/font.geom", "shaders/font.frag");
	objVao = new VertexArrayObject(1, GL_DYNAMIC_DRAW);
	
}


void GLFont::setSize(GLfloat size) { this->size = size;}
GLfloat GLFont::getSize() { return this->size; }

void GLFont::setColor(vec4 color) { this->color = color; }

void GLFont::strokeString(string s, mat4 modelViewProjectionMatrix) {
	
	Vertex * data = new Vertex[s.length()];

	for(int k = 0; k < s.length(); k++) {
		data[k].position = vec2(k * this->size, 0);
		data[k].color = this->color;
		data[k].asciiCode = s.at(k);
	}

	objVao->setBufferData(0, sizeof(Vertex) * s.length(), data);
	objVao->setVertexAttribute(0, 0, 2, GL_FLOAT, GL_FALSE, 32, 0);
	objVao->setVertexAttribute(0, 1, 4, GL_FLOAT, GL_FALSE, 32, ((char*)0 + 8)); 
	objVao->setVertexAttribute(0, 2, 1, GL_FLOAT, GL_FALSE, 32, ((char*)0 + 24));
	objVao->enableVertexAttribute(0);
	objVao->enableVertexAttribute(1);
	objVao->enableVertexAttribute(2);

	this->fontShader->enable();
	this->fontShader->setUniformMatrix("in_ModelViewProjectionMatrix", modelViewProjectionMatrix);
	this->fontShader->setUniform("in_Size", (GLfloat)this->size);
	this->fontShader->setUniformTexture("in_Font", 0, this->fontTexture);

	glBindVertexArray(objVao->getID());
	glDrawArrays(GL_POINTS, 0, s.length());
	glBindVertexArray(0);

	this->fontShader->disable();
}

GLFont::~GLFont(void)
{
}
