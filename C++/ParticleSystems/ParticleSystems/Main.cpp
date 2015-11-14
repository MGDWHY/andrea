#include <iostream>
#include <ctime>
#include <cmath>
#include <GL/glut.h>
#include "SimpleSystem.h"
#include "SimpleParticle.h"
#include "Randomizer.h"

using namespace std;

float theta = 0;
float PI = 3.141592;

SimpleSystem *s;

long currentMillis() {
	return (1000 * clock() / CLOCKS_PER_SEC);
}

long getMax(long a, long b) {
	return a > b ? a : b;
}

void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	s->render();
	glFlush();
	glutSwapBuffers();
}

void keyb(unsigned char key, int x, int y) {

}

void refresh(int value) {
	static long prevTime = currentMillis(), elapsed;
	
	elapsed = currentMillis() - prevTime;
	prevTime = currentMillis();
	
	glutPostRedisplay();
	
	glutTimerFunc(getMax(20-elapsed, 0), refresh, 0);
}

void update(int value) {
	static long prevTime = currentMillis(), elapsed;
	static float seconds;

	elapsed = currentMillis() - prevTime;
	prevTime = currentMillis();

	seconds = elapsed / 1000.0f;

	s->moveSystem(seconds);
	
	cout << elapsed << endl;

	glutTimerFunc(getMax(20-elapsed, 0), update, 0);
}

void resize(int width, int height) {
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1);
	glMatrixMode(GL_MODELVIEW);
}

void main( int argc, char** argv ) {
	int mode = GLUT_RGBA|GLUT_SINGLE;
	
	s = new SimpleSystem;

	s->position->set(100,100);
	s->setMaxParticles(100000);
	s->setSpawnRate(10000);
	s->setMinLifeTime(10);
	s->setMaxLifeTime(10);

	Randomizer::init();
	
	glutInitDisplayMode( mode ); 
	glutCreateWindow( "Prova" ); 

	SimpleParticle::init();
	
	glutDisplayFunc( renderScene );
	glutReshapeFunc( resize ); 
	glutKeyboardFunc( keyb );	

	glutTimerFunc(0, update, 0);
	glutTimerFunc(0, refresh, 0);

	glutMainLoop();
}