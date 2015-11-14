#include "DeferredRendering.h"


DeferredRendering::DeferredRendering(GLfloat viewWidth, GLfloat viewHeight)
{
	this->viewHeight = viewHeight;
	this->viewWidth = viewWidth;
	this->modelStack = new MatrixStack(16);
	this->viewStack = new MatrixStack(16);
	this->projStack = new MatrixStack(4);
	this->stencilInterlaced = false;

	this->shadowMapSize = Global::QUALITY_LOW;
	this->quality = 2;

	this->InitBuffers();
	this->InitShaders();
	this->InitTextures();
}


DeferredRendering::~DeferredRendering(void)
{

}

void DeferredRendering::Configure(const GameConfig& gameConfig) {
	this->shadowMapSize = gameConfig.shadowQuality;
	this->shadowsEnabled = gameConfig.shadowsEnabled;
	this->quality = gameConfig.quality;

	ShaderManager * manager = ShaderManager::getDefaultManager();

	if(this->sBlur) {
		delete this->sBlur;
		this->sBlur = NULL;
	}

	switch(this->quality) {
		case 0:
			this->sBlur = manager->createShader("shaders/blur.vert", "shaders/blurLow.frag");
			break;
		case 1:
			this->sBlur = manager->createShader("shaders/blur.vert", "shaders/blurMedium.frag");
			break;
		case 2:
			this->sBlur = manager->createShader("shaders/blur.vert", "shaders/blurHigh.frag");
			break;
	}

	this->ResizeShadowMap();
}

void DeferredRendering::InitTextures() {
	this->tStar = Global::PngTexture("textures/star.png", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
}

void DeferredRendering::InitBuffers() {

	vec2 vertices_texCoords[4] = { vec2(0, 0), vec2(1, 0), vec2(1, 1), vec2(0, 1) };
	GLuint indices[6] = { 0, 1, 2, 2, 3, 0 };
	
	/* Vertex array for fullscreen quad */

	this->v_quad = new VertexArrayObject(1, GL_STATIC_DRAW);
	this->v_quad->setBufferData(0, sizeof(vec2) * 4, vertices_texCoords);
	this->v_quad->setElementsData(sizeof(GLuint), 6, indices);
	this->v_quad->setVertexAttribute(0, 0, 2, GL_FLOAT);
	this->v_quad->enableVertexAttribute(0);

	/* create frame buffer */

	glGenFramebuffers(1, &this->fboShadows);
	glGenFramebuffers(1, &this->fboRender);

	/* create attachments */

	glGenTextures(1, &this->bShadowMap);
	glGenTextures(1, &this->bDiffuseAndShadows);
	glGenTextures(1, &this->bNormals);
	glGenTextures(1, &this->bLighting);
	glGenTextures(1, &this->bDepth);
	glGenTextures(1, &this->bZBuffer);
	glGenTextures(1, &this->bShadowZBuffer);
	glGenTextures(1, &this->bAlbedo);
	glGenTextures(1, &this->bBloom);
	glGenTextures(1, &this->bBlurredShadows0);
	glGenTextures(1, &this->bBlurredShadows1);

	/* resize attachment data */
	this->ResizeShadowMap();
	this->ResizeViewport(this->viewWidth, this->viewHeight);

}

void DeferredRendering::InitShaders() {
	ShaderManager * manager = ShaderManager::getDefaultManager();
	this->sCurveWorldShadow = manager->createShader("shaders/curveWorld_Shadow.vert", "shaders/curveWorld_Shadow.frag");
	this->sSonicShadow = manager->createShader("shaders/sonic_Shadow.vert", "shaders/sonic_Shadow.frag");
	this->sCurveWorld = manager->createShader("shaders/curveWorld_deferred.vert", "shaders/curveWorld_deferred.frag");
	this->sSonic = manager->createShader("shaders/sonic_deferred.vert", "shaders/sonic_deferred.frag");
	this->sRenderAlbedoAndShadows = manager->createShader("shaders/renderAlbedoShadows_deferred.vert", "shaders/renderAlbedoShadows_deferred.frag");
	this->sRender = manager->createShader("shaders/render.vert", "shaders/render.frag");	
	this->sBloom = manager->createShader("shaders/bloom.vert", "shaders/bloom.frag");
	this->sSky = manager->createShader("shaders/renderSky.vert", "shaders/renderSky.geom", "shaders/renderSky.frag");
	this->sBackground = manager->createShader("shaders/renderBg.vert", "shaders/renderBg.frag");
	this->sTest = manager->createShader("shaders/render_test.vert", "shaders/render_test.frag");
	this->sBlur = NULL;
}

void DeferredRendering::ResizeBuffer(GLint id, GLint width, GLint height, GLenum format, GLenum internalFormat, GLenum dataType) {

	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, dataType, 0);
}

