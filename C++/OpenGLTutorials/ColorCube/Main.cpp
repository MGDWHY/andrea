#include <gl\glut.h>

float xrot, yrot, zrot;

void InitGL();
void Render();
void Reshape(int, int);
void Idle();

int main(int argc, char **argv) {

	// prima di TUTTO (TUTTO TUTTO TUTTO) creare la finestra sennò questa libereria del cazzo
	// comincia a dare errori incomprensibili di tutti i tipi. Non fare NIENTE PRIMA DI AVER
	// CREATO LA FINESTRA.. NIENTE
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(640, 480);
	glutCreateWindow("ColorCube");
	// Finestra creata... Adesso dovrebbe andare, ma 
	
	InitGL();

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
	gluPerspective(45, (float)w/h, 0.1, 100);
	glMatrixMode(GL_MODELVIEW);
}

void Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	glTranslatef(0,0,-6);
	glRotatef(90, 0, 1, 0);
	glRotatef(xrot, 1, 0, 0);
//	glRotatef(yrot, 0, 1, 0);
//	glRotatef(zrot, 0, 0, 1);


	//front face
	glColor3f(1, 0, 0);	
	glBegin(GL_QUADS);
		glVertex3f(1, 1, 1);
		glVertex3f(-1, 1, 1);
		glVertex3f(-1, -1, 1);
		glVertex3f(1, -1, 1);
	glEnd();
	
	//back face
	glColor3f(0,1,0);
	glBegin(GL_QUADS);
		glVertex3f(-1, 1, -1);
		glVertex3f(1, 1, -1);
		glVertex3f(1, -1, -1);
		glVertex3f(-1, -1, -1);
	glEnd();

	//left face
	glColor3f(0,0,1);
	glBegin(GL_QUADS);
		glVertex3f(-1, 1, 1);
		glVertex3f(-1, 1, -1);
		glVertex3f(-1, -1, -1);
		glVertex3f(-1, -1, 1);
	glEnd();

	//rightface
	glColor3f(1,1,0);
	glBegin(GL_QUADS);
		glVertex3f(1, 1, 1);
		glVertex3f(1, 1, -1);
		glVertex3f(1, -1, -1);
		glVertex3f(1, -1, 1);
	glEnd();

	//topface
	glColor3f(1,0,1);
	glBegin(GL_QUADS);
		glVertex3f(-1, 1, 1);
		glVertex3f(-1, 1, -1);
		glVertex3f(1, 1, -1);
		glVertex3f(1, 1, 1);
	glEnd();

	//bottomface
	glColor3f(0,1,1);
	glBegin(GL_QUADS);
		glVertex3f(-1, -1, 1);
		glVertex3f(-1, -1, -1);
		glVertex3f(1, -1, -1);
		glVertex3f(1, -1, 1);
	glEnd();

	glutSwapBuffers();

	xrot += 0.3;
	yrot += 0.05;
	zrot += 0.02;
}

void InitGL() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);	
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}