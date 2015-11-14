#include <iostream>
#include <string>
#include <gl\glew.h>
#include <gl\glut.h>
#include "vecmath.h"
#include "VertexArrayObject.h"
#include "MatrixStack.h"
#include "ShaderManager.h"
#include "ObjectFile.h"
#include "FrameBufferObject.h"
#include "Timer.h"

#define FBO_WIDTH 2048
#define FBO_HEIGHT 2048

#define PRINT_VERTEX(v) cout << "Vertex " << v.x <<  " " << v.y << " " << v.z << endl

using namespace std;
using namespace glutils;

#define PLANE_WIDTH 40.0f
#define PLANE_DEPTH 40.0f

void InitGL();
void Render();
void Reshape(int, int);
void Keyboard(unsigned char, int, int);
void InitBuffers();
void InitShaders();
void DrawScene(Shader*,bool,mat4,mat4);
void Mouse(int,int,int,int);
void MouseMotion(int,int);
void ShowData();

struct Vertex {
	vec3 position;
	vec3 normal;
	vec2 texCoord;
};

VertexArrayObject *objPlane, *objBunny, *objQuad;
FrameBufferObject * shadowMapFBO, * shadowBlurFBO;

GLuint shadowMap, shadowMapDB;

GLuint buffer0, buffer1, shadowBlurDB;

MatrixStack * projStack, * viewStack, * modelStack;

Shader * s_shadowMapping, * s_shadowRender, * s_shadowBlur, * s_shadowConvolve;

vec4 lightVector;


GLfloat viewWidth = 640, viewHeight = 480;

GLfloat zcam = -10.0f, rotX = 0, rotY;

bool dragging = false;

int prevX, prevY;


int main(int argc, char **argv) {
	
	// prima di TUTTO (TUTTO TUTTO TUTTO) creare la finestra sennò non funziona una mazza
	glutInit(&argc, argv);
	// rgba mode, double buffering, depth buffering, stencil buffering
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	// window' top left corner position
	glutInitWindowPosition(0,0);
	// window's size
	glutInitWindowSize((int)viewWidth, (int)viewHeight);
	// create window
	glutCreateWindow("Shadow Mapping");
	// Finestra creata... Adesso dovrebbe andare, ma 

	glewInit();
	InitGL();

	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Render);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(MouseMotion);

	glutMainLoop();

}

// called when window is resized
void Reshape(int w, int h) {
	viewWidth = w;
	viewHeight = h;
}

