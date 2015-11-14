#include <iostream>
#include <gl\glut.h>
#include "Water.h"
#include "Vec3.h"

void InitGL();
void Render();
void Reshape(int, int);
void Keyboard(unsigned char, int, int);

float aspectRatio;

Water *water;

GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f};
GLfloat light_diffuse[] = { 1.0f, 1.0f, 0.7f, 1.0f};
GLfloat light_specular[] = { 1.0f, 1.0f, 0.7f, 1.0f};
GLfloat light_position[] = { 0, 2, 0, 1};

GLfloat water_diffuse[] = { .8f, 1.0f, 1.0f, 1.0f};
GLfloat water_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};

GLfloat ground_diffuse[] = { 0.5f, 0.5f, 0.0f, 1.0f};
GLfloat ground_specular[] = {0.0f, 0.0f, 0.0f, 1.0f};


int main(int argc, char **argv) {
	
	// prima di TUTTO (TUTTO TUTTO TUTTO) creare la finestra sennò questa libereria del cazzo
	// comincia a dare errori incomprensibili di tutti i tipi. Non fare NIENTE PRIMA DI AVER
	// CREATO LA FINESTRA.. NIENTE
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Water");
	// Finestra creata... Adesso dovrebbe andare, ma 

	water = new Water(10, 10);
	
	InitGL();

	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Render);
	glutKeyboardFunc(Keyboard);

	glutMainLoop();
	return 0;
}

void Reshape(int w, int h) {
	aspectRatio = (float)w/h;
	glViewport(0,0,w,h); // imposta la viewport sulla finestra

}

void Render() {
	static int prevTime = glutGet(GLUT_ELAPSED_TIME);
	static int time = 0;
	static int fps = 0;

	float dt = (glutGet(GLUT_ELAPSED_TIME) - prevTime) / 1000.0f;
	
	time += glutGet(GLUT_ELAPSED_TIME) - prevTime;
	prevTime = glutGet(GLUT_ELAPSED_TIME);
	fps++;
	
	if(time >= 1000) {
		std::cout << "FPS: " << fps << std::endl;
		time = 0;
		fps = 0;
	}

	water->update(dt);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, aspectRatio, 0.1, 100);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 5, 10, 0, 0, 0, 0, 1, 0);

	glLightfv(GL_LIGHT1, GL_POSITION, light_position);

	glTranslatef(-5, 0, -5);

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ground_diffuse);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, ground_specular);
	glBegin(GL_QUADS);
		glNormal3f(0, 1, 0);
		glVertex3f(0, -1, 0);
		glVertex3f(0, -1, 10);
		glVertex3f(10, -1, 10);
		glVertex3f(10, -1, 0);
	glEnd();

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, water_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, water_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 128);
	for(int x = 0; x < WATER_RESOLUTION_X - 1; x++) {
		glBegin(GL_TRIANGLE_STRIP);
			for(int z = 0; z < WATER_RESOLUTION_Z; z++) {

				Vec3* n = water->getNormal(x, z);
				glNormal3f(n->getX(), n->getY(), n->getZ());
				glVertex3f(x*water->getStepX(), water->getHeight(x, z), z*water->getStepZ());

				n = water->getNormal(x + 1, z);
				glNormal3f(n->getX(), n->getY(), n->getZ());
				glVertex3f((x+1)*water->getStepX(), water->getHeight(x + 1, z), z*water->getStepZ());
			}
		glEnd();
	}

	glutSwapBuffers(); // scambia il backbuffer con il frontbuffer
}

void InitGL() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);
	glEnable(GL_BLEND);

	glDisable(GL_COLOR_MATERIAL);

	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Keyboard(unsigned char key, int x, int y) {

}