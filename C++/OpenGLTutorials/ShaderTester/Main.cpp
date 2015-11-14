#include <iostream>
#include <sstream>
#include <string>
#include "lodepng.h"
#include "vecmath.h"
#include "VertexArrayObject.h"
#include "MatrixStack.h"
#include "ShaderManager.h"
#include "GLFont.h"

#define LINE_HEIGHT 15

#define IN_SHADER_FILE "shaders/tessellation_hf"
#define IN_GEOMETRY true

#define PLANE_TESSELLATION 16

//mode
#define M_POINTS 0
#define M_TRIANGLES 1
#define M_TRIANGLE_STRIP 2

//conding mode
#define C_NONE 0
#define C_VERTEX 1
#define C_GEOMETRY 2
#define C_FRAGMENT 3

using namespace std;
using namespace glutils;

struct Vertex {
	vec4 position;
	vec3 normal;
	vec2 texCoord;
	vec2 hfCoord;
	GLfloat padding[5];
};

const string KEYWORDS[] = { "#version", "if", "else", "void", "int", "float",
							"vec2", "vec3", "vec4", "mat2", "mat3", "mat4" 
							};

Shader * shader;
VertexArrayObject * plane;

GLFont * font;

GLuint texHF, texTerrain, texFont;

int cursorPos = 0;
string code[3] = { "", "",""};

// 0 - points
// 1 - triangles
// 2 - triangle strip
int mode = M_POINTS;

int codingMode = C_VERTEX & C_NONE;

const string mode_Descr[3] = { "POINTS", "TRIANGLES", "TRIANGLE STRIP" };

string shaderFile = IN_SHADER_FILE;
bool geometry = IN_GEOMETRY;

GLfloat zcam = -5, rotX = 0, rotY;
int prevX, prevY;
bool dragging;

MatrixStack *viewStack, *projStack;

GLuint ** planeIndices = new GLuint*[3];

GLfloat viewWidth, viewHeight;

void Render();
void Reshape(int, int);
void Keyboard(unsigned char, int, int);
void KeyboardSpecial(int, int, int);
void Mouse(int, int, int, int);
void MouseMotion(int, int);

void DrawString(string, int , int);
void DrawCodingWindow();
void DrawCode(vec2);

vec2 FindCursorPos();


void PngLoad(const char* );
GLuint PngTexture(const char*);

void InitGL();
void InitTestShapes();

void main(int argc, char **argv) {
	// prima di TUTTO (TUTTO TUTTO TUTTO) creare la finestra sennò non funziona una mazza
	glutInit(&argc, argv);
	// rgba mode, double buffering, depth buffering, stencil buffering
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
	// window' top left corner position
	glutInitWindowPosition(0,0);
	// window's size
	glutInitWindowSize(640, 480);
	// create window
	glutCreateWindow("Shader Tester");
	// Finestra creata... Adesso dovrebbe andare, ma
	
	glewInit();
	InitGL();

	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Render);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(KeyboardSpecial);
	glutMouseFunc(Mouse);
	glutMotionFunc(MouseMotion);

	glutMainLoop();
}

// called when window is resized
void Reshape(int w, int h) {
	viewWidth = w;
	viewHeight = h;
	glViewport(0,0,w,h); // viewport resize
}

