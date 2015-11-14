#include <iostream>
#include "GameLogic.h"

using namespace std;

/* Constants for directions */
const ivec2 GameLogic::DIRECTION_LEFT = ivec2(-1, 0);
const ivec2 GameLogic::DIRECTION_RIGHT = ivec2(1, 0);
const ivec2 GameLogic::DIRECTION_UP = ivec2(0, -1);
const ivec2 GameLogic::DIRECTION_DOWN = ivec2(0, 1);

/* Internal direction... Only used for the "Transform Rings" algorithm */
const ivec2 GameLogic::DIRECTION_UP_LEFT = GameLogic::DIRECTION_LEFT + GameLogic::DIRECTION_UP;
const ivec2 GameLogic::DIRECTION_UP_RIGHT = GameLogic::DIRECTION_RIGHT + GameLogic::DIRECTION_UP;
const ivec2 GameLogic::DIRECTION_DOWN_LEFT = GameLogic::DIRECTION_LEFT + GameLogic::DIRECTION_DOWN;
const ivec2 GameLogic::DIRECTION_DOWN_RIGHT = GameLogic::DIRECTION_RIGHT + GameLogic::DIRECTION_DOWN;

/* Constants for rotations */
const GLuint GameLogic::ROTATE_NONE = 0;
const GLuint GameLogic::ROTATE_LEFT = 1;
const GLuint GameLogic::ROTATE_RIGHT = 2;

/* Constants for edge crossing */
const GLuint GameLogic::CROSS_NONE = 0;
const GLuint GameLogic::CROSS_HORIZONTAL = 1;
const GLuint GameLogic::CROSS_VERTICAL = 2;

/* Game velocity defaults */
const GLfloat GameLogic::BASE_VELOCITY = 3.5f;
const GLfloat GameLogic::ANGULAR_VELOCITY = 6.283185f;
const GLfloat GameLogic::MAX_VELOCITY = 5.5f;

/* game speed up */
const GLfloat GameLogic::GAME_SPEEDUP_TIME = 30.0f;
const GLfloat GameLogic::GAME_SPEEDUP_INCREMENT = 0.5f;
const GLfloat GameLogic::GAME_MUSIC_TEMPO_SPEEDUP = 5.0f;

/* sphere radius for collision detection with star spheres */
const GLfloat GameLogic::SPHERE_RADIUS = 0.1f;

/* maximum sonic height (jump) for collision */
const GLfloat GameLogic::COLLIDE_MAX_HEIGHT = 0.2f;

/* game status */
const GLuint GameLogic::STATUS_NONE = -1;
const GLuint GameLogic::STATUS_STARTING = 0;
const GLuint GameLogic::STATUS_GAMEOVER = 1;
const GLuint GameLogic::STATUS_EMERALD = 2;
const GLuint GameLogic::STATUS_PLAYING = 3;
const GLuint GameLogic::STATUS_FINISHED = 4;

/* fading time */
const GLfloat GameLogic::START_TIME = 4.0f;
const GLfloat GameLogic::START_FADE_TIME = 2.0f;
const GLfloat GameLogic::GAMEOVER_FADE_TIME = 2.0f;


/* game over rotation acceleration over time */
const GLfloat GameLogic::GAMEOVER_ROTATION_ACCEL = GameLogic::ANGULAR_VELOCITY * 2.0f;

/* velocity of the items when they go up after the last blue sphere has ben taken */
const GLfloat GameLogic::EMERALD_ITEMS_VELOCITY = 2.0f;

/* velocity which the emerald travels towards sonic */
const GLfloat GameLogic::EMERALD_TRAVEL_TIME = 4.0f;

/* distance which the emerald appears */
const GLfloat GameLogic::EMERALD_START_DISTANCE = 15.0f;

/* emerald velocity */
const GLfloat GameLogic::EMERALD_VELOCITY = GameLogic::EMERALD_START_DISTANCE /
												GameLogic::EMERALD_TRAVEL_TIME;


/* sonic run animation time */
const GLfloat GameLogic::SONIC_RUN_ANIMATION_TIME = 0.5f;

/* sonic jump */
const GLfloat GameLogic::SONIC_JUMP_DISTANCE = 2.0f;
const GLfloat GameLogic::SONIC_JUMP_HEIGHT = .5f;
const GLfloat GameLogic::SONIC_JUMP_REVOLUTIONS = 1.0f;

/* yellow spheres */
const GLfloat GameLogic::YELLOW_SPHERE_JUMP_DISTANCE = 6.0f;
const GLfloat GameLogic::YELLOW_SPHERE_JUMP_HEIGHT = .8f;


/* ring sprite lifetime */
const GLfloat GameLogic::RING_SPRITE_LIFETIME = 0.2f;

