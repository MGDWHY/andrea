#include "InterfaceRendering.h"

const GLfloat InterfaceRendering::IFACE_HEIGHT = 100.0f;
const string InterfaceRendering::IFACE_DISCLAIMER[] =  {
											"DISCLAIMER",
											"This is a fan game and is not related to sega",
											"Sonic the Hedgehog is a sega registered T.M."
										};

const GLfloat InterfaceRendering::DISCLAIMER_TIME = 8.0f;
const GLfloat InterfaceRendering::INTRO_TIME = 5.0f;

const GLfloat InterfaceRendering::IN_GAME_MESSAGE_TRANSITION_TIME = .3f;
const GLfloat InterfaceRendering::IN_GAME_MESSAGE_DURATION = 3.0f;
const GLfloat InterfaceRendering::IN_GAME_MESSAGE_TOTAL_TIME = 2 * InterfaceRendering::IN_GAME_MESSAGE_TRANSITION_TIME +
																InterfaceRendering::IN_GAME_MESSAGE_DURATION;

const GLint InterfaceRendering::TITLE_STARS = 100;


const vec4 InterfaceRendering::EMERALD_COLORS[7] = { 
											vec4(1.0, 0.0, 0.0, .9),
											vec4(1.0, 1.0, 0.0, .9),
											vec4(0.0, 1.0, 0.0, .9),
											vec4(0.0, 1.0, 1.0, .9),
											vec4(0.7, 0.7, 0.7, .9),
											vec4(1.0, 0.0, 1.0, .9),
											vec4(0.5, 0.0, 1.0, .9),
										};

InterfaceRendering::InterfaceRendering(AudioEngine * audioEngine)
{

	this->projStack = new MatrixStack(1);
	this->modelStack = new MatrixStack(8);
	this->audioEngine = audioEngine;
	this->stars = new StarVertex[InterfaceRendering::TITLE_STARS];
	this->activeMenu = NULL;
	this->displayingInGameMessage = false;
	this->inGameMessage = "";
	this->quality = 2;

	this->InitInterpolators();
	this->InitBuffers();
	this->InitShaders();
	this->InitTextures();

	this->modelStack->loadIdentity();
}

void InterfaceRendering::SetActiveMenu(GameMenuParentItem * activeMenu) { this->activeMenu = activeMenu; }

void InterfaceRendering::Configure(const GameConfig& gameConfig) {
	this->quality = gameConfig.quality;

	ShaderManager * manager = ShaderManager::getDefaultManager();
	
	if(this->s_convolve) {
		delete this->s_convolve;
		this->s_convolve = NULL;
	}

	switch(this->quality) {
		case 0:
			this->s_convolve = manager->createShader("shaders/convolve.vert", "shaders/convolveLow.frag");
			break;
		case 1:
			this->s_convolve = manager->createShader("shaders/convolve.vert", "shaders/convolveMedium.frag");
			break;
		case 2:
			this->s_convolve = manager->createShader("shaders/convolve.vert", "shaders/convolveHigh.frag");
			break;
	}


}

void InterfaceRendering::ResizeViewport(GLfloat viewWidth, GLfloat viewHeight) {

	this->viewWidth = viewWidth;
	this->viewHeight = viewHeight;

	this->ratio = this->viewWidth / this->viewHeight;
	this->width = InterfaceRendering::IFACE_HEIGHT * ratio;

	this->projStack->loadIdentity();
	this->projStack->ortho(0, InterfaceRendering::IFACE_HEIGHT * ratio, 
		0, InterfaceRendering::IFACE_HEIGHT, -1, 1);
	
	this->orthoProjectionMatrix = this->projStack->current();

	this->projStack->loadIdentity();
	this->projStack->perspective(45, ratio, 0.1, 50);

	this->perspProjectionMatrix = this->projStack->current();

	this->InitStars();
	this->ResizeBuffers();
}

void InterfaceRendering::DrawGameDisclaimer(GLfloat dt, bool reset) {
	static GLfloat time = 0.0f;

	GLfloat discWidth = this->width * 0.7f;
	GLfloat discHeight = discWidth / 16.0f * 9.0f;
	GLfloat discX = this->width / 2.0f - discWidth / 2.0f;
	GLfloat discY = InterfaceRendering::IFACE_HEIGHT  / 2.0 - discHeight / 2.0;

	if(reset) {
		time = 0.0f;
		return;
	}
	
	glPushAttrib(GL_ENABLE_BIT);
		glEnable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		this->s_basic2D->enable();

			this->s_basic2D->setUniformMatrix("in_ProjectionMatrix", this->orthoProjectionMatrix);
			this->s_basic2D->setUniform("in_UseTexture", true);
			this->s_basic2D->setUniform("in_AlphaFactor", 1.0f);
			this->s_basic2D->setUniformTexture("in_Texture", 0, this->texDisclaimer, GL_TEXTURE_2D);

			this->DrawQuad(discX, discY, discWidth, discHeight, this->s_basic2D);
			
			if(time <= 1.0f)
				this->DrawFading(vec3(0, 0, 0), this->intDisclaimer.GetValueAt(time), GL_ONE_MINUS_SRC_ALPHA);
			else
				this->DrawFading(vec3(1, 1, 1), this->intDisclaimer.GetValueAt(time), GL_ONE);

		this->s_basic2D->disable();
	glPopAttrib();

	time += dt;
	
	if(time > InterfaceRendering::DISCLAIMER_TIME)
		time = InterfaceRendering::DISCLAIMER_TIME;
}

