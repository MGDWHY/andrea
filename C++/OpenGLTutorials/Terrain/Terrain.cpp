#include <iostream>
#include <gl\glut.h>
#include "Terrain.h"

void Terrain::compile(GLenum mode) {

	//normals generation
	for(int x = 0; x < width; x++)
		for(int z = 0; z < length; z++) {
			Vec3* sum = new Vec3();
			Vec3 *right, *left, *up, *down;

			//facciamolo meglio (ora va bene, avevo sbagliato la funzione cross)
			if(x > 0)
				left = new Vec3(-1, heights[x - 1][z] - heights[x][z], 0);

			if(x < width-1)
				right = new Vec3(1, heights[x + 1][z] - heights[x][z], 0);

			if(z > 0)
				up = new Vec3(0, heights[x][z - 1] - heights[x][z], -1);

			if(z < length-1)
				down = new Vec3(0, heights[x][z + 1] - heights[x][z], 1);

			if(x > 0 && z > 0)
				sum = sum->add(up->cross(left))->normalise();;

			if(x < width-1 && z < length-1)
				sum = sum->add(down->cross(right))->normalise();

			if(x > 0 && z < length-1)
				sum = sum->add(left->cross(down))->normalise();

			if(x < width-1 && z > 0)
				sum = sum->add(right->cross(up))->normalise();

			normals[x][z].set(sum);
		}



		// (per generare correttamente le coordinate delle texture bisogna usare i triangoli e non i triangle strip)
		//terrain list generation
		terrainList = glGenLists(1);
		glNewList(terrainList, GL_COMPILE);
			for(int x = 0; x < width-1; x++) {
				float s = 0;
				glBegin(mode);
				for(int z = 0; z < length; z++) {
				
					glTexCoord2f(0, s);
					glNormal3f(normals[x][z].getX(), normals[x][z].getY(), normals[x][z].getZ());
					glVertex3f(x - width/2.0, heights[x][z], z - length /2.0);
					
					glTexCoord2f(1, s);
					glNormal3f(normals[x+1][z].getX(), normals[x+1][z].getY(), normals[x+1][z].getZ());
					glVertex3f(x+1 - width/2.0, heights[x+1][z], z - length/2.0);

					if(s == 0) s = 1;
					else s = 0;
				}
				glEnd();
			}
		glEndList();

		grassList = glGenLists(1);
		glNewList(grassList, GL_COMPILE);
			glBegin(GL_QUADS);
				for(int x = 0; x < width-1; x++) {
					for(int z = 0; z < length-1; z++) {
						// diagonale 1
						glTexCoord2f(0, 1);
						glVertex3f(x - width / 2.0, heights[x][z], z - length / 2.0);
						
						glTexCoord2f(1, 1);
						glVertex3f(x + 1 - width /2.0, heights[x+1][z+1], z + 1 - length / 2.0);
						
						glTexCoord2f(1, 0);
						glVertex3f(x + 1 - width /2.0, heights[x+1][z+1] + 1, z + 1 - length / 2.0);
						
						glTexCoord2f(0, 0);
						glVertex3f(x - width / 2.0, heights[x][z] + 1, z - length / 2.0);


						//diagonale 2
						glTexCoord2f(0, 1);
						glVertex3f(x + 1 - width / 2.0, heights[x + 1][z], z - length / 2.0);
						
						glTexCoord2f(1, 1);
						glVertex3f(x - width / 2.0, heights[x][z + 1], z + 1 - length / 2.0);
						
						glTexCoord2f(1, 0);
						glVertex3f(x - width / 2.0, heights[x][z + 1] + 1, z + 1 - length / 2.0);
						
						glTexCoord2f(0, 0);
						glVertex3f(x + 1 - width / 2.0, heights[x + 1][z] + 1, z - length / 2.0);

						// perpendicolare 1
						glTexCoord2f(0, 1);
						glVertex3f(x - width / 2.0, heights[x][z], z + 0.5 - length / 2.0);
						
						glTexCoord2f(1, 1);
						glVertex3f(x + 1 - width / 2.0, heights[x][z], z + 0.5 - length / 2.0);
						
						glTexCoord2f(1, 0);
						glVertex3f(x + 1 - width / 2.0, heights[x][z] + 1, z + 0.5 - length / 2.0);
						
						glTexCoord2f(0, 0);
						glVertex3f(x - width / 2.0, heights[x][z] + 1, z + 0.5 - length / 2.0);
					}
				}
			glEnd();
		glEndList();
}

Terrain::Terrain(int width, int length)
{
	this->width = width;
	this->length = length;

	this->heights = new float*[width];
	this->normals = new Vec3*[width];

	for(int i = 0; i < width; i++) {
		heights[i] = new float[length];
		normals[i] = new Vec3[length];
	}
}


Terrain::~Terrain(void)
{
	for(int i = 0; i < width; i++)
		delete[] heights[i];

	delete[] heights;
}

void Terrain::setHeight(int x, int z, float value) {
	heights[x][z] = value;
}

float Terrain::getHeight(int x, int z) {
	return heights[x][z];
}

void Terrain::render() {
	
	glCallList(terrainList);

	// disegna anche i normali ai vertici
	glColor3f(1,0,0);
	glBegin(GL_LINES);
	for(int x = 0; x < width-1; x++) {
		for(int z = 0; z < length; z++) {
			glVertex3f(x - width/2.0, heights[x][z], z - length /2.0);
			glVertex3f(x - width/2.0 + normals[x][z].getX(), heights[x][z]  + normals[x][z].getY(), z - length /2.0  + normals[x][z].getZ());
		}
	}
	glEnd();
}