/* Sets the current direction */
void GameLogic::SetDirection(ivec2 direction) {
	if(direction == GameLogic::DIRECTION_UP)
		this->rotationAngle = 0.0f;
	else if(direction == GameLogic::DIRECTION_DOWN)
		this->rotationAngle =  Global::PI;
	else if(direction == GameLogic::DIRECTION_LEFT)
		this->rotationAngle = - Global::PI / 2;
	else if(direction == GameLogic::DIRECTION_RIGHT)
		this->rotationAngle = Global::PI / 2;
		
	this->direction = direction;
}

/* Sets the current position */
void GameLogic::SetPosition(vec2 position) {
	this->position = position;
}

/* Returns the current game velocity */
GLfloat GameLogic::GetVelocity() {
	return this->velocity;
}

/* Returns the current position */
vec2 GameLogic::GetPosition() {
	return this->position;
}

/* Returns the current movement direction */
ivec2 GameLogic::GetDirection() {
	return this->direction;
}

GLuint GameLogic::GetGameStatus() {
	return this->gameStatus;
}

GLfloat GameLogic::GetFadingFactor() {
	return this->fadingFactor;
}

GLfloat GameLogic::GetWorldItemsHeight() {
	return this->worldItemsHeight;
}

GLfloat GameLogic::GetEmeraldDistance() {
	return this->emeraldDistance;
}

bool GameLogic::IsEmeraldVisible() {
	return this->emeraldDistance != -1.0f;
}

bool GameLogic::IsSonicJumping() {
	return this->jumping;
}

GLfloat GameLogic::GetSonicHeight() {
	return this->sonicHeight;
}

GLfloat GameLogic::GetSonicRotationX() {
	return this->sonicRotationX;
}

