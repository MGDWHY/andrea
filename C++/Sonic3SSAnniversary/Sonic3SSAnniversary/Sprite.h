#pragma once

#include <vector>
#include "Global.h"


class Sprite
{
private:
	int numFrames;
	float executionTime;
	GLuint * textures;
public:

	GLuint GetFrame(float time);
	Sprite(vector<string> textureFiles, float executionTime);
	~Sprite(void);
};

class SpriteTimer {
private:
	float lifeTime;
	float currentTime;
public:
	SpriteTimer();
	SpriteTimer(float lifeTime);
	void Animate(float dt);
	void Reset();
	void SetLifeTime(float lifeTime);
	float GetLifeTime() const;
	void SetTime(float time);
	float GetTime() const;
	bool IsRunning() const;
};