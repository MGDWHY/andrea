#pragma once

#include <Windows.h>
#include <string>
#include <iostream>
#include "GameEngine.h"
#include "ShaderManager.h"
#include "FrameBufferObject.h"
#include "Randomizer.h"
#include "ObjectFile.h"
#include "MaterialLibrary.h"
#include "Bspline.h"
#include "Timer.h"
#include "types.h"
#include "lodepng.h"


using namespace glutils;

class GraphicsEngine
{
private:

	static const int NUM_PP_FILTERS;

	static const int PP_NOFILTER; //  no post processing filters
	static const int PP_DEPTH; // bloom filter
	static const int PP_BLOOM; // bloom filter
	static const int PP_OLDTV; // old tv filter
	static const int PP_WAVE; // shear wave filter
	static const int PP_POOR; // edge detection filter

	static const string PP_FILTER_NAMES[];

	static const int NUM_CAMERA_MODES;

	static const int CAMERA_FREE;
	static const int CAMERA_SHIP;
	
	

	GLuint postprocessingFilter, cameraMode;

	bool wireframe;

	//lights
	light light1, light2, light3;

	// materials
	material floor, pads, ground;
	MaterialLibrary *shipMaterials;

	// lists
	GLuint padList, ballList, fieldList, wallsList, pylonList, 
		pointLightList, waterList, palmList[2], *shipLists;

	// textures
	GLuint fieldTexture, fieldBump, waterTexture, waterNormal1, waterNormal2, 
		noiseTexture, palmLeaves, palmLog, *shipTextures, *skyTextures;

	// rendering targets (textures)
	GLuint screenColorTarget, reflectionsColorTarget, screenDepthTarget;

	// Palms
	vec3 palmsDisplacement[4]; // translate
	GLfloat palmsRotY[4]; // rotateY

	// splines
	Bspline *splineLight1, *splineLight2, *sailShip;

	// shaders
	ShaderManager *shaderManager;
	Shader *pplighting, *bumpMapping, *water, *bloom, *oldTV, *wave, *poor, *depth;

	// Frame buffers
	FrameBufferObject *fbo;

	// game logic
	GameEngine *engine;

	// ship position
	vec3 shipPoint1, shipPoint2;

	//lights position
	vec4 light1Pos, light2Pos;

	void initShaders();
	void initTextures();
	void initLights();
	void initDisplayLists();
	void initMaterials();
	void initFrameBuffer();
	void initSplines();

	void pngLoad(const char* filename);
	GLuint pngTexture(const char* filename);

	void drawBox(GLfloat width, GLfloat height, GLfloat length, GLfloat tx, GLfloat ty, GLfloat tz);
	void drawString(string s);

	void renderScene(float dt, float time);
	void renderInfo(float dt, float time);
	void renderSailShip();
	void renderPalms();
	void renderWater(float time);
	void renderSky(vec3 cameraPos);

	void update(float dt, float time);

public:
	GraphicsEngine(GameEngine *engine);
	~GraphicsEngine(void);
	
	void render(float cameraRotX, float cameraRotY, float cameraZoom, int viewWidth, int viewHeight);

	void toggleFilters();
	void toggleWireframe();
	void toggleCamera();
};