/*
	Performs world animation for a given time lapse
*/
void GameLogic::Animate(GLfloat dt) {

	static vec2 currentPosition;
	static vec2 intPos, fractPos;
	static ivec2 prevRotationPosition;
	static GLuint rotationDirection;
	static GLfloat startTimeCounter;
	static GLfloat finalAngle;
	static GLfloat maxMusicVolume;
	static GLfloat remainingJumpDistance;
	static GLfloat velocityScale;
	static GLfloat prevBounceDistance;
	static GLfloat prevBackwardBlueSphereDistance;
	static bool rotating;
	static bool bounce;
	static bool emeraldSound;

	/* The game has already finished. Calls to this function do nothing */
	if(this->gameStatus == STATUS_FINISHED)
		return;

	/* The first call to this function starts the game */
	if(this->gameStatus == GameLogic::STATUS_NONE) {
		this->gameStatus = GameLogic::STATUS_STARTING;
		this->audioEngine->RestartMusic();
		this->audioEngine->SetMusicVolume(1.0f);
		this->audioEngine->SetMusicTempo(0.0f);
		this->fadingFactor = 1.0f;
		
		prevRotationPosition = ivec2(-1,-1);
		prevBounceDistance = 0.0f;
		prevBackwardBlueSphereDistance = 0.0f;
		rotationDirection = ROTATE_NONE;
		finalAngle = 0.0f;
		maxMusicVolume = 0.0f;
		remainingJumpDistance = 0.0f;
		startTimeCounter = 0.0f;
		velocityScale = 1.0f;
		rotating = false;
		bounce = false;
		emeraldSound = true;
	}

	/* Update rings rotation angle */
	this->ringsRotatationAngle += dt * 3.0f;
	if(this->ringsRotatationAngle >= 2 * Global::PI)
		this->ringsRotatationAngle -= 2 * Global::PI;

	/* Update ring sprites */
	this->ringSpriteTimer.Animate(dt);
	
	/* Status starting: 
		The screen fades from white and the player is kept in starting position
	*/
	if(this->gameStatus == GameLogic::STATUS_STARTING) {
		this->fadingFactor -= dt /  GameLogic::START_FADE_TIME;
		startTimeCounter += dt;
		if(startTimeCounter >= GameLogic::START_TIME) {
			startTimeCounter = 0.0f;
			this->fadingFactor = 0.0f;
			this->gameStatus = GameLogic::STATUS_PLAYING;
			this->FireInGameMessage("GET BLUE SPHERES");
		}

		if(rotating) {
			this->PerformRotation(dt, rotationDirection, rotating, finalAngle);
		} else {
			if(rotate != ROTATE_NONE) {
				this->ServeRotation(finalAngle, rotationDirection);

				rotating = true;
			}
		}

		return;
	}

	/*Status game over:
		The screen fades to white and the world spins round
	*/
	if(this->gameStatus == GameLogic::STATUS_GAMEOVER) {
		this->fadingFactor += dt /  GameLogic::GAMEOVER_FADE_TIME;

		this->rotationAngle += dt * (GameLogic::ANGULAR_VELOCITY + 
								this->fadingFactor * GameLogic::GAMEOVER_ROTATION_ACCEL);
		this->audioEngine->SetMusicVolume(maxMusicVolume - this->fadingFactor * maxMusicVolume);
		
		if(this->fadingFactor > 1.0f) { // end of the fading out of game over. The game is REALLY OVER now
			this->fadingFactor = 1.0f;
			this->audioEngine->PauseMusic();
			this->gameStatus = STATUS_FINISHED;
		}
		
		return;
	}

	/* 
		Stage clear and get the emerald. Sonic speed is halved
		and all the spheres in the stage start floating up.
	*/

	if(this->gameStatus == GameLogic::STATUS_EMERALD) {

		// update sonic animation (half speed)
		this->SonicRunAnimationAdvance(dt / 2.0f, GameLogic::BASE_VELOCITY);

		this->position = this->position + this->direction * (GameLogic::BASE_VELOCITY / 2.0f * dt);
		this->worldItemsHeight += dt * GameLogic::EMERALD_ITEMS_VELOCITY;
		this->emeraldDistance -= GameLogic::EMERALD_VELOCITY * dt;

		if(this->emeraldDistance / this->EMERALD_VELOCITY  < 2.5f && emeraldSound) {
			this->audioEngine->PlayFX(AudioEngine::FX_EMERALD);
			this->audioEngine->SetMusicVolume(0.3f);
			if(this->goingBackward) {
				this->goingBackward = false;
				this->direction = this->direction * -1;
			}
			emeraldSound = false;
		}

		if(this->emeraldDistance <= 0.0f) {
			this->emeraldDistance = 0.0f;
			this->fadingFactor = 0.0f;
			this->audioEngine->PlayFX(AudioEngine::FX_GAMEOVER);
			this->gameStatus = GameLogic::STATUS_GAMEOVER;
			maxMusicVolume = 0.3f;
		}
		return;
	}

	// update sonic animation
	this->SonicRunAnimationAdvance(dt, this->velocity);

	// update game speed
	this->speedUpTimer += dt;
	if(this->speedUpTimer >= GameLogic::GAME_SPEEDUP_TIME) {
		this->speedUpTimer -= GameLogic::GAME_SPEEDUP_TIME;
		this->velocity += GameLogic::GAME_SPEEDUP_INCREMENT;
		if(this->velocity > GameLogic::MAX_VELOCITY)
			this->velocity = GameLogic::MAX_VELOCITY;
		else
			this->audioEngine->SetMusicTempo(this->audioEngine->GetMusicTempo() + GameLogic::GAME_MUSIC_TEMPO_SPEEDUP);
	}

	// update step vector
	currentPosition = this->position;
	vec2 step = this->direction * dt * this->velocity;
	

	if(rotating) { // While a rotation is being executed, position is constant
		
		this->PerformRotation(dt, rotationDirection, rotating, finalAngle);

	} else { // no rotation, so the character can be moved

		prevBounceDistance -= length(step);
		prevBackwardBlueSphereDistance -= length(step);

		if(prevBounceDistance < 0.0f)
			prevBounceDistance = 0.0f;
		
		if(prevBackwardBlueSphereDistance < 0.0f)
			prevBackwardBlueSphereDistance = 0.0f;

		/* Checking for "go forward" requests
			Only accepeted if the distance between the current position and the position
			of the star sphere that has been hit is greater that 1
		*/
		if(this->goingBackward && !jumping && this->runForward && prevBounceDistance == 0.0f) {
			prevRotationPosition = ivec2(-1, -1);
			this->direction = this->direction * -1;
			goingBackward = false;
		}

		// Checking for jump requests 
		if(this->jump && !jumping) {
			jumping = true;
			remainingJumpDistance = this->sonicJumpDistance;
			this->audioEngine->PlayFX(this->sonicJumpSoundEffect);
		}

		// Performing jump
		if(jumping) {
			
			static GLfloat deltaJump;

			step = step * velocityScale; // velocity is increased during jumps

			if(remainingJumpDistance > 0.0f) {

				remainingJumpDistance -= length(step);
				deltaJump = ( this->sonicJumpDistance - remainingJumpDistance ) / this->sonicJumpDistance;
				
				this->sonicRotationX = (this->goingBackward ? - 1.0f : 1.0f) * deltaJump 
					* this->sonicJumpDistance * GameLogic::SONIC_JUMP_REVOLUTIONS * 2.0 * Global::PI;
				this->sonicHeight = (1.0f - pow( deltaJump * 2.0f - 1.0f, 2.0f)) * this->sonicJumpHeight;
			}

			if(remainingJumpDistance <= 0.0f) {
				jumping = false;
				remainingJumpDistance = 0.0f;
				velocityScale = 1.0f;
				this->sonicJumpDistance = 0.0f;
				this->jump = false;
				this->sonicHeight = 0.0f;
				this->sonicRotationX = 0.0f;
			}
		}

		this->position = this->position + step;

		// Normalizing position in float range [0, worldWidth) X [0, worldDepth)
		if(this->position.x < 0)
			this->position.x = this->gameData->GetWorldWidth() + this->position.x;

		if(this->position.x >= this->gameData->GetWorldWidth())
			this->position.x = this->position.x - this->gameData->GetWorldWidth();

		if(this->position.y < 0)
			this->position.y = this->gameData->GetWorldDepth() + this->position.y;

		if(this->position.y >= this->gameData->GetWorldDepth())
			this->position.y = this->position.y - this->gameData->GetWorldDepth();
			

		// Checking if an edge has been crossed
		GLuint crossEdge = CheckCrossEdge(currentPosition, this->position);
		
		// Round the position to the nearest integer
		ivec2 roundedPosition = this->RoundNearestPosition(this->position);

		// Check collisions with star spheres (DING DING DING)
		if(gameData->GetValueAt(roundedPosition) == GameData::C_STARSPHERE && this->sonicHeight <= GameLogic::COLLIDE_MAX_HEIGHT) {

			// Getting the integer and fractional part of position (need only the fract for this)
			this->GetModPosition(intPos, fractPos);

			if(this->direction == GameLogic::DIRECTION_LEFT) {
				if(fractPos.x < GameLogic::SPHERE_RADIUS) {
					this->direction = GameLogic::DIRECTION_RIGHT;
					bounce = true;
				}
			} else if(this->direction == GameLogic::DIRECTION_RIGHT) {
				if(fractPos.x > (1.0f - GameLogic::SPHERE_RADIUS)) {
					this->direction = GameLogic::DIRECTION_LEFT;
					bounce = true;
				}
			} else if(this->direction == GameLogic::DIRECTION_UP) {
				if(fractPos.y < GameLogic::SPHERE_RADIUS) {
					this->direction = GameLogic::DIRECTION_DOWN;
					bounce = true;
				}		
			} else if(this->direction == GameLogic::DIRECTION_DOWN) {
				if(fractPos.y > (1.0f - GameLogic::SPHERE_RADIUS)) {
					this->direction = GameLogic::DIRECTION_UP;
					bounce = true;
				}
			}
		}

		/*	
			If the character bounced on a star sphere, I must allow him to rotate in
			the last position he came from, so i do a little trick and set te last rotation
			position with the current rounded position (which is where the star sphere was) 
		*/
		if(bounce) {
			prevRotationPosition = roundedPosition;
			this->audioEngine->PlayFX(AudioEngine::FX_BOUNCE);
			this->goingBackward = !this->goingBackward;
			this->runForward = false;
			bounce = false;
			prevBounceDistance = 1.0f;
		}
		
		/*	
			Checking for collisions with other world objects. If the previous rotation
			position is equal to actual position, it means that this collisions (if any)
			has been processed before rotating, so let's ignore it 
		*/

		if(crossEdge && !(roundedPosition == prevRotationPosition) && this->sonicHeight <= GameLogic::COLLIDE_MAX_HEIGHT) {

			if(this->gameData->GetValueAt(roundedPosition) == GameData::C_BLUESPHERE) {
				
				// transform blue sphere into red sphere
				this->gameData->SetValueAt(roundedPosition, GameData::C_REDSPHERE);
				this->audioEngine->PlayFX(AudioEngine::FX_BLUESPHERE);
				
				if(goingBackward)
					prevBackwardBlueSphereDistance = GameLogic::SPHERE_RADIUS * 6.0f;
				
				if(this->gameData->GetAvoidSearchAt(roundedPosition) == GameData::AS_NO) { // check if search must be avoided
					TransformRingsAlgorithm::CurrentGameData = this->gameData;
					TransformRingsAlgorithm::CurrentGoal = roundedPosition;
					vector<ivec2> * block = TransformRingsAlgorithm::Calculate();
					if(block != NULL) {
						for(int i = 0; i < block->size(); i++)
							gameData->SetValueAt(block->at(i), GameData::C_RING);

						// Collect the ring in the current position (if any)
						if(this->gameData->GetValueAt(roundedPosition) == GameData::C_RING) {
							this->gameData->SetValueAt(roundedPosition, GameData::C_NOTHING);
							this->gameData->IncrementCollectedRings();
							this->ringSpriteTimer.Reset();
							this->audioEngine->PlayFX(AudioEngine::FX_RING);
						}

						// Free some memory

						delete block;
					}
				}
				if(this->gameData->GetObjectCount(GameData::C_BLUESPHERE) == 0) {
					// No more blue spheres... Get the Emerald :)
					this->gameStatus = GameLogic::STATUS_EMERALD;
					this->worldItemsHeight = 0.0f;
					this->emeraldDistance = GameLogic::EMERALD_START_DISTANCE;
					this->audioEngine->PlayFX(AudioEngine::FX_SPLASH);
					return;
				}
			} else if(this->gameData->GetValueAt(roundedPosition) == GameData::C_YELLOWSPHERE) {
				/* A yellow sphere. Set up a jump request and return */
				velocityScale = 2.0f;
				this->jump = true;
				this->sonicJumpSoundEffect = AudioEngine::FX_TONG;
				this->sonicJumpDistance = GameLogic::YELLOW_SPHERE_JUMP_DISTANCE - length(step);
				this->sonicJumpHeight = GameLogic::YELLOW_SPHERE_JUMP_HEIGHT;
				jumping = false;
				return;
			} else if(this->gameData->GetValueAt(roundedPosition) == GameData::C_RING) { 
				// A ring! Let's take it!
				this->gameData->SetValueAt(roundedPosition, GameData::C_NOTHING);
				this->gameData->IncrementCollectedRings();
				this->ringSpriteTimer.Reset();
				this->audioEngine->PlayFX(AudioEngine::FX_RING);

				if(this->gameData->IsPerfect()) {
					this->audioEngine->PlayFX(AudioEngine::FX_PERFECT);
					this->FireInGameMessage("PERFECT");
				}
			} else if(this->gameData->GetValueAt(roundedPosition) == GameData::C_REDSPHERE) {
				
				if(prevBackwardBlueSphereDistance > 0.0f)
					return;

				// Game Over :(

				this->gameStatus = GameLogic::STATUS_GAMEOVER;
				this->fadingFactor = 0.0f;
				this->audioEngine->PlayFX(AudioEngine::FX_GAMEOVER);
				maxMusicVolume = 1.0f;
				
				return;
			}
		}

		if(this->rotate != ROTATE_NONE && !jumping) { // checking for rotation requests

			
			// if an edge has been crossed, check that the current position (rounded to nearest integer) is different from the previous rotation
			// position ( only one rotation per point is allowed ) (CI POTREBBE ESSERE UN BUG ANCHE SE E' IMPOSSIBILE CHE SI VERIFICHI LA SITUAZIONE)
			if(crossEdge && !(prevRotationPosition == roundedPosition)) {
				
				// update the rotation position with the current position (rounded to nearest integer)
				prevRotationPosition = roundedPosition;
				this->position = vec2((GLfloat)prevRotationPosition.x, (GLfloat)prevRotationPosition.y);

				// Calculate the new movement direction after rotation and the rotation angle to reach
				this->ServeRotation(finalAngle, rotationDirection);

				rotating = true;
			}
		}
	}
}