void InterfaceRendering::DrawGameIntro(GLfloat dt, bool reset) {
	static GLfloat time = 0.0f, discAlpha = 0.0f;
	static GLuint drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	GLfloat deltaRadius;

	if(reset) {
		time = 0.0f;
		return;
	}

	if(time == 0.0f) {

		if(this->texDisclaimer != 0) { // Freeing disclaimer texture if needed
			glDeleteTextures(1, &this->texDisclaimer);
			this->texDisclaimer = 0;
		}

		this->audioEngine->PlayTitleScreen();
		this->InitEmeralds();
		this->UpdateEmeralds(0, 0, true);
		this->DrawEmeraldScreen(0, true);
	}

	deltaRadius = max((InterfaceRendering::INTRO_TIME - time) / InterfaceRendering::INTRO_TIME, 0);

	this->UpdateEmeralds(dt, deltaRadius, false);

	this->DrawEmeraldScreen(dt, false);

	this->DrawFading(vec3(1.0, 1.0, 1.0), this->intIntro.GetValueAt(time), GL_ONE);

	time += dt;
	
	if(time > InterfaceRendering::INTRO_TIME)
		time = InterfaceRendering::INTRO_TIME;
}

void InterfaceRendering::DrawEmeraldScreen(GLfloat dt, bool reset) {
	static GLuint drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	static GLfloat time = 0.0f;

	if(reset) {
		time = 0.0f;
		return;
	}

	time += dt;

	glBindFramebuffer(GL_FRAMEBUFFER, this->fboFilter);
		
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->texFilter1, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->texFilterDepth, 0);


		glDrawBuffers(1, drawBuffers);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
		glPushAttrib(GL_ENABLE_BIT);
			
			glEnable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			this->s_basic2D->enable();

				this->s_basic2D->setUniformMatrix("in_ProjectionMatrix", this->orthoProjectionMatrix);
				this->s_basic2D->setUniform("in_UseTexture", true);
				this->s_basic2D->setUniform("in_AlphaFactor", 1.0f);
				this->s_basic2D->setUniformTexture("in_Texture", 0, this->texFilter2, GL_TEXTURE_2D);
			
				DrawQuad(0,0, this->width, InterfaceRendering::IFACE_HEIGHT, this->s_basic2D);


			this->s_basic2D->disable();
		glPopAttrib();

		glPushAttrib(GL_ENABLE_BIT);

			glEnable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			this->s_intro->enable();
				this->s_intro->setUniformMatrix("in_ProjectionMatrix", this->perspProjectionMatrix);

				for(int i = 0; i < 7; i++) {
			
					this->modelStack->push();
					this->modelStack->loadIdentity();
					this->modelStack->translate(0, 0, -5);
					this->modelStack->translate(this->emeralds[i].position.x, this->emeralds[i].position.y, this->emeralds[i].position.z);
					this->modelStack->rotate(time * this->emeralds[i].angularVelocity.x, 1, 0, 0);
					this->modelStack->rotate(time * this->emeralds[i].angularVelocity.y, 0, 1, 0);
					this->modelStack->scale(1.3f, 1.3f, 1.3f);

					this->s_intro->setUniformMatrix("in_ModelViewMatrix", this->modelStack->current());
					this->s_intro->setUniform("in_Shading", false);
					this->s_intro->setUniform("in_Color", this->emeralds[i].color);
			

					glBindVertexArray(this->v_emerald->getID());
					glDrawElements(GL_TRIANGLES, this->v_emerald->getElementsCount(), GL_UNSIGNED_INT, 0);
					glBindVertexArray(0);

					this->modelStack->pop();
				}

			this->s_intro->disable();

		glPopAttrib();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Convolve emeralds glow
	this->Filter(this->s_convolve, texFilter1, texFilter2);
	this->Filter(this->s_convolve, texFilter2, texFilter1);
	this->Filter(this->s_convolve, texFilter1, texFilter2);
	
	glPushAttrib(GL_ENABLE_BIT);

		glEnable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Draw stars background
		this->DrawStarsBackground(dt);

		// Draw emeralds glow
		this->s_basic2D->enable();

			this->s_basic2D->setUniformMatrix("in_ProjectionMatrix", this->orthoProjectionMatrix);
			this->s_basic2D->setUniform("in_UseTexture", true);
			this->s_basic2D->setUniform("in_AlphaFactor", 1.0f);
			this->s_basic2D->setUniformTexture("in_Texture", 0, this->texFilter2, GL_TEXTURE_2D);
			
			DrawQuad(0,0, this->width, InterfaceRendering::IFACE_HEIGHT, this->s_basic2D);

		this->s_basic2D->disable();
	glPopAttrib();




	// Draw emeralds
	this->s_intro->enable();
		this->s_intro->setUniformMatrix("in_ProjectionMatrix", this->perspProjectionMatrix);

		for(int i = 0; i < 7; i++) {
			
			this->modelStack->push();
			this->modelStack->loadIdentity();
			this->modelStack->translate(0, 0, -5);
			this->modelStack->translate(this->emeralds[i].position.x, this->emeralds[i].position.y, this->emeralds[i].position.z);
			this->modelStack->rotate(time * this->emeralds[i].angularVelocity.x, 1, 0, 0);
			this->modelStack->rotate(time * this->emeralds[i].angularVelocity.y, 0, 1, 0);

			this->s_intro->setUniformMatrix("in_ModelViewMatrix", this->modelStack->current());
			this->s_intro->setUniform("in_Shading", true);
			this->s_intro->setUniform("in_Color", this->emeralds[i].color);
			

			glBindVertexArray(this->v_emerald->getID());
			glDrawElements(GL_TRIANGLES, this->v_emerald->getElementsCount(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			this->modelStack->pop();
		}


	this->s_intro->disable();

}

void InterfaceRendering::DrawGameMenu(GLfloat dt, bool reset) {
	static GLfloat time = 0.0f;

	GLfloat titleWidth = this->width * 0.6f;
	GLfloat titleHeight = titleWidth / 16.0f * 9.0f;
	GLfloat titleX = this->width / 2.0f - titleWidth / 2.0f;
	GLfloat titleY = InterfaceRendering::IFACE_HEIGHT * 3.0f / 4.0f - titleHeight / 1.5f;

	GLfloat menuY = InterfaceRendering::IFACE_HEIGHT / 4.2;
	GLfloat menuBackHeight = InterfaceRendering::IFACE_HEIGHT / 2.5f;
	GLfloat menuBackStep = 10.0f;

	if(reset) {
		time = 0.0f;
		return;
	}


	glPushAttrib(GL_ENABLE_BIT);
		glEnable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Draw stars background
		this->DrawStarsBackground(dt);

	glPopAttrib();

	glPushAttrib(GL_ENABLE_BIT);
		glEnable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		this->s_basic2D->enable();
			// Draw Game Title

			this->s_basic2D->setUniformMatrix("in_ProjectionMatrix", this->orthoProjectionMatrix);
			this->s_basic2D->setUniform("in_UseTexture", true);
			this->s_basic2D->setUniform("in_AlphaFactor", 1.0f);
			this->s_basic2D->setUniformTexture("in_Texture", 0, this->texTitle, GL_TEXTURE_2D);

			this->DrawQuad(titleX, titleY, titleWidth, titleHeight, this->s_basic2D);
		this->s_basic2D->disable();

		// Draw Game Menu
		vector<GameMenuItem*> menuItems = this->activeMenu->GetChildren();
		for(int i = 0; i < this->activeMenu->GetChildCount(); i++) {
			GLfloat menuLength = menuItems[i]->GetDisplayText().size() * 4.0f;
			GLfloat menuX = this->width / 2.0f - menuLength / 2.0f;

			GameMenuCodeInputItem * inputMenu = dynamic_cast<GameMenuCodeInputItem*>(menuItems[i]);

			if(inputMenu && inputMenu->IsInputMode()) {
					
				this->s_basic2D->enable();

				this->s_basic2D->setUniformMatrix("in_ProjectionMatrix", this->orthoProjectionMatrix);
				this->s_basic2D->setUniform("in_UseTexture", false);
				this->s_basic2D->setUniform("in_AlphaFactor", 1.0f);
				this->s_basic2D->setUniform("in_Color", vec4(1.0, 0.0, 0.0, 0.5));			
						
				this->DrawQuad(menuX + 4.0f * string("Level Code: ").length() + 4.0f * (inputMenu->GetSelectedIndex() + inputMenu->GetSelectedIndex() / 4),
					menuY - 5.0f, 4.0f, 4.0f, this->s_basic2D);

				this->s_basic2D->disable();
			}

			vec3 color = i == this->activeMenu->GetSelectedChild() ? vec3(1.0, 1.0, 0.0) : vec3(1.0, 1.0, 1.0);
			this->DrawString(menuX, menuY - i * 5.0f, 4.0f, color, menuItems[i]->GetDisplayText(), this->texFontMenu);
		}

		// Draw version
		this->DrawString(0, 0.0f, 1.5f, vec3(1.0f, 1.0f, 1.0f), "V1.0.1c BUILD 137", this->texFontGame); 

		
	glPopAttrib();

	this->DrawFading(vec3(1.0, 1.0, 1.0), this->intMenu.GetValueAt(time), GL_ONE);

	time += dt;
}

void InterfaceRendering::DrawStageClear(GLfloat dt, int level, bool perfect, bool reset) {
	static GLfloat time = 0.0f;
	static string congratsString = "CONGRATULATIONS!";
	static string perfectString = "PERFECT";

	GLfloat levelStringX, levelStringY;
	GLfloat congratsStringX, congratsStringY;
	GLfloat perfectStringX, perfectStringY;
	GLfloat nextLevelStringX, nextLevelStringY;
	GLfloat deltaRadius;
	int nextLevel;

	string levelString;
	string nextLevelString;

	char buf[30];

	if(reset) {
		time = 0.0f;
		return;
	}

	if(time == 0.0f) {
		this->InitEmeralds();
		this->UpdateEmeralds(0, 0, true);
		this->DrawEmeraldScreen(0, true);
		this->audioEngine->PlayStageClear();
	}
	
	if(perfect)
		nextLevel = level + 10;
	else
		nextLevel = level + 1;

	sprintf(buf, "LEVEL %d CLEAR", level);
	levelString = string(buf);

	sprintf(buf, "NEXT LEVEL: %d", nextLevel);
	nextLevelString = string(buf);

	deltaRadius = 0.35f + sin(5.0f * time) * 0.05;

	this->UpdateEmeralds(dt, deltaRadius, false);

	this->DrawEmeraldScreen(dt, false);


	glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		// Draw "Level x" string
		levelStringX = this->width / 2.0f - levelString.size() * 4.0f / 2.0f;
		levelStringY = this->IFACE_HEIGHT - 6.0f;

		this->DrawString(levelStringX, levelStringY, 4.0f, vec3(1.0f, 1.0f, 1.0f), levelString, this->texFontGame);

		// Draw "Congratulations" string
		congratsStringX = this->width / 2.0f - congratsString.size() * 6.0f / 2.0f;
		congratsStringY = levelStringY - 5.0f * 2;

		this->DrawString(congratsStringX, congratsStringY, 6.0f, vec3(1.0f, 1.0f, 1.0f), congratsString, this->texFontGame);

		// Draw perfect string
		if(perfect) {
			perfectStringX = this->width / 2.0f - perfectString.size() * 4.0f / 2.0f;
			perfectStringY = InterfaceRendering::IFACE_HEIGHT / 2.0f - 4.0f / 2.0f;

			this->DrawString(perfectStringX, perfectStringY, 4.0f, vec3(abs(sin(5.0 * time)), 0.0f, 1.0f), perfectString, this->texFontGame);
		}

		// Draw next level string

		nextLevelStringX = this->width / 2.0f - nextLevelString.size() * 4.0f / 2.0f;
		nextLevelStringY = 4.0f;

		this->DrawString(nextLevelStringX, nextLevelStringY, 4.0f, vec3(1.0f, 1.0f, 0.0f), nextLevelString, this->texFontGame);

	glPopAttrib();

	this->DrawFading(vec3(1.0f, 1.0f, 1.0f), this->intStageClear.GetValueAt(time), GL_ONE);

	time += dt;
}

void InterfaceRendering::DrawGameStats(const GameStats& gameStats) {
	static char buf[50];
	static string fps, logic;
	static float y1 = InterfaceRendering::IFACE_HEIGHT - 2.0f, y2 = InterfaceRendering::IFACE_HEIGHT - 5.0f;
	static float x1, x2;

	sprintf(buf, "RENDER: %d fps - %dms", gameStats.fps.frequency, gameStats.fps.time);
	fps = string(buf);
	sprintf(buf, "UPDATE: %d Hz - %dms", gameStats.logic.frequency, gameStats.logic.time);
	logic = string(buf);

	x1 = this->width - fps.length() * 2.0f;
	x2 = this->width - logic.length() * 2.0f;

	this->DrawString(x1, y1, 2.0f, vec3(1, 1, 1), fps, this->texFontGame);
	this->DrawString(x2, y2, 2.0f, vec3(1, 1, 1), logic, this->texFontGame);

}

void InterfaceRendering::DrawRingSprite(const SpriteTimer& spriteTimer) {
	if(spriteTimer.IsRunning()) {
		this->s_basic2D->enable();
			this->s_basic2D->setUniformMatrix("in_ProjectionMatrix", this->orthoProjectionMatrix);
			this->s_basic2D->setUniform("in_UseTexture", true);
			this->s_basic2D->setUniform("in_AlphaFactor", 1.0f);
			this->s_basic2D->setUniformTexture("in_Texture", 0, this->ringSprite->GetFrame(spriteTimer.GetTime()));
			this->DrawQuad(this->width/2 - 5, InterfaceRendering::IFACE_HEIGHT /2 - 25, 10, 10, this->s_basic2D);
		this->s_basic2D->disable();
	}
}

void InterfaceRendering::DrawGameInterface(GLint collectedRings, GLint blueSpheresLeft, GLfloat dt) {

	static char buf[4];

	this->s_basic2D->enable();
		
		this->s_basic2D->setUniformMatrix("in_ProjectionMatrix", this->orthoProjectionMatrix);
		this->s_basic2D->setUniform("in_UseTexture", true);
		this->s_basic2D->setUniform("in_AlphaFactor", 1.0f);

		this->s_basic2D->setUniformTexture("in_Texture", 0, this->texBlueSphere, GL_TEXTURE_2D);
		this->DrawQuad(1.0, InterfaceRendering::IFACE_HEIGHT - 6.0f, 5.0f, 5.0f, this->s_basic2D); 

		this->s_basic2D->setUniformTexture("in_Texture", 0, this->texRing, GL_TEXTURE_2D);
		this->DrawQuad(1.0f, InterfaceRendering::IFACE_HEIGHT - 12.0f, 
			5.0f, 5.0f, this->s_basic2D); 

	this->s_basic2D->disable();

	sprintf(buf, "%03d", blueSpheresLeft);

	this->DrawString(7.0f, InterfaceRendering::IFACE_HEIGHT - 5.0f, 3.0f,
		vec3(1.0, 1.0, 1.0), string(buf), this->texFontGame);

	sprintf(buf, "%03d", collectedRings);

	this->DrawString(7.0f, InterfaceRendering::IFACE_HEIGHT - 11.0f, 3.0f,
		vec3(1.0, 1.0, 1.0), string(buf), this->texFontGame);

	if(this->displayingInGameMessage) {

		static GLfloat inGameMessageY = InterfaceRendering::IFACE_HEIGHT / 4.0f * 3.0f - 1.5f;
		GLfloat inGameMessageX, inGameMessageWidth, delta;

		this->inGameMessageTimer += dt;
		
		inGameMessageWidth = this->inGameMessage.length() * 3.0;

		if(this->inGameMessageTimer > InterfaceRendering::IN_GAME_MESSAGE_TOTAL_TIME ) {
			this->displayingInGameMessage = false;
			this->inGameMessageTimer = InterfaceRendering::IN_GAME_MESSAGE_TOTAL_TIME;
		}
	
		if(this->inGameMessageTimer < InterfaceRendering::IN_GAME_MESSAGE_TRANSITION_TIME) 
		{ // message jump in
			delta = (this->inGameMessageTimer /  InterfaceRendering::IN_GAME_MESSAGE_TRANSITION_TIME);
			inGameMessageX = (1.0f - delta) * this->width + delta * (this->width / 2.0f - inGameMessageWidth / 2.0f); 
		} 
		else if(this->inGameMessageTimer <= (InterfaceRendering::IN_GAME_MESSAGE_DURATION + InterfaceRendering::IN_GAME_MESSAGE_TRANSITION_TIME))
		{
			inGameMessageX = this->width / 2.0f - inGameMessageWidth / 2.0f;
		} 
		else if(this->inGameMessageTimer <= InterfaceRendering::IN_GAME_MESSAGE_TOTAL_TIME)
		{
			
			delta = 1.0f - (InterfaceRendering::IN_GAME_MESSAGE_TOTAL_TIME - this->inGameMessageTimer) / 
				InterfaceRendering::IN_GAME_MESSAGE_TRANSITION_TIME;
			inGameMessageX = (1.0f - delta) * (this->width / 2.0f - inGameMessageWidth / 2.0f)
				+ delta * (-inGameMessageWidth);
			
		}

		glPushAttrib(GL_ENABLE_BIT);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			this->s_basic2D->enable();
				// draw message quad
				this->s_basic2D->setUniformMatrix("in_ProjectionMatrix", this->orthoProjectionMatrix);
				this->s_basic2D->setUniform("in_Color", vec4(0.0, 0.0, 1.0, .8)); 
				this->s_basic2D->setUniform("in_UseTexture", false);
				this->s_basic2D->setUniform("in_AlphaFactor", 1.0f);
				this->DrawQuad(inGameMessageX - 4.0f, inGameMessageY - 1.0f,  inGameMessageWidth + 8.0f, 5.0f, this->s_basic2D);

				// draw stars
				this->s_basic2D->setUniformTexture("in_Texture", 0, this->texInGameMessageStar); 
				this->s_basic2D->setUniform("in_UseTexture", true);
				this->DrawQuad(inGameMessageX - 9.0f, inGameMessageY - 3.5f,  10.0f, 10.0f, this->s_basic2D);
				this->DrawQuad(inGameMessageX + inGameMessageWidth - 1.0f, inGameMessageY - 3.5f,  10.0f, 10.0f, this->s_basic2D);

			this->s_basic2D->disable();

			this->DrawString(inGameMessageX, inGameMessageY, 3.0f, vec3(1.0, 1.0, 1.0), this->inGameMessage, this->texFontGame);
		glPopAttrib();
	}
}

void InterfaceRendering::DrawFading(const vec3& color, GLfloat fadingFactor, GLenum blendDFactor) {
	glPushAttrib(GL_ENABLE_BIT);
		glEnable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, blendDFactor); // additive blending

		this->s_basic2D->enable();
			
			this->s_basic2D->setUniformMatrix("in_ProjectionMatrix", this->orthoProjectionMatrix);
			this->s_basic2D->setUniform("in_UseTexture", false);
			this->s_basic2D->setUniform("in_Color", vec4(color, fadingFactor));
			this->s_basic2D->setUniform("in_AlphaFactor", 1.0f);

			this->DrawQuad(0, 0, this->width, InterfaceRendering::IFACE_HEIGHT, this->s_basic2D);

		this->s_basic2D->disable();
	glPopAttrib();

}