// called when window is drawn
void Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPointSize(20);

	projStack->loadIdentity();
	projStack->perspective(45, viewWidth / viewHeight, 0.1, 500);

	viewStack->loadIdentity();
	viewStack->translate(0, -3, zcam);
	viewStack->rotate(rotX, 1, 0, 0);
	viewStack->rotate(rotY, 0, 1, 0);
	
	static vec4 hfNormal4;
	static vec3 hfNormal3;

	hfNormal4 = multvec(viewStack->current(), vec4(0.0, 1.0, 0.0, 0.0));
	hfNormal3 = vec3(hfNormal4.x, hfNormal4.y, hfNormal4.z);
	
	shader->enable();
	shader->setUniformMatrix("in_ModelViewMatrix", viewStack->current());
	shader->setUniformMatrix("in_ProjectionMatrix", projStack->current());
	shader->setUniform("in_HFNormal", hfNormal3);
	shader->setUniform("in_HFMaxHeight", 100.0f);
	shader->setUniform("in_FrustumDepth", 499.9f);
	shader->setUniformTexture("in_HF", 0, texHF);
	shader->setUniformTexture("in_Terrain", 1, texTerrain);


	glBindVertexArray(plane->getID());

	static int numElements;

	switch(mode) {
	case M_POINTS:
		numElements = PLANE_TESSELLATION * PLANE_TESSELLATION;
		plane->setElementsData(sizeof(GLuint), numElements, planeIndices[mode]);
		glBindVertexArray(plane->getID());
		glDrawElements(GL_POINTS, numElements, GL_UNSIGNED_INT, 0);
		break;
	case M_TRIANGLES:
		numElements = 6 * (PLANE_TESSELLATION - 1) * (PLANE_TESSELLATION - 1);
		plane->setElementsData(sizeof(GLuint), numElements, planeIndices[mode]);
		glBindVertexArray(plane->getID());
		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
		break;
	}

	glBindVertexArray(0);

	shader->disable();
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT), -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glPushAttrib(GL_POLYGON_BIT);
	glLineWidth(2);

	glColor3f(1,1,1);
	
	if(codingMode == C_NONE)
		DrawString("Mode (press 'm' to toggle'): " + mode_Descr[mode], 0, 0);
	else
		DrawCodingWindow();

	glPopAttrib();

	glutSwapBuffers();
}

void InitGL() {
	// Init opengl(depth test, blending, lighting and so on...)
	glEnable(GL_DEPTH_TEST);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	InitTestShapes();

	ShaderManager * manager = ShaderManager::getDefaultManager();

	if(geometry)
		shader = manager->createShader(shaderFile + ".vert", shaderFile + ".geom", shaderFile + ".frag");
	else
		shader = manager->createShader(shaderFile + ".vert", shaderFile + ".frag");

	viewStack = new MatrixStack(16);
	projStack = new MatrixStack(4);
	
	texFont = PngTexture("textures/font.png");
	texHF = PngTexture("textures/hf.png");
	texTerrain = PngTexture("textures/texture.png");

	font = new GLFont(texFont);
	font->setColor(vec4(1,1,1,1));
	font->setSize(14);
}

// Called by keyboard events
void KeyboardSpecial(int key, int x, int y) {
	if(key == GLUT_KEY_F1) { 
		if(codingMode == C_VERTEX)
			codingMode = C_GEOMETRY;
		else if(codingMode == C_GEOMETRY)
			codingMode =  C_FRAGMENT;
		else if(codingMode == C_FRAGMENT)
			codingMode = C_NONE;
		else
			codingMode = C_VERTEX;

		cursorPos = 0;
	}
	if(codingMode != C_NONE) {
		if(key == GLUT_KEY_LEFT && cursorPos > 0)
			cursorPos--;
		else if(key == GLUT_KEY_RIGHT && cursorPos < code[codingMode].length())
			cursorPos++;
		else if(key == GLUT_KEY_UP && cursorPos > 0) {
			int ind = code[codingMode].rfind('\n', cursorPos - 1);

			if(ind != string::npos)
				cursorPos = ind;
			else
				cursorPos = 0;

		} else if(key == GLUT_KEY_DOWN && cursorPos < code[codingMode].length()) {
			int ind = code[codingMode].find('\n', cursorPos + 1);

			if(ind != string::npos)
				cursorPos = ind;
			else
				cursorPos = code[codingMode].length();

		}
	}
}
void Keyboard(unsigned char key, int x, int y) {
	if(codingMode == C_NONE) {
		switch(key) {
			case '+':
				zcam += 0.5;
				break;
			case '-':
				zcam -= 0.5;
				break;
			case 'm':
			case 'M':
				if(mode == 2)
					mode = 0;
				else
					mode++;
				break;
		}
	} else {
		if((key >= 'a' && key <= 'z') ||
			(key >= 'A' && key <= 'Z') ||
			(key >= '0' && key <= '9') ||
			key == '_' || key == '#' || key == '.' || key == ',' ||
			key == '(' || key == ')' || key == '[' || key == ']' ||
			key == '{' || key == '}' || key == '+' || key == '-' ||
			key == '*' || key == '/' || key == '%' || key == '&' ||
			key == '|' || key == '^' || key == '<' || key == '>' ||
			key == '=' || key == '#' || key == ';' || key == ':'
			) {
				
			code[codingMode].insert(cursorPos++, (char*)&key, 1);
		} else if(key == 13) { // carriage return
			code[codingMode].insert(cursorPos++, "\n", 1);
		} else if(key == 32) { // space
			code[codingMode].insert(cursorPos++, " ", 1);
		} else if(key == 8 && cursorPos > 0) { // backspace
			code[codingMode].replace(--cursorPos, 1, "");
		} else if(key == 9) { // tab
			for(int i = 0; i < 3; i++)
				code[codingMode].insert(cursorPos++, " ", 1);
		}
	}
}

