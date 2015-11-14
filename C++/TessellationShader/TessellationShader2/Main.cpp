#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glut32.lib")

#include <Windows.h>
#include <gl\glew.h>
#include <gl\glut.h>
#include <ShaderManager.h>
#include <MatrixStack.h>
#include <wfo.h>
#include <VertexArrayObject.h>
#include <vecmath.h>
#include <lodepng.h>
#include <iostream>

#define GL_ERROR_CHECK { int x = glGetError(); if(x != GL_NO_ERROR) std::cout << "GL Error: " << x << std::endl; }
#define PI 3.141592f

struct Vertex {
	vec3 Position;
};

using namespace Vecmath;
using namespace WaveFront;
using namespace glutils;


void InitGL();
void InitShaders();
void InitBuffers();
void InitTextures();
void Render();
void Reshape(int, int);
void Keyboard(unsigned char, int, int);
void MouseMotion(int, int);
void Mouse(int, int, int, int);
void PngLoad(const char* filename);
GLuint PngTexture(const char * fileName, GLint minFilter, GLint magFilter);

MatrixStack *gStack;
Shader *gBezierShader;
Shader *gLineShader;
VertexArrayObject *gVAO;

mat4 gModelView, gProjection;

float gWidth = 640, gHeight = 480;
float gTessLevel = 2.0f;
float rotX = 0.0f, rotY = 0.0f;
float prevX, prevY;
float zCam = -5;

bool dragging = false;

Vertex CPS[] = {
	// z = - 3
	{vec3(-3,	4,	-3)},
	{vec3(-1,	1,	-3)},
	{vec3(1,	-5,	-3)},
	{vec3(3,	2,	-3)},

	// z = -1
	{vec3(-3,	2,	-1)},
	{vec3(-1,	4,	-1)},
	{vec3(1,	-3,	-1)},
	{vec3(3,	-4,	-1)},

	// z = 1

	{vec3(-3,	-6,	1)},
	{vec3(-1,	3,	1)},
	{vec3(1,	4,	1)},
	{vec3(3,	1,	1)},

	// z = 3
	{vec3(-3,	0,	3)},
	{vec3(-1,	2,	3)},
	{vec3(1,	5,	3)},
	{vec3(3,	-1,	3)},
};


int main(int argc, char **argv) {
	
	// prima di TUTTO (TUTTO TUTTO TUTTO) creare la finestra sennò non funziona una mazza
	glutInit(&argc, argv);
	// rgba mode, double buffering, depth buffering, stencil buffering
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
	// window' top left corner position
	glutInitWindowPosition(0,0);
	// window's size
	glutInitWindowSize((int)gWidth, (int)gHeight);
	// create window
	glutCreateWindow("Tessellation Shader #2");
	// Finestra creata... Adesso dovrebbe andare, ma

	glewInit();
	
	InitGL();
	InitShaders();
	InitBuffers();
	InitTextures();

	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Render);
	glutKeyboardFunc(Keyboard);
	glutMotionFunc(MouseMotion);
	glutMouseFunc(Mouse);

	glutMainLoop();

}

// called when window is resized
void Reshape(int w, int h) {
	glViewport(0,0,w,h); // viewport resize
	gWidth = w;
	gHeight = h;

	gStack->loadIdentity();
	gStack->perspective(45, gWidth /gHeight, 0.1, 100);
	gProjection = gStack->current();
}