void InterfaceRendering::DisplayInGameMessage(string message) {
	this->inGameMessage = message;
	this->displayingInGameMessage = true;
	this->inGameMessageTimer = 0.0f;
}


InterfaceRendering::~InterfaceRendering(void)
{
	delete this->projStack;
}

void InterfaceRendering::ResizeBuffers() {

	char * pixels = new char[(int)this->viewWidth * (int)this->viewHeight * 4];

	ZeroMemory(pixels, (int)this->viewWidth * (int)this->viewHeight * 4);
	
	glBindTexture(GL_TEXTURE_2D, this->texFilter0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (int)this->viewWidth, (int)this->viewHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glBindTexture(GL_TEXTURE_2D, this->texFilter1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (int)this->viewWidth, (int)this->viewHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glBindTexture(GL_TEXTURE_2D, this->texFilter2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (int)this->viewWidth, (int)this->viewHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	glBindTexture(GL_TEXTURE_2D, this->texFilterDepth);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, (int)this->viewWidth, (int)this->viewHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0);

	delete pixels;
}

void InterfaceRendering::InitEmeralds() {
	for(int i = 0; i < 7; i++) {
		GLfloat angle = 2.0 * Global::PI / 7.0f * i;
		this->emeralds[i].position = vec3(0, sin(angle), 0.0f) * 5.0f;
		this->emeralds[i].color = InterfaceRendering::EMERALD_COLORS[i];
		this->emeralds[i].angularVelocity = vec2(
			Global::Random(Global::PI, Global::PI * 2.0),
			Global::Random(Global::PI, Global::PI * 2.0)
		);
		this->emeralds[i].radius = 4.0f;
	}
}

void InterfaceRendering::UpdateEmeralds(GLfloat dt, GLfloat radiusDelta, bool reset) {
	static GLfloat time;

	if(reset) {
		time = 0;
		return;
	}

	time += dt;

	for(int i = 0; i < 7; i++) {
		GLfloat angle = 2.0 * Global::PI / 7.0f * i +  time * Global::PI;
		this->emeralds[i].position = vec3(cos(angle), sin(angle), 0.0f) * this->emeralds[i].radius * radiusDelta;
	}
}

void InterfaceRendering::Filter(Shader * filterShader, GLuint texSrc, GLuint texDst) {
	static GLuint drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };

	glClearColor(0, 0, 0, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, this->fboFilter);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->texFilter0, 0);

		glDrawBuffers(1, drawBuffers);

		glClear(GL_COLOR_BUFFER_BIT);

		glPushAttrib(GL_ENABLE_BIT);
			glDisable(GL_DEPTH_TEST);

			filterShader->enable();

			filterShader->setUniformMatrix("in_ProjectionMatrix", this->orthoProjectionMatrix);
			filterShader->setUniformTexture("in_Src", 0, texSrc);
			filterShader->setUniform("in_Horizontal", true);
			filterShader->setUniform("in_Step", 1.0f / this->viewWidth);

			this->DrawQuad(0, 0, this->width, InterfaceRendering::IFACE_HEIGHT, filterShader);

			filterShader->disable();

		glPopAttrib();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, this->fboFilter);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texDst, 0);

		glDrawBuffers(1, drawBuffers);

		glClear(GL_COLOR_BUFFER_BIT);

		glPushAttrib(GL_ENABLE_BIT);
			glDisable(GL_DEPTH_TEST);

			filterShader->enable();

			filterShader->setUniformMatrix("in_ProjectionMatrix", this->orthoProjectionMatrix);
			filterShader->setUniformTexture("in_Src", 0, this->texFilter0);
			filterShader->setUniform("in_Horizontal", false);
			filterShader->setUniform("in_Step", 1.0f / this->viewHeight);

			this->DrawQuad(0, 0, this->width, InterfaceRendering::IFACE_HEIGHT, filterShader);

			filterShader->disable();

		glPopAttrib();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void InterfaceRendering::InitStars() {
	for(int i = 0; i < InterfaceRendering::TITLE_STARS; i++) {
		this->stars[i].position = vec2(Global::Random(this->width), Global::Random(InterfaceRendering::IFACE_HEIGHT));
		this->stars[i].color = vec4(Global::Random(0.5f, 1.0f), Global::Random(0.7f, 1.0f), 1.0f, 1);
		this->stars[i].speed = Global::Random(20.0f, 80.0f);
	}
}

