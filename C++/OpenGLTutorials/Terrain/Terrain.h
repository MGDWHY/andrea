#pragma once

#include "Vec3.h"

class Terrain
{
private:
	int width, length;
	
	float **heights;
	
	Vec3 **normals;

	GLuint terrainList, grassList;

public:
	Terrain(int width, int length);
	~Terrain(void);

	void setHeight(int x, int z, float value);
	float getHeight(int x, int z);

	float getWidth() { return width; }
	float getLength() { return length; }

	void compile(GLenum mode);
	void render(void);

	GLuint getTerrainList() { return terrainList; }
	GLuint getGrassList() { return grassList; }
};

