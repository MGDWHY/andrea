#pragma once

#include "vecmath.h"
#include "ShaderManager.h"
#include "GameLogic.h"
#include "GameRendering.h"
#include "InterfaceRendering.h"
#include "MatrixStack.h"
#include "VertexArrayObject.h"
#include "Global.h"

#define DISPLAY_SIZE 17
#define DISPLAY_RADIUS DISPLAY_SIZE / 2

#define SHADOW_DISPLAY_SIZE 9
#define SHADOW_DISPLAY_RADIUS SHADOW_DISPLAY_SIZE / 2

using namespace Vecmath;

struct LightingParameters {
	vec4 shadowPosition;
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec3 attenuation;
	GLfloat bloomExposure;
};

struct ProjectionParameters {
	vec4 planeNormal;
	vec4 planeTangent;
	vec4 planeCenter;
};

struct ViewParameters {
	GLfloat zCamera;
	GLfloat xCamera;
	GLfloat rotateX;
	mat4 projectionMatrix;
};

struct TextureParameters {
	GLuint floorDiffuse;
	GLuint floorNormalMap;
	GLuint starSphere;
};

class DeferredRendering
{
private:

	GLfloat viewWidth, viewHeight;

	GLuint fboShadows, fboRender;

	GLuint bShadowMap, bDiffuseAndShadows, bNormals, bLighting, bDepth, bZBuffer, bShadowZBuffer, bAlbedo, bBloom, bBlurredShadows0, bBlurredShadows1;

	GLuint tStar;

	Shader * sSonic, * sCurveWorld, * sSonicShadow, * sCurveWorldShadow, * sRenderAlbedoAndShadows, * sRender, * sBlur, * sBloom, * sSky, * sTest, *sBackground;

	LightingParameters pLighting;
	ProjectionParameters pProjection;
	ViewParameters pView;
	TextureParameters pTexture;

	MatrixStack * modelStack, * viewStack, * projStack;

	mat4 shadowViewMatrix, shadowProjectionMatrix, viewMatrix, projectionMatrix, projectionMatrixInverse;

	VertexArrayObject * v_quad;
	
	int shadowMapSize;
	int shadowsEnabled;
	int quality;
	bool stencilInterlaced;

	void InitBuffers();
	void InitShaders();
	void InitTextures();

	void ResizeShadowMap();

	static void ResizeBuffer(GLint id, GLint width, GLint height, GLenum format, GLenum internalFormat, GLenum dataType);
public:

	
	void ResizeViewport(GLfloat viewWidth, GLfloat viewHeight);

	void InterlaceStencil();

	void RenderShadowMap(GameRendering * rendering, GameLogic * logic, GameData * data);
	void RenderBuffers(GameRendering * rendering, GameLogic * logic, GameData * data);
	void RenderAlbedoAndShadows();
	void RenderBlurredShadows();
	void RenderBloomLighting();
	void RenderTest();
	void Render();

	void Configure(const GameConfig& gameConfig);
	void SetLightingParameters(LightingParameters params);
	void SetProjectionParameters(ProjectionParameters params);
	void SetViewParamters(ViewParameters params);
	void SetTextureParameters(TextureParameters params);
	

	DeferredRendering(GLfloat viewWidth, GLfloat viewHeight);
	~DeferredRendering(void);
};

