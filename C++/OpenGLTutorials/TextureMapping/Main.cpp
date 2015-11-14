#include <Windows.h>
#include <gl\glut.h>
#include "tgaload.h"

void InitGL();
void Render();
void Reshape(int, int);
void LoadTextures();

GLuint texture;

float xrot, yrot, zrot;

int main(int argc, char **argv) {
	
	// prima di TUTTO (TUTTO TUTTO TUTTO) creare la finestra sennò questa libereria del cazzo
	// comincia a dare errori incomprensibili di tutti i tipi. Non fare NIENTE PRIMA DI AVER
	// CREATO LA FINESTRA.. NIENTE
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Texture Mapping");
	// Finestra creata... Adesso dovrebbe andare. 
	
	InitGL();
	LoadTextures();

	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Render);

	glutMainLoop();
	return 0;
}

void Reshape(int w, int h) {
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)w/h,0.1,100);
	glMatrixMode(GL_MODELVIEW);
}

void Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	glTranslatef(0,0,-6);
	glRotatef(xrot, 1, 0, 0);
	glRotatef(yrot, 0, 1, 0);
	glRotatef(zrot, 0, 0, 1);


	//front face
	glBegin(GL_QUADS);
		glTexCoord2f(0,1); glVertex3f(1, 1, 1);
		glTexCoord2f(0,0); glVertex3f(-1, 1, 1);
		glTexCoord2f(1,0); glVertex3f(-1, -1, 1);
		glTexCoord2f(1,1); glVertex3f(1, -1, 1);
	glEnd();
	
	//back face
	glBegin(GL_QUADS);
		glTexCoord2f(0,1); glVertex3f(-1, 1, -1);
		glTexCoord2f(0,0); glVertex3f(1, 1, -1);
		glTexCoord2f(1,0); glVertex3f(1, -1, -1);
		glTexCoord2f(1,1); glVertex3f(-1, -1, -1);
	glEnd();

	//left face
	glBegin(GL_QUADS);
		glTexCoord2f(0,1); glVertex3f(-1, 1, 1);
		glTexCoord2f(0,0); glVertex3f(-1, 1, -1);
		glTexCoord2f(1,0); glVertex3f(-1, -1, -1);
		glTexCoord2f(1,1); glVertex3f(-1, -1, 1);
	glEnd();

	//rightface
	glBegin(GL_QUADS);
		glTexCoord2f(0,1); glVertex3f(1, 1, 1);
		glTexCoord2f(0,0); glVertex3f(1, 1, -1);
		glTexCoord2f(1,0); glVertex3f(1, -1, -1);
		glTexCoord2f(1,1); glVertex3f(1, -1, 1);
	glEnd();

	//topface
	glBegin(GL_QUADS);
		glTexCoord2f(1,0); glVertex3f(1, 1, 1);
		glTexCoord2f(0,0); glVertex3f(-1, 1, 1);
		glTexCoord2f(0,1); glVertex3f(-1, 1, -1);
		glTexCoord2f(1,1); glVertex3f(1, 1, -1);
	glEnd();

	//bottomface
	glBegin(GL_QUADS);
		glTexCoord2f(1,0); glVertex3f(-1, -1, 1);
		glTexCoord2f(0,0); glVertex3f(1, -1, 1);
		glTexCoord2f(0,1); glVertex3f(1, -1, -1);
		glTexCoord2f(1,1); glVertex3f(-1, -1, -1);
	glEnd();

	glutSwapBuffers();

	xrot += 0.1;
	yrot += 0.05;
	zrot += 0.02;
}

void InitGL() {
	glClearColor(0,0,0,1);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
}

void LoadTextures() {
	image_t temp;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D,texture);
	tgaLoad("texture.tga", &temp, TGA_DEFAULT);
}