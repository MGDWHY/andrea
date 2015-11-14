/*
 * Univ. Bologna CG LAB6 camerapath over a landscape
 *
 * based on a Mikael SkiZoWalker's (MoDEL) / France (Skizo@Hol.Fr) demo
 *          and David Bucciarelli (tech.hmw@plus.it)
 */

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Bspline.h"

#include "vecmath.h"

using namespace glutils;
using namespace std;

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glut.h>

#ifndef M_PI
#define M_PI 3.14159265
#endif

#define	FRAME 50

#define FOV 85

#define M_SCENE 0
#define M_EDIT 1

#define MAP_SIZE 512
#define MAP_SCALE 1
#define heightMnt 30

static GLfloat terrain[MAP_SIZE * MAP_SIZE];
static GLfloat terraincolor[MAP_SIZE * MAP_SIZE][3];

static int mode = M_SCENE;

static int fog = 1;
static int bfcull = 1;
static int textures = 1;
static int poutline = 0;
static int help = 0;
static int camera = 0;

#define WIDTH 640
#define HEIGHT 480
static int scrwidth = WIDTH;
static int scrheight = HEIGHT;

#define OBSSTARTX (MAP_SIZE*MAP_SCALE/2)
#define OBSSTARTY (MAP_SIZE*MAP_SCALE/2)

static float obs[3] = { OBSSTARTX, heightMnt * 0.7, OBSSTARTY };

static float dir[3], v1[2], v2[2];
static float v = 0.0;
static float alpha = 75.0;
static float beta = 90.0;

static GLuint texTerrain, texWater, texSky;

static int dragIndex = -1;

static vector<vec3> pathCPS;
static Bspline *path;

static GLfloat pathTime = 0.0f;

static int modelList, skyList;

static void loadsky() {
	GLUquadricObj *sphere=NULL;

	sphere = gluNewQuadric();

	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricTexture(sphere, TRUE);
	gluQuadricOrientation(sphere, GLU_INSIDE);
	
	skyList = glGenLists(1);
	glNewList(skyList, GL_COMPILE);
		glPushMatrix();
			glTranslatef(MAP_SIZE, 0, MAP_SIZE);
			glColor4f(0.0f, 0.7f, 1.0f, 1.0f);
			gluSphere(sphere, MAP_SIZE*2, 20, 20);
		glPopMatrix();
	glEndList();

}


static void loadmodel() {

	ifstream file;
	vector<vec3> vertices;
	file.open("x-wing.m");

	if(!file.is_open()) {
		cout << "Errore apertura file" << endl;
	}

	modelList = glGenLists(1);

	glNewList(modelList, GL_COMPILE);

	glPushMatrix();

	glRotatef(90, 1, 0, 0);
	glColor3f(0.5, 0.5, 0.5);

	while(file.good()) {
		string line, op;
		getline(file, line);
		stringstream ss(line);

		ss >> op;

		if(op.compare("Vertex") == 0) {
			int index;
			GLfloat x, y, z;
			ss >> index >> x >> y >> z;
			vertices.push_back(vec3(x, y, z));
		} else if(op.compare("Face") == 0) {
			int index, t1, t2, t3;
			ss >> index >> t1 >> t2 >> t3;
			glBegin(GL_TRIANGLES);
				glVertex3fv(vertices[t1-1].xyz);
				glVertex3fv(vertices[t2-1].xyz);
				glVertex3fv(vertices[t3-1].xyz);
			glEnd();
		} else {
			cout << "Token non riconosciuto: " << op << endl;
		}

	}

	glPopMatrix();
	glEndList();

	file.close();
}