void GameLogic::ServeRotation(float& finalAngle, GLuint& rotationDirection) {
	// Calculate the new movement direction after rotation and the rotation angle to reach
	if(this->rotate == GameLogic::ROTATE_LEFT) {
		if(this->direction == GameLogic::DIRECTION_UP) this->direction = GameLogic::DIRECTION_LEFT;
		else if(this->direction == GameLogic::DIRECTION_LEFT) this->direction = GameLogic::DIRECTION_DOWN;
		else if(this->direction == GameLogic::DIRECTION_DOWN) this->direction = GameLogic::DIRECTION_RIGHT;
		else this->direction = GameLogic::DIRECTION_UP;
		finalAngle = this->rotationAngle - Global::PI/2.0f; // angle to reach with the rotation
		rotationDirection = ROTATE_LEFT;
	} else {
		if(this->direction == GameLogic::DIRECTION_UP) this->direction = GameLogic::DIRECTION_RIGHT;
		else if(this->direction == GameLogic::DIRECTION_LEFT) this->direction = GameLogic::DIRECTION_UP;
		else if(this->direction == GameLogic::DIRECTION_DOWN) this->direction = GameLogic::DIRECTION_LEFT;
		else this->direction = GameLogic::DIRECTION_DOWN;
		finalAngle = this->rotationAngle + Global::PI/2.0f; // angle to reach with the rotation
		rotationDirection = ROTATE_RIGHT;
	}

	this->rotate = GameLogic::ROTATE_NONE; // TEMP

}

