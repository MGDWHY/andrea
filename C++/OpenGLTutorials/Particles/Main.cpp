#include <Windows.h>
#include <iostream>
#include <cstdlib>
#include <gl/glew.h>
#include <gl/glut.h>
#include "vecmath.h"
#include "ShaderManager.h"
#include "Timer.h"
#include "lodepng.h"

#define NUM_PARTICLES 1000000
#define LIFETIME_REF 20.0f

#define ACTION_NONE 0
#define ACTION_ATTRACT 1
#define ACTION_REPULSE 2

#define RND(x) (float) rand() / RAND_MAX * x

using namespace std;
using namespace Vecmath;
using namespace glutils;

struct Particle {
	vec2 position; // window space [-1, 1] x [-1, 1]
	vec2 velocity;
	GLfloat lifetime;
	GLfloat padding[3];
};

void InitGL();
void Render();
void Reshape(int, int);
void Keyboard(unsigned char, int, int);
void Mouse(int, int, int, int);
void MouseMotion(int, int);

void InitParticles();
void EvolveParticles();
void DrawParticles();
void DrawString(string s);
void DrawInfo();

Particle * particles;

Shader *renderShader;

GLuint vaoObj, vboObj;

GLuint texStar;

vec2 actionTarget, wcsToPixel;

int action = ACTION_NONE;
bool displayHelp = true;

int renderedParticles = NUM_PARTICLES / 4;
GLfloat particleSize = 5.0f;

Timer * timer;

void pngLoad(const char* filename) {
 
  unsigned int width, height;
  std::vector<unsigned char> *image = new std::vector<unsigned char>;
  unsigned error = LodePNG::decode(*image, width, height, filename);
  unsigned char* data = new unsigned char[width*height*4];

  for(unsigned int i = 0; i < width * height * 4; i++)
	  data[i] = (*image)[i];


  gluBuild2DMipmaps ( GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data );

  delete image;
  delete data;
}
GLuint pngTexture(const char* filename) {
	GLuint texid;

	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	pngLoad(filename);

	#ifdef GL_UTILS_LOG_ENABLED
		stringstream msgss;
		msgss << "Texture loaded: " << filename;
		Logger::getDefaultLogger()->writeMessageStream(0, "pngTexture()", msgss);
	#endif

	return texid;
}

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
	
	InitGL();

	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Render);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(MouseMotion);

	glutFullScreen();

	glutMainLoop();
	return 0;
}

// called when window is resized
void Reshape(int w, int h) {
	wcsToPixel.x = 1.0f / w;
	wcsToPixel.y = 1.0f / h;
	glViewport(0,0,w,h); // viewport resize
}

// called when window is drawn
void Render() {
	static Timer * timer = new Timer();
	static int fps = 0;
	static float fpsTimer = 0.0f;

	#ifdef SHOW_FPS

	fpsTimer += timer->dtSecs();

	if(fpsTimer > 1.0f) {
		fpsTimer -= 1.0f;
		cout << "FPS: " << fps << endl; 
		fps = 0;
	} else
		fps++;

	#endif

	glClear(GL_COLOR_BUFFER_BIT );

	// evolve particles
	EvolveParticles();
	
	//draw particles
	DrawParticles();

	// draw info
	if(displayHelp)
		DrawInfo();

	glutSwapBuffers();
}

