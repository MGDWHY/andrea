#include <Windows.h>
#include <random>
#include <iostream>
#include <vector>
#include <iomanip>
#include <gl\glut.h>
#include "tgaload.h"
#include "lodepng.h"
#include "Terrain.h"

#define T_WIDTH 50
#define T_LENGTH 50
#define PI 3.141592

void InitGL();
void Render();
void Reshape(int, int);
void Keyboard(unsigned char, int, int);

Terrain *terrain;

GLuint texture, instr, grass;

Vec3* pos; // camera position
Vec3* dir; // camera direction
float angle; // angle to compute direction

GLfloat position[] = {0.0f, 50.0f, 0.0f, 1.0f}; //light's position
GLfloat ambient[] = {0.2f, 0.2f, 0.2f, 1.0f}; // light's ambient (dark gray)
GLfloat diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f}; // light's diffuse (yellow);

GLfloat fog[] = {0.5,0.5,0.5,1.0f}; // fog's color (gray)

int viewWidth, viewHeight;

typedef struct {
	GLuint w, h;
	GLubyte *data;
} img_data;

img_data pngLoad(char* filename) {
	using namespace std;
	using namespace LodePNG;
	img_data data;

	vector<unsigned char> out;
	decode(out, data.w, data.h, filename);

	data.data = new GLubyte[data.w*data.h*4];

	for(int i = 0; i < out.size(); i++)
		data.data[i] = out[i];

	return data;
}

void loadMap() {
	image_t image;
	tgaLoad("map.tga", &image, TGA_NO_PASS);
	
	int width = image.info.width;
	int height = image.info.height;

	terrain = new Terrain(width, height);
	for(int z = 0; z < height; z++)
		for(int x = 0; x < width; x++)
			terrain->setHeight(x, z, image.data[3*(z*width + x)] / (float)10);
	
	terrain->compile(GL_TRIANGLE_STRIP);
}

int main(int argc, char **argv) {
	
	// prima di TUTTO (TUTTO TUTTO TUTTO) creare la finestra sennò questa libereria del cazzo
	// comincia a dare errori incomprensibili di tutti i tipi. Non fare NIENTE PRIMA DI AVER
	// CREATO LA FINESTRA.. NIENTE
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Terrain");

	// Finestra creata... Adesso dovrebbe andare, ma 
	loadMap();
	InitGL();

	pos = new Vec3();
	dir = new Vec3(0,0,1);

	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Render);
	glutKeyboardFunc(Keyboard);

	glutMainLoop();
	return 0;
}

void Reshape(int w, int h) {
	viewWidth = w;
	viewHeight = h;
	glViewport(0,0,w,h); // imposta la viewport sulla finestra
}

void Render() {
	static int ix, iz;

	ix = terrain->getWidth()/2 - (int)pos->getX();
	iz = terrain->getLength()/2 - (int)pos->getZ();


	// switching to perspective projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)viewWidth/viewHeight,0.1, 1000);
	

	// drawing the terrain
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glRotatef(-angle/PI*180,0,1,0);
	glTranslatef(pos->getX(), -terrain->getHeight(ix, iz) - 2, pos->getZ());
	
	glBindTexture(GL_TEXTURE_2D, texture);
	glCallList(terrain->getTerrainList());

	// erba... per adesso non ci siamo, troppi problemi di prestazioni
	//glBindTexture(GL_TEXTURE_2D, grass);
	//glCallList(terrain->getGrassList());

	// disable lighting for drawing in orthogonal mode
	bool lighting = glIsEnabled(GL_LIGHTING) ? true : false;
	glDisable(GL_LIGHTING);

	//swithing to ortho projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,viewWidth,0,viewHeight, -2, 2); // setting orthogonal matrix
	
	//drawing instructions
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glBindTexture(GL_TEXTURE_2D, instr);
	//glColor4f(1,1,1,0.5);
	glBegin(GL_QUADS);
		glTexCoord2f(0,0); glVertex2f(0,0);
		glTexCoord2f(1,0); glVertex2f(200,0);
		glTexCoord2f(1,1); glVertex2f(200, 60);
		glTexCoord2f(0,1); glVertex2f(0, 60);
	glEnd();
	glColor4f(1,1,1,1);
	
	// re-enable lighting if it was enabled
	if(lighting)
		glEnable(GL_LIGHTING);

	glutSwapBuffers(); // swap front buffer
}

void InitGL() {
	glEnable(GL_DEPTH_TEST); // enable depth test
	glEnable(GL_TEXTURE_2D); // enable textures
	glEnable(GL_LIGHT1); // enable light one

	glClearColor(0,0,0,1);

	//LIGHT SETUP
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, position);

	//TEXTURE SETUP
	image_t tmp;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	tgaLoad("texture.tga", &tmp, TGA_DEFAULT);

	glGenTextures(1, &instr);
	glBindTexture(GL_TEXTURE_2D, instr);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	tgaLoad("instr.tga", &tmp, TGA_DEFAULT | TGA_ALPHA);
	
	glGenTextures(1, &grass);
	glBindTexture(GL_TEXTURE_2D, grass);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	img_data data = pngLoad("grass.png");
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, data.w, data.h, GL_RGBA, GL_UNSIGNED_BYTE, data.data);

	//FOG SETUP
	glFogfv(GL_FOG_COLOR, fog); // fog color
	glFogi(GL_FOG_MODE, GL_EXP); // fog function: linear, exp or exp2
	glFogf(GL_FOG_DENSITY, 0.01f); // fog density, parameter for function exp and exp2

	//ALPHA BLENDING
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//ALPHA TEST
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.7);
}

void Keyboard(unsigned char key, int x, int y) {
	switch(key) {
	case 'a':
		angle += PI/45;
		dir->set(sin(angle), 0, cos(angle));
		break;
	case 'd':
		angle -= PI/45;
		dir->set(sin(angle), 0, cos(angle));
		break;
	case 'w':
		pos = pos->add(dir);
		break;
	case 's':
		pos = pos->sub(dir);
		break;
	case 'f':
		if(glIsEnabled(GL_FOG)) {
			glDisable(GL_FOG);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		} else {
			glEnable(GL_FOG);
			glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		}
		break;
	case 'l':
		if(glIsEnabled(GL_LIGHTING)) {
			glDisable(GL_LIGHTING);
		} else {
			glEnable(GL_LIGHTING);
		}
		break;
	}
}