void GameLogic::PerformRotation(float dt, GLuint& rotationDirection, bool& rotating, float& finalAngle) {
	if(rotationDirection == ROTATE_LEFT) {

		this->rotationAngle -= GameLogic::ANGULAR_VELOCITY * dt;

		if(this->rotationAngle < finalAngle) {
			this->rotationAngle = finalAngle;
			rotating = false;
			finalAngle = 0.0f;
			rotationDirection = ROTATE_NONE;
		}
	} else if(rotationDirection == ROTATE_RIGHT) {
			
		this->rotationAngle += GameLogic::ANGULAR_VELOCITY * dt;

		if(this->rotationAngle > finalAngle) {
			this->rotationAngle = finalAngle;
			rotating = false;
			finalAngle = 0.0f;
			rotationDirection = ROTATE_NONE;
		}
	}
}

SpriteTimer GameLogic::GetRingSpriteTimer() {
	return this->ringSpriteTimer;
}


/* Returns the position rounded to the nearest integer */
ivec2 GameLogic::GetRoundedPosition() {
	return this->RoundNearestPosition(this->position);
}

/*
	Returns the current position splitted in integer part 
	and fractional part
*/

void GameLogic::GetModPosition(vec2 &intPos, vec2 &fractPos) {
	fractPos.x = modf(this->position.x, &(intPos.x));
	fractPos.y = modf(this->position.y, &(intPos.y));
}

/*
	Returns the texture coordinate offset for the world floor
*/
vec2 GameLogic::GetTexturePosition() {
	vec2 result;
	
	result.x = (this->position.x - (GLint)(this->position.x / 2) * 2) / 2.0f;
	result.y = (this->position.y - (GLint)(this->position.y / 2) * 2) / 2.0f;

	return result;
}

