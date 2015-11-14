#pragma once

#include "xbo.h"
#include "VertexArrayObject.h"
#include "ObjectFile.h"
#include "WorldVertexArray.h"
#include "SkyVertexArray.h"
#include "Sonic.h"

using namespace glutils;

class GameRendering
{

private:

	void InitBuffers();

	int quality;

	ObjectFile * ring, * sphere, * emerald, * cube;
	
	VertexArrayObject * testMatrixVAO, * sphereHighVAO, * sphereLowVAO, * ringVAO, *emeraldVAO;
	SkyVertexArray * skyVAO;
	WorldVertexArray * worldVAOHigh, * worldVAOLow;

	Sonic * sonicRun, * sonicJump, * sonicStand;

	void DrawVAO(VertexArrayObject * vao);
	void DrawVAO(VertexArrayObject * vao, GLenum mode);

public:
	void Configure(const GameConfig& gameConfig);
	void RenderWorldFloor();
	void RenderSonicRun(Shader * shader, GLuint frame);
	void RenderSonicJump(Shader * shader);
	void RenderSonicStand(Shader * shader);
	void RenderSphere();
	void RenderRing();
	void RenderEmerald();
	void RenderSky();

	SkyVertexArray * GetSkyVertexArray();

	GameRendering(void);
	~GameRendering(void);
};

