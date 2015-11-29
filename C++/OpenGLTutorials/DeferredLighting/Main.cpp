#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <gl/glew.h>
#include <gl/glut.h>
#include "vecmath.h"
#include "lodepng.h"
#include "MatrixStack.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "FrameBufferObject.h"
#include "Timer.h"
#include "ObjectFile.h"
#include "VertexArrayObject.h"
#include "MaterialLibrary.h"

using namespace std;
using namespace glutils;

#define PLANE_TESSELLATION 40
#define NUM_ELEMENTS 6 * (PLANE_TESSELLATION - 1) * (PLANE_TESSELLATION - 1)

#define F_PLANE(x,z) vec3(x, sin((GLfloat)x) + cos((GLfloat)z), z)
#define DFX_PLANE(x,z) vec3(1, cos((GLfloat)x), 0)
#define DFZ_PLANE(x,z) vec3(0, -sin((GLfloat)z), 1)

#define MAX_LIGHTS 20

#define RND (float)rand() / RAND_MAX

struct Vertex {
	vec4 position;
	vec3 normal;
	vec3 tangent;
	vec2 texCoords;
	GLfloat padding[4];
};

void InitGL();
void InitLights();
void Render();
void Reshape(int, int);
void Keyboard(unsigned char, int, int);
void Mouse(int button, int state, int x, int y);
void MouseMotion(int x, int y);
vec4 RandomPos();
void SendLightsToShader();
void BuildSurface();

struct light {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 position;
	vec3 spotDirection;
	GLfloat spotCutoff;
	GLfloat constantAttenuation, linearAttenuation, quadraticAttenuation;
	GLfloat maxDistance;
	light() {
		ambient = vec4(0.0f, 0.0f, 0.0f, 1.0f);
		diffuse = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		specular = vec4(0.0f, 0.0f, 0.0f, 1.0f);
		position = vec4(0.0f, 0.0f, 0.0f, 1.0f);
		spotDirection = vec3(0.0f, -1.0f, 0.0f);
		constantAttenuation = 1.0f;
		linearAttenuation = 1.0f;
		quadraticAttenuation = 0.0f;
		maxDistance = -1.0f;
		spotCutoff = 180.0f; 
	}
};


GLfloat screenTexCoords[4][2] = {
	{0, 0},
	{1, 0},
	{1, 1},
	{0, 1},
};


GLuint screenIndices[] = { 0, 1, 2, 2, 3, 0};

light lights[MAX_LIGHTS];

vec4 initLightPos[MAX_LIGHTS];

GLfloat viewWidth = 640, viewHeight = 480;

Shader *store, *render;

FrameBufferObject *renderTargets;

ObjectFile *lightMesh;
VertexArrayObject *screen, *lgt, *surface;

MatrixStack *projStack, *viewStack;

GLuint drawBuffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };

GLuint texNormal, texColor, texDepth;

GLuint texDiffuseMat, texNormalMat;

GLfloat zcam = -PLANE_TESSELLATION / 2.0f, rotX = 0, rotY;

bool dragging = false;

int prevX, prevY;

GLint debugMode = 0;

void pngLoad(const char* filename) {
 
  unsigned int width, height;
  std::vector<unsigned char> *image = new std::vector<unsigned char>;
  unsigned error = LodePNG::decode(*image, width, height, filename);
  unsigned char* data = new unsigned char[width*height*4];

  for(unsigned int i = 0; i < width * height * 4; i++)
	  data[i] = (*image)[i];


  gluBuild2DMipmaps ( GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data );

  delete image;
  delete data;
}
GLuint pngTexture(const char* filename) {
	GLuint texid;

	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	pngLoad(filename);

	#ifdef GL_UTILS_LOG_ENABLED
		stringstream msgss;
		msgss << "Texture loaded: " << filename;
		Logger::getDefaultLogger()->writeMessageStream(0, "pngTexture()", msgss);
	#endif

	return texid;
}

int main(int argc, char **argv) {
	
	// prima di TUTTO (TUTTO TUTTO TUTTO) creare la finestra sennò non funziona una mazza
	glutInit(&argc, argv);
	// rgba mode, double buffering, depth buffering, stencil buffering
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	// window' top left corner position
	glutInitWindowPosition(0,0);
	// window's size
	glutInitWindowSize(640, 480);
	// create window
	glutCreateWindow("Deferred Lighting");
	// Finestra creata... Adesso dovrebbe andare, ma

	glewInit();
	InitGL();

	glutReshapeFunc(Reshape);
	glutDisplayFunc(Render);
	glutIdleFunc(Render);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(MouseMotion);

	glutFullScreen();

	glutMainLoop();
	return 0;


}

// called when window is resized
void Reshape(int w, int h) {
	viewWidth = w;
	viewHeight = h;
	glViewport(0,0,w,h); // viewport resize
	glutPostRedisplay();
}