void InterfaceRendering::UpdateStars(GLfloat dt) {
	static vec2 movementDirection = vec2(1, 0);
	for(int i = 0; i < InterfaceRendering::TITLE_STARS; i++) {
		this->stars[i].position = this->stars[i].position + movementDirection * this->stars[i].speed * dt;
		if(this->stars[i].position.x > this->width)
			this->stars[i].position.x = 0.0f;
	}
}

void InterfaceRendering::InitInterpolators() {

	this->intDisclaimer = LinearInterpolator();
	this->intIntro = LinearInterpolator();
	this->intMenu = LinearInterpolator();
	this->intStageClear = LinearInterpolator();

	this->intDisclaimer.AddFunctionPoint(0, 1.0f);
	this->intDisclaimer.AddFunctionPoint(1, 0.0f);
	this->intDisclaimer.AddFunctionPoint(InterfaceRendering::DISCLAIMER_TIME - 1.0f, 0.0f);
	this->intDisclaimer.AddFunctionPoint(InterfaceRendering::DISCLAIMER_TIME, 1.0f);

	this->intIntro.AddFunctionPoint(0.0f, 1.0f);
	this->intIntro.AddFunctionPoint(1.0f, 0.0f);
	this->intIntro.AddFunctionPoint(InterfaceRendering::INTRO_TIME - 1.0f, 0.0f);
	this->intIntro.AddFunctionPoint(InterfaceRendering::INTRO_TIME, 1.0f);

	this->intMenu.AddFunctionPoint(0.0f, 1.0f);
	this->intMenu.AddFunctionPoint(2.0, 0.0f);

	this->intStageClear.AddFunctionPoint(0.0f, 1.0f);
	this->intStageClear.AddFunctionPoint(1.0, 0.0f);
}