void DeferredRendering::ResizeShadowMap() {
	DeferredRendering::ResizeBuffer(this->bShadowMap, (int)this->shadowMapSize, (int)this->shadowMapSize, GL_RED, GL_R32F, GL_FLOAT);
	DeferredRendering::ResizeBuffer(this->bShadowZBuffer, (int)this->shadowMapSize, (int)this->shadowMapSize, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT24, GL_UNSIGNED_INT);
}

void DeferredRendering::ResizeViewport(GLfloat width, GLfloat height) {

	DeferredRendering::ResizeBuffer(this->bDiffuseAndShadows, (int)width, (int)height, GL_RGBA, GL_RGBA8, GL_UNSIGNED_BYTE);
	DeferredRendering::ResizeBuffer(this->bNormals, (int)width, (int)height, GL_RGBA, GL_RGBA8, GL_UNSIGNED_BYTE);
	DeferredRendering::ResizeBuffer(this->bLighting, (int)width, (int)height, GL_RED, GL_R8, GL_UNSIGNED_BYTE);	
	DeferredRendering::ResizeBuffer(this->bDepth, (int)width, (int)height, GL_RED, GL_R32F, GL_FLOAT);
	DeferredRendering::ResizeBuffer(this->bZBuffer, (int)width, (int)height, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT24, GL_UNSIGNED_INT);
	DeferredRendering::ResizeBuffer(this->bAlbedo, (int)width, (int)height, GL_RGB, GL_RGB8, GL_UNSIGNED_BYTE);
	DeferredRendering::ResizeBuffer(this->bBloom, (int)width, (int)height, GL_RGB, GL_RGB8, GL_UNSIGNED_BYTE);
	DeferredRendering::ResizeBuffer(this->bBlurredShadows0, (int)width, (int)height, GL_RED, GL_R16F, GL_FLOAT);
	DeferredRendering::ResizeBuffer(this->bBlurredShadows1, (int)width, (int)height, GL_RED, GL_R16F, GL_FLOAT);

	this->viewWidth = width;
	this->viewHeight = height;

}

void DeferredRendering::InterlaceStencil() {

	if(this->stencilInterlaced)
		return;

	glPushAttrib(GL_ENABLE_BIT | GL_STENCIL_BUFFER_BIT);
		
		glViewport(0, 0, (int)this->viewWidth, (int)this->viewHeight);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, this->viewWidth - 1, 0, this->viewHeight - 1, -1, 1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glDisable(GL_MULTISAMPLE);
		glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
		glStencilFunc(GL_ALWAYS, 1, 1);

		glClearStencil(0);
		glClear(GL_STENCIL_BUFFER_BIT);

		glColor4f(1.0, 1.0, 1.0, 0.0);
		glBegin(GL_LINES);
			for(int i = 0; i < viewHeight; i += 2) {
				glVertex3f(0, i, 0);
				glVertex3f(this->viewWidth, i, 0);
			}
		glEnd();

	glPopAttrib();

	this->stencilInterlaced = true;
}