// called when window is drawn
void Render() {

	static Timer *timer = new Timer();
	static mat4 projInverse;

	projStack->loadIdentity();
	projStack->perspective(45, viewWidth/viewHeight, 0.1, 50);

	viewStack->loadIdentity();

	projInverse = invert(projStack->current());

	for(int i = 0; i < MAX_LIGHTS;  i++) {
		lights[i].position = initLightPos[i];
		lights[i].position.y += sin(timer->timeSecs() * 4 + lights[i].position.x);
		lights[i].position.x += sin(timer->timeSecs() + lights[i].position.x) * 4.0f;
	}

	renderTargets->enable();

	glDrawBuffers(2, drawBuffers);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack->push();	
	viewStack->translate(0.0, -0.5f, zcam);
	viewStack->rotate(rotX, 1, 0, 0);
	viewStack->rotate(rotY, 0, 1, 0);
	viewStack->translate(-PLANE_TESSELLATION / 2, 0.0f, -PLANE_TESSELLATION / 2);

	store->enable();
	store->setUniform("in_UseTexture", true);
	store->setUniformTexture("in_TexDiffuse", 0, texDiffuseMat);
	store->setUniformTexture("in_TexNormal", 1, texNormalMat);
	store->setUniform("in_Color", vec4(0.5, 0.5, 0.5, 1.0));
	store->setUniformMatrix("in_ProjectionMatrix", projStack->current());
	store->setUniformMatrix("in_ModelViewMatrix", viewStack->current());

	glBindVertexArray(surface->getID());
	glDrawElements(GL_TRIANGLES, NUM_ELEMENTS, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	viewStack->pop();

	viewStack->translate(0.0, -0.5f, zcam);
	viewStack->rotate(rotX, 1, 0, 0);
	viewStack->rotate(rotY, 0, 1, 0);

	store->disable();

	renderTargets->disable();
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texDepth);
	glGenerateMipmap(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texColor);
	glGenerateMipmap(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texNormal);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	glPushAttrib(GL_ENABLE_BIT);

	glDisable(GL_DEPTH_TEST);

	glClear(GL_COLOR_BUFFER_BIT);

	render->enable();

	render->setUniformTexture("in_TexDiffuse", 0, texColor);
	render->setUniformTexture("in_TexNormal", 1, texNormal);
	render->setUniformTexture("in_TexDepth", 2, texDepth);

	render->setUniformMatrix("in_InverseProjMatrix", projInverse);

	render->setUniform("in_DebugMode", debugMode); 
	
	for(int i = 0; i < MAX_LIGHTS;  i++)
		lights[i].position = multvec(viewStack->current(), lights[i].position);
	
	SendLightsToShader();

	glBindVertexArray(screen->getID());

	glDrawElements(GL_TRIANGLES, screen->getElementsCount(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	
	render->disable();

	glPopAttrib();
	

	glutSwapBuffers(); // swap backbuffer with frontbuffer
}

void InitGL() {
	// Init opengl(depth test, blending, lighting and so on...)

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);

	texDiffuseMat = pngTexture("textures/diffuse1.png");
	texNormalMat = pngTexture("textures/normal2.png");

	ShaderManager *manager = ShaderManager::getDefaultManager();
	store = manager->createShader("D:/Andrea/GLSL/def_lighting/def_lighting.vert", "D:/Andrea/GLSL/def_lighting/def_lighting.frag");
	render = manager->createShader("D:/Andrea/GLSL/def_lighting/render.vert", "D:/Andrea/GLSL/def_lighting/render.frag");

	screen = new VertexArrayObject(1, GL_STATIC_DRAW);
	screen->setElementsData(sizeof(GLuint), 6, screenIndices);
	screen->setBufferData(0, sizeof(GLfloat) * 4 * 2, screenTexCoords);
	screen->setVertexAttribute(0, 0, 2, GL_FLOAT);
	screen->enableVertexAttribute(0);

	BuildSurface();

	lightMesh = ObjectFile::load("models/light.obj");
	lgt = lightMesh->createVertexArrayObject(NULL, NULL, GL_STATIC_DRAW);
	lgt->enableVertexAttribute(0);
	lgt->enableVertexAttribute(1);
	lgt->enableVertexAttribute(2);

	renderTargets = new FrameBufferObject(1366, 768);

	texNormal = renderTargets->createAttachment(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
	texColor = renderTargets->createAttachment(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
	texDepth = renderTargets->createAttachment(GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE);

	renderTargets->attach(GL_COLOR_ATTACHMENT0, texNormal, GL_TEXTURE_2D);
	renderTargets->attach(GL_COLOR_ATTACHMENT1, texColor, GL_TEXTURE_2D);
	renderTargets->attach(GL_DEPTH_ATTACHMENT, texDepth, GL_TEXTURE_2D);

	viewStack = new MatrixStack(16);
	projStack = new MatrixStack(4);

	InitLights();
}

void InitLights() {
	for(int i = 0; i < MAX_LIGHTS; i++) {
		lights[i].diffuse = lights[i].specular = vec4(RND, RND, RND, 1.0);
		initLightPos[i] = lights[i].position = RandomPos();
		lights[i].quadraticAttenuation = 0.0f;
		lights[i].linearAttenuation = 1.0f;
		lights[i].maxDistance = -1.0;
	}
}

vec4 RandomPos() {
	return vec4(-10.0f + RND * 20.0f, RND + 1.0f, -10.0f + RND * 20.0f, 1.0f);
}

void SendLightsToShader() {
	char a[20]; string s;

	for(int i = 0; i < MAX_LIGHTS; i++) {
		sprintf(a, "lights[%d]", i);

		s = string(a) + ".ambient";
		render->setUniform(s.c_str(), lights[i].ambient);

		s = string(a) + ".diffuse";
		render->setUniform(s.c_str(), lights[i].diffuse);

		s = string(a) + ".specular";
		render->setUniform(s.c_str(), lights[i].specular);

		s = string(a) + ".position";
		render->setUniform(s.c_str(), lights[i].position);

		s = string(a) + ".spotDirection";
		render->setUniform(s.c_str(), lights[i].spotDirection);

		s = string(a) + ".spotCutoff";
		render->setUniform(s.c_str(), lights[i].spotCutoff);

		s = string(a) + ".constantAttenuation";
		render->setUniform(s.c_str(), lights[i].constantAttenuation);

		s = string(a) + ".linearAttenuation";
		render->setUniform(s.c_str(), lights[i].linearAttenuation);

		s = string(a) + ".quadraticAttenuation";
		render->setUniform(s.c_str(), lights[i].quadraticAttenuation);

		s = string(a) + ".maxDistance";
		render->setUniform(s.c_str(), lights[i].maxDistance);
	}
}

// Called by keyboard events
void Keyboard(unsigned char key, int x, int y) {
	switch(key) {
		case '+':
			zcam += 0.1;
			break;
		case '-':
			zcam -= 0.1;
			break;
		case 'd':
		case 'D':
			if(debugMode == 4)
				debugMode = 0;
			else
				debugMode++;
			break;
		case 'q':
			exit(0);
			break;
		default:
			break;
	}
}

void Mouse(int button, int state, int x, int y) {
	
	if(button == GLUT_LEFT_BUTTON)
		if(state == GLUT_DOWN) {
			dragging = true;
			prevX = x;
			prevY = y;
		} else {
			dragging = false;
		}
}

void MouseMotion(int x, int y) {
	if(dragging) {
		
		rotY += (x - prevX) / 50.0f;
		rotX += (y - prevY) / 50.0f;

		prevX = x;
		prevY = y;
	}
}

void BuildSurface() {

	Vertex *vertices = new Vertex[PLANE_TESSELLATION * PLANE_TESSELLATION];
	GLuint *indices = new GLuint[NUM_ELEMENTS];

	//plane

	for(int z = 0; z < PLANE_TESSELLATION; z++)
		for(int x = 0; x < PLANE_TESSELLATION; x++) {
			vertices[z*PLANE_TESSELLATION + x].position = vec4(F_PLANE(x, z), 1.0);
			vertices[z*PLANE_TESSELLATION + x].tangent = normalize(DFZ_PLANE(x,z));
			vertices[z*PLANE_TESSELLATION + x].normal = normalize(cross(DFZ_PLANE(x,z), DFX_PLANE(x,z)));
			vertices[z*PLANE_TESSELLATION + x].texCoords = vec2(x, z);
		}

	for(int z = 0; z < PLANE_TESSELLATION - 1; z++) {
		for(int x = 0; x < PLANE_TESSELLATION - 1; x++) {
			int base = 6 * (PLANE_TESSELLATION - 1) * z + x * 6;
			indices[base] = z * PLANE_TESSELLATION + x;
			indices[base + 1] = (z + 1) * PLANE_TESSELLATION + x;
			indices[base + 2] = z * PLANE_TESSELLATION + x + 1;
			indices[base + 3] = z * PLANE_TESSELLATION + x + 1;
			indices[base + 4] = (z + 1) * PLANE_TESSELLATION + x;
			indices[base + 5] = (z + 1) * PLANE_TESSELLATION + x + 1;
		}
	}

	surface = new VertexArrayObject(1, GL_STATIC_DRAW);

	surface->setElementsData(sizeof(GLuint), NUM_ELEMENTS, indices);
	surface->setBufferData(0, sizeof(Vertex) * PLANE_TESSELLATION * PLANE_TESSELLATION, vertices);
	surface->setVertexAttribute(0, 0, 4, GL_FLOAT, GL_FALSE, 64, 0);
	surface->setVertexAttribute(0, 1, 3, GL_FLOAT, GL_FALSE, 64, ((char*)0 + 16));
	surface->setVertexAttribute(0, 2, 3, GL_FLOAT, GL_FALSE, 64, ((char*)0 + 28)); 
	surface->setVertexAttribute(0, 3, 2, GL_FLOAT, GL_FALSE, 64, ((char*)0 + 40));
	surface->enableVertexAttribute(0);
	surface->enableVertexAttribute(1);
	surface->enableVertexAttribute(2);
	surface->enableVertexAttribute(3);
}