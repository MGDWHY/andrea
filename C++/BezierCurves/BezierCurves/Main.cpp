#include <vector>
#include <string>
#include <iostream>
#include <gl\glew.h>
#include <gl\glut.h>
#include "vecmath.h"
#include "Randomizer.h"

#define C0 0
#define C1 1
#define G1 2

using namespace std;
using namespace glutils;

void InitGL();
void Render();
void Reshape(int, int);
void Keyboard(unsigned char, int, int);
void Mouse(int, int, int, int);
void MouseMotion(int, int);

void DrawCurve(const vector<vec2> &points);

vec2 DeCasteljau(const vector<vec2> &points, const GLfloat t);

vector<vec2> points;
vector<int> connections;

int connType = C0;

int viewWidth, viewHeight;
int dragPoint = -1;

vec3* colors;

int main(int argc, char **argv) {
	// prima di TUTTO (TUTTO TUTTO TUTTO) creare la finestra sennò non funziona una mazza
	glutInit(&argc, argv);
	// rgba mode, double buffering, depth buffering, stencil buffering
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	// window' top left corner position
	glutInitWindowPosition(0,0);
	// window's size
	glutInitWindowSize(640, 480);
	// create window
	glutCreateWindow("Curve di Bezier");
	// da qui in poi forse funziona tutto

	colors = new vec3[16];
	for(int i = 0; i < 16; i++)
		colors[i] = vec3(Randomizer::nextFloat(), Randomizer::nextFloat(), 0.0f);


	Randomizer::init();

	InitGL();

	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);
	//glutIdleFunc(Render);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(MouseMotion);
	glutFullScreen();
	glutMainLoop();
	return 0;
}

void Mouse(int button, int state, int x, int y) {
	
	GLfloat TL = 10; // 10 pixel

	GLfloat xPos = x;
	GLfloat yPos = viewHeight - y;

	if(state == GLUT_UP) {
		dragPoint = -1;
		return;
	}

	//check if a point has been clicked
	if(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
		for(unsigned int i = 0; i < points.size(); i++) {
			vec2* pt = &(points[i]);
			if(pt->x >= xPos - TL && pt->x <= xPos + TL
				&& pt->y >= yPos - TL && pt->y <= yPos + 10) { // hit
					dragPoint = i;
					return;
			}
		}

		connections.push_back(connType);
		points.push_back(vec2(xPos, yPos));
	}

	glutPostRedisplay();
}

void MouseMotion(int x, int y) {
	static vec2 temp;
	if(dragPoint >= 0) { // a point is being dragged
		GLfloat xPos = x;
		GLfloat yPos = viewHeight - y;

		points[dragPoint].x = xPos;
		points[dragPoint].y = yPos;
		
		/*
			Controllo sul tipo di continuità
			-	Se è c1 impongo che i punti siano "riflessi" rispetto al punto di connessione tra le due curve
				e abbiano la stessa distanza da questo
			-	Se è g1 è sufficiente che si trovino sulla stessa retta
		*/

		if((dragPoint + 1) % 3 == 0 && dragPoint + 2 < points.size()) { // next point is a joint
			temp = sub(points[dragPoint+1], points[dragPoint]);
			if(connections[dragPoint+1] == C1)
				points[dragPoint+2] = add(points[dragPoint+1], temp);
			else if(connections[dragPoint+1] == G1) {
				GLfloat m = length(sub(points[dragPoint+2], points[dragPoint+1]));
				points[dragPoint+2] = add(points[dragPoint+1], scale(normalize(temp), m));
			}
		} else if((dragPoint - 1) % 3 == 0 && dragPoint - 2 >= 0) { // previous point is a joint
			temp = sub(points[dragPoint-1], points[dragPoint]);
			if(connections[dragPoint-1] == C1 ) 
				points[dragPoint-2] = add(points[dragPoint-1], temp);
			else if(connections[dragPoint-1] == G1) {
				GLfloat m = length(sub(points[dragPoint-2], points[dragPoint-1]));
				points[dragPoint-2] = add(points[dragPoint-1], scale(normalize(temp), m));
			}			
		} else if(dragPoint % 3 == 0 && dragPoint - 1 >= 0 && dragPoint + 1 < points.size()) { // this point is a joint
			if(connections[dragPoint] == C1 ||  connections[dragPoint] == G1) {
				temp = sub(points[dragPoint-1], points[dragPoint]);
				points[dragPoint+1] = sub(points[dragPoint], temp);
			}
		}

		glutPostRedisplay();
	}
}