void DeferredRendering::RenderShadowMap(GameRendering * rendering, GameLogic * logic, GameData * data) {

	static GLuint drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	static vec2 intPos, fractPos;
	static GLuint * subData = 0;

	logic->GetModPosition(intPos, fractPos);
	subData = data->GetRoundSubData((GLuint)intPos.x, (GLuint)intPos.y, SHADOW_DISPLAY_RADIUS);

	glBindFramebuffer(GL_FRAMEBUFFER, this->fboShadows);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->bShadowMap, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->bShadowZBuffer, 0);

		glDrawBuffers(1, drawBuffers);

		glViewport(0,0,this->shadowMapSize, this->shadowMapSize);
		glClearColor(1,1,1,1);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		this->viewStack->LoadMatrix(this->shadowViewMatrix);
		this->projStack->LoadMatrix(this->shadowProjectionMatrix);

		this->modelStack->loadIdentity();

		// render sonic

		this->sSonicShadow->enable();

		this->modelStack->push();
			
			this->modelStack->translate(0, logic->GetSonicHeight(), 0);
			if(logic->IsSonicJumping())
				this->modelStack->translate(0, 0.1, 0);
			this->modelStack->rotate(Global::PI / 2, 0, 1, 0);
			this->modelStack->rotate(-logic->GetSonicRotationX(), 0, 0, 1);
			this->modelStack->scale(0.05f, 0.05f, 0.05f);

			this->sSonicShadow->setUniformMatrix("in_ProjectionMatrix", projStack->current());
			this->sSonicShadow->setUniformMatrix("in_ViewMatrix", viewStack->current());
			this->sSonicShadow->setUniformMatrix("in_ModelMatrix", modelStack->current());

			this->sSonicShadow->setUniform("in_T", logic->GetSonicAnimationDelta());

			if(logic->GetGameStatus() == GameLogic::STATUS_STARTING)
				rendering->RenderSonicStand(this->sSonicShadow);
			else {
				if(logic->IsSonicJumping())
					rendering->RenderSonicJump(this->sSonicShadow);
				else
					rendering->RenderSonicRun(this->sSonicShadow, logic->GetSonicAnimationFrame());	
			}

		this->modelStack->pop();

		this->sSonicShadow->disable();

		// render world

		this->modelStack->rotate(logic->GetRotationAngle(), 0, 1, 0);

		this->sCurveWorldShadow->enable();

		this->sCurveWorldShadow->setUniformMatrix("in_ViewMatrix", this->viewStack->current());
		this->sCurveWorldShadow->setUniformMatrix("in_ModelMatrix", this->modelStack->current());
		this->sCurveWorldShadow->setUniformMatrix("in_ProjectionMatrix", this->projStack->current());

		this->sCurveWorldShadow->setUniform("in_ProjectionPlaneNormal", this->pProjection.planeNormal);
		this->sCurveWorldShadow->setUniform("in_ProjectionCenter", this->pProjection.planeCenter);

		// draw floor
		// rendering->RenderWorldFloor();
		


		// draw spheres and rings
		for(int i = 0; i < SHADOW_DISPLAY_SIZE; i++) {
			for(int j = 0; j < SHADOW_DISPLAY_SIZE; j++) {
				static float translate = (SHADOW_DISPLAY_SIZE - 1) / 2.0f;
			

				this->modelStack->push();
			
				this->modelStack->translate(i - translate, logic->GetWorldItemsHeight(), j - translate);
				this->modelStack->translate(- fractPos.x, 0, - fractPos.y);		

				this->sCurveWorldShadow->setUniformMatrix("in_ModelMatrix", this->modelStack->current());

				if(subData[j * SHADOW_DISPLAY_SIZE + i] == GameData::C_BLUESPHERE
					|| subData[j * SHADOW_DISPLAY_SIZE + i] == GameData::C_REDSPHERE
					|| subData[j * SHADOW_DISPLAY_SIZE + i] == GameData::C_STARSPHERE
					|| subData[j * SHADOW_DISPLAY_SIZE + i] == GameData::C_YELLOWSPHERE)
					rendering->RenderSphere();

				else if(subData[j * SHADOW_DISPLAY_SIZE + i] == GameData::C_RING) {
					modelStack->rotate(logic->GetRingsRotationAngle(), 0, 1, 0);
					this->sCurveWorldShadow->setUniformMatrix("in_ModelMatrix", this->modelStack->current());
					rendering->RenderRing();
				}

				this->modelStack->pop();
			}
		}

		if(logic->IsEmeraldVisible()) {
			this->modelStack->push();
			this->modelStack->loadIdentity();

			this->modelStack->translate(0, 0.6f, -logic->GetEmeraldDistance());
			this->modelStack->rotate(logic->GetRingsRotationAngle(), 0, 1, 0);

			this->sCurveWorldShadow->setUniformMatrix("in_ModelMatrix", this->modelStack->current());

			rendering->RenderEmerald();

			this->modelStack->pop();
		}

		
		this->sCurveWorldShadow->disable();
		
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Free some memory

	delete[] subData;
}

