#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ctime>
#include "gl/glew.h"
#include "gl/glut.h"
#include "cuda_runtime.h"
#include "cuda_gl_interop.h"

using namespace std;

#define W_LEFT -150.0f
#define W_RIGHT 150.0f
#define W_BOTTOM -150.0f
#define W_TOP 150.0f


#define GL_ERROR(x, i) std::cout << x << ": Error code -> " << i << std::endl
#define MSG(x) std::cout << (x) << std::endl

#define RND(i) ((float) rand() / RAND_MAX) * (i)

#define NUM_PARTICLES 512 * 15000
#define THREADS_PER_BLOCK 128
#define BLOCKS NUM_PARTICLES / THREADS_PER_BLOCK
#define TIME_SECS (float) clock() / CLOCKS_PER_SEC

#define ACTION_NONE 0
#define ACTION_ATTRACT 1
#define ACTION_REPULSE 2

#define G_CONSTANT 9.8f

#define SHOW_FPS

void InitGL();
void Render();
void Reshape(int, int);
void Keyboard(unsigned char, int, int);
void Mouse(int, int, int, int);
void MouseMotion(int, int);

void InitBuffers();
void InitShaders();
void InitCUDA();

void LastGLError(const char*);

void DrawParticles(float3);

char * LoadTextFile(const char *);

GLuint CreateShader(char*,int,GLenum);

struct Particle {
	float2 position;
	float2 velocity;
};

const float3 colors[5] = {
						make_float3(1.0, 0.2, 0.2),
						make_float3(0.7, 0.7, 0.2),
						make_float3(0.0, 0.3, 0.8),
						make_float3(0.8, 0.2, 0.8),
						make_float3(0.2, 1.0, 0.2)

					};

GLuint vao, vbo;
GLuint shaderProgram;

cudaGraphicsResource * vboCuda;

int action = ACTION_NONE;
GLfloat actionX, actionY;

__global__ void EvolveParticles(Particle * particles, GLfloat dt, int action, GLfloat actionX, GLfloat actionY) {
	int index = blockDim.x * blockIdx.x + threadIdx.x;
	
	// Load particle from global memory
	Particle p = particles[index];
	
	p.position.x += p.velocity.x * dt;
	p.position.y += p.velocity.y * dt;

	if(action == ACTION_ATTRACT) {
		float dx = actionX - p.position.x;
		float dy = actionY - p.position.y;
		float module = sqrtf(dx*dx + dy*dy);

		if(module < 1.0f) module = 1.0f;

		p.velocity.x += dx / module * G_CONSTANT * 8.0f / module;
		p.velocity.y += dy / module * G_CONSTANT * 8.0f / module;
	} else if(action == ACTION_REPULSE) {
		float dx = actionX - p.position.x;
		float dy = actionY - p.position.y;
		float module = sqrtf(dx*dx + dy*dy);

		if(module < 1.0f) module = 1.0f;

		p.velocity.x -= dx / module * G_CONSTANT * 60.0f / module;
		p.velocity.y -= dy / module * G_CONSTANT * 60.0f / module;
	}

	p.velocity.y -= dt * G_CONSTANT;

	if(p.position.y < W_BOTTOM && p.velocity.y < 0.0f) {
		p.velocity.y = - p.velocity.y * 0.5f;
		p.velocity.x = p.velocity.x * 0.8f;
	}
	else if(p.position.y > W_TOP && p.velocity.y > 0.0f)
		p.velocity.y = - p.velocity.y;

	else if(p.position.x < W_LEFT && p.velocity.x < 0.0f)
		p.velocity.x = - p.velocity.x * 0.8f;
	else if(p.position.x > W_RIGHT && p.velocity.x > 0.0f)
		p.velocity.x = - p.velocity.x * 0.8f;
	
	// Store updated particle on global memory
	particles[index] = p;
}


int main(int argc, char **argv) {
	
	// prima di TUTTO (TUTTO TUTTO TUTTO) creare la finestra sennò non funziona una mazza
	glutInit(&argc, argv);
	// rgba mode, double buffering, depth buffering, stencil buffering
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	// window' top left corner position
	glutInitWindowPosition(0,0);
	// window's size
	glutInitWindowSize(640, 480);
	// create window
	glutCreateWindow("CUDA Particles");
	// Finestra creata... Adesso dovrebbe andare, ma


	glewInit();
	
	InitGL();

	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Render);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(MouseMotion);

	//glutFullScreen();

	glutMainLoop();

}



// called when window is resized
void Reshape(int w, int h) {
	glViewport(0,0,w,h); // viewport resize
}


// called when window is drawn
void Render() {
	static float prevTime = TIME_SECS;
	static float dtsecs = 0.0f;
	static float fpsTimer = 0.0f;
	static float colorTimer = 0.0f;
	static float3 color1, color2, colorResult;

	static int colorIndex = 0;
	static int fps;

	dtsecs = TIME_SECS - prevTime;
	prevTime = TIME_SECS;

	colorTimer += dtsecs / 10.0f;
	
	if(colorTimer >= 2.0f) {
		colorTimer = 0.0f;
		if(colorIndex == 4)
			colorIndex = 0;
		else
			colorIndex++;
	}

	color1 = colors[colorIndex];
	color2 = colorIndex == 4 ? colors[0] : colors[colorIndex+1];
	
	if(colorTimer < 1.0f)
		colorResult = color1;
	else {
		float t = colorTimer - 1.0f;
		colorResult.x = color2.x * t + color1.x * (1-t);
		colorResult.y = color2.y * t + color1.y * (1-t);
		colorResult.z = color2.z * t + color1.z * (1-t);
	}

	#ifdef SHOW_FPS
		fpsTimer += dtsecs;
		fps++;
		
		if(fpsTimer > 1.0f) {
			std::cout << "FPS: " << fps << std::endl;
			fpsTimer -= 1.0f;
			fps = 0;
		}
	#endif

	Particle * particles;
	size_t length;

	cudaGraphicsMapResources(1, &vboCuda, 0);
	cudaGraphicsResourceGetMappedPointer((void**)&particles, &length, vboCuda);

	EvolveParticles<<<BLOCKS, THREADS_PER_BLOCK>>>(particles, dtsecs, action, actionX, actionY);

	cudaThreadSynchronize();

	cudaGraphicsUnmapResources(1, &vboCuda, 0);

	DrawParticles(colorResult);

	glutSwapBuffers(); // swap backbuffer with frontbuffer
}