static float gettime (void)
{
	static clock_t told = 0;
	clock_t tnew, ris;
	tnew = clock ();
	ris = tnew - told;
	told = tnew;
	return (ris / (float) CLOCKS_PER_SEC);
}
static void calcposobs (void)
{
	float alpha1, alpha2;

	dir[0] = sin (alpha * M_PI / 180.0);
	dir[1] = cos (beta * M_PI / 180.0);
	dir[2] = cos (alpha * M_PI / 180.0) * sin (beta * M_PI / 180.0);

	alpha1 = alpha + FOV / 2.0;
	v1[0] = sin (alpha1 * M_PI / 180.0);
	v1[1] = cos (alpha1 * M_PI / 180.0);

	alpha2 = alpha - FOV / 2.0;
	v2[0] = sin (alpha2 * M_PI / 180.0);
	v2[1] = cos (alpha2 * M_PI / 180.0);

	obs[0] += v * dir[0];
	obs[1] += v * dir[1];
	obs[2] += v * dir[2];

	if (obs[1] < 0.0)
		obs[1] = 0.0;

	// pac-man sytle periodicity
	if( obs[0] > MAP_SIZE*MAP_SCALE )
		obs[0] = 0.0;
	if( obs[0] < 0.0 )
		obs[0] = MAP_SIZE*MAP_SCALE;

	if( obs[2] > MAP_SIZE*MAP_SCALE )
		obs[2] = 0.0;
	if( obs[2] < 0.0 )
		obs[2] = MAP_SIZE*MAP_SCALE;
}
static void reshape (int width, int height)
{
	scrwidth = width;
	scrheight = height;
	glViewport (0, 0, (GLint) width, (GLint) height);
}

static void printstring (void *font, char *string)
{
	int len, i;

	len = (int) strlen (string);
	for (i = 0; i < len; i++)
		glutBitmapCharacter (font, string[i]);
}

static void printhelp (void) 
{
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f (0.0, 0.0, 0.0, 0.5);
	glRecti (40, 40, 600, 440);
	glDisable (GL_BLEND);

	glColor3f (1.0, 0.0, 0.0);
	glRasterPos2i (300, 420);
	printstring (GLUT_BITMAP_TIMES_ROMAN_24, "Help");

	glRasterPos2i (60, 390);
	printstring (GLUT_BITMAP_TIMES_ROMAN_24, "h - Toggle Help");
	glRasterPos2i (60, 360);
	printstring (GLUT_BITMAP_TIMES_ROMAN_24, "f - Toggle Fog");
	glRasterPos2i (60, 300);
	printstring (GLUT_BITMAP_TIMES_ROMAN_24, "p - Wire frame");
	glRasterPos2i (60, 270);
	printstring (GLUT_BITMAP_TIMES_ROMAN_24, "b - Togle Back face culling");
	glRasterPos2i (60, 240);
	printstring (GLUT_BITMAP_TIMES_ROMAN_24, "Arrow Keys - Rotate");
	glRasterPos2i (60, 210);
	printstring (GLUT_BITMAP_TIMES_ROMAN_24, "a - Increase velocity");
	glRasterPos2i (60, 180);
	printstring (GLUT_BITMAP_TIMES_ROMAN_24, "z - Decrease velocity");
	glRasterPos2i (60, 150);
	printstring (GLUT_BITMAP_TIMES_ROMAN_24, "m - Toggle edit path mode");
	glRasterPos2i (60, 120);
	printstring (GLUT_BITMAP_TIMES_ROMAN_24, "v - Toggle view");
}

int roundInt( float a )
{
	return floorf( a>0 ? a+0.5 : a-0.5 );
}
float getHeight(float p[2])
{
	int x = roundInt(p[0]) % MAP_SIZE;
	int y = roundInt(p[1]) % MAP_SIZE;
	return terrain[x + (y * MAP_SIZE)];
}
float* getColor(float p[2])
{
	int x = roundInt(p[0]) % MAP_SIZE;
	int y = roundInt(p[1]) % MAP_SIZE;
	return terraincolor[x + (y * MAP_SIZE)];
}
void set3f(float v[3], float x, float y, float z)
{
	v[0] = x;
	v[1] = y;
	v[2] = z;
}
void set2f(float v[2], float x, float y)
{
	v[0] = x;
	v[1] = y;
}

