/* NOTA: il blending fatto cosi non è giusto... è un trucco perchè disabilita il depth test*/
#include <Windows.h>
#include <iostream>
#include <gl\glut.h>
#include <gl\GLAUX.H>
#include "tgaload.h"

using namespace std;

void InitGL();
void Render();
void Reshape(int, int);
void Keyboard(unsigned char, int, int);

void LoadTextures();
void SetupLights();

/* Luci: i primi tre numeri sono il colore, l'ultima è il valore alpha */
GLfloat AmbientLight[] = {0.2,0.2,0.2,1.0}; // luce ambiente (non ha sorgente) grigia
GLfloat DiffuseLight[] = {1, 1, 1, 1}; // luce diffusa bianca

/* Posizione della luce: i primi tre numeri sono le coordinate x,y,z. Il quarto per ora non si sa */
GLfloat LightPosition[] = {0, 0, 2, 1};

GLuint filter; // indica quale texture utilizzeremo
GLuint textures[3]; // storage delle texture

GLfloat xrot, yrot;
GLfloat xrotspeed, yrotspeed;
GLfloat z = -5;

bool lighting, blending;

int main(int argc, char **argv) {
	
	// prima di TUTTO (TUTTO TUTTO TUTTO) creare la finestra sennò questa libereria del cazzo
	// comincia a dare errori incomprensibili di tutti i tipi. Non fare NIENTE PRIMA DI AVER
	// CREATO LA FINESTRA.. NIENTE
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Blending 1");
	// Finestra creata... Adesso dovrebbe andare. 
	
	InitGL();
	LoadTextures();
	SetupLights();

	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Render);
	glutKeyboardFunc(Keyboard);

	glutMainLoop();
	return 0;
}

void Reshape(int w, int h) {
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)w/h,0.1,100);
	glMatrixMode(GL_MODELVIEW);
}

void Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	glTranslatef(0,0,z);
	glRotatef(xrot, 1, 0, 0);
	glRotatef(yrot, 0, 1, 0);

	glBindTexture(GL_TEXTURE_2D, textures[filter]);

	//front face
	glBegin(GL_QUADS);
		glNormal3f(0,0,1);
		glTexCoord2f(0,1); glVertex3f(1, 1, 1);
		glTexCoord2f(0,0); glVertex3f(-1, 1, 1);
		glTexCoord2f(1,0); glVertex3f(-1, -1, 1);
		glTexCoord2f(1,1); glVertex3f(1, -1, 1);
	glEnd();
	
	//back face
	glBegin(GL_QUADS);
		glNormal3f(0,0,-1);
		glTexCoord2f(0,1); glVertex3f(-1, 1, -1);
		glTexCoord2f(0,0); glVertex3f(1, 1, -1);
		glTexCoord2f(1,0); glVertex3f(1, -1, -1);
		glTexCoord2f(1,1); glVertex3f(-1, -1, -1);
	glEnd();

	//left face
	glBegin(GL_QUADS);
		glNormal3f(-1,0,0);
		glTexCoord2f(0,1); glVertex3f(-1, 1, 1);
		glTexCoord2f(0,0); glVertex3f(-1, 1, -1);
		glTexCoord2f(1,0); glVertex3f(-1, -1, -1);
		glTexCoord2f(1,1); glVertex3f(-1, -1, 1);
	glEnd();

	//rightface
	glBegin(GL_QUADS);
		glNormal3f(1,0,0);
		glTexCoord2f(0,1); glVertex3f(1, 1, 1);
		glTexCoord2f(0,0); glVertex3f(1, 1, -1);
		glTexCoord2f(1,0); glVertex3f(1, -1, -1);
		glTexCoord2f(1,1); glVertex3f(1, -1, 1);
	glEnd();

	//topface
	glBegin(GL_QUADS);
		glNormal3f(0,1,0);
		glTexCoord2f(1,0); glVertex3f(1, 1, 1);
		glTexCoord2f(0,0); glVertex3f(-1, 1, 1);
		glTexCoord2f(0,1); glVertex3f(-1, 1, -1);
		glTexCoord2f(1,1); glVertex3f(1, 1, -1);
	glEnd();

	//bottomface
	glBegin(GL_QUADS);
		glNormal3f(0,-1,0);
		glTexCoord2f(1,0); glVertex3f(-1, -1, 1);
		glTexCoord2f(0,0); glVertex3f(1, -1, 1);
		glTexCoord2f(0,1); glVertex3f(1, -1, -1);
		glTexCoord2f(1,1); glVertex3f(-1, -1, -1);
	glEnd();

	glutSwapBuffers();

	xrot += xrotspeed;
	yrot += yrotspeed;
}

void InitGL() {
	glClearColor(0,0,0,1); // colore che viene usato dal comando glClear
	glEnable(GL_TEXTURE_2D); // abilita le texture 2D
	glEnable(GL_DEPTH_TEST); // abilita il test sulla profondita

	glBlendFunc(GL_SRC_ALPHA, GL_ONE); // parametri funzione di blending (credo sia SRC)

	glShadeModel(GL_SMOOTH); // modalita di shading (non si sa di preciso)
	glClearDepth(1.0f);	 // non si sa bene
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // un suggerimento che dovrebbe migliorare la visione prospettica
}

void SetupLights() {
	glLightfv(GL_LIGHT1, GL_AMBIENT, AmbientLight);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, DiffuseLight);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT1);
}

void LoadTextures() {
	image_t temp;
	glGenTextures(3, textures);

	// FILTRO NEAREST (NO-SMOOTHING)
	glBindTexture(GL_TEXTURE_2D,textures[0]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	tgaLoad("texture.tga", &temp, TGA_DEFAULT | TGA_NO_MIPMAPS);

	// FILTRO LINEAR (NO-SMOOTHING)
	glBindTexture(GL_TEXTURE_2D,textures[1]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	tgaLoad("texture.tga", &temp, TGA_DEFAULT | TGA_NO_MIPMAPS);

	// FILTRO MIPMAPPED
	glBindTexture(GL_TEXTURE_2D,textures[2]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	tgaLoad("texture.tga", &temp, TGA_DEFAULT);
}


void Keyboard(unsigned char key, int x, int y) {
	switch(key) {
	case 'l':
		lighting = !lighting;
		
		if(lighting) {
			glEnable(GL_LIGHTING);
			cout << "Lighting On" << endl;
		}else{
			glDisable(GL_LIGHTING);
			cout << "Lighting Off" << endl;
		}
		break;
	case 'a': // rotate left
		yrotspeed += 0.01;
		break;
	case 'd': // rotate right
		yrotspeed -= 0.01;
		break;
	case 'w': // rotate up
		xrotspeed -= 0.01;
		break;
	case 's': // rotate down
		xrotspeed += 0.01;
		break;
	case '+':
		z += 0.1;
		break;
	case '-':
		z -= 0.1;
	case 'f':
		if(filter < 2)
			filter++;
		else
			filter = 0;
		cout << "Using Filter " << filter << endl;
		break;
	case 'b':
		blending = !blending;

		if(blending) {
			glEnable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
			cout << "Blending On" <<endl;
		}else {
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_BLEND);
			cout << "Blending Off" <<endl;
		}
		break;
	default:
		break;
	}
}