void DeferredRendering::RenderBuffers(GameRendering * rendering, GameLogic * logic, GameData * data) {
	static GLuint drawBuffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	static vec2 intPos, fractPos;
	static ivec2 pos;
	static GLuint * subData = 0;

	logic->GetModPosition(intPos, fractPos);
	subData = data->GetRoundSubData((GLint)intPos.x, (GLint)intPos.y, DISPLAY_RADIUS);

	glBindFramebuffer(GL_FRAMEBUFFER, this->fboRender);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->bDiffuseAndShadows, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, this->bNormals, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, this->bDepth, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, this->bLighting, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->bZBuffer, 0);

		glDrawBuffers(4, drawBuffers);

		glViewport(0,0,(GLint)this->viewWidth, (GLint)this->viewHeight);
		glCullFace(GL_BACK);
		glClearColor(0,0,0,0);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		this->projStack->LoadMatrix(this->projectionMatrix);
		this->viewStack->LoadMatrix(this->viewMatrix);

		vec4 lightPos =  viewStack->current() * this->pLighting.position;

		this->modelStack->loadIdentity();

		// render background
		this->sBackground->enable();
			glPushAttrib(GL_ENABLE_BIT);
				glDisable(GL_CULL_FACE);
				glDisable(GL_DEPTH_TEST);
				glDisable(GL_BLEND);

				this->sBackground->setUniformMatrix("in_ProjectionMatrix", this->projStack->current());
				this->sBackground->setUniform("in_Color0", vec4(data->GetSkyBackColor(0), 1.0));
				this->sBackground->setUniform("in_Color1", vec4(data->GetSkyBackColor(1), 1.0));

				glBindVertexArray(this->v_quad->getID());
				glDrawElements(GL_TRIANGLES, this->v_quad->getElementsCount(), GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);

			glPopAttrib();
		this->sBackground->disable();

		// render sky

		this->sSky->enable();
		this->modelStack->push();
			
			glPushAttrib(GL_ENABLE_BIT);
				glDisable(GL_CULL_FACE);
				glDisable(GL_DEPTH_TEST);
				glEnable(GL_BLEND);
				
				this->modelStack->loadIdentity();
				this->modelStack->translate(this->pProjection.planeCenter.x,
											this->pProjection.planeCenter.y,
											this->pProjection.planeCenter.z);
			
			
				if(logic->GetDirection() == GameLogic::DIRECTION_LEFT || logic->GetDirection() == GameLogic::DIRECTION_RIGHT)
					this->modelStack->rotate(logic->GetPosition().x / data->GetWorldWidth() * 2 * Global::PI,
						logic->GetDirection().x, 0, 0);
				else
					this->modelStack->rotate(logic->GetPosition().y / data->GetWorldDepth() * 2 * Global::PI,
						logic->GetDirection().y, 0, 0); 


				this->modelStack->rotate(logic->GetRotationAngle(), 0, 1, 0);
				

				this->sSky->setUniformMatrix("in_ProjectionMatrix", this->projStack->current());
				this->sSky->setUniformMatrix("in_ViewMatrix", this->viewStack->current());
				this->sSky->setUniformMatrix("in_ModelMatrix", this->modelStack->current());
			
				this->sSky->setUniformTexture("in_Texture", 0, this->tStar);

				this->sSky->setUniform("in_PixelStep", vec2(0.001f, 0.001f * this->viewWidth / this->viewHeight));

				rendering->RenderSky();

			glPopAttrib();

		this->modelStack->pop();
		this->sSky->disable();
		
		// draw sonic

		this->sSonic->enable();

		this->modelStack->push();
			
			this->modelStack->translate(0, logic->GetSonicHeight(), 0);
			if(logic->IsSonicJumping())
				this->modelStack->translate(0, 0.3, 0);
			this->modelStack->rotate(Global::PI / 2, 0, 1, 0);
			this->modelStack->rotate(-logic->GetSonicRotationX(), 0, 0, 1);
			this->modelStack->scale(0.05f, 0.05f, 0.05f);

			this->sSonic->setUniformTexture("in_Shadows", 0, this->bShadowMap);

			this->sSonic->setUniformMatrix("in_ProjectionMatrix", this->projStack->current());
			this->sSonic->setUniformMatrix("in_ViewMatrix", this->viewStack->current());
			this->sSonic->setUniformMatrix("in_ModelMatrix", this->modelStack->current());

			this->sSonic->setUniformMatrix("in_ShadowViewMatrix", this->shadowViewMatrix);
			this->sSonic->setUniformMatrix("in_ShadowProjectionMatrix", this->shadowProjectionMatrix);

			this->sSonic->setUniform("in_SpecularPower", 1.0f);
			this->sSonic->setUniform("in_Shininess", 128.0f);
			this->sSonic->setUniform("in_EnableLighting", true);

			this->sSonic->setUniform("in_T", logic->GetSonicAnimationDelta());

			if(logic->GetGameStatus() == GameLogic::STATUS_STARTING) 
				rendering->RenderSonicStand(this->sSonic);
			else {
				if(logic->IsSonicJumping())
					rendering->RenderSonicJump(this->sSonic);
				else
					rendering->RenderSonicRun(this->sSonic, logic->GetSonicAnimationFrame());
			}


		this->modelStack->pop();

		this->sSonic->disable();

	
		// setting up default parameters for curve world shader
		this->sCurveWorld->enable();

		this->sCurveWorld->setUniformMatrix("in_ShadowViewMatrix", this->shadowViewMatrix);
		this->sCurveWorld->setUniformMatrix("in_ShadowProjectionMatrix", this->shadowProjectionMatrix);
	
		this->sCurveWorld->setUniform("in_ProjectionPlaneNormal", this->pProjection.planeNormal);
		this->sCurveWorld->setUniform("in_ProjectionPlaneTangent", this->pProjection.planeTangent);
		this->sCurveWorld->setUniform("in_ProjectionCenter", this->pProjection.planeCenter);

		this->sCurveWorld->setUniformMatrix("in_ViewMatrix", this->viewStack->current());
		this->sCurveWorld->setUniformMatrix("in_ProjectionMatrix", this->projStack->current());

		// render floor
		this->modelStack->rotate(logic->GetRotationAngle(), 0, 1, 0);

		this->sCurveWorld->setUniform("in_EnableLighting", true);
		this->sCurveWorld->setUniform("in_NormalMapping", data->IsBumpMappingEnabled() == GameData::AS_YES ? true : false);
		this->sCurveWorld->setUniform("in_ColorMode", data->GetFloorRenderingMode() == GameData::RM_TEXTURE ? 1 : 2);
		this->sCurveWorld->setUniform("in_Color", vec4(data->GetCheckerBoardColors(0), 1.0));
		this->sCurveWorld->setUniform("in_SecondaryColor", vec4(data->GetCheckerBoardColors(1), 1.0));
		this->sCurveWorld->setUniform("in_SpecularPower", 0.5f);
		this->sCurveWorld->setUniform("in_Shininess", 128.0f);
		this->sCurveWorld->setUniform("in_TexCoordOffset", logic->GetTexturePosition());

		this->sCurveWorld->setUniformMatrix("in_ModelMatrix", this->modelStack->current());

		this->sCurveWorld->setUniformTexture("in_Texture", 0, this->pTexture.floorDiffuse);
		this->sCurveWorld->setUniformTexture("in_Shadows", 1, this->bShadowMap);
		this->sCurveWorld->setUniformTexture("in_NormalMap", 2, this->pTexture.floorNormalMap);

		rendering->RenderWorldFloor();

		// draw spheres and rings
		this->sCurveWorld->setUniform("in_NormalMapping", false);
		this->sCurveWorld->setUniform("in_ColorMode", 0);
		this->sCurveWorld->setUniform("in_SpecularPower", 1.0f);
		this->sCurveWorld->setUniform("in_TexCoordOffset", vec2());

		for(int i = 0; i < DISPLAY_SIZE; i++) {
			for(int j = 0; j < DISPLAY_SIZE; j++) {
				static float translate = (DISPLAY_SIZE - 1) / 2.0f;

				this->modelStack->push();
			
				this->modelStack->translate(i - translate, logic->GetWorldItemsHeight(), j - translate);
				this->modelStack->translate(- fractPos.x, 0, - fractPos.y);

				this->sCurveWorld->setUniformMatrix("in_ModelMatrix", this->modelStack->current());
			
				if(subData[j * DISPLAY_SIZE + i] == GameData::C_BLUESPHERE) {
				
					this->sCurveWorld->setUniform("in_Shininess", 128.0f);
					this->sCurveWorld->setUniform("in_Color", vec4(0.0, 0.0, 1.0, 1.0));
					rendering->RenderSphere();
			
				} else if(subData[j * DISPLAY_SIZE + i] == GameData::C_REDSPHERE) {

					this->sCurveWorld->setUniform("in_Shininess", 128.0f);
					this->sCurveWorld->setUniform("in_Color", vec4(1.0, 0.0, 0.0, 1.0));
					rendering->RenderSphere();
				} else if(subData[j * DISPLAY_SIZE + i] == GameData::C_YELLOWSPHERE) {

					this->sCurveWorld->setUniform("in_Shininess", 128.0f);
					this->sCurveWorld->setUniform("in_Color", vec4(1.0, 0.75, 0.0, 1.0));
					rendering->RenderSphere();
				} else if(subData[j * DISPLAY_SIZE + i] == GameData::C_STARSPHERE) {

					this->sCurveWorld->setUniform("in_Shininess", 128.0f);
					this->sCurveWorld->setUniform("in_ColorMode", 1);
					this->sCurveWorld->setUniformTexture("in_Texture", 0, this->pTexture.starSphere);

					rendering->RenderSphere();

					this->sCurveWorld->setUniform("in_ColorMode", 0);
				} else if(subData[j * DISPLAY_SIZE + i] == GameData::C_RING) {
				
					modelStack->rotate(logic->GetRingsRotationAngle(), 0, 1, 0);

					this->sCurveWorld->setUniform("in_Shininess", 16.0f);
					this->sCurveWorld->setUniform("in_Color", vec4(0.95f, 0.7f, 0.007f, 1.0f));
					this->sCurveWorld->setUniformMatrix("in_ModelMatrix", this->modelStack->current());
				
					rendering->RenderRing();
				}

				this->modelStack->pop();
			}
		}

		// render emerald
		if(logic->IsEmeraldVisible()) {

			this->modelStack->push();
			this->modelStack->loadIdentity();

			this->modelStack->translate(0, 0.6f, -logic->GetEmeraldDistance());
			this->modelStack->rotate(logic->GetRingsRotationAngle(), 0, 1, 0);

			this->sCurveWorld->setUniformMatrix("in_ModelMatrix", this->modelStack->current());
			this->sCurveWorld->setUniform("in_SpecularPower", 1.0f);
			this->sCurveWorld->setUniform("in_Shininess", 128.0f);
			this->sCurveWorld->setUniform("in_ColorMode", 0);
			this->sCurveWorld->setUniform("in_NormalMapping", false);
			this->sCurveWorld->setUniform("in_Color", vec4(data->GetEmeraldColor(), 1.0f));

			rendering->RenderEmerald();

			this->modelStack->pop();
		}


		this->sCurveWorld->disable();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Free some memory

	delete[] subData;
}