void drawterrain (void)
{
	int X = 0, Y = 0, k = 0;
	float quad[2][2];
	float s;
	int step = 4;

	/* Draw the terrain as opengl quads with stepping some pixels for efficiency. */
	//glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texTerrain);

	for( X=1; X<MAP_SIZE-step; X+=step )
	{
		glBegin( GL_QUAD_STRIP);
		for( Y=1; Y<MAP_SIZE-step; Y+=step )
		{
			set2f(quad[0], X+step      ,Y     );
			set2f(quad[1], X ,Y     );

			for( k=0 ; k<2 ; k++ )
			{
				glColor3fv(getColor(quad[k]));
				glTexCoord2f(quad[k][0] / MAP_SIZE, quad[k][1] / MAP_SIZE);
				glVertex3f(quad[k][0]*MAP_SCALE, getHeight(quad[k]), quad[k][1]*MAP_SCALE);
			}
		}
		glEnd();
	}

	/* Draw the water as a semi-transparent quad. */

	float t = glutGet(GLUT_ELAPSED_TIME) / 5000.0f;
	s = (MAP_SIZE-1)*MAP_SCALE;


	glDisable (GL_CULL_FACE);
	glEnable (GL_BLEND);

	glBindTexture(GL_TEXTURE_2D, texWater);
	
	glBegin (GL_QUADS);
		glColor4f (0.1, 0.7, 1.0, 0.4);
		glTexCoord2f(t,		t);			glVertex3f(0*s, heightMnt * 0.3,0*s);
		glTexCoord2f(20+t,	t);			glVertex3f(1*s, heightMnt * 0.3,0*s);
		glTexCoord2f(20+t,	20+t);		glVertex3f(1*s, heightMnt * 0.3,1*s);
		glTexCoord2f(t,		20+t);		glVertex3f(0*s, heightMnt * 0.3,1*s);
	glEnd ();

	glDisable (GL_BLEND);


	if (bfcull)
		glEnable (GL_CULL_FACE);

	
	//glPopMatrix ();
}