void InitTestShapes() {

	Vertex *vertices = new Vertex[PLANE_TESSELLATION * PLANE_TESSELLATION];

	//plane

	for(int z = 0; z < PLANE_TESSELLATION; z++)
		for(int x = 0; x < PLANE_TESSELLATION; x++) {
			vertices[z*PLANE_TESSELLATION + x].position = vec4(scale(vec3(x - PLANE_TESSELLATION / 2.0f, 0.0f, z - PLANE_TESSELLATION / 2.0f), 100), 1.0f);
			vertices[z*PLANE_TESSELLATION + x].normal = vec3(0, 1, 0);
			vertices[z*PLANE_TESSELLATION + x].texCoord = vec2(x/(PLANE_TESSELLATION - 1.0f), z/(PLANE_TESSELLATION - 1.0f));
			vertices[z*PLANE_TESSELLATION + x].hfCoord = vec2(x/(PLANE_TESSELLATION - 1.0f), z/(PLANE_TESSELLATION - 1.0f));
		}

	planeIndices[M_POINTS] = new GLuint[PLANE_TESSELLATION * PLANE_TESSELLATION]; // points indices
	planeIndices[M_TRIANGLES] = new GLuint[6 * (PLANE_TESSELLATION - 1) * (PLANE_TESSELLATION - 1)]; // triangle indices

	for(int i = 0; i < PLANE_TESSELLATION * PLANE_TESSELLATION; i++)
		planeIndices[M_POINTS][i] = (GLuint)i;

	for(int z = 0; z < PLANE_TESSELLATION - 1; z++) {
		for(int x = 0; x < PLANE_TESSELLATION - 1; x++) {
			planeIndices[M_TRIANGLES][6 * (PLANE_TESSELLATION - 1) * z + x * 6] = z * PLANE_TESSELLATION + x;
			planeIndices[M_TRIANGLES][6 * (PLANE_TESSELLATION - 1) * z + x * 6 + 1] = (z + 1) * PLANE_TESSELLATION + x;
			planeIndices[M_TRIANGLES][6 * (PLANE_TESSELLATION - 1) * z + x * 6 + 2] = z * PLANE_TESSELLATION + x + 1;
			planeIndices[M_TRIANGLES][6 * (PLANE_TESSELLATION - 1) * z + x * 6 + 3] = z * PLANE_TESSELLATION + x + 1;
			planeIndices[M_TRIANGLES][6 * (PLANE_TESSELLATION - 1) * z + x * 6 + 4] = (z + 1) * PLANE_TESSELLATION + x;
			planeIndices[M_TRIANGLES][6 * (PLANE_TESSELLATION - 1) * z + x * 6 + 5] = (z + 1) * PLANE_TESSELLATION + x + 1;
		}
	}

	plane = new VertexArrayObject(1, GL_DYNAMIC_DRAW);

	plane->setBufferData(0, sizeof(Vertex) * PLANE_TESSELLATION * PLANE_TESSELLATION, vertices);
	plane->setVertexAttribute(0, 0, 4, GL_FLOAT, GL_FALSE, 64, 0);
	plane->setVertexAttribute(0, 1, 3, GL_FLOAT, GL_FALSE, 64, ((char*)0 + 16)); 
	plane->setVertexAttribute(0, 2, 2, GL_FLOAT, GL_FALSE, 64, ((char*)0 + 28));
	plane->setVertexAttribute(0, 3, 2, GL_FLOAT, GL_FALSE, 64, ((char*)0 + 36));

	plane->enableVertexAttribute(0);
	plane->enableVertexAttribute(1);
	plane->enableVertexAttribute(2);
	plane->enableVertexAttribute(3);
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
		
		rotY += (x - prevX) / 50.0f;
		rotX += (y - prevY) / 50.0f;

		prevX = x;
		prevY = y;
	}
}