void DeferredRendering::RenderAlbedoAndShadows() {

	static GLuint drawBuffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };

	glBindFramebuffer(GL_FRAMEBUFFER, this->fboRender);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->bAlbedo, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, this->bBlurredShadows0, 0);

		glDrawBuffers(2, drawBuffers);

		glViewport(0, 0, (GLint)this->viewWidth, (GLint)this->viewHeight);
		glClearColor(0, 0, 0, 1);
		//glClear(GL_COLOR_BUFFER_BIT);

		glPushAttrib(GL_ENABLE_BIT);
			glDisable(GL_DEPTH_TEST);

			this->projStack->loadIdentity();
			this->projStack->ortho(-1, 1, -1, 1, -1, 1);

			this->sRenderAlbedoAndShadows->enable();

			this->sRenderAlbedoAndShadows->setUniformMatrix("in_ProjectionMatrixInverse", this->projectionMatrixInverse);
			this->sRenderAlbedoAndShadows->setUniformMatrix("in_ProjectionMatrix", this->projStack->current());

			this->sRenderAlbedoAndShadows->setUniformTexture("in_DiffuseAndShadows", 1, this->bDiffuseAndShadows);
			this->sRenderAlbedoAndShadows->setUniformTexture("in_Normals", 0, this->bNormals);
			this->sRenderAlbedoAndShadows->setUniformTexture("in_Depth", 2, this->bDepth);

			this->sRenderAlbedoAndShadows->setUniform("in_ShadowsEnabled", this->shadowsEnabled);

			this->sRenderAlbedoAndShadows->setUniform("in_LightPosition", this->viewMatrix * this->pLighting.position);
			this->sRenderAlbedoAndShadows->setUniform("in_LightAmbient", this->pLighting.ambient);
			this->sRenderAlbedoAndShadows->setUniform("in_LightDiffuse", this->pLighting.diffuse);
			this->sRenderAlbedoAndShadows->setUniform("in_LightSpecular", this->pLighting.specular);
			this->sRenderAlbedoAndShadows->setUniform("in_LightAttenuation", this->pLighting.attenuation);

			this->sRenderAlbedoAndShadows->setUniform("in_BlurStep", vec2(1.0 / this->viewWidth, 1.0 / this->viewHeight));

			glBindVertexArray(this->v_quad->getID());
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			this->sRenderAlbedoAndShadows->disable();

		glPopAttrib();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredRendering::RenderBlurredShadows() {

	static GLuint drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };

	glViewport(0, 0, (GLint)this->viewWidth, (GLint)this->viewHeight);
	//glClearColor(0, 0, 0, 1);

	glBindFramebuffer(GL_FRAMEBUFFER, this->fboRender);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->bBlurredShadows1, 0);

		glDrawBuffers(1, drawBuffers);

		//glClear(GL_COLOR_BUFFER_BIT);

		glPushAttrib(GL_ENABLE_BIT);
			glDisable(GL_DEPTH_TEST);

			this->projStack->loadIdentity();
			this->projStack->ortho(-1, 1, -1, 1, -1, 1);

			this->sBlur->enable();

			this->sBlur->setUniformMatrix("in_ProjectionMatrix", this->projStack->current());
			this->sBlur->setUniformTexture("in_RTexture", 0, this->bBlurredShadows0);
			this->sBlur->setUniform("in_Horizontal", true);
			this->sBlur->setUniform("in_Step", 1.0f / this->viewWidth);

			glBindVertexArray(this->v_quad->getID());
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			this->sBlur->disable();

		glPopAttrib();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, this->fboRender);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->bBlurredShadows0, 0);

		glDrawBuffers(1, drawBuffers);

		//glClear(GL_COLOR_BUFFER_BIT);

		glPushAttrib(GL_ENABLE_BIT);
			glDisable(GL_DEPTH_TEST);

			this->projStack->loadIdentity();
			this->projStack->ortho(-1, 1, -1, 1, -1, 1);

			this->sBlur->enable();

			this->sBlur->setUniformMatrix("in_ProjectionMatrix", this->projStack->current());
			this->sBlur->setUniformTexture("in_RTexture", 0, this->bBlurredShadows1);
			this->sBlur->setUniform("in_Horizontal", false);
			this->sBlur->setUniform("in_Step", 1.0f / this->viewHeight);

			glBindVertexArray(this->v_quad->getID());
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			this->sBlur->disable();

		glPopAttrib();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
}