// called when window is drawn
void Render() {

	// model-view transform

	glClearColor(0,0,0.5,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	gStack->loadIdentity();
	gStack->translate(0, 0, zCam);
	gStack->rotate(rotX, 1, 0, 0);
	gStack->rotate(rotY, 0, 1, 0);
	gModelView = gStack->current();

	gLineShader->enable();

		gLineShader->setUniformMatrix("in_ProjectionMatrix", gProjection);
		gLineShader->setUniformMatrix("in_ModelViewMatrix", gModelView);

		glPointSize(10);
		glBindVertexArray(gVAO->getID());
		glDrawArrays(GL_POINTS, 0, ARRAYSIZE(CPS));
		glBindVertexArray(0);

	gLineShader->disable();
	
	gBezierShader->enable();

		gBezierShader->setUniformMatrix("in_ProjectionMatrix", gProjection);
		gBezierShader->setUniformMatrix("in_ModelViewMatrix", gModelView);
		gBezierShader->setUniform("in_TessLevel", gTessLevel);

		glPatchParameteri(GL_PATCH_VERTICES, 16);

		glBindVertexArray(gVAO->getID());
		glDrawArrays(GL_PATCHES, 0, 16);
		glBindVertexArray(0);

	gBezierShader->disable();

	glutSwapBuffers(); // swap backbuffer with frontbuffer
}

void InitGL() {
	// Init opengl(depth test, blending, lighting and so on...)

	glPolygonMode(GL_FRONT, GL_LINE);
	glPolygonMode(GL_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);

	gStack = new MatrixStack(16);

	gStack->loadIdentity();
	gStack->perspective(45, gWidth /gHeight, 0.1, 100);
	gProjection = gStack->current();

	gStack->loadIdentity();
	gStack->translate(0, 0, zCam);
	gStack->rotate(rotX, 1, 0, 0);
	gStack->rotate(rotY, 0, 1, 0);
	gModelView = gStack->current();

	GL_ERROR_CHECK

}

void InitBuffers() {
	gVAO = new VertexArrayObject(1, GL_STATIC_DRAW);

	gVAO->setBufferData(0, sizeof(Vertex) * ARRAYSIZE(CPS), &CPS);
	gVAO->setVertexAttribute(0, 0, 3, GL_FLOAT);

	gVAO->enableVertexAttribute(0);

	GL_ERROR_CHECK
}

void InitShaders() {
	gBezierShader = ShaderManager::getDefaultManager()->createShader("bezier.vert", "bezier.tsco", "bezier.tsev", "bezier.frag");
	gLineShader = ShaderManager::getDefaultManager()->createShader("line.vert", "line.frag");
}

void InitTextures() {

}

// Called by keyboard events
void Keyboard(unsigned char key, int x, int y) {
	int b[2];
	switch(key) {
	case '+':
		gTessLevel += 0.1f;
		break;
	case '-':
		gTessLevel -= 0.1f;
		break;
	case '1':
		zCam += 0.1f;
		break;
	case '2':
		zCam -= 0.1f;
		break;
	case 'w':
		glGetIntegerv(GL_POLYGON_MODE, b);
		if(b[0] == GL_LINE)
			glPolygonMode(GL_FRONT, GL_FILL);
		else
			glPolygonMode(GL_FRONT, GL_LINE);
	}

	if(gTessLevel < 1.0f)
		gTessLevel = 1.0f;
	else if(gTessLevel > 64.0f)
		gTessLevel = 64.0f;
}

void MouseMotion(int x, int y) {
	if(dragging) {
		float dx = x - prevX;
		float dy = y - prevY;
		rotX += (dy / gWidth) * PI;
		rotY += (dx / gHeight) * PI;
		prevX = x;
		prevY = y;
	}
}
void Mouse(int button, int state, int x, int y) {
	if(button == GLUT_LEFT_BUTTON) {
		if(state == GLUT_DOWN) 
		{
			prevX = x;
			prevY = y;
			dragging = true;
		}
		else
			dragging = false;
	}
}

void PngLoad(const char* filename) {
 
	unsigned int width, height;
	std::vector<unsigned char> *image = new std::vector<unsigned char>;
	unsigned error = LodePNG::decode(*image, width, height, filename);
	unsigned char* data = new unsigned char[width*height*4];

	for(unsigned int i = 0; i < width * height * 4; i++)
		data[i] = (*image)[i];

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	delete image;
	delete data;
}


GLuint PngTexture(const char * fileName, GLint minFilter, GLint magFilter) {
	GLuint result;

	glGenTextures(1, &result);
	glBindTexture(GL_TEXTURE_2D, result);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	PngLoad(fileName);

	if(minFilter == GL_LINEAR_MIPMAP_LINEAR || minFilter == GL_LINEAR_MIPMAP_NEAREST)
		glGenerateMipmap(GL_TEXTURE_2D);

	return result;

}