/* 
	Sets up a rotation request. Rotation requests are only served
	when an edge has been crossed 
*/
void GameLogic::SetRotate(GLuint rotate) {
	this->rotate = rotate;
}

/* Sets up a jump request */
void GameLogic::SetJump() {
	if(!this->jump) {
		this->jump = true;
		this->sonicJumpDistance = GameLogic::SONIC_JUMP_DISTANCE;
		this->sonicJumpHeight = GameLogic::SONIC_JUMP_HEIGHT;
		this->sonicJumpSoundEffect = AudioEngine::FX_JUMP;
	}
}

/* Sets up a "run forward" request */
void GameLogic::SetRunForward() {
	if(this->goingBackward)
		this->runForward = true;
}

/* Returns the current rotation angle around the y axis */
GLfloat GameLogic::GetRotationAngle() {
	return this->rotationAngle;
}

/* Returns the current angle for rings rotation */
GLfloat GameLogic::GetRingsRotationAngle() {
	return this->ringsRotatationAngle;
}

GLuint GameLogic::GetSonicAnimationFrame() {
	return this->sonicRunAnimationFrame;
}

GLfloat GameLogic::GetSonicAnimationDelta() {
	static float timePerFrame = GameLogic::SONIC_RUN_ANIMATION_TIME / 12;
	return this->sonicRunAnimationFract / timePerFrame;
}

/* Given 2 world positions, checks if and edge has been crossed */
GLuint GameLogic::CheckCrossEdge(vec2 currentPosition, vec2 futurePosition) {
	if(this->direction == GameLogic::DIRECTION_LEFT || this->direction == GameLogic::DIRECTION_RIGHT) {
		if((int)currentPosition.x != (int)futurePosition.x)
			return GameLogic::CROSS_HORIZONTAL;
	} else {
		if((int)currentPosition.y != (int)futurePosition.y)
			return GameLogic::CROSS_VERTICAL;
	}

	return GameLogic::CROSS_NONE;
}

/* 
	Given a position vector v, this function rounds v to
	the nearest integer. The result is also normalized in range
	[0, worldWidth - 1] x [0, worldDepth - 1]
*/
ivec2 GameLogic::RoundNearestPosition(const vec2 &v) {
	GLfloat z;
	ivec2 result;

	if(modf(v.x, &z) > 0.5)
		result.x = (GLint)ceil(v.x) % this->gameData->GetWorldWidth();
	else
		result.x = (GLint)floor(v.x);

	if(modf(v.y, &z) > 0.5)
		result.y = (GLint)ceil(v.y) % this->gameData->GetWorldDepth();
	else
		result.y = (GLint)floor(v.y);
	
	return result;
}

ivec2 GameLogic::NormalizePosition(const ivec2& v) {
	return ivec2(v.x % this->gameData->GetWorldWidth(), v.y % this->gameData->GetWorldDepth());
}

void GameLogic::SonicRunAnimationAdvance(GLfloat dt, GLfloat velocity) {
	static float timePerFrame = GameLogic::SONIC_RUN_ANIMATION_TIME / 12;

	float deltaSpeed = (velocity / GameLogic::BASE_VELOCITY - 1.0f) / 2.0f;

	this->sonicRunAnimationFract += dt * (1.0f + deltaSpeed);

	while(sonicRunAnimationFract >= timePerFrame) {
		this->sonicRunAnimationFract -= timePerFrame;
		this->sonicRunAnimationFrame = (this->sonicRunAnimationFrame + 1) % 12;
	}
}

void GameLogic::AddGameLogicListener(GameLogicListener * listener) {
	if(listener != NULL)
		this->listeners.push_back(listener);
}

void GameLogic::FireInGameMessage(string message) {
	for(int i = 0; i < this->listeners.size(); i++) 
		this->listeners[i]->InGameMessage(message);
}

GameLogic::GameLogic(GameData * gameData, AudioEngine * audioEngine)
{
	this->audioEngine = audioEngine;
	this->gameData = gameData;
	this->position = vec2(0.0f, 0.0f);
	this->direction = GameLogic::DIRECTION_UP;
	this->rotate = GameLogic::ROTATE_NONE;
	this->rotationAngle = 0.0f;
	this->ringsRotatationAngle = 0.0f;
	this->velocity = GameLogic::BASE_VELOCITY;
	this->gameStatus = GameLogic::STATUS_NONE;
	this->speedUpTimer = 0.0f;
	this->worldItemsHeight = 0.0f;
	this->emeraldDistance = -1.0f;
	this->listeners = vector<GameLogicListener*>();
	this->ringSpriteTimer = SpriteTimer();
	this->ringSpriteTimer.SetLifeTime(GameLogic::RING_SPRITE_LIFETIME);
	this->ringSpriteTimer.SetTime(GameLogic::RING_SPRITE_LIFETIME);
	this->jump = false;
	this->jumping = false;
	this->goingBackward = false;
	this->sonicJumpSoundEffect = 0;
	this->sonicRotationX = 0.0f;
	this->sonicHeight = 0.0f;
	this->sonicJumpHeight = 0.0f;
	this->sonicJumpDistance = 0.0f;
	this->sonicRunAnimationFrame = 3;
	this->sonicRunAnimationFract = 0.0f;

}