void drawModel(void) {
	static GLfloat prevTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	static GLfloat t = 0.0f;

	GLfloat dt = glutGet(GLUT_ELAPSED_TIME) / 1000.0f - prevTime;
	prevTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

	if(pathTime > 0.0f)
		t += (dt / pathTime );

	if(t > 0.98f)
		t = 0.0f;

	if(path != NULL) {
		static GLfloat rotMatrix[16];
		
		vec3 posObs = path->evaluate(t);
		vec3 posObj = path->evaluate(t + 0.02);

		vec3 dir = normalize(sub(posObs, posObj));
		vec3 up = vec3(0.0f, 1.0f, 0.0f);
		vec3 tan = cross(dir, up);
		
		rotMatrix[0] = tan.x;
		rotMatrix[1] = up.x;
		rotMatrix[2] = dir.x;
		rotMatrix[3] = 0.0f;

		rotMatrix[4] = tan.y;
		rotMatrix[5] = up.y;
		rotMatrix[6] = dir.y;
		rotMatrix[7] = 0.0f;

		rotMatrix[8] = tan.z;
		rotMatrix[9] = up.z;
		rotMatrix[10] = dir.z;
		rotMatrix[11] = 0.0f;

		rotMatrix[12] = 0.0f;
		rotMatrix[13] = 0.0f;
		rotMatrix[14] = 0.0f;
		rotMatrix[15] = 1.0f;

		GLfloat p[] = { posObj.x, posObj.z };
		
		if(camera == 1)
			gluLookAt(posObs.x, getHeight(p) + 20, posObs.z, posObj.x, getHeight(p) + 20, posObj.z, 0, 1, 0);
		
		glPushAttrib(GL_ENABLE_BIT);

		glDisable(GL_TEXTURE_2D);

		glPushMatrix();
			
			glTranslatef(posObj.x, getHeight(p) + 20, posObj.z);

			glMultMatrixf(rotMatrix);

			glTranslatef(-2.5f, 0.0f, 0.0f);
			
			glCallList(modelList);
			
		glPopMatrix();

		glPopAttrib();
	}

}
void drawscene (void)
{
	static int count = 0;
	static char frbuf[80];
	float fr;

	glShadeModel (GL_SMOOTH);
	glEnable (GL_DEPTH_TEST);

	if (fog)
		glEnable (GL_FOG);
	else
		glDisable (GL_FOG);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (45.0, ((GLfloat) scrwidth / (GLfloat) scrheight), MAP_SIZE*MAP_SCALE * 0.001, MAP_SIZE*MAP_SCALE * 4.0);

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();

	glPushMatrix ();

	if(camera == 0) {
		calcposobs ();
		gluLookAt (obs[0], obs[1], obs[2], obs[0] + dir[0], obs[1] + dir[1], obs[2] + dir[2], 0.0, 1.0, 0.0);
	}

	drawModel();
	
	drawterrain();
		
	glBindTexture(GL_TEXTURE_2D, texSky);
	glCallList(skyList);

	glPopMatrix ();

	if ((count % FRAME) == 0)
	{
		fr = gettime ();
		sprintf (frbuf, "Frame rate: %.3f", FRAME / fr);
	}

	glDisable (GL_DEPTH_TEST);
	glDisable (GL_FOG);
	glShadeModel (GL_FLAT);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glOrtho (-0.5, 639.5, -0.5, 479.5, -1.0, 1.0);
	
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();

	glColor3f (1.0, 0.0, 0.0);
	glRasterPos2i (10, 10);
	printstring (GLUT_BITMAP_HELVETICA_18, frbuf);

	if (help)
		printhelp ();

	glutSwapBuffers ();

	count++;
}
void editpath(void) {

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushAttrib(GL_ENABLE_BIT);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, MAP_SIZE, 0, MAP_SIZE, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPointSize(5);

	glBegin(GL_POINTS);

	for(int i = 0; i < pathCPS.size(); i++)
		glVertex3f(pathCPS[i].x, pathCPS[i].z, 0.0f);
	
	glEnd();

	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0xf0f0);

	glBegin(GL_LINE_STRIP);
		for(int i = 0; i < pathCPS.size(); i++)
			glVertex3f(pathCPS[i].x, pathCPS[i].z, 0.0f);		
	glEnd();

	glDisable(GL_LINE_STIPPLE);

	if(path != NULL) {
		glBegin(GL_LINE_STRIP);
		for(GLfloat t = 0.0f; t <= 1.0f; t += 0.001f) {
			vec3 result = path->evaluate(t);
			glVertex3f(result.x, result.z, 0.0f);
		}
		glEnd();
	}

	glPopAttrib();

	glutSwapBuffers();
}
void render(void) {
	if(mode == M_SCENE)
		drawscene();
	else 
		editpath();
}

