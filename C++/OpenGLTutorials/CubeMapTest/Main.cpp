#include <sstream>
#include <iostream>
#include <vector>
#include <cmath>
#include "FrameBufferObject.h"
#include "lodepng.h"

using namespace std;
using namespace glutils;

bool merda = true;

int face = 0;

int useMap = 0;

void InitGL();
void Render();
void Reshape(int, int);
void Keyboard(unsigned char, int, int);

void renderViewPoint(int index);
void drawBox(GLfloat width, GLfloat height, GLfloat length, GLfloat tx, GLfloat ty, GLfloat tz);

#define PI 3.141592


GLuint cubemap[6];

GLfloat viewWidth, viewHeight;

GLuint colorAttach, depthAttach;

GLuint drawList;

GLuint textures[8];

GLuint cubemap2[6];

GLfloat cmd[][2] = {
	{0, 90}, // pos x
	{0, -90}, // neg x
	{-90, 0}, // pos y
	{90, 0}, // neg y
	{0, 180},  // pos z
	{0, 0} // neg z
};

FrameBufferObject *fbo;



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
	
	glewInit();

	InitGL();


	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Render);
	glutKeyboardFunc(Keyboard);

	glutMainLoop();
	return 0;
}


void pngLoad(const char* filename, int target) {
 
  unsigned int width, height;
  std::vector<unsigned char> *image = new std::vector<unsigned char>;
  unsigned error = LodePNG::decode(*image, width, height, filename);
  unsigned char* data = new unsigned char[width*height*4];

  for(int i = 0; i < width * height * 4; i++)
	  data[i] = (*image)[i];

  glTexImage2D(target, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  //gluBuild2DMipmaps ( target, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data );

  delete image;
  delete data;
}

void draw2DQuad() {
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1);	glVertex2f(-1, 1);
		glTexCoord2f(0, 0);	glVertex2f(-1, -1);
		glTexCoord2f(1, 0);	glVertex2f(1, -1);
		glTexCoord2f(1, 1);	glVertex2f(1, 1);
	glEnd();
}

void drawBox1(GLfloat width, GLfloat height, GLfloat length, GLfloat tx, GLfloat ty, GLfloat tz) {
	glBegin(GL_QUADS);
		// FRONT FACE
		glNormal3f(0,0,1);
		
		glTexCoord2f(0, 0);		glVertex3f(-width/2, height/2, length/2);
		glTexCoord2f(0, ty);	glVertex3f(-width/2, -height/2, length/2);
		glTexCoord2f(tx, ty);	glVertex3f(width/2, -height/2, length/2);
		glTexCoord2f(tx, 0);	glVertex3f(width/2, height/2, length/2);

		// BACK FACE

		glNormal3f(0,0,-1);	
		glTexCoord2f(0, 0);		glVertex3f(-width/2, height/2, -length/2);
		glTexCoord2f(tx, 0);	glVertex3f(width/2, height/2, -length/2);
		glTexCoord2f(tx, ty);	glVertex3f(width/2, -height/2, -length/2);
		glTexCoord2f(0, ty);	glVertex3f(-width/2, -height/2, -length/2);

		// LEFT FACE
		glNormal3f(-1, 0, 0);
		glTexCoord2f(0, 0);		glVertex3f(-width/2, height/2, -length/2);
		glTexCoord2f(0, ty);	glVertex3f(-width/2, -height/2, -length/2);
		glTexCoord2f(tz, ty);	glVertex3f(-width/2, -height/2, length/2);
		glTexCoord2f(tz, 0);	glVertex3f(-width/2, height/2, length/2);

		// RIGHT FACE
		glNormal3f(1, 0, 0);
		glTexCoord2f(0, 0);		glVertex3f(width/2, height/2, -length/2);
		glTexCoord2f(tz, 0);	glVertex3f(width/2, height/2, length/2);
		glTexCoord2f(tz, ty);	glVertex3f(width/2, -height/2, length/2);
		glTexCoord2f(0, ty);	glVertex3f(width/2, -height/2, -length/2);

		//TOP FACE
		glNormal3f(0, 1, 0);
		glTexCoord2f(0, 0);		glVertex3f(-width/2, height/2, -length/2);
		glTexCoord2f(0, tz);	glVertex3f(-width/2, height/2, length/2);
		glTexCoord2f(tx, tz);	glVertex3f(width/2, height/2, length/2);
		glTexCoord2f(tx, 0);	glVertex3f(width/2, height/2, -length/2);

		//BOTTOM FACE
		glNormal3f(0, -1, 0);
		glTexCoord2f(0, 0);		glVertex3f(-width/2, -height/2, -length/2);
		glTexCoord2f(tx, 0);	glVertex3f(width/2, -height/2, -length/2);
		glTexCoord2f(tx, tz);	glVertex3f(width/2, -height/2, length/2);
		glTexCoord2f(0, tz);	glVertex3f(-width/2, -height/2, length/2);
	glEnd();
}


