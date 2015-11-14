#include <iostream>
#include "ShaderManager.h"
#include "MatrixStack.h"
#include "Timer.h"

using namespace glutils;
using namespace std;

void InitGL();
void Render();
void Reshape(int, int);
void Keyboard(unsigned char, int, int);

#define PI 3.141592f

GLuint vao, vbo[2];

GLfloat viewWidth, viewHeight;

GLfloat vertices[4][3] = {
	{-1, -1, 0},
	{-1, 1, 0},
	{1, 1, 0},
	{1, -1, 0}
};

GLuint indices[] = { 0, 1, 2, 2, 3, 0};

Shader *shader;

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
	glutCreateWindow("GLSL 1.30 & VAO Test");
	// Finestra creata... Adesso dovrebbe andare, ma 
	
	glewInit();
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
	viewWidth = w;
	viewHeight = h;
	glViewport(0,0,w,h); // viewport resize
	glutPostRedisplay();
}

// called when window is drawn
void Render() {
	static Timer *timer = new Timer();
	static MatrixStack *mvstack = new MatrixStack(16);
	static MatrixStack *pstack = new MatrixStack(4);
	static GLfloat fps = 0, tfps = 0;
	static vec3 axis(3,2,-3);

	fps++;
	tfps += timer->dtSecs();

	if(tfps >= 1.0f) {
		cout << "fps: " << fps << endl;
		fps = 0.0f;
		tfps = 0.0f;
	}

	axis = normalize(axis);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//projection
	pstack->loadIdentity();
	pstack->perspective(PI/4, viewWidth/viewHeight, 0.1, 100);
	//pstack->ortho(-5, 5, -5, 5, 1, 20);
	
	//modelview
	mvstack->loadIdentity();
	mvstack->translate(0,0,-10);
	mvstack->rotate(timer->timeSecs(), axis.x, axis.y, axis.z);

	shader->enable();
	int loc1 = shader->getUniformLocation("modelViewMatrix");
	int loc2 = shader->getUniformLocation("projectionMatrix");

	shader->setUniformMatrix("projectionMatrix", pstack->current());

	glBindVertexArray(vao);

	mvstack->push();
	mvstack->translate(-2, 0, 0);
	shader->setUniformMatrix("modelViewMatrix", mvstack->current());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	mvstack->pop();

	mvstack->push();
	mvstack->translate(2, 0, 0);
	shader->setUniformMatrix("modelViewMatrix", mvstack->current());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	mvstack->pop();

	mvstack->push();
	mvstack->translate(0, 2, 0);
	shader->setUniformMatrix("modelViewMatrix", mvstack->current());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	mvstack->pop();

	mvstack->push();
	
	mvstack->translate(0, -2, 0);
	shader->setUniformMatrix("modelViewMatrix", mvstack->current());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	mvstack->push();
	mvstack->translate(0, 2, 0);
	mvstack->rotate(PI/2, 1, 0, 0);
	shader->setUniformMatrix("modelViewMatrix", mvstack->current());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	mvstack->pop();

	mvstack->pop();

	glBindVertexArray(0);

	shader->disable();
	// model-view transform

	glutSwapBuffers(); // swap backbuffer with frontbuffer
}

void InitGL() {
	// Init opengl(depth test, blending, lighting and so on...)
	glEnable(GL_DEPTH_TEST);

	glClearColor(0,0,0,1);

	ShaderManager *sMan = ShaderManager::getDefaultManager();
	shader = sMan->createShader("C:/Andrea/GLSL/version130/prova2.vert", "C:/Andrea/GLSL/version130/prova2.geom", "C:/Andrea/GLSL/version130/prova2.frag");

	glGenVertexArrays(1, &vao); // create vao
	glGenBuffers(2, vbo); // create vbo

	glBindVertexArray(vao);

	int posLocation = shader->getAttributeLocation("in_Position");

	cout << posLocation << endl;

	//vertices position attribute
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//vertices indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	
	glBindVertexArray(0);
}

// Called by keyboard events
void Keyboard(unsigned char key, int x, int y) {
	
}