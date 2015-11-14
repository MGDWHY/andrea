#include <iostream>
#include <string>
#include "vecmath.h"

using namespace std;
using namespace glutils;

#define MEGA_MACRO(i,k) add(add(scale(p1, i), scale(p2, k)), p0); cout << "i: " << i << " k: " << k << endl
#define PRINT_VERTEX(v) cout << "Vertex " << v.x <<  " " << v.y << " " << v.z << endl

void InitGL();
void Render();
void Reshape(int, int);
void Keyboard(unsigned char, int, int);

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

	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Render);
	glutKeyboardFunc(Keyboard);

	vec3 p0, p1, p2;

	p0 = vec3(-2.0, 0.0, -2.0);
	p1 = sub(vec3(2.0, 0.0,-2.0), p0);
	p2 = sub(vec3(0.0, 0.0, 1.0), p0);
	
	float ztep = 1.0f / 2.0f;
	vec3 temp;
	
	for(float i = 0.0; i < 1.0f; i += ztep) {
		float k = 0.0;
		for(k = 0.0; k < 1.0f - i; k += ztep) {
			temp = MEGA_MACRO(i, k);
			PRINT_VERTEX(temp);
			temp = MEGA_MACRO(i + ztep, k);
			PRINT_VERTEX(temp);

		}
		temp = MEGA_MACRO(i, k);
		PRINT_VERTEX(temp);
		cout << "EndPrimitive" << endl;
	}

	glutMainLoop();

}

// called when window is resized
void Reshape(int w, int h) {
	glViewport(0,0,w,h); // viewport resize
}

// called when window is drawn
void Render() {
	glMatrixMode(GL_PROJECTION);	
	glLoadIdentity();

	// projection transform

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	// model-view transform

	glutSwapBuffers(); // swap backbuffer with frontbuffer
}

void InitGL() {
	// Init opengl(depth test, blending, lighting and so on...)
}

// Called by keyboard events
void Keyboard(unsigned char key, int x, int y) {
	
}