void DeferredRendering::RenderBloomLighting() {
	static GLuint drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };

	glViewport(0, 0, (GLint)this->viewWidth, (GLint)this->viewHeight);
	//glClearColor(0, 0, 0, 1);

	glBindFramebuffer(GL_FRAMEBUFFER, this->fboRender);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->bBloom, 0);

		glDrawBuffers(1, drawBuffers);

		//glClear(GL_COLOR_BUFFER_BIT);

		glPushAttrib(GL_ENABLE_BIT);
			glDisable(GL_DEPTH_TEST);

			this->projStack->loadIdentity();
			this->projStack->ortho(-1, 1, -1, 1, -1, 1);

			this->sBloom->enable();

			this->sBloom->setUniformMatrix("in_ProjectionMatrix", this->projStack->current());
			this->sBloom->setUniformTexture("in_Src", 0, this->bAlbedo);
			this->sBloom->setUniformTexture("in_Depth", 1, this->bDepth);
			this->sBloom->setUniform("in_Horizontal", true);
			this->sBloom->setUniform("in_Exposure", this->pLighting.bloomExposure);
			this->sBloom->setUniform("in_Step", 1.0f / this->viewWidth);

			glBindVertexArray(this->v_quad->getID());
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			this->sBloom->disable();

		glPopAttrib();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, this->fboRender);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->bAlbedo, 0);

		glDrawBuffers(1, drawBuffers);

		//glClear(GL_COLOR_BUFFER_BIT);

		glPushAttrib(GL_ENABLE_BIT);
			glDisable(GL_DEPTH_TEST);

			this->projStack->loadIdentity();
			this->projStack->ortho(-1, 1, -1, 1, -1, 1);

			this->sBloom->enable();

			this->sBloom->setUniformMatrix("in_ProjectionMatrix", this->projStack->current());
			this->sBloom->setUniformTexture("in_Src", 0, this->bBloom);
			this->sBloom->setUniformTexture("in_Depth", 1, this->bDepth);
			this->sBloom->setUniform("in_Horizontal", false);
			this->sBloom->setUniform("in_Exposure", this->pLighting.bloomExposure);
			this->sBloom->setUniform("in_Step", 1.0f / this->viewHeight);

			glBindVertexArray(this->v_quad->getID());
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			this->sBloom->disable();

		glPopAttrib();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredRendering::RenderTest() {

	glViewport(0, 0, (GLint)this->viewWidth, (GLint)this->viewHeight);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_DEPTH_TEST);

		this->projStack->loadIdentity();
		this->projStack->ortho(-1, 1, -1, 1, -1, 1);

		this->sTest->enable();

		this->sTest->setUniformMatrix("in_ProjectionMatrix", this->projStack->current());
		this->sTest->setUniformTexture("in_Texture", 0, this->bShadowMap);

		glBindVertexArray(this->v_quad->getID());
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		this->sTest->disable();

	glPopAttrib();

}

