#include <Windows.h>
#include <gl\glut.h>
#include "SimpleParticle.h"

GLuint SimpleParticle::list;

SimpleParticle::SimpleParticle(void)
{
	color = new Color3f;
	color->r = 0;
	color->b = 0;
	color->g = 0;
}


SimpleParticle::~SimpleParticle(void)
{
	delete color;
}

void SimpleParticle::render(void) {
	glPushMatrix();
		glColor3f(color->r, color->g, color->b);
		glTranslatef(position->getX(), position->getY(), 0);
		glCallList(list);
	glPopMatrix();
}

void SimpleParticle::init(void) {
	list = glGenLists(1);
	glNewList(list, GL_COMPILE);
		glBegin(GL_QUADS);
			glVertex2f(-1, 1);
			glVertex2f(1, 1);
			glVertex2f(1, -1);
			glVertex2f(-1, -1);
		glEnd();
	glEndList();
}