// called when window is resized
void Reshape(int w, int h) {
	viewWidth = w;
	viewHeight = h;
	glViewport(0,0,w,h); // viewport resize

}

// called when window is drawn
void Render() {

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	
	if(merda) {
		glMatrixMode(GL_PROJECTION);	
		glLoadIdentity();
		gluPerspective(90, viewWidth/viewHeight,0.1, 20.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef(cmd[face][1], 0, 1, 0);
		glRotatef(cmd[face][0], 1, 0, 0);

		glCallList(drawList);
	} else {
		GLuint *map;

		if(useMap == 0)
			map = cubemap;
		else
			map = cubemap2;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-5,5,-5,5,0.1,20.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0,0,-10);


		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, map[4]); // pos z
		draw2DQuad();
		glPopMatrix();

		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, map[1]); // neg x
		glTranslatef(-2,0,0);
		draw2DQuad();
		glPopMatrix();

		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, map[0]); // pos x
		glTranslatef(2,0,0);
		draw2DQuad();
		glPopMatrix();

		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, map[5]); // neg z
		glTranslatef(4,0,0);
		draw2DQuad();
		glPopMatrix();

		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, map[2]); // pos y
		glTranslatef(0,2,0);
		draw2DQuad();
		glPopMatrix();

		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, map[3]); // neg y
		glTranslatef(0,-2,0);
		draw2DQuad();
		glPopMatrix();
	}

	glutSwapBuffers(); // swap backbuffer with frontbuffer
}

void InitGL() {
	// Init opengl(depth test, blending, lighting and so on...)
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	fbo = new FrameBufferObject(512, 512);

	depthAttach = fbo->createAttachment(GL_DEPTH_COMPONENT);

	fbo->attach(GL_DEPTH_ATTACHMENT, depthAttach, GL_TEXTURE_2D);

	glGenTextures(8, textures);
	glGenTextures(6, cubemap);
	glGenTextures(6, cubemap2);

	for(int i = 0; i < 8; i++) {
		stringstream ss;
		string file;
		ss << "n" << (i+1) << ".png";
		getline(ss, file);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		pngLoad(file.c_str(), GL_TEXTURE_2D);
	}

	for(int i = 0; i < 6; i++) {
		stringstream ss;
		string file;
		ss << i << ".png";
		getline(ss, file);
		glBindTexture(GL_TEXTURE_2D, cubemap2[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		pngLoad(file.c_str(), GL_TEXTURE_2D);
	}

	drawList = glGenLists(1);
	glNewList(drawList, GL_COMPILE);
		for(int y = -1; y <= 1; y +=2)
			for(int x = -1; x <= 1; x +=2)
				for(int z = -1; z <= 1; z +=2) {
					static int i = 0;
					glColor3f((x+1)/2.0f + 0.5,(y+1)/2.0f,(z+1)/2.0f);
					glBindTexture(GL_TEXTURE_2D, textures[i]);
					glPushMatrix();
					glTranslatef(x,y,z);
					drawBox1(1,1,1,1,1,1);
					glPopMatrix();
					glBindTexture(GL_TEXTURE_2D, 0);

					i++;
				}
	glEndList();
	

	for(int i = 0; i < 6; i++) {
		glBindTexture(GL_TEXTURE_2D, cubemap[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		renderViewPoint(i);
	}
	
}

void renderViewPoint(int index) {
	fbo->attach(GL_COLOR_ATTACHMENT0, cubemap[index], GL_TEXTURE_2D);

	fbo->enable();

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, 512, 512); 

	glMatrixMode(GL_PROJECTION);	
	glLoadIdentity();
	gluPerspective(90, 1, 0.1, 100);
			
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(cmd[index][1], 0, 1, 0);
	glRotatef(cmd[index][0], 1, 0, 0);
			
	glCallList(drawList);

	fbo->disable();	
}

// Called by keyboard events
void Keyboard(unsigned char key, int x, int y) {
	switch(key) {
	case 'c':
		merda = !merda;
		break;
	case 'f':
		face = (face+1)%6;
		cout << "face: " << face << "\n";
		break;
	case 'x':
		useMap = (useMap+1)%2;
		break;
	default:
		break;
	}
}