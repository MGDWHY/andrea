#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")

#include <Windows.h>
#include "gl\glew.h"
#include "gl\glut.h"
#include <iostream>
#include "vecmath.h"
#include "Spline.h"

using namespace std;
using namespace Vecmath;
using namespace LibSpline;

void InitGL();
void Render();
void Reshape(int, int);
void Keyboard(unsigned char, int, int);

Spline<vec3> * gSpline;

vec3 CPS[7] = {
	vec3(-10, 4, 0),
	vec3(-8, -2, 0),
	vec3(-6, 6, 0),
	vec3(-5, 3, 0),
	vec3(2, -5, 0),
	vec3(4, 7, 0),
	vec3(7, 3, 0)
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
	glutCreateWindow("Window Title");
	// Finestra creata... Adesso dovrebbe andare, ma 
	
	InitGL();

	gSpline = new Spline<vec3>(CPS, 7, 4, 0, 1);
	gSpline->BuildOpenUniformPartition();

	for(int i = 0; i < gSpline->GetTotalKnots(); i++)
		cout << gSpline->Knots(i) << " ";

	cout << endl;

	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Render);
	glutKeyboardFunc(Keyboard);


	glutMainLoop();

}

// called when window is resized
void Reshape(int w, int h) {
	glViewport(0,0,w,h); // viewport resize
}

// called when window is drawn
void Render() {
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);	
	glLoadIdentity();
	glOrtho(-10, 10, -10, 10, -1, 1);

	// projection transform

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1, 0, 0);
	glBegin(GL_LINE_STRIP);
	for(int i = 0; i < ARRAYSIZE(CPS); i++)
		glVertex3fv(CPS[i].xyz);
	glEnd();

	glColor3f(1, 1, 1);
	glBegin(GL_LINE_STRIP);
		for(int i = 0; i <= 5000; i++) {
			float t = (1.0f / 5000) * i;
			vec3 p = gSpline->Evaluate(t);
			glVertex3fv(p.xyz);
		}
	glEnd();


	// model-view transform

	glutSwapBuffers(); // swap backbuffer with frontbuffer
}

void InitGL() {
	// Init opengl(depth test, blending, lighting and so on...)
	glClearColor(0,0,0,0);
	glDisable(GL_DEPTH_TEST);
}

// Called by keyboard events
void Keyboard(unsigned char key, int x, int y) {
	
}