void InterfaceRendering::InitBuffers() {
	vec2 vertices_texCoords[4] = { vec2(0, 0), vec2(1, 0), vec2(1, 1), vec2(0, 1) };
	GLuint indices[6] = { 0, 1, 2, 2, 3, 0 };
	XBO * emerald = ExtendedBinary3DObject::FromFile("models/emerald2.xbo");

	this->v_quad = new VertexArrayObject(1, GL_STATIC_DRAW);
	this->v_quad->setBufferData(0, sizeof(vec2) * 4, vertices_texCoords);
	this->v_quad->setElementsData(sizeof(GLuint), 6, indices);
	this->v_quad->setVertexAttribute(0, 0, 2, GL_FLOAT);
	this->v_quad->setVertexAttribute(0, 1, 2, GL_FLOAT);
	this->v_quad->enableVertexAttribute(0);
	this->v_quad->enableVertexAttribute(1);

	this->v_string = new VertexArrayObject(2, GL_DYNAMIC_DRAW);

	this->v_string->setVertexAttribute(0, 0, 2, GL_FLOAT);
	this->v_string->setVertexAttribute(1, 1, 1, GL_FLOAT);
	this->v_string->enableVertexAttribute(0);
	this->v_string->enableVertexAttribute(1);

	this->v_stars = new VertexArrayObject(1, GL_DYNAMIC_DRAW);

	this->v_stars->setVertexAttribute(0, 0, 2, GL_FLOAT, GL_FALSE, sizeof(StarVertex), 0);
	this->v_stars->setVertexAttribute(0, 1, 4, GL_FLOAT, GL_FALSE, sizeof(StarVertex), (char*)0 + 8);
	this->v_stars->enableVertexAttribute(0);
	this->v_stars->enableVertexAttribute(1);

	this->v_emerald = CreateVertexArray(emerald, 0);
	this->v_emerald->enableVertexAttribute(0);
	this->v_emerald->enableVertexAttribute(1);
	this->v_emerald->enableVertexAttribute(2);

	glGenFramebuffers(1, &this->fboFilter);

	delete emerald;
}