// called when window is drawn
void Render() {
	// rendering shadow map
	static mat4 lightView, lightProj;
	static Timer * timer = new Timer();
	static float t = -1.0f;

	t += timer->dtSecs()/5.0f;
	if(t > 1.0f)
		t = -1.0f;

	lightVector = vec4(18 * t, 18, 18, 0.0);

	shadowMapFBO->enable();
		
		glViewport(0, 0, (GLsizei)FBO_WIDTH, (GLsizei)FBO_HEIGHT); 

		glCullFace(GL_FRONT);
		glClearColor(1.0,1.0,1.0,1.0);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		
		projStack->loadIdentity();
		projStack->perspective(45, 1, 0.1f, 100);

		viewStack->loadIdentity();
		vec3 temp = vec3(lightVector.x, lightVector.y, lightVector.z);
		viewStack->lookat(temp, vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));
	
		modelStack->loadIdentity();

		lightView = mat4(viewStack->current());
		lightProj = mat4(projStack->current());

		DrawScene(s_shadowMapping, true, lightView, lightProj); 

	shadowMapFBO->disable();

	
	// rendering aliased shadows ( normal pov) 

	shadowBlurFBO->attach(GL_COLOR_ATTACHMENT0, buffer0);

	shadowBlurFBO->enable();

		glViewport(0,0,(GLsizei)viewWidth,(GLsizei)viewHeight);

		glCullFace(GL_BACK);
		glClearColor(0.0,0.0,0.0,1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		projStack->loadIdentity();
		projStack->perspective(45, viewWidth / viewHeight, 0.1f, 100);

		viewStack->loadIdentity();
		viewStack->translate(0,0,zcam);
		viewStack->rotate(rotX, 1, 0, 0);
		viewStack->rotate(rotY, 0, 1, 0);

		modelStack->loadIdentity();

		DrawScene(s_shadowRender, false, lightView, lightProj); 
	
		 // swap backbuffer with frontbuffer

	shadowBlurFBO->disable();

	// blurring shadows

	shadowBlurFBO->attach(GL_COLOR_ATTACHMENT0, buffer1);

	shadowBlurFBO->enable();

		glViewport(0,0,(GLsizei)viewWidth,(GLsizei)viewHeight);

		glCullFace(GL_BACK);
		glClearColor(0.0,0.0,0.0,1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		projStack->loadIdentity();
		projStack->ortho(-1,1,-1,1,-1,1);

		s_shadowBlur->enable();
			s_shadowBlur->setUniformMatrix("in_ProjectionMatrix", projStack->current());
			s_shadowBlur->setUniformTexture("in_buffer0", 0, buffer0);
			s_shadowBlur->setUniform("in_StepX", 1.0f / viewWidth);
			s_shadowBlur->setUniform("in_StepY", 1.0f / viewHeight);

			glBindVertexArray(objQuad->getID());
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

		s_shadowBlur->disable();


	shadowBlurFBO->disable();
	
	// convolve h

	shadowBlurFBO->attach(GL_COLOR_ATTACHMENT0, buffer0);

	shadowBlurFBO->enable();

		glViewport(0,0,(GLsizei)viewWidth,(GLsizei)viewHeight);

		glCullFace(GL_BACK);
		glClearColor(0.0,0.0,0.0,1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		projStack->loadIdentity();
		projStack->ortho(-1,1,-1,1,-1,1);

		s_shadowConvolve->enable();
		s_shadowConvolve->setUniformMatrix("in_ProjectionMatrix", projStack->current());
		s_shadowConvolve->setUniformTexture("in_BlurredShadows", 0, buffer1);
		s_shadowConvolve->setUniform("in_ConvolveH", true);
		s_shadowConvolve->setUniform("in_Step", 1.0f / viewWidth);

		glBindVertexArray(objQuad->getID());
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		s_shadowConvolve->disable();

	shadowBlurFBO->disable();

	// convolve v

	shadowBlurFBO->attach(GL_COLOR_ATTACHMENT0, buffer1);

	shadowBlurFBO->enable();

		glViewport(0,0,(GLsizei)viewWidth,(GLsizei)viewHeight);

		glCullFace(GL_BACK);
		glClearColor(0.0,0.0,0.0,1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		projStack->loadIdentity();
		projStack->ortho(-1,1,-1,1,-1,1);

		s_shadowConvolve->enable();
		s_shadowConvolve->setUniformMatrix("in_ProjectionMatrix", projStack->current());
		s_shadowConvolve->setUniformTexture("in_BlurredShadows", 0, buffer0);
		s_shadowConvolve->setUniform("in_ConvolveH", false);
		s_shadowConvolve->setUniform("in_Step", 1.0f / viewHeight);

		glBindVertexArray(objQuad->getID());
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		s_shadowConvolve->disable();

	shadowBlurFBO->disable();

	
	// Prova depth texture
	
	glViewport(0,0,(GLsizei)viewWidth,(GLsizei)viewHeight);

	glPushAttrib(GL_ENABLE_BIT);

	glActiveTexture(GL_TEXTURE0);
	
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, buffer1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1,1,-1,1,-1,1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1,1,1);

	glBegin(GL_QUADS);
		glTexCoord2f(0,1); glVertex3f(-1, 1, 0);
		glTexCoord2f(0,0); glVertex3f(-1, -1, 0);
		glTexCoord2f(1,0); glVertex3f(1, -1, 0);
		glTexCoord2f(1,1); glVertex3f(1, 1, 0);
	glEnd();

	glPopAttrib();

	if(glGetError())
		cout << "Error";

	glutSwapBuffers();
}

void DrawScene(Shader * shader, bool isShadowMap, mat4 lightViewMatrix, mat4 lightProjectionMatrix) {
	shader->enable();

	shader->setUniformMatrix("in_ModelMatrix", modelStack->current());
	shader->setUniformMatrix("in_ViewMatrix", viewStack->current());
	shader->setUniformMatrix("in_ProjectionMatrix", projStack->current());
	shader->setUniformMatrix("in_LightViewMatrix", lightViewMatrix);
	shader->setUniformMatrix("in_LightProjectionMatrix", lightProjectionMatrix);
	shader->setUniform("in_LightVector", multvec(viewStack->current(), lightVector)); 

	if(!isShadowMap) {
		shader->setUniformTexture("in_Shadows", 0, shadowMapDB);
		shader->setUniformTexture("in_ShadowsR", 1, shadowMap);
	} else {
		shader->setUniformTexture("in_Shadows", 0, 0);
		shader->setUniformTexture("in_ShadowsR", 1, 0);
	}
	glBindVertexArray(objPlane->getID());

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(objBunny->getID());

	modelStack->push();
	modelStack->translate(0, 5, 0);
	modelStack->scale(3.0,3.0,3.0);
	shader->setUniformMatrix("in_ModelMatrix", modelStack->current());
	glDrawElements(GL_TRIANGLES, objBunny->getElementsCount(), GL_UNSIGNED_INT, 0);
	modelStack->pop();

	for(int i = -3; i < 4; i++)
		for(int j = -3; j < 4; j++) {
			modelStack->push();
			modelStack->translate(i*1.5, 3.0 + (i+j)/3.0f, j*1.5);
			modelStack->scale(1.0,1.0,1.0);
			shader->setUniformMatrix("in_ModelMatrix", modelStack->current());
			glDrawElements(GL_TRIANGLES, objBunny->getElementsCount(), GL_UNSIGNED_INT, 0);
			modelStack->pop();
		}
	glBindVertexArray(0);

	shader->disable();
}

void InitGL() {
	// Init opengl(depth test, blending, lighting and so on...)

	glEnable(GL_DEPTH_TEST);

	//glDisable(GL_CULL_FACE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	viewStack = new MatrixStack(16);
	modelStack = new MatrixStack(16);
	projStack = new MatrixStack(4);

	InitBuffers();
	InitShaders();
}

// Called by keyboard events
void Keyboard(unsigned char key, int x, int y) {
	switch(key) {
		case 'l':
			ShowData();
			break;
		case '+':
			zcam += 0.1;
			break;
		case '-':
			zcam -= 0.1;
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

void ShowData() {
	float * data = new float[(int)(FBO_WIDTH*FBO_HEIGHT)];
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glGetTexImage(GL_TEXTURE_2D,0,GL_RED,GL_FLOAT,data);

	for(int i = 0; i < FBO_WIDTH*FBO_HEIGHT; i++)
		if(data[i] != 1.0)
			cout << data[i] << endl;
}

void InitBuffers() {
	Vertex *planeVerts = new Vertex[4];
	Vertex *quadVerts = new Vertex[4];
	GLuint planeIndices[6] = {0, 3, 2, 2, 1, 0};
	GLuint quadIndices[6] = {0, 1, 2, 2, 3, 0};

	planeVerts[0].position = vec3(-PLANE_WIDTH / 2, 0.0f, -PLANE_DEPTH / 2);
	planeVerts[1].position = vec3(PLANE_WIDTH / 2, 0.0f, -PLANE_DEPTH / 2);
	planeVerts[2].position = vec3(PLANE_WIDTH / 2, 0.0f, PLANE_DEPTH / 2);
	planeVerts[3].position = vec3(-PLANE_WIDTH / 2, 0.0f, PLANE_DEPTH / 2);

	
	for(int i = 0; i < 4; i++)
		planeVerts[i].normal = vec3(0, 1, 0);

	objPlane = new VertexArrayObject(1, GL_STATIC_DRAW);
	objPlane->setBufferData(0, sizeof(Vertex) * 4, planeVerts);
	objPlane->setElementsData(sizeof(GLuint), 6, planeIndices);
	objPlane->setVertexAttribute(0, 0, 3, GL_FLOAT, GL_FALSE, 32, 0);
	objPlane->setVertexAttribute(0, 1, 3, GL_FLOAT, GL_FALSE, 32, (char*)0 + 12);
	objPlane->enableVertexAttribute(0);
	objPlane->enableVertexAttribute(1);

	ObjectFile * objLoader = ObjectFile::load("dragon.obj");
	objBunny = objLoader->createVertexArrayObject(NULL, NULL, GL_STATIC_DRAW);
	objBunny->enableVertexAttribute(0);
	objBunny->enableVertexAttribute(1);

	quadVerts[0].position = vec3(-1, 1, 0);
	quadVerts[1].position = vec3(-1, -1, 0);
	quadVerts[2].position = vec3(1, -1, 0);
	quadVerts[3].position = vec3(1, 1, 0);

	quadVerts[0].texCoord = vec2(0, 1);
	quadVerts[1].texCoord = vec2(0, 0);
	quadVerts[2].texCoord = vec2(1, 0);
	quadVerts[3].texCoord = vec2(1, 1);

	objQuad = new VertexArrayObject(1, GL_STATIC_DRAW);
	objQuad->setBufferData(0, sizeof(Vertex) * 4, quadVerts);
	objQuad->setElementsData(sizeof(GLuint), 6, quadIndices);

	objQuad->setVertexAttribute(0, 0, 3, GL_FLOAT, GL_FALSE, 32, 0);
	objQuad->setVertexAttribute(0, 1, 2, GL_FLOAT, GL_FALSE, 32, (char*)0+24);

	objQuad->enableVertexAttribute(0);
	objQuad->enableVertexAttribute(1);


	shadowMapFBO = new FrameBufferObject(FBO_WIDTH, FBO_HEIGHT);

	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, shadowMapFBO->getWidth(), shadowMapFBO->getHeight(),
		0, GL_RED, GL_UNSIGNED_INT, 0);


	glGenTextures(1, &shadowMapDB);
	glBindTexture(GL_TEXTURE_2D, shadowMapDB);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, shadowMapFBO->getWidth(), shadowMapFBO->getHeight(),
		0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0);

	shadowMapFBO->attach(GL_DEPTH_ATTACHMENT, shadowMapDB, GL_TEXTURE_2D);
	shadowMapFBO->attach(GL_COLOR_ATTACHMENT0, shadowMap, GL_TEXTURE_2D);
	
	shadowBlurFBO = new FrameBufferObject(viewWidth, viewHeight);

	glGenTextures(1, &buffer1);
	glBindTexture(GL_TEXTURE_2D, buffer1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLint)viewWidth, (GLint)viewHeight,
		0, GL_RGBA, GL_UNSIGNED_INT, 0);

	glGenTextures(1, &buffer0);
	glBindTexture(GL_TEXTURE_2D, buffer0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLint)viewWidth, (GLint)viewHeight,
		0, GL_RGBA, GL_UNSIGNED_INT, 0);

	glGenTextures(1, &shadowBlurDB);
	glBindTexture(GL_TEXTURE_2D, shadowBlurDB);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, (GLint)viewWidth, (GLint)viewHeight,
		0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0);

	shadowBlurFBO->attach(GL_COLOR_ATTACHMENT0, buffer0);
	shadowBlurFBO->attach(GL_DEPTH_ATTACHMENT, shadowBlurDB);

}
void InitShaders() {
	ShaderManager * manager = ShaderManager::getDefaultManager();
	s_shadowMapping = manager->createShader("shadows.vert", "shadows.frag");
	s_shadowRender = manager->createShader("render_shadows.vert", "render_shadows.frag");
	s_shadowBlur = manager->createShader("blur_shadows.vert", "blur_shadows.frag");
	s_shadowConvolve = manager->createShader("convolve_shadows.vert", "convolve_shadows.frag");
}