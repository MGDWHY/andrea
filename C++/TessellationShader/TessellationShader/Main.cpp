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
	vec2 TexCoord;
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

Shader * gShader;
MatrixStack * gStack;
VertexArrayObject * gVAO;
GLuint gHeightMap, gNormalMap, gTexture;
mat4 gModelView, gProjection;

float gWidth = 640, gHeight = 480;
float gTessLevel = 2.0f;
int gTessMode = 0;


float rotX = 0.0f, rotY = 0.0f;
float prevX, prevY;
float zCam = -3;

bool dragging = false;

Vertex vertices[] = {
	{ vec3(-1, 0, 1), vec2(0, 0) },
	{ vec3(1, 0, 1), vec2(1, 0)}, 
	{ vec3(-1, 0, -1), vec2(0, 1)},
	{ vec3(-1, 0, -1), vec2(0, 1)},
	{ vec3(1, 0, 1), vec2(1, 0)},
	{ vec3(1, 0, -1), vec2(1, 1)}
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
	glutCreateWindow("Tessellation Shader #1");
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
	gStack->perspective(45, gWidth / gHeight, 0.1, 100);
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

	gShader->enable();
	gShader->setUniform("in_TessLevel", gTessLevel);
	gShader->setUniform("in_SurfaceNormal", vec3(0, 1, 0));
	gShader->setUniform("in_SurfaceTangent", vec3(1, 0, 0));
	gShader->setUniform("in_Viewport", vec2(gWidth, gHeight));
	gShader->setUniform("in_TessMode", gTessMode);
	gShader->setUniformTexture("in_Texture", 2, gTexture, GL_TEXTURE_2D);
	gShader->setUniformTexture("in_NormalMap", 1, gNormalMap, GL_TEXTURE_2D);
	gShader->setUniformTexture("in_HeightMap", 0, gHeightMap, GL_TEXTURE_2D);
	gShader->setUniformMatrix("in_ProjectionMatrix", gProjection);
	gShader->setUniformMatrix("in_ModelViewMatrix", gModelView);
	
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	glBindVertexArray(gVAO->getID());
	glDrawElements(GL_PATCHES, gVAO->getElementsCount(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	gShader->disable();

	glutSwapBuffers(); // swap backbuffer with frontbuffer
}

void InitGL() {
	// Init opengl(depth test, blending, lighting and so on...)

	glPolygonMode(GL_FRONT, GL_LINE);
	//glPolygonMode(GL_BACK, GL_LINE);
	
	
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	gStack = new MatrixStack(16);

	gStack->loadIdentity();
	gStack->perspective(45, gWidth / gHeight, 0.1, 100);

	gProjection = gStack->current();

	gStack->loadIdentity();
	gStack->translate(0, 0, -3);

	gModelView = gStack->current();

	GL_ERROR_CHECK

}

void InitBuffers() {

	WFObject * obj = WFObject::FromFile("plane.obj");
	vector<GLuint> indexes = vector<GLuint>();
	
	vector<WFFace*> * faces = obj->GetFaces();

	for(int i = 0; i < faces->size(); i++)
		for(int j = 0; j < 3; j++)
			indexes.push_back(faces->at(i)->GetVertexIndex(j));

	gVAO = new VertexArrayObject(2, GL_STATIC_DRAW);

	gVAO->setBufferData(0, sizeof(vec3) * obj->GetVertices()->size(), &(obj->GetVertices()->at(0)));
	gVAO->setBufferData(1, sizeof(vec2) * obj->GetTexCoords()->size(), &(obj->GetTexCoords()->at(0)));
	gVAO->setElementsData(sizeof(GLuint), indexes.size(), &indexes[0]);

	gVAO->setVertexAttribute(0, 0, 3, GL_FLOAT);
	gVAO->setVertexAttribute(1, 1, 2, GL_FLOAT);

	gVAO->enableVertexAttribute(0);
	gVAO->enableVertexAttribute(1);

	delete obj;

	GL_ERROR_CHECK
}

void InitShaders() {
	gShader = ShaderManager::getDefaultManager()->createShader("shader.vert", "shader.tsco", "shader.tsev", "shader.frag");
}

void InitTextures() {
	gTexture = PngTexture("rocks.png", GL_LINEAR,GL_LINEAR);
	gHeightMap = PngTexture("rocksmap.png", GL_LINEAR, GL_LINEAR);
	gNormalMap = PngTexture("rocksnormal.png", GL_LINEAR, GL_LINEAR);
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