void DeferredRendering::Render() {
	glViewport(0, 0, (GLint)this->viewWidth, (GLint)this->viewHeight);
	// glClearColor(0, 0, 0, 1);
	// glClear(GL_COLOR_BUFFER_BIT);

	glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_DEPTH_TEST);

		this->projStack->loadIdentity();
		this->projStack->ortho(-1, 1, -1, 1, -1, 1);

		this->sRender->enable();

		this->sRender->setUniformMatrix("in_ProjectionMatrix", this->projStack->current());
		this->sRender->setUniformTexture("in_Albedo", 0, this->bAlbedo);
		this->sRender->setUniformTexture("in_Shadows", 1, this->bBlurredShadows0);
		this->sRender->setUniformTexture("in_Diffuse", 2, this->bDiffuseAndShadows);
		this->sRender->setUniformTexture("in_Lighting", 3, this->bLighting);
		this->sRender->setUniform("in_ShadowsEnabled", this->shadowsEnabled);
		this->sRender->setUniform("in_LightAmbient", this->pLighting.ambient);

		glBindVertexArray(this->v_quad->getID());
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		this->sRender->disable();

	glPopAttrib();
}

void DeferredRendering::SetLightingParameters(LightingParameters params) {
	this->pLighting = params;

	this->projStack->push();
		
		this->projStack->loadIdentity();
		this->projStack->perspective(90, 1, 0.1, 50);

		this->shadowProjectionMatrix = mat4(this->projStack->current());

		this->projStack->loadIdentity();
		this->projStack->lookat(vec3(this->pLighting.shadowPosition.xyzw), vec3(0,0,0), vec3(1,0,0));

		this->shadowViewMatrix = mat4(this->projStack->current());

	this->projStack->pop();

}
void DeferredRendering::SetProjectionParameters(ProjectionParameters params) {
	this->pProjection = params;
}
void DeferredRendering::SetViewParamters(ViewParameters params) {
	this->pView = params;

	this->projStack->push();
		
		this->projStack->loadIdentity();
		this->projStack->translate(this->pView.xCamera, -0.5, this->pView.zCamera);
		this->projStack->rotate(this->pView.rotateX, 1, 0, 0);
		this->viewMatrix = mat4(projStack->current());

		this->projectionMatrix = this->pView.projectionMatrix;
		this->projectionMatrixInverse = invert(this->projectionMatrix);

	this->projStack->pop();
}
void DeferredRendering::SetTextureParameters(TextureParameters params) {
	this->pTexture = params;
}
