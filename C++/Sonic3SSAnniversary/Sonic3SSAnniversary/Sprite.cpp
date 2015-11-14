#include "Sprite.h"


Sprite::Sprite(vector<string> textureFiles, float executionTime)
{
	this->numFrames = textureFiles.size();
	this->textures = new GLuint[this->numFrames];
	this->executionTime = executionTime;
	
	for(int i = 0; i < this->numFrames; i++)
		this->textures[i] = Global::PngTexture((Global::TEXTURE_DIR + "/" + textureFiles[i]).c_str(), GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
}


Sprite::~Sprite(void)
{
	glDeleteTextures(numFrames, this->textures);
	delete this->textures;
}

GLuint Sprite::GetFrame(float time) {
	if(time <= 0.0f)
		return this->textures[0];
	else if(time >= this->executionTime)
		return this->textures[this->numFrames - 1];
	else
		return this->textures[ (int) ((time / this->executionTime) * this->numFrames)];
}

SpriteTimer::SpriteTimer(float lifeTime) {
	this->lifeTime = lifeTime;
	this->currentTime = 0.0f;
}

SpriteTimer::SpriteTimer()  {
	this->lifeTime = 0.0;
	this->currentTime = 0.0;
}

void SpriteTimer::Animate(float dt) {
	this->currentTime += dt;
}

void SpriteTimer::SetLifeTime(float lifeTime) { this->lifeTime = lifeTime; }
float SpriteTimer::GetLifeTime() const { return this->lifeTime; }

void SpriteTimer::SetTime(float time) {this->currentTime = time;}
float SpriteTimer::GetTime() const {return this->currentTime;}
void SpriteTimer::Reset() {this->SetTime(0);}

bool SpriteTimer::IsRunning() const {return this->currentTime <= lifeTime;}


