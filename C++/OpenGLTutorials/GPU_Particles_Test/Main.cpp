#include <random>
#include "vecmath.h"
#include "ShaderManager.h"
#include "Shader.h"
#include "Timer.h"
#include "FrameBufferObject.h"
#include "tgaload.h"

#define RND (rand() / (float)(RAND_MAX+1))

using namespace glutils;

void InitGL();
void Render();
void Reshape(int, int);
void Keyboard(unsigned char, int, int);

FrameBufferObject *fbo;

GLuint particlesPositionTarget, velocityTexture;

Shader *particles;

GLfloat pixels[512*512*3];

float viewWidth, viewHeight;

GLuint vertexBuffer;

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
	glutCreateWindow("GPU Particles Test");
	// Finestra creata... Adesso dovrebbe andare, ma 
	
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
	viewWidth = w;
	viewHeight = h;
}

// called when window is drawn
void Render() {
	static Timer *timer = new Timer();

	static GLfloat fps = 0, tfps = 0;

	fps++;
	tfps += timer->dtSecs();

	if(tfps >= 1.0f) {
		cout << "fps: " << fps << endl;
		fps = 0.0f;
		tfps = 0.0f;
	}

	fbo->enable();

	glClear(GL_COLOR_BUFFER_BIT);

	glPushAttrib(GL_ENABLE_BIT);

	glDisable(GL_DEPTH_TEST);
	glViewport(0,0,512,512);

	glMatrixMode(GL_PROJECTION);	
	glLoadIdentity();
	glOrtho(-1, 1,-1, 1, 0.1, 10);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0,0,-2);

	particles->enable();
	particles->setUniform("time", glutGet(GLUT_ELAPSED_TIME) / 8000.0f);
	particles->setUniformTexture("velocity", 0, velocityTexture);

	glBegin(GL_QUADS);
		glTexCoord2f(0,0); glVertex3f(-1, 1, 0);
		glTexCoord2f(0,1); glVertex3f(-1, -1, 0);
		glTexCoord2f(1,1); glVertex3f(1, -1, 0);
		glTexCoord2f(1,0); glVertex3f(1, 1, 0);
	glEnd();
	
	particles->disable();

	glPopAttrib();

	fbo->disable();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, particlesPositionTarget);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0,0,viewWidth,viewHeight);

	glMatrixMode(GL_PROJECTION);	
	glLoadIdentity();
	gluPerspective(45, viewWidth/viewHeight, 0.1, 10);

	// projection transform

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0,0,-3);
	glRotatef(45, 0, 1, 0);
	glRotatef(45, 1, 0, 0);

	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, pixels);

	
/*	glBegin(GL_POINTS);
	for(int i = 0; i < 512*512; i++) {
		float x = pixels[i*4];
		float y = pixels[i*4+1];
		float z = pixels[i*4+2];
		glColor4f(x, y, z, 0.3);
		glVertex3f(x*2.0-1.0, y*2.0-1.0, z*2.0-1.0);
	}
	glEnd();*/

	//glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	//glBufferData(GL_ARRAY_BUFFER, 512*512*4*sizeof(GLfloat),pixels,GL_STREAM_DRAW);
	glVertexPointer(3, GL_FLOAT, 0, pixels);
	glColorPointer(3, GL_FLOAT, 0, pixels);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glDrawArrays(GL_POINTS, 0, 512*512);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glutSwapBuffers(); // swap backbuffer and frontbuffer
}

void InitGL() {
	// Init opengl(depth test, blending, lighting and so on...)

	GLfloat *velocity = new GLfloat[512*512*4];

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glPointSize(2.0);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	fbo = new FrameBufferObject(512,512);
	particlesPositionTarget = fbo->createAttachment(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
	fbo->attach(GL_COLOR_ATTACHMENT0, particlesPositionTarget, GL_TEXTURE_2D);

	particles = ShaderManager::getDefaultManager()->createShader("particles.vert", "particles.frag");

	for(int i = 0; i < 512*512; i++) {
		velocity[i*4] = RND;
		velocity[i*4+1] = RND;
		velocity[i*4+2] = RND;
		velocity[i*4+3] = 0.0f;
	}
	

	glGenTextures(1, &velocityTexture);
	glBindTexture(GL_TEXTURE_2D, velocityTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_FLOAT, velocity);
	glBindTexture(GL_TEXTURE_2D, 0);

	delete[] velocity;
}

// Called by keyboard events
void Keyboard(unsigned char key, int x, int y) {
	
}