GameLogic::~GameLogic(void)
{
}

// -------------------- TRANSFORM RINGS ALGORITHM ---------------------------------

ivec2 TransformRingsAlgorithm::CurrentGoal;
GameData * TransformRingsAlgorithm::CurrentGameData;

vector<ivec2>* TransformRingsAlgorithm::Calculate() {
	static ivec2 directions[8] = {
		GameLogic::DIRECTION_LEFT,
		GameLogic::DIRECTION_RIGHT,
		GameLogic::DIRECTION_UP,
		GameLogic::DIRECTION_DOWN,
		GameLogic::DIRECTION_UP_LEFT,
		GameLogic::DIRECTION_DOWN_LEFT,
		GameLogic::DIRECTION_UP_RIGHT,
		GameLogic::DIRECTION_DOWN_RIGHT
	};

	TransformRingsState* initial = new TransformRingsState();
	vector<TransformRingsState*> states = vector<TransformRingsState*>(); // States to evaluate
	vector<TransformRingsState*> evaluated = vector<TransformRingsState*>(); // States evaluated
	vector<ivec2> nearBlueSpheres = vector<ivec2>();


	// Blue spheres near the goal (all directions)
	for(int i = 0; i < 8; i++) {
		ivec2 pos = directions[i] + CurrentGoal;
		if(CurrentGameData->GetValueAt(pos) == GameData::C_BLUESPHERE)
			nearBlueSpheres.push_back(pos);
	}

	// No near blue spheres (return)
	if(nearBlueSpheres.empty())
		return NULL;

	initial->GetPath()->push_back(CurrentGoal);

	TransformRingsAlgorithm::EvaluateState(initial, &states, &evaluated);

	delete initial;

	while(!states.empty()) {
		TransformRingsState * front = states.front();

		evaluated.push_back(front);
		states.erase(states.begin());

		if(TransformRingsState::GoalTest(*front, CurrentGoal)) { // A closed path has been found

			bool found = false;

			vector<ivec2>* result = new vector<ivec2>();

			for(int i = 0; i < nearBlueSpheres.size(); i++) { // Checking if any of the blue sphere near the goal is contained in the found path
				if(!Global::Contains<ivec2>(*result, nearBlueSpheres[i]) && 
					TransformRingsAlgorithm::IsInsidePath(nearBlueSpheres[i], front->GetPath())) { // Blue sphere is inside path -> TransformRings
					
					found = true;

					TransformRingsAlgorithm::FindTransformBlock(nearBlueSpheres[i], result);

				}
			}

			if(found) { // a block to transform has been found

				// Free some memory

				for(int i = 0; i < states.size(); i++)
					delete states[i];

				for(int i = 0; i < evaluated.size(); i++)
					delete evaluated[i];

				return result;
			} else { // No adjacent blue spheres is included in the path -> continue search

				// Free some memory
				delete result;
			}


		} else {
			TransformRingsAlgorithm::EvaluateState(front, &states, &evaluated);
			sort(states.begin(), states.end(), TransformRingsAlgorithm::EuclideanDistanceComparator);
		}
	}

	// Free some memory

	for(int i = 0; i < states.size(); i++)
		delete states[i];

	for(int i = 0; i < evaluated.size(); i++)
		delete evaluated[i];

	// No solution found
	return NULL;
}

void TransformRingsAlgorithm::FindTransformBlock(const ivec2& currentPosition, vector<ivec2> * block) {
	static ivec2 directions[8] = {
		GameLogic::DIRECTION_LEFT,
		GameLogic::DIRECTION_RIGHT,
		GameLogic::DIRECTION_UP,
		GameLogic::DIRECTION_DOWN,
		GameLogic::DIRECTION_UP_LEFT,
		GameLogic::DIRECTION_DOWN_LEFT,
		GameLogic::DIRECTION_UP_RIGHT,
		GameLogic::DIRECTION_DOWN_RIGHT
	};

	// Find blue and red spheres in all directions starting from currentPosition, and recursively fill the vector

	GLuint val = CurrentGameData->GetValueAt(currentPosition);

	if(val == GameData::C_REDSPHERE) 
		block->push_back(currentPosition); // Add the redsphere to the block
	else if(val == GameData::C_BLUESPHERE) {
		block->push_back(currentPosition); // Add the blue sphere to the block

		// Function recursion
		for(int i = 0; i < 8; i++) {

			ivec2 pos = currentPosition + directions[i];

			bool duplicate = Global::Contains<ivec2>(*block, pos);

/*			for(int i = 0; i < block->size(); i++) // Search is the element is already in the block (to prevent infinite recursions)
				if(block->at(i) == pos) {
					duplicate = true;
					break;
				}*/

			if(duplicate)
				continue;

			TransformRingsAlgorithm::FindTransformBlock(pos, block);
		}
	}
}

