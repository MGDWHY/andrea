#pragma once

#include <algorithm>
#include "vecmath.h"
#include "GameData.h"
#include "AudioEngine.h"
#include "Sonic.h"
#include "Sprite.h"

using namespace glutils;
using namespace Vecmath;

class GameLogicListener
{
public:
	virtual void InGameMessage(string message) = 0;
};

class GameLogic
{

private:

	/* constants for the CheckCrossEdge function */ 
	static const GLuint CROSS_NONE;
	static const GLuint CROSS_HORIZONTAL;
	static const GLuint CROSS_VERTICAL;

	/* game velocity defaults */
	static const GLfloat BASE_VELOCITY;
	static const GLfloat ANGULAR_VELOCITY;
	static const GLfloat MAX_VELOCITY;

	/* game speed up */
	static const GLfloat GAME_SPEEDUP_TIME;
	static const GLfloat GAME_SPEEDUP_INCREMENT;
	static const GLfloat GAME_MUSIC_TEMPO_SPEEDUP;

	/* sphere radius for collision detection with star spheres */
	static const GLfloat SPHERE_RADIUS;

	/* maximum sonic height (jump) for collision */
	static const GLfloat COLLIDE_MAX_HEIGHT;

	/* fading time for game start and game over (in seconds) */
	static const GLfloat START_TIME;
	static const GLfloat START_FADE_TIME;
	static const GLfloat GAMEOVER_FADE_TIME;

	/* game over rotation acceleration over time */
	static const GLfloat GAMEOVER_ROTATION_ACCEL;

	/* velocity of the items when they go up after the last blue sphere has ben taken */
	static const GLfloat EMERALD_ITEMS_VELOCITY;

	/* time-lapse before emerald arrival after clearing the stage (in seconds */
	static const GLfloat EMERALD_TRAVEL_TIME;
	
	/* starting distance where the emerald appears */
	static const GLfloat EMERALD_START_DISTANCE;

	/* velocity which the emerald moves towards sonic */
	static const GLfloat EMERALD_VELOCITY;

	/* sonic run animation time */
	static const GLfloat SONIC_RUN_ANIMATION_TIME;

	/* sonic jump */
	static const GLfloat SONIC_JUMP_DISTANCE;
	static const GLfloat SONIC_JUMP_HEIGHT;
	static const GLfloat SONIC_JUMP_REVOLUTIONS;

	/* yellow spheres settings */
	static const GLfloat YELLOW_SPHERE_JUMP_DISTANCE;
	static const GLfloat YELLOW_SPHERE_JUMP_HEIGHT;

	vec2 position;
	ivec2 direction;
	GLfloat velocity;
	GLfloat speedUpTimer;

	bool goingBackward;
	bool jumping;
	bool jump;
	
	bool runForward;

	GLuint rotate;

	GLfloat rotationAngle;
	
	GLfloat ringsRotatationAngle;

	GLfloat fadingFactor;

	GLfloat worldItemsHeight;

	GLfloat emeraldDistance;

	GLfloat sonicHeight;
	GLfloat sonicJumpDistance;
	GLfloat sonicJumpHeight;
	GLfloat sonicRotationX;
	GLuint sonicRunAnimationFrame;
	GLint sonicJumpSoundEffect;
	GLfloat sonicRunAnimationFract;

	GLuint gameStatus;
	
	GameData * gameData;
	AudioEngine * audioEngine;

	vector<GameLogicListener*> listeners;

	SpriteTimer ringSpriteTimer;

	GLuint CheckCrossEdge(vec2 currentPosition, vec2 futurePosition);
	ivec2 RoundNearestPosition(const vec2 &v);
	ivec2 NormalizePosition(const ivec2 &v);

	void SonicRunAnimationAdvance(GLfloat dt, GLfloat velocity);

	void FireInGameMessage(string message);

	void ServeRotation(float& finalAngle, GLuint& rotationDirection);
	void PerformRotation(float dt, GLuint& rotationDirection, bool& rotating, float& finalAngle);

public:

	/* constants for rotations */
	static const GLuint ROTATE_NONE;
	static const GLuint ROTATE_LEFT;
	static const GLuint ROTATE_RIGHT;

	/* constants for character directions */
	static const ivec2 DIRECTION_LEFT;
	static const ivec2 DIRECTION_RIGHT;
	static const ivec2 DIRECTION_UP;
	static const ivec2 DIRECTION_DOWN;

	/* constants for diagonal directions */
	static const ivec2 DIRECTION_UP_LEFT;
	static const ivec2 DIRECTION_UP_RIGHT;
	static const ivec2 DIRECTION_DOWN_LEFT;
	static const ivec2 DIRECTION_DOWN_RIGHT;

	/* Game Status */
	static const GLuint STATUS_STARTING;
	static const GLuint STATUS_GAMEOVER;
	static const GLuint STATUS_FINISHED;
	static const GLuint STATUS_EMERALD;
	static const GLuint STATUS_PLAYING;
	static const GLuint STATUS_NONE;

	/* Ring sprite lifetime */
	static const GLfloat RING_SPRITE_LIFETIME;

	GameLogic(GameData * gameData, AudioEngine * audioEngine);
	~GameLogic(void);

	void SetDirection(ivec2 direction);
	void SetPosition(vec2 position);

	GLfloat GetVelocity();
	ivec2 GetDirection();
	vec2 GetPosition();
	ivec2 GetRoundedPosition();
	void GetModPosition(vec2 &intPos, vec2 &fractPos);
	vec2 GetTexturePosition();
	
	GLfloat GetRotationAngle();

	GLfloat GetRingsRotationAngle();

	GLfloat GetFadingFactor();

	GLfloat GetWorldItemsHeight();

	GLfloat GetEmeraldDistance();

	bool IsSonicGoingBackward();
	bool IsSonicJumping();
	bool IsEmeraldVisible();

	GLfloat GetSonicHeight();
	GLfloat GetSonicRotationX();
	GLuint GetSonicAnimationFrame();
	GLfloat GetSonicAnimationDelta();

	SpriteTimer GetRingSpriteTimer();

	GLuint GetGameStatus();

	void Animate(GLfloat dt);
	void SetRotate(GLuint rotate);
	void SetJump();
	void SetRunForward();
	
	void AddGameLogicListener(GameLogicListener * listener);

};
class TransformRingsState {
private:
	vector<ivec2> path;
	ivec2 cantgo;

public:
	static TransformRingsState * Go(const TransformRingsState& current, ivec2 direction, ivec2 goal, GameData * gameData);
	static bool GoalTest(const TransformRingsState& current, ivec2 goal);
	vector<ivec2> * GetPath();

	bool operator==(const TransformRingsState& other);
};

class TransformRingsAlgorithm {
private:
	static void EvaluateState(TransformRingsState * state, vector<TransformRingsState*>* states, vector<TransformRingsState*>* evaluated);
	static void FindTransformBlock(const ivec2& currentPosition, vector<ivec2> * block);
	static bool IsInsidePath(ivec2 position, vector<ivec2> * path);
public:

	static ivec2 CurrentGoal;
	static GameData * CurrentGameData;

	static vector<ivec2> * Calculate();

	static bool EuclideanDistanceComparator(TransformRingsState * v1, TransformRingsState * v2);
};