void InitGL() {
	// Init opengl(depth test, blending, lighting and so on...)
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE); // additive blending

	InitBuffers();
	InitCUDA();
	InitShaders();

	LastGLError("InitGL():");
}


// Called by keyboard events
void Keyboard(unsigned char key, int x, int y) {
	if(key == 'q') {
		exit(0);
	}
}



// Called by mouse events
void Mouse(int button, int state, int x, int y) {
	if(state == GLUT_DOWN) {
		actionX = (GLfloat) x / glutGet(GLUT_WINDOW_WIDTH) * (W_RIGHT - W_LEFT) + W_LEFT;
		actionY = (W_TOP - W_BOTTOM) - (GLfloat) y / glutGet(GLUT_WINDOW_HEIGHT) * (W_TOP - W_BOTTOM) + W_BOTTOM;		
		if(button == GLUT_LEFT)
			action = ACTION_ATTRACT;
		else
			action = ACTION_REPULSE;
	} else
		action = ACTION_NONE;
}

void MouseMotion(int x, int y) {
	if(action != ACTION_NONE) {
		actionX = (GLfloat) x / glutGet(GLUT_WINDOW_WIDTH) * (W_RIGHT - W_LEFT) + W_LEFT;
		actionY = (W_TOP - W_BOTTOM) - (GLfloat) y / glutGet(GLUT_WINDOW_HEIGHT) * (W_TOP - W_BOTTOM) + W_BOTTOM;		
	}
}

void InitBuffers() {

	Particle * particles = new Particle[NUM_PARTICLES];

	for(int i = 0; i < NUM_PARTICLES; i++) {
		particles[i].position.x = RND(W_RIGHT - W_LEFT) + W_LEFT;
		particles[i].position.y = RND(W_TOP - W_BOTTOM) + W_BOTTOM;

		particles[i].velocity.x = RND(20.0f) - 10.0f;
		particles[i].velocity.y = RND(20.0f) - 10.0f;
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * NUM_PARTICLES, (void*) particles, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 16, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 16, (char*)NULL + 8);

	glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	
	LastGLError("InitBuffers()");
}

void InitCUDA() {
	cudaGLSetGLDevice(0);
	cudaGraphicsGLRegisterBuffer(&vboCuda, vbo, cudaGraphicsMapFlagsNone);
}


void DrawParticles(float3 color) {

	static float tmatrix[16];

	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);	
	glLoadIdentity();
	glOrtho(W_LEFT, W_RIGHT, W_BOTTOM, W_TOP, -1, 1);

	glUseProgram(shaderProgram);

	int locMat = glGetUniformLocation(shaderProgram, "in_ModelViewProjectionMatrix");
	int locColor = glGetUniformLocation(shaderProgram, "in_Color");

	glGetFloatv(GL_PROJECTION_MATRIX, tmatrix);

	glUniformMatrix4fv(locMat, 1, GL_FALSE, tmatrix);
	glUniform4f(locColor, color.x, color.y, color.z, 0.1f);

	glBindVertexArray(vao);

	glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);

	glBindVertexArray(0);

	glUseProgram(0);
}


void LastGLError(const char * msg) {
	GLuint error = glGetError();
	if(error != GL_NO_ERROR)
		GL_ERROR(msg, error);
}

char * LoadTextFile(const char * fileName, int * length) {
	ifstream file;
	char * data = NULL;
	int len;

	file.open(fileName, ifstream::binary);
	
	file.seekg(0, ios_base::end);
	len = file.tellg();
	file.seekg(0, ios_base::beg);

	data = new char[len];

	if(!file.eof())
		file.read(data, len);
	else
		MSG("Bad file!");

	

	file.close();

	*length = len;

	return data;
}

void InitShaders() {
	GLuint vs, gs, fs;
	int vsl, gsl, fsl;
	char *vsSrc, *gsSrc, *fsSrc;
	
	vsSrc = LoadTextFile("particles.vert", &vsl);
	gsSrc = LoadTextFile("particles.geom", &gsl);
	fsSrc = LoadTextFile("particles.frag", &fsl);

	vs = CreateShader(vsSrc, vsl, GL_VERTEX_SHADER);
	gs = CreateShader(gsSrc, gsl, GL_GEOMETRY_SHADER);
	fs = CreateShader(fsSrc, fsl, GL_FRAGMENT_SHADER);

	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vs);
	glAttachShader(shaderProgram, gs);
	glAttachShader(shaderProgram, fs);
	
	glLinkProgram(shaderProgram);

	int linkStatus = 0;

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);
	if(linkStatus)
		MSG("Program linked!");
	else
		MSG("Can't link the program!");
}

GLuint CreateShader(char * source, int length, GLenum type) {
	GLuint shader = glCreateShader(type);

	glShaderSource(shader, 1, (const char**) &source, &length);

	glCompileShader(shader);

	int compiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if(compiled)
		MSG("Shader compiled!");
	else	
		MSG("Shader not compiled!");

	return shader;
}