// called when window is resized
void Reshape(int w, int h) {
	viewWidth = w;
	viewHeight = h;
	glViewport(0,0,w,h); // viewport resize
	
}

// called when window is drawn
void Render() {

	static vector<vec2> temp;

	glClear(GL_COLOR_BUFFER_BIT);


	// projection transform
	glMatrixMode(GL_PROJECTION);	
	glLoadIdentity();
	/* Imposto la proiezione ortogonale in modo da avere una corrispondeza 1 a 1 con la
	viewport */
	glOrtho(0, viewWidth, 0, viewHeight, -0.1, 0.1);

	// model-view transform
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//draw points
	glBegin(GL_POINTS);
		for(unsigned int  i = 0; i < points.size(); i++) {
			if(i % 3 == 0) {
				switch(connections[i]) {
					case C0: glColor3f(1.0f, 0.0f, 0.0f); break; // red = c0
					case C1: glColor3f(0.0f, 1.0f, 0.0f); break; // green = c1
					case G1: glColor3f(0.0f, 0.0f, 1.0f); break; // blue = g1
				}
					
			} else
				glColor3f(0.0f, 0.0f, 0.0f); // black = non-joint points
			glVertex3f(points[i].x, points[i].y, 0.0);
		}
	glEnd();

	//draw connecting lines and curves
	if(points.size() > 1) {
		glEnable(GL_LINE_STIPPLE);

		glLineWidth(1.0);
		glColor3f(0.0f, 0.0f, 0.0f);
		glBegin(GL_LINE_STRIP);
			for(unsigned int  i = 0; i < points.size(); i++)
				glVertex3f(points[i].x, points[i].y, 0.0);
		glEnd();

		glDisable(GL_LINE_STIPPLE);

		glLineWidth(3.0);
		glBegin(GL_LINE_STRIP);	
			/*	facendo i calcoli, per ogni 3 punti che ho sull'array ho anche una curva
				ad esempio per 14 punti, le curve hanno definite sono 
				0-3, 3-6, 6-9, 9-12, 12-13 */
			int numCurves = points.size() % 3 == 0 ? points.size() / 3 : points.size() / 3 + 1;

			for(int i = 0; i < numCurves; i++) {

				temp.clear();
				
				for(int j = 0; j < 4; j++) {
					if(i * 3 + j < points.size()) // controllo perchè l'ultima curva potrebbe non avere 4 punti
						temp.push_back(points[i*3 + j]);
				}

				glColor3fv(colors[i % 16].rgb);

				DrawCurve(temp);
			}

		glEnd();

	}

	glFlush();
	

	//glutSwapBuffers(); // swap backbuffer with frontbuffer
}

void DrawCurve(const vector<vec2> &points) {
	for(unsigned int  i = 0; i <= 100; i++) {
		vec2 tmp = DeCasteljau(points, i / 100.0f);
		glVertex3f(tmp.x, tmp.y, 0.0);
	}
}

void InitGL() {
	// Init opengl(depth test, blending, lighting and so on...)
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glLineStipple (3, 0xAAAA);
	glPointSize(10);
}

// Keyboard handler
void Keyboard(unsigned char key, int x, int y) {
	switch(key) {
		case 'l':
			if(points.size() > 0) {
				points.pop_back();
				connections.pop_back();
			}
			break;
		case 'f': 
			if(points.size() > 0) {
				points.erase(points.begin());	
				connections.erase(connections.begin());
			}
			break;
		case '1':
			connType = C0;
			cout << "Connections: C0" << endl;
			break;
		case '2':
			connType = C1;
			cout << "Connections: C1" << endl;
			break;
		case '3':
			connType = G1;
			cout << "Connections: G1" << endl;
			break;
		case 27:
			exit(0);
			break;
		default: break;
	}
	glutPostRedisplay();
}

vec2 DeCasteljau(const vector<vec2> &points, const GLfloat t) {
	vector<vec2> temp = points;
	vector<vec2> temp2;
	while(temp.size() > 1) {
		temp2.clear();

		for(int i = 0; i < temp.size() - 1; i++)
			temp2.push_back(lerp(temp[i], temp[i+1], t));

		temp = temp2;
	}

	return temp.front();
}