void InitGL() {
	// Init opengl(depth test, blending, lighting and so on...)

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0, 0, 0, 1);
	glPointSize(1);

	renderShader = ShaderManager::getDefaultManager()->createShader("particles2.vert", "particles2.geom", "particles2.frag");

	texStar = pngTexture("particle3.png");

	InitParticles();

	if(glGetError() != GL_NO_ERROR)
		cout << "InitGL(): error " << glGetError();
}
void InitParticles() {
	particles = new Particle[NUM_PARTICLES];

	for(int i = 0; i < NUM_PARTICLES; i++) {
		particles[i].position = vec2(RND(2.0f) - 1.0f, RND(2.0f) - 1.0f);
		particles[i].velocity = vec2(RND(1.0f) - 0.5f, RND(0.5f) - 0.25f);
		particles[i].lifetime = RND(LIFETIME_REF);
	}

	glGenVertexArrays(1, &vaoObj);
	glGenBuffers(1, &vboObj);

	glBindVertexArray(vaoObj);
	glBindBuffer(GL_ARRAY_BUFFER, vboObj);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * NUM_PARTICLES, (GLvoid*) particles, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 32, 0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 32, (char*)NULL + 16);
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	timer = new Timer();
}
void EvolveParticles() {

	static vec2 gravity = vec2(0.0, -1.0);

	float dt = timer->dtSecs();

	for(int i = 0; i < renderedParticles; i++) {
		particles[i].position = add(particles[i].position, scale(particles[i].velocity, dt));
		particles[i].velocity = add(particles[i].velocity, scale(gravity, dt));

		if(action != ACTION_NONE) {
			static vec2 attractForceDir;
			attractForceDir = normalize(sub(actionTarget, particles[i].position));
			if(action == ACTION_ATTRACT)
				particles[i].velocity = add(particles[i].velocity, scale(attractForceDir, dt*2.0f));
			else {
				particles[i].velocity = add(particles[i].velocity, scale(attractForceDir, -dt*16.0));
			}
		}

		if(particles[i].position.y < -1.0) {
			//particles[i].position.y = -1.0f;

			if(particles[i].velocity.y < 0.0)
				particles[i].velocity.y = particles[i].velocity.y * -0.5f;
		}

		if(particles[i].position.y > 1.0) {
			//particles[i].position.y = 1.0f;

			if(particles[i].velocity.y > 0.0)
				particles[i].velocity.y = particles[i].velocity.y * -0.5f;
		}

		if(particles[i].position.x < -1.0f) {
			//particles[i].position.x = -1.0f;

			if(particles[i].velocity.x < 0.0)
				particles[i].velocity.x *= -0.5f;
		}

		if(particles[i].position.x > 1.0f) {
			//particles[i].position.x = 1.0f;

			if(particles[i].velocity.x > 0.0)
				particles[i].velocity.x *= -0.5f;
		}

		particles[i].lifetime += dt;

		if(particles[i].lifetime > LIFETIME_REF) {
			particles[i].lifetime = 0.0f;
			particles[i].position = vec2(RND(2.0f) - 1.0f, RND(2.0f) - 1.0f);
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, vboObj);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * NUM_PARTICLES, 0, GL_DYNAMIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * NUM_PARTICLES, (GLvoid*) particles, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if(glGetError() != GL_NO_ERROR)
		cout << "EvolveParticles(): error " << glGetError();
}
void DrawParticles() {

	renderShader->enable();

	renderShader->setUniform("in_LifetimeRef", LIFETIME_REF);
	renderShader->setUniform("in_WCStoPixel", wcsToPixel);
	renderShader->setUniform("in_PointSize", (GLfloat)particleSize);
	renderShader->setUniform("in_Time", timer->timeSecs());
	renderShader->setUniformTexture("in_Sprite", 0, texStar);

	glBindVertexArray(vaoObj);

	glDrawArrays(GL_POINTS, 0, renderedParticles);

	glBindVertexArray(0);

	renderShader->disable();

	if(glGetError() != GL_NO_ERROR)
		cout << "DrawParticles(): error " << glGetError() << endl;

}

// Called by keyboard events
void Keyboard(unsigned char key, int x, int y) {
	switch(key) {
		case 'q':
			exit(0);
			break;
		case '+':
			if(renderedParticles < NUM_PARTICLES)
				renderedParticles += 5000;
			break;
		case '-':
			if(renderedParticles > 10000)
				renderedParticles -= 5000;
			break;
		case '1':
			if(particleSize < 30)
				particleSize += 2.5f;
			break;
		case '2':
			if(particleSize > 2.5f)
				particleSize -= 2.5f;
			break;
		case 'h':
			displayHelp = !displayHelp;
			break;
		case VK_ESCAPE:
			exit(0);
	}
}

void Mouse(int button, int state, int x, int y) {
	if(state == GLUT_DOWN) {
		actionTarget.x = (GLfloat) x / glutGet(GLUT_WINDOW_WIDTH) * 2.0f - 1.0f;
		actionTarget.y = (1.0f - (GLfloat) y / glutGet(GLUT_WINDOW_HEIGHT)) * 2.0f - 1.0f;		
		if(button == GLUT_LEFT)
			action = ACTION_ATTRACT;
		else
			action = ACTION_REPULSE;
	} else
		action = ACTION_NONE;
}
void MouseMotion(int x, int y) {
	if(action != ACTION_NONE) {
		actionTarget.x = (GLfloat) x / glutGet(GLUT_WINDOW_WIDTH) * 2.0f - 1.0f;
		actionTarget.y = (1.0f - (GLfloat) y / glutGet(GLUT_WINDOW_HEIGHT)) * 2.0f - 1.0f;			
	}
}

void DrawInfo() {
	static char buffer[200];

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT), -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glLineWidth(2);
	glColor3f(1.0f, 1.0f, 1.0f);

	glPushMatrix();
	glTranslatef(10, glutGet(GLUT_WINDOW_HEIGHT) - 20, 0);
	glScalef(0.1, 0.1, 0.1);
	sprintf(buffer, "Press +/- to increase/decrease number of rendered particles");
	DrawString(buffer);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10, glutGet(GLUT_WINDOW_HEIGHT) - 40, 0);
	glScalef(0.1, 0.1, 0.1);
	sprintf(buffer, "Press 1/2 to increase/decrease particle size");
	DrawString(buffer);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10, glutGet(GLUT_WINDOW_HEIGHT) - 60, 0);
	glScalef(0.1, 0.1, 0.1);
	sprintf(buffer, "Press Mouse LEFT/RIGHT to attract/repulse particles");
	DrawString(buffer);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(10, 40, 0);
	glScalef(0.1, 0.1, 0.1);
	sprintf(buffer, "Particles [10-200K]: %d", renderedParticles);
	DrawString(buffer);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10, 20, 0);
	glScalef(0.1, 0.1, 0.1);
	sprintf(buffer, "Particle size [5-30]: %.2f", particleSize);
	DrawString(buffer);
	glPopMatrix();

}
void DrawString(string s) {

	glPushMatrix();

	for(unsigned int i = 0; i < s.length(); i++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, s[i]);

	glPopMatrix();

}