static void key (unsigned char k, int x, int y)
{
	switch (k)
	{
		case 27:
			exit (0);
			break;
		case 'a':
			v += 0.0005;
			break;
		case 'z':
			v -= 0.0005;
			break;
		case 'p':
			if (poutline)
			{
				glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
				poutline = 0;
			}
			else
			{
				glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
				poutline = 1;
			}
			break;
		case 'h':
			help = (!help);
			break;
		case 'f':
			fog = (!fog);
			break;
		case 'b':
			if (bfcull)
			{
				glDisable (GL_CULL_FACE);
				bfcull = 0;
			}
			else
			{
				glEnable (GL_CULL_FACE);
				bfcull = 1;
			}
			break;
		case 't':
			if(textures)
				glDisable(GL_TEXTURE_2D);
			else
				glEnable(GL_TEXTURE_2D);

			textures = !textures;

			std::cout << textures;
			break;
		case 'm':
			if(mode == M_SCENE)
				mode = M_EDIT;
			else
				mode = M_SCENE;
			break;
		case 'v':
			if(camera == 1)
				camera = 0;
			else
				camera = 1;
			break;
	}
}
void mouse(int button, int state, int x, int y) {
		
	if(button == GLUT_LEFT_BUTTON && mode == M_EDIT) {
		if(state == GLUT_DOWN) {
			GLfloat xView = (GLfloat) x *  MAP_SIZE / (GLfloat)scrwidth;
			GLfloat yView = MAP_SIZE - ((GLfloat) y * MAP_SIZE / (GLfloat)scrheight);

			for(int i = 0; i < pathCPS.size(); i++) {
				if(pathCPS[i].x >= xView - 5 && pathCPS[i].x <= xView + 5 &&
					pathCPS[i].z >= yView - 5 && pathCPS[i].z <= yView + 5) {
						dragIndex = i;
						break;
				}
			}

			if(dragIndex == -1)
				pathCPS.push_back(vec3(xView, 0.0f, yView));
			
		} else
			dragIndex = -1;

		
		int totalKnots = pathCPS.size() + 3;
		int internalKnots = pathCPS.size() - 3;

		if(internalKnots > 0) {

			GLfloat *knots = new GLfloat[totalKnots];

			for(int i = 0; i < 3; i++) {
				knots[i] = 0.0f;
				knots[totalKnots - i - 1] = 1.0f;
			}

			for(int i = 3; i < internalKnots + 3; i++) {
				knots[i] = 1.0f / (internalKnots+1) * (i-2);
			}

			if(path != NULL)
				delete path;

			path = new Bspline(3, pathCPS.size(), pathCPS.data(), knots);

			pathTime = totalKnots * 2; // secs

		}
		

		
	}
}

void mouseMotion(int x, int y){
	if(dragIndex != -1) {
		GLfloat xView = (GLfloat) x *  MAP_SIZE / (GLfloat)scrwidth;
		GLfloat yView = MAP_SIZE - ((GLfloat) y * MAP_SIZE / (GLfloat)scrheight);
		pathCPS[dragIndex].x = xView;
		pathCPS[dragIndex].z = yView;
	}
}

