#include <cmath>
#include "vecmath.h"

using namespace glutils;

void InitGL();
void Render();
void Reshape(int, int);
void Keyboard(unsigned char, int, int);

float width, height;

float a1 = 0.15;
float a2 = 0.1;
float w1 = 1.0;
float w2 = 1.7;
float t1 = 2.0;
float t2 = 3.0;

float time;

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
	glutCreateWindow("Tangent Space Test");
	// Finestra creata... Adesso dovrebbe andare, ma 

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
	time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);	
	glLoadIdentity();
	gluPerspective(45, width/height, 0.1, 100);
	// projection transform

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0,5,10,0,0,0,0,1,0);
	for(int x = -10; x < 10; x++)
		for(int z = -10; z < 10; z ++) {
			glColor3f(1, 0, 0);
			
			/*
				p(x,y,z) = (x, f(x,z), z) position
				dpDx = (1, dfDx(x,z), 0) tangent
				dpDz = (0, dfDz(x,z), 1) binormal
				cross(dpDz, dpDx) normal
			*/

			/* WAVE1 + WAVE2 */
			float y = a1 * sin(w1 * x + time * t1) + a2 * sin(w2 * (x + z * 0.5)+time * t2);
			float dx = a1*w1*cos(w1*x+time*t1) + a2*w2*cos(w2*(x+z*0.5)+time*t2);
			float dz = 0.5*a2*w2*cos(w2*(x+z/2.0) + time*t2);

			/*float y = 0;
			float dx = 0;
			float dz = 0;*/
			
			/* WAVE 1
			float y = a1 * sin(w1 * x + time * t1);
			float dx = a1*w1*cos(w1*x+time*t1);
			float dz = 0.0f;
			*/

			/* WAVE 2
			float y = a2 * sin(w2 * (x + z * 0.5)+time * t2);
			float dx = a2*w2*cos(w2*(x+z*0.5)+time*t2);
			float dz = 0.5*a2*w2*cos(w2*(x+z/2.0) + time*t2);
			*/

			vec3 p(x, y, z);
			vec3 vt(1.0, dx, 0.0);
			vec3 vb(0.0, dz, 1.0);
			vec3 vn = cross(vb, vt);

			vn = scale(vn, 0.5);
			vt = scale(vt, 0.5);
			vb = scale(vb, 0.5);
			
			glBegin(GL_LINES);
				// tangent
				glColor3f(1, 0, 0);
				glVertex3fv(p.xyz);
				glVertex3fv(add(p,vt).xyz);

				//binormal
				glColor3f(0, 1, 0);
				glVertex3fv(p.xyz);
				glVertex3fv(add(p,vb).xyz);

				//normal
				glColor3f(0, 0, 1);
				glVertex3fv(p.xyz);
				glVertex3fv(add(p,vn).xyz);

			glEnd();
		}
	// model-view transform

	glutSwapBuffers(); // swap backbuffer with frontbuffer
}

void InitGL() {
	// Init opengl(depth test, blending, lighting and so on...)
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glClearColor(0,0, 0,1);
	glPointSize(2);
}

// Called by keyboard events
void Keyboard(unsigned char key, int x, int y) {
	
}