void PngLoad(const char* filename) {
 
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
GLuint PngTexture(const char* filename) {
	GLuint texid;

	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	PngLoad(filename);

	#ifdef GL_UTILS_LOG_ENABLED
		stringstream msgss;
		msgss << "Texture loaded: " << filename;
		Logger::getDefaultLogger()->writeMessageStream(0, "pngTexture()", msgss);
	#endif

	return texid;
}

void DrawString(string s, int windowX, int windowY) {

	glPushMatrix();

	glTranslatef(windowX, glutGet(GLUT_WINDOW_HEIGHT) - windowY - LINE_HEIGHT, 0);
	glScalef(0.1, 0.1, 1);
	

	for(unsigned int i = 0; i < s.length(); i++)
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, s[i]);

	glPopMatrix();

}
void DrawCodingWindow() {
	if(codingMode == C_NONE)
		return;

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(1.0, 1.0, 1.0, 0.5);

	glBegin(GL_QUADS);
		glVertex3f(0, 0, 0);
		glVertex3f(glutGet(GLUT_WINDOW_WIDTH), 0, 0);
		glVertex3f(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0);
		glVertex3f(0, glutGet(GLUT_WINDOW_HEIGHT), 0);
	glEnd();

	glColor3f(0,0,0);

	DrawCode(vec2(0, glutGet(GLUT_WINDOW_HEIGHT) - font->getSize()));

	vec2 pos = FindCursorPos();

	glColor3f(1,0,0);

	glBegin(GL_LINES);
		glVertex3f(pos.x, pos.y, 0);
		glVertex3f(pos.x, pos.y - font->getSize(), 0);		
	glEnd();

	glPopAttrib();
}

vec2 FindCursorPos() {
	int linesCount = 0, lastOccurrence = -1;
	
	for(int i = 0; i < cursorPos; i++)
		if(code[codingMode].at(i) == '\n') {
			lastOccurrence = i;
			linesCount++;
		}

	int len = cursorPos - lastOccurrence - 1 ;

	return vec2(font->getSize() * len, glutGet(GLUT_WINDOW_HEIGHT) - linesCount * font->getSize());
}
void DrawCode(vec2 position) {

	static const vec4 black(0,0,0,1);
	static const vec4 blue(0,0,1,1); 

	int i = 0;

	// temporaneo
	projStack->loadIdentity();
	projStack->ortho(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT), -1, 1);
	// fine temporaneo

	viewStack->push();

	viewStack->loadIdentity();
	viewStack->translate(position.x, position.y, 0);

	stringstream ss = stringstream(code[codingMode], ios_base::in);

	while(i != -1) {

		int ind = code[codingMode].find('\n', i);
		string line; 

		if(ind != string::npos) {
			line = code[codingMode].substr(i, ind - i);
			i = ind + 1;
		} else {
			line = code[codingMode].substr(i, string::npos);
			i = -1;
		}

		string token = "";

		viewStack->push();

		for(int k = 0; k < line.length(); k++) {
			

			char ch = line.at(k);

			if(ch == ' ' || k == line.length() - 1) {

				if(k == line.length() - 1 && ch != ' ')
					token += ch;

				if(token.compare("") != 0) {
					
					for(int h = 0; h < 12; h ++) 
						if(token.compare(KEYWORDS[h]) == 0)
							font->setColor(blue);

					font->strokeString(token, mult(projStack->current(), viewStack->current()));
				
					font->setColor(black);
					
					viewStack->translate(font->getSize() * (token.length() + 1), 0, 0);		

				} else {
					viewStack->translate(font->getSize(), 0, 0);
					
				}

				token = "";
			} else {
				token += ch;

			}
		}

		viewStack->pop();

		viewStack->translate(0, - font->getSize(), 0);
	}

	viewStack->pop();

}