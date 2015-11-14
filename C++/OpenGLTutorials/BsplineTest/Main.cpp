#include <iostream>
#include "Bspline.h"

void InitSpline();
void InitGL();
void Render();
void Reshape(int, int);
void Keyboard(unsigned char, int, int);

using namespace glutils;

Bspline *s;

float knots[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

vec3 cps[] = { vec3(-3, -3, -0.5),
				vec3(-3, 3, -0.5),
				vec3(3, 3, -0.5),
				vec3(3, -3, -0.5),
				vec3(-3, -3, -0.5),
				vec3(-3, 3, -0.5),
				vec3(3, 3, -0.5)
	};

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
	glutCreateWindow("Tangent Space");
	// Finestra creata... Adesso dovrebbe andare, ma 
	
	InitGL();
	InitSpline();

	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Render);
	glutKeyboardFunc(Keyboard);

	glutMainLoop();
	return 0;
}

void InitSpline() {
	s = new Bspline(4, 7, cps, knots);
}

// called when window is resized
void Reshape(int w, int h) {
	glViewport(0,0,w,h); // viewport resize
}

// called when window is drawn
void Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);	
	glLoadIdentity();
	glOrtho(-5, 5, -5, 5, 0.1, 1.0);
	// projection transform

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(1,0,0);
	glBegin(GL_POINTS);
	for(int i = 0; i< s->getTotalKnots(); i++)
		glVertex3fv(cps[i].xyz);
	glEnd();
	glBegin(GL_LINE_STRIP);
	for(float t = 3.0; t <= 7.0; t +=0.01) {
		glVertex3fv(s->evaluate(t).xyz);	
	}
	glEnd();
	// model-view transform

	glutSwapBuffers(); // swap backbuffer with frontbuffer
}

void InitGL() {
	// Init opengl(depth test, blending, lighting and so on...)
	glEnable(GL_DEPTH_TEST);
	glClearColor(0,0,0,1);
	glPointSize(5);
}

// Called by keyboard events
void Keyboard(unsigned char key, int x, int y) {
	
}