void TransformRingsAlgorithm::EvaluateState(TransformRingsState * state, vector<TransformRingsState*>* states, vector<TransformRingsState*>* evaluated) {
	static ivec2 directions[4] = {
		GameLogic::DIRECTION_LEFT,
		GameLogic::DIRECTION_RIGHT,
		GameLogic::DIRECTION_UP,
		GameLogic::DIRECTION_DOWN
	};

	for(int i = 0; i < 4; i++) {
		TransformRingsState * eval = TransformRingsState::Go(*state, directions[i], CurrentGoal, CurrentGameData);
		if(eval != NULL) {
			bool duplicate = false;

			for(int k = 0; k < evaluated->size(); k++)
				if(*(evaluated->at(k)) == *eval) {
					duplicate = true;
					break;
				}
			
			if(duplicate)
				continue;

			states->push_back(eval);
		}
	}

	return;
}

bool TransformRingsAlgorithm::EuclideanDistanceComparator(TransformRingsState * v1, TransformRingsState * v2) {
	ivec2 d1 = v1->GetPath()->back() - CurrentGoal;
	ivec2 d2 = v2->GetPath()->back() - CurrentGoal;
	return length(d1) < length(d2);
}

bool TransformRingsAlgorithm::IsInsidePath(ivec2 position, vector<ivec2> * path) {
	
	bool ok;

	//Check x (left)

	ok = false;

	for(int i = 0; i < path->size(); i++) {
		if(path->at(i).y == position.y && path->at(i).x < position.x) {
			ok = true;
			break;
		}
	}

	if(!ok)
		return false;

	// Check x (right)

	ok = false;

	for(int i = 0; i < path->size(); i++) {
		if(path->at(i).y == position.y && path->at(i).x > position.x) {
			ok = true;
			break;
		}
	}

	if(!ok)
		return false;

	// Check y (up)

	ok = false;

	for(int i = 0; i < path->size(); i++) {
		if(path->at(i).x == position.x && path->at(i).y < position.y) {
			ok = true;
			break;
		}
	}

	if(!ok)
		return false;

	// Check y (down)

	ok = false;

	for(int i = 0; i < path->size(); i++) {
		if(path->at(i).x == position.x && path->at(i).y > position.y) {
			ok = true;
			break;
		}
	}

	if(!ok)
		return false;

	return true;

}

TransformRingsState* TransformRingsState::Go(const TransformRingsState& current, ivec2 direction, ivec2 goal, GameData * gameData) {

	static ivec2 directions[8] = {
		GameLogic::DIRECTION_LEFT,
		GameLogic::DIRECTION_RIGHT,
		GameLogic::DIRECTION_UP,
		GameLogic::DIRECTION_DOWN,
		GameLogic::DIRECTION_UP_LEFT,
		GameLogic::DIRECTION_DOWN_LEFT,
		GameLogic::DIRECTION_UP_RIGHT,
		GameLogic::DIRECTION_DOWN_RIGHT
	};

	ivec2 nextPosition = current.path.back() + direction;
	GLuint val = gameData->GetValueAt(nextPosition);

	if(val != GameData::C_REDSPHERE || nextPosition == current.cantgo)
		return NULL;
	else {

		bool loopingPath = false;
		int redSpheresSurround =0;

		if(gameData->GetAvoidSearchAt(nextPosition) == GameData::AS_YES)
			return NULL;

		for(int i = 0; i < 8; i++) {
			if(gameData->GetValueAt(nextPosition + directions[i]) == GameData::C_REDSPHERE)
				redSpheresSurround++;
		}

		if(redSpheresSurround == 8)
			return NULL;

		for(int i = 0; i < current.path.size(); i++) {
			if(gameData->Normalize(current.path[i]) == gameData->Normalize(nextPosition) && !(gameData->Normalize(nextPosition) == goal)) {
				loopingPath = true;
				break;
			}
		}
		
		if(loopingPath)
			return NULL;

		TransformRingsState* result = new TransformRingsState();

		result->path = current.path;
		result->cantgo = current.path.back();
		result->path.push_back(nextPosition);

		return result;
	}
}


vector<ivec2>* TransformRingsState::GetPath() {
	return &(this->path);
}

bool TransformRingsState::operator==(const TransformRingsState& other) {
	if(this->path.size() != other.path.size())
		return false;
	else {
		bool equal = true;

		for(int i = 0; i < this->path.size(); i++)
			equal = equal & this->path[i] == other.path[i];

		return equal;
	}
}

bool TransformRingsState::GoalTest(const TransformRingsState& current, ivec2 goal) {
	static ivec2 directions[4] = {
		GameLogic::DIRECTION_LEFT,
		GameLogic::DIRECTION_RIGHT,
		GameLogic::DIRECTION_UP,
		GameLogic::DIRECTION_DOWN
	};

	for(int i = 0; i < 4; i++) {
		if(current.path.back() == goal)
			return true;
	}
	
	return false;
}