#include <Windows.h>
#include <iostream>
#include "GraphicsEngine.h"

#define PI 3.141592

#define FIELD_WIDTH 10
#define FIELD_LENGTH 15
#define PADS_WIDTH 2

GameEngine *game;
GraphicsEngine *graphics;

float rotX = 30, rotY = 0, cameraZoom = -25;

bool dragging;
int prevX, prevY;

void InitGL();
void Render();
void Reshape(int, int);
void Keyboard(unsigned char, int, int);
void Mouse(int, int, int, int);
void MouseMotion(int, int);

int viewWidth, viewHeight;

int main(int argc, char **argv) {
	
	// prima di TUTTO (TUTTO TUTTO TUTTO) creare la finestra sennò questa libereria del cazzo
	// comincia a dare errori incomprensibili di tutti i tipi. Non fare NIENTE PRIMA DI AVER
	// CREATO LA FINESTRA.. NIENTE
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(640, 480);
	glutCreateWindow("PongGL");

	// Finestra creata... Adesso dovrebbe andare
	glewInit();
	InitGL();

	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Render);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(MouseMotion);

	glutFullScreen();

	game = new GameEngine(FIELD_WIDTH, FIELD_LENGTH, PADS_WIDTH, 0.5, true);
	graphics = new GraphicsEngine(game);

	glutMainLoop();
	return 0;
}

void Reshape(int w, int h) {
	viewWidth = w;
	viewHeight = h;
	glViewport(0,0,w,h); // imposta la viewport sulla finestra
}


void Render() {
	static Timer* timer = new Timer();
	static float dt;
	
	dt = timer->dtSecs();

	game->update(dt);

	graphics->render(rotX, rotY, cameraZoom, viewWidth, viewHeight);
	
	glutSwapBuffers(); // swap front buffer
}

void InitGL() {
	glEnable(GL_DEPTH_TEST); // enable depth test
	glEnable(GL_TEXTURE_2D); // enable textures
	glEnable(GL_LIGHTING); // enable lighting
	glEnable(GL_BLEND); // enable alpha blending
	glEnable(GL_CULL_FACE); // enable face culling

	glShadeModel(GL_SMOOTH);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //blending function
	glAlphaFunc(GL_GREATER, 0.8f); // alpha test function
	glCullFace(GL_BACK); // culling back faces
	glClearColor(0,0,0,1); // black on clear
	glLineWidth(2);
}

void Keyboard(unsigned char key, int x, int y) {
	switch(key) {
	case 'a':
		game->setPad1VelX(-4);
		break;
	case 'd':
		game->setPad1VelX(4);
		break;
/*	case 'j':
		game->setPad2VelX(-4);
		break;
	case 'l':
		game->setPad2VelX(4);
		break;*/
	case 'x':
		game->throwBall();
		break;
	case 'r':
		game->reset();
		break;
	case 'f':
		graphics->toggleFilters();
		break;
	case 'm':
		graphics->toggleWireframe();
		break;
	case 'c':
		graphics->toggleCamera();
		break;
	case '+':
		cameraZoom += 0.2f;
		break;
	case '-':
		cameraZoom -= 0.2f;
		break;
	case 27:
		exit(0);
		break;
	default:
		game->setPad1VelX(0);
		break;
	}
}

void Mouse(int button, int state, int x, int y) {
	
	if(button == GLUT_LEFT_BUTTON)
		if(state == GLUT_DOWN) {
			dragging = true;
			prevX = x;
			prevY = y;
		} else {
			dragging = false;
		}
}

void MouseMotion(int x, int y) {
	if(dragging) {
		
		rotY += (x - prevX) / 5.0f;
		rotX += (y - prevY) / 5.0f;

		prevX = x;
		prevY = y;
	}
}
