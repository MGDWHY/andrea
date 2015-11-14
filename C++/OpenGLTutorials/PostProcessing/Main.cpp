#include <iostream>
#include <gl\glew.h>
#include <gl\glut.h>
#include "Shader.h"
#include "ShaderManager.h"
#include "FrameBufferObject.h"
#include "tgaload.h"
#include "vecmath.h"

using namespace std;
using namespace glutils;

float angle;

GLuint fboId;

void InitGL();
void Render();
void Reshape(int, int);
void Keyboard(unsigned char, int, int);

FrameBufferObject *fbo;

GLuint texture, depth;

int width, height;

ShaderManager* manager;
Shader* convolve;

string path = "C:\\Andrea\\GLSL\\";

int main(int argc, char **argv) {
	
	// prima di TUTTO (TUTTO TUTTO TUTTO) creare la finestra sennò non funziona una mazza
	glutInit(&argc, argv);
	// rgba mode, double buffering, depth buffering, stencil buffering
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
	// window' top left corner position
	glutInitWindowPosition(0,0);
	// window's size
	glutInitWindowSize(640, 480);
	// create window
	glutCreateWindow("Window Title");
	// Finestra creata... Adesso dovrebbe andare, ma 

	manager = ShaderManager::getDefaultManager();
	convolve = manager->createShader(path + "postprocessing\\convolve.vert", path + "postprocessing\\convolve.frag");

	glewInit();

	InitGL();

	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Render);
	glutKeyboardFunc(Keyboard);

	glutMainLoop();
	return 0;
}

// called when window is resized
void Reshape(int w, int h) {
	glViewport(0,0,w,h); // viewport resize
	width = w;
	height = h;
}

// called when window is drawn
void Render() {
	// projection transform

	angle += 1;
	if(angle > 360)
		angle -= 360;

	// rendering to fbo
	fbo->enable();
	glViewport(0,0,512,512);
	
	glClearColor(1.0, 0.0, 0.0, 1.0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1,  0.1, 10);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0,0,-5);
	glRotatef(angle, 0, 1, 0);
	glColor3f(1.0, 1.0, 0.0);
	glutSolidDodecahedron();

	fbo->disable();

	glBindTexture(GL_TEXTURE_2D, texture);
	glGenerateMipmap(GL_TEXTURE_2D);

	// rendering to screen
	glViewport(0,0,width,height);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);	
	glLoadIdentity();
	glOrtho(-2.0, 2.0, -2.0, 2.0, 0.2, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	convolve->enable();
	glBegin(GL_QUADS);
		glTexCoord2f(0,0); glVertex3f(-1, 1, -1); 
		glTexCoord2f(0,1); glVertex3f(-1, -1, -1);
		glTexCoord2f(1,1); glVertex3f(1, -1, -1);
		glTexCoord2f(1,0); glVertex3f(1, 1, -1);
	glEnd();
	convolve->disable();
	glBindTexture(GL_TEXTURE_2D, 0);

	glutSwapBuffers(); // swap backbuffer with frontbuffer
}

void InitGL() {
	// Init opengl(depth test, blending, lighting and so on...)
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	fbo = new FrameBufferObject(512, 512);
	texture = fbo->createAttachment(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
	depth = fbo->createAttachment(GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE);

	fbo->attach(GL_COLOR_ATTACHMENT0, texture, GL_TEXTURE_2D);
	fbo->attach(GL_DEPTH_ATTACHMENT, depth, GL_TEXTURE_2D);

/*	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0,
				 GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &depth);
	glBindTexture(GL_TEXTURE_2D, depth);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 512, 512, 0,
				 GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// create a framebuffer object
	glGenFramebuffers(1, &fboId);
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);

	// attach the texture to FBO color attachment point
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
							  GL_TEXTURE_2D, texture, 0);
	// attach the texture to FBO color attachment point
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
							  GL_TEXTURE_2D, depth, 0);


	// check FBO status
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(status != GL_FRAMEBUFFER_COMPLETE)
		cout << "not complete";

	// switch back to window-system-provided framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);*/
}

// Called by keyboard events
void Keyboard(unsigned char key, int x, int y) {
	Render();
}