/* ARGSUSED1 */
static void special (int k, int x, int y)
{
	switch (k)
	{
		case GLUT_KEY_LEFT:
			alpha += 2.0;
			break;
		case GLUT_KEY_RIGHT:
			alpha -= 2.0;
			break;
		case GLUT_KEY_DOWN:
			beta -= 2.0;
			break;
		case GLUT_KEY_UP:
			beta += 2.0;
			break;
	}
}
static void calccolor (GLfloat height, GLfloat c[3])
{
	GLfloat color[4][3] = {
		{1.0, 1.0, 1.0},
		{0.0, 0.8, 0.0},
		{1.0, 1.0, 0.3},
		{0.0, 0.0, 0.8}
	};
	GLfloat fact;

	height = height * (1.0/255.0);

	if (height >= 0.8)
	{
		c[0] = color[0][0];
		c[1] = color[0][1];
		c[2] = color[0][2];
		return;
	}

	if ((height < 0.8) && (height >= 0.5))
	{
		fact = (height - 0.5) * 5.0;
		c[0] = fact * color[0][0] + (1.0 - fact) * color[1][0];
		c[1] = fact * color[0][1] + (1.0 - fact) * color[1][1];
		c[2] = fact * color[0][2] + (1.0 - fact) * color[1][2];
		return;
	}

	if ((height < 0.5) && (height >= 0.4))
	{
		fact = (height - 0.4) * 10.0;
		c[0] = fact * color[1][0] + (1.0 - fact) * color[2][0];
		c[1] = fact * color[1][1] + (1.0 - fact) * color[2][1];
		c[2] = fact * color[1][2] + (1.0 - fact) * color[2][2];
		return;
	}

	if ((height < 0.4) && (height >= 0.3))
	{
		fact = (height - 0.3) * 10.0;
		c[0] = fact * color[2][0] + (1.0 - fact) * color[3][0];
		c[1] = fact * color[2][1] + (1.0 - fact) * color[3][1];
		c[2] = fact * color[2][2] + (1.0 - fact) * color[3][2];
		return;
	}

	c[0] = color[3][0];
	c[1] = color[3][1];
	c[2] = color[3][2];
}
static void loadpic (void)
{
	GLubyte bufferter[MAP_SIZE * MAP_SIZE];
	FILE *FilePic;
	int i, tmp;
	GLenum gluerr;

#ifdef _WIN32
	//if ((FilePic = fopen ("../../texture-map/heightmap.bmp", "r")) == NULL)  //Needs MAP_SIZE 256
	if ((FilePic = fopen ("texture-map/Height.bmp", "r")) == NULL)     //Needs MAP_SIZE 512
	//if ((FilePic = fopen ("../../texture-map/Land.bmp", "r")) == NULL)       //Needs MAP_SIZE 512
#else
	//if ((FilePic = fopen ("texture-map/heightmap.bmp", "r")) == NULL)        //Needs MAP_SIZE 256
	if ((FilePic = fopen ("texture-map/Height.bmp", "r")) == NULL)           //Needs MAP_SIZE 512
	//if ((FilePic = fopen ("texture-map/Land.bmp", "r")) == NULL)             //Needs MAP_SIZE 512
#endif
	{
		fprintf (stderr, "Error loading heightfield file\n");
		exit (-1);
	}
	fread (bufferter, MAP_SIZE * MAP_SIZE, 1, FilePic);
	fclose (FilePic);

	for (i = 0; i < (MAP_SIZE * MAP_SIZE); i++)
	{
		//terrain[i] = (bufferter[i] * (heightMnt / (MAP_SIZE-1.0f)));
		terrain[i] = (bufferter[i]/255.0) * heightMnt;
		calccolor ((GLfloat) bufferter[i], terraincolor[i]);
	}
}
static GLuint loadTexture(char* fileName, int width, int height) {
	GLubyte* data = new GLubyte[width*height*3];
	FILE *FilePic;
	int a;
#ifdef _WIN32
	if ((FilePic = fopen (fileName, "r")) == NULL)     //Needs MAP_SIZE 512
#else
	if ((FilePic = fopen (fileName, "r")) == NULL)           //Needs MAP_SIZE 512
#endif
	{
		fprintf (stderr, "Error loading heightfield file\n");
		std::cin >> a;
		exit (-1);
	}
	fread (data, width * height * 3, 1, FilePic);
	fclose (FilePic);

	GLuint texID;

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);

	//delete data;

	return texID;

}

static void init (void)
{
	float fogcolor[4] = { 0.6, 0.7, 0.7, 1.0 };

	glClearColor (fogcolor[0], fogcolor[1], fogcolor[2], fogcolor[3]);
	glClearDepth (1.0);
	glDepthFunc (GL_LEQUAL);
	glShadeModel (GL_SMOOTH);
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_CULL_FACE);

	glDisable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable (GL_FOG);
	glFogi (GL_FOG_MODE, GL_EXP2);
	glFogfv (GL_FOG_COLOR, fogcolor);
	glFogf (GL_FOG_DENSITY, 0.01);

	glHint (GL_FOG_HINT, GL_NICEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_TEXTURE_2D);
	texTerrain = loadTexture("texture-map/terrain-texture3.bmp", 512, 512);
	texWater = loadTexture("texture-map/Water.bmp", 256, 256);
	texSky = loadTexture("texture-map/SkyBox4.bmp", 256, 256);

	loadmodel();
	loadsky();

	reshape (scrwidth, scrheight);
}

int main (int ac, char **av)
{
	glutInitWindowPosition (0, 0);
	glutInitWindowSize (WIDTH, HEIGHT);
	glutInit (&ac, av);

	glutInitDisplayMode (GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

	glutCreateWindow ("Terrain");

	loadpic ();

	init ();

	glutReshapeFunc (reshape);
	glutDisplayFunc (render);
	glutKeyboardFunc (key);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutSpecialFunc (special);
	glutIdleFunc (render);

	glutMainLoop ();

	return 0;
}
