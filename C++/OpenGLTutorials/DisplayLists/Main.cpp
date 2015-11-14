// Oltre alle liste qua c'è il vero alpha blending per la trasparenza (regola SRC OVER)
// Per ottenerlo bisogna prima disegnare gli oggetti opachi nella scena, poi si passa
// a quelli trasparenti, e si disegnano da quello più lontano al più vicino
#include <Windows.h>
#include <cmath>
#include <gl\glut.h>
#include "tgaload.h"

using namespace std;

void InitGL();
void Render();
void Reshape(int, int);
void Keyboard(unsigned char, int, int);

void CreateLists();
void LoadTextures();

GLuint lists[1];
GLuint texture[2];

float xrot, yrot;

float LightA[] =  {0.2, 0.2, 0.2, 1};
float LightD[] = {1, 1, 1, 1};
float LightP[] = {0, 2, 0, 1};

int main(int argc, char **argv) {
	
	// prima di TUTTO (TUTTO TUTTO TUTTO) creare la finestra sennò questa libereria del cazzo
	// comincia a dare errori incomprensibili di tutti i tipi. Non fare NIENTE PRIMA DI AVER
	// CREATO LA FINESTRA.. NIENTE
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(640, 480);
	glutCreateWindow("DisplayLists");
	// Finestra creata... Adesso dovrebbe andare, ma 
	
	CreateLists();
	LoadTextures();
	InitGL();


	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Render);
	glutKeyboardFunc(Keyboard);

	glutMainLoop();
	return 0;
}

void Reshape(int w, int h) {
	glViewport(0,0,w,h); // imposta la viewport sulla finestra
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)w/h,0.1,100);
	glMatrixMode(GL_MODELVIEW);
}

void Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0,0,-5);
	glRotatef(xrot, 1, 0, 0);
	glRotatef(yrot, 0, 1, 0);
	
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glCallList(lists[0]); // richiama la lista memorizzata in precedenza e la disegna

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1,0,0,0.5);
	glBegin(GL_QUADS);
		glTexCoord2f(1, 0); glVertex3f(0.5, 0.5, 1);
		glTexCoord2f(0, 0); glVertex3f(-0.5, 0.5, 1);
		glTexCoord2f(0, 1); glVertex3f(-0.5, -0.5, 1);
		glTexCoord2f(1, 1); glVertex3f(0.5, -0.5, 1);
	glEnd();
	glutSwapBuffers(); // scambia il backbuffer con il frontbuffer
}

void InitGL() {
	glClearColor(0,0,0,0.5);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // funzione di blending srcover
	
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);

	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	glLightfv(GL_LIGHT1, GL_AMBIENT,LightA);
	glLightfv(GL_LIGHT1, GL_DIFFUSE,LightD);
	glLightfv(GL_LIGHT1, GL_POSITION, LightP);
	glEnable(GL_LIGHT1);
}

void Keyboard(unsigned char key, int x, int y) {
	switch(key) {
	case 'a':
		yrot += 5;
		break;
	case 'd':
		yrot -= 5;
		break;
	case 'w':
		xrot += 5;
		break;
	case 's':
		xrot -= 5;
		break;
	default:
		break;
	}
}

void CreateLists() {
	lists[0] = glGenLists(1); // crea una lista

	float sq2 = sqrt((float)2);
	// Pyramid
	glNewList(lists[0], GL_COMPILE); //inizio lista
		
		glBegin(GL_TRIANGLES);
			//front face
			glNormal3f(0,sq2,sq2);
			glTexCoord2f(0.5, 0); glVertex3f(0, 0.5, 0); // top vertex
			glTexCoord2f(0, 1); glVertex3f(-0.5, -0.5, 0.5); // left vertex
			glTexCoord2f(1, 1); glVertex3f(0.5, -0.5, 0.5); // right vertex

			//back face
			glNormal3f(0,sq2,-sq2);
			glTexCoord2f(0.5, 0); glVertex3f(0, 0.5, 0); // top vertex
			glTexCoord2f(0, 1); glVertex3f(0.5, -0.5, -0.5); // left vertex
			glTexCoord2f(1, 1); glVertex3f(-0.5, -0.5, -0.5); // right vertex

			// left face
			glNormal3f(-sq2,sq2,0);
			glTexCoord2f(0.5, 0); glVertex3f(0, 0.5, 0); // top vertex
			glTexCoord2f(0, 1); glVertex3f(-0.5, -0.5, -0.5); // left vertex
			glTexCoord2f(1, 1); glVertex3f(-0.5, -0.5, 0.5); // right vertex

			// right face
			glNormal3f(sq2,sq2,0);
			glTexCoord2f(0.5, 0); glVertex3f(0, 0.5, 0); // top vertex
			glTexCoord2f(0, 1); glVertex3f(0.5, -0.5, 0.5); // left vertex
			glTexCoord2f(1, 1); glVertex3f(0.5, -0.5, -0.5); // right vertex
		glEnd();

	glEndList(); // fine lista

}

void LoadTextures() {
	image_t temp;
	glGenTextures(2, texture);

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	tgaLoad("texture.tga", &temp, TGA_DEFAULT);

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	tgaLoad("texture2.tga", &temp, TGA_DEFAULT);

}