void InterfaceRendering::InitShaders() {
	ShaderManager * manager = ShaderManager::getDefaultManager();
	this->s_basic2D = manager->createShader("shaders/basic2D.vert", "shaders/basic2D.frag");
	this->s_font = manager->createShader("shaders/font.vert", "shaders/font.geom", "shaders/font.frag");
	this->s_stars = manager->createShader("shaders/stars.vert", "shaders/stars.geom", "shaders/stars.frag");
	this->s_intro = manager->createShader("shaders/intro.vert", "shaders/intro.frag");
	this->s_convolve = NULL;
}

void InterfaceRendering::InitTextures() {

	glGenTextures(1, &this->texFilter0);
	glGenTextures(1, &this->texFilter1);
	glGenTextures(1, &this->texFilter2);

	glGenTextures(1, &this->texFilterDepth);

	this->texBlueSphere = Global::PngTexture("textures/bluesphere.png", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	this->texRing = Global::PngTexture("textures/ring.png", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	this->texFontMenu = Global::PngTexture("textures/font.png", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	this->texFontGame = Global::PngTexture("textures/font2.png", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	this->texDisclaimer = Global::PngTexture("textures/disc.png", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	this->texTitle = Global::PngTexture("textures/bs.png", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	this->texStar = Global::PngTexture("textures/star.png", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	this->texInGameMessageStar = Global::PngTexture("textures/staringgamemsg2.png", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

	vector<string> ringSpriteFrames;
	ringSpriteFrames.push_back("ringglow_01.png");
	ringSpriteFrames.push_back("ringglow_02.png");
	ringSpriteFrames.push_back("ringglow_03.png");
	ringSpriteFrames.push_back("ringglow_04.png");
	ringSpriteFrames.push_back("ringglow_05.png");
	ringSpriteFrames.push_back("ringglow_06.png");
	this->ringSprite = new Sprite(ringSpriteFrames, GameLogic::RING_SPRITE_LIFETIME);

}

void InterfaceRendering::DrawStarsBackground(GLfloat dt) {
	this->s_stars->enable();

		this->s_stars->setUniformMatrix("in_ProjectionMatrix", this->orthoProjectionMatrix);
		this->s_stars->setUniformTexture("in_Texture", 0, this->texStar, GL_TEXTURE_2D);
		this->s_stars->setUniform("in_StarSize", 1.0f);

		this->UpdateStars(dt);

		this->v_stars->setBufferData(0, sizeof(StarVertex) * InterfaceRendering::TITLE_STARS, this->stars);

		glBindVertexArray(this->v_stars->getID());
		glDrawArrays(GL_POINTS, 0, InterfaceRendering::TITLE_STARS);
		glBindVertexArray(0);

	this->s_stars->disable();
}

void InterfaceRendering::DrawQuad(GLfloat x, GLfloat y, GLfloat width, GLfloat height, Shader * activeShader) {
	this->modelStack->push();

	this->modelStack->translate(x, y, 0.0f);
	this->modelStack->scale(width, height, 1.0f);

	activeShader->setUniformMatrix("in_ModelMatrix", modelStack->current());

	glBindVertexArray(this->v_quad->getID());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	this->modelStack->pop();
}

void InterfaceRendering::DrawString(GLfloat x, GLfloat y, GLfloat size, const vec3 &color, string text, GLuint fontTexture) {
	
	int length = text.length();
	vec2 * points = new vec2[length];
	GLint * asciiCodes = new GLint[length];

	for(int i = 0; i < length; i++) {
		points[i].x = i * size;
		points[i].y = 0.0f;
		asciiCodes[i] = (GLuint) text.at(i);
	}

	this->v_string->setBufferData(0, sizeof(vec2) * length, points);
	this->v_string->setBufferData(1, sizeof(GLint) * length, asciiCodes);

	this->modelStack->push();

	this->modelStack->translate(x, y, 0.0f);

	this->s_font->enable();

		this->s_font->setUniformMatrix("in_ProjectionMatrix", this->orthoProjectionMatrix);
		this->s_font->setUniformMatrix("in_ModelMatrix", this->modelStack->current());
		this->s_font->setUniformTexture("in_Font", 0, fontTexture, GL_TEXTURE_2D);
		this->s_font->setUniform("in_Size", size);
		this->s_font->setUniform("in_Color", color);

		glBindVertexArray(this->v_string->getID());
		glDrawArrays(GL_POINTS, 0, length);
		glBindVertexArray(0);

	this->s_font->disable();

	this->modelStack->pop();

	// Free some memory

	delete[] points;
	delete[] asciiCodes;
}
