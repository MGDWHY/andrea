#pragma once

#include "vecmath.h"
#include "MatrixStack.h"
#include "VertexArrayObject.h"
#include "ObjectFile.h"
#include "ShaderManager.h"
#include "AudioEngine.h"
#include "LinearInterpolator.h"
#include "Global.h"
#include "Sprite.h"
#include "GameMenu.h"
#include "lodepng.h"

using namespace GameMenu;
using namespace Vecmath;
using namespace glutils;

class InterfaceRendering
{
private:
	static const GLfloat IFACE_HEIGHT;
	static const string IFACE_DISCLAIMER[];
	
	static const GLint TITLE_STARS;

	static const GLfloat IN_GAME_MESSAGE_TRANSITION_TIME;
	static const GLfloat IN_GAME_MESSAGE_DURATION;
	static const GLfloat IN_GAME_MESSAGE_TOTAL_TIME;

	struct StarVertex {
		vec2 position;
		vec4 color;
		GLfloat speed;
	};

	struct Emerald {
		vec3 position;
		vec4 color;
		vec2 angularVelocity;
		GLfloat radius;
	};

	int quality;

	GLfloat ratio, viewWidth, viewHeight;

	GLfloat width;

	AudioEngine * audioEngine;

	GameMenuParentItem * activeMenu;

	LinearInterpolator intDisclaimer, intIntro, intMenu, intStageClear;

	MatrixStack * projStack;
	MatrixStack * modelStack;

	mat4 orthoProjectionMatrix, perspProjectionMatrix;

	VertexArrayObject * v_quad, * v_string, * v_stars, * v_emerald;

	Shader * s_basic2D, * s_font, * s_stars, * s_intro, * s_convolve;

	GLuint texBlueSphere, texRing, texFontMenu, texFontGame, texDisclaimer, texTitle, texStar, texInGameMessageStar,
		texFilter0, texFilter1, texFilter2, texFilterDepth;

	GLuint fboFilter;

	Sprite * ringSprite;
	StarVertex * stars;
	Emerald emeralds[7];

	bool displayingInGameMessage;
	string inGameMessage;
	GLfloat inGameMessageTimer;

	void InitInterpolators();
	void InitBuffers();
	void InitShaders();
	void InitTextures();
	void InitStars();
	void InitEmeralds();

	void UpdateStars(GLfloat dt);
	void UpdateEmeralds(GLfloat dt, GLfloat radiusDelta, bool reset);

	void DrawStarsBackground(GLfloat dt);
	void DrawEmeraldScreen(GLfloat dt, bool reset);
	void DrawQuad(GLfloat x, GLfloat y, GLfloat width, GLfloat height, Shader * activeShader);
	void DrawString(GLfloat x, GLfloat y, GLfloat size, const vec3 &color, const string text, GLuint fontTexture);
	
	void Filter(Shader * filterShader, GLuint texSrc, GLuint texDst);

	void ResizeBuffers();
public:

	static const GLfloat INTRO_TIME;
	static const GLfloat DISCLAIMER_TIME;

	static const vec4 EMERALD_COLORS[7];

	void Configure(const GameConfig& gameConfig);

	void ResizeViewport(GLfloat viewWidth, GLfloat viewHeight);

	void DrawRingSprite(const SpriteTimer& timers);
	void DrawGameStats(const GameStats& gameStats);
	void DrawGameInterface(GLint collectedRings, GLint blueSpheresLeft, GLfloat dt);
	void DrawGameDisclaimer(GLfloat dt, bool reset);
	void DrawGameIntro(GLfloat dt, bool reset);
	void DrawGameMenu(GLfloat dt, bool reset);
	void DrawStageClear(GLfloat dt, int level, bool perfect, bool reset);
	void DrawFading(const vec3& color, GLfloat fadingFactor, GLenum blendDFactor);

	void DisplayInGameMessage(string message);

	void SetActiveMenu(GameMenuParentItem * activeMenu);

	InterfaceRendering(AudioEngine * audioEngine);
	~InterfaceRendering(void);
};

