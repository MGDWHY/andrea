#pragma once

#include "VertexArrayObject.h"
#include "ObjectFile.h"
#include "MaterialLibrary.h"
#include "ShaderManager.h"
#include "Global.h"
#include "xbo.h"

using namespace glutils;
using namespace Vecmath;
using namespace ExtendedBinary3DObject;
using namespace Global;

class Sonic
{
private:
	
	int numFrames;

	XBO ** meshes;

public:

	VertexArrayObject *** vaos;


	Sonic(vector<string> frames);
	~Sonic(void);

	int GetFramesCount();
	void DrawAnimation(Shader * shader, GLuint frame);
};

