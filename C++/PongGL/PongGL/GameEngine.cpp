#include <iostream>
#include "GameEngine.h"
#include "Randomizer.h"

GameEngine::GameEngine(GLfloat fieldWidth, GLfloat fieldLength, GLfloat padsWidth, GLfloat ballRadius)
{
	Randomizer::init();

	this->cpuOpponent = false;
	
	this->fieldWidth = fieldWidth;
	this->fieldLength = fieldLength;
	this->padsWidth = padsWidth;
	this->ballRadius = ballRadius;

	this->reset();
}

GameEngine::GameEngine(GLfloat fieldWidth, GLfloat fieldLength, GLfloat padsWidth, GLfloat ballRadius, bool cpuOpponent)
{
	Randomizer::init();

	this->cpuOpponent = cpuOpponent;
	
	this->fieldWidth = fieldWidth;
	this->fieldLength = fieldLength;
	this->padsWidth = padsWidth;
	this->ballRadius = ballRadius;

	this->reset();
}

GameEngine::~GameEngine(void)
{
}

void GameEngine::reset()
{ 
	pX = pY = vX = vY = pad1VelX = pad2VelX = 0;
	pad1X = pad2X = 0;

}

void GameEngine::throwBall() {
	GLfloat *angle = Randomizer::nextTrigonometricAngle();

	vX = angle[1]*8;
	vY = angle[0]*8;
}

void GameEngine::update(GLfloat dt) {

	// controlling cpu opponent
	if(cpuOpponent)
		controlCPUOpponent();

	// updating ball position
	pX += vX * dt;
	pY += vY * dt;

	// updating pads position
	pad1X += pad1VelX * dt;
	pad2X += pad2VelX * dt;

	// checking pad1 bounds
	if(pad1X < -fieldWidth / 2 + padsWidth / 2)
		pad1X = -fieldWidth / 2 + padsWidth / 2;
	else if(pad1X > fieldWidth / 2 - padsWidth / 2)
		pad1X = fieldWidth /2 - padsWidth / 2;
	
	// cheking pad2 bounds
	if(pad2X < -fieldWidth / 2 + padsWidth / 2)
		pad2X = -fieldWidth / 2 + padsWidth / 2;
	else if(pad2X > fieldWidth / 2 - padsWidth / 2)
		pad2X = fieldWidth /2 - padsWidth / 2;

	// checking ball collisions on walls
	if(pX < -fieldWidth/2 + ballRadius) {
		pX = -fieldWidth/2 + ballRadius;
		vX = -vX;
	} else if(pX > fieldWidth / 2 - ballRadius) {
		pX = fieldWidth/2 - ballRadius;
		vX = -vX;
	}


	// checking ball collisions on pads
	if(pY < - fieldLength / 2 + ballRadius && pY > - fieldLength / 2) {
		if(pad2X - padsWidth / 2 <= pX && pad2X + padsWidth / 2 >= pX) {
			pY = - fieldLength /2 + ballRadius;
			float ratio = (pX - pad2X + padsWidth/2)/padsWidth - 0.5f;
			vY =  -vY / 4.0f + (1.0f - abs(ratio)) * 8.0f;
			vX = ratio * 8.0f;
		}
	}

	if(pY > fieldLength / 2 - ballRadius && pY < fieldLength / 2) {
		if(pad1X - padsWidth / 2 <= pX && pad1X + padsWidth / 2 >= pX) {
			pY = fieldLength /2 - ballRadius;
			float ratio = (pX - pad1X + padsWidth/2)/padsWidth - 0.5f;
			vY = - vY / 4.0f - (1.0f - abs(ratio)) * 8.0f;
			vX = ratio * 8.0f;
		}
	}
}

void GameEngine::controlCPUOpponent() {
	if(pY > 0 || vY > 0) {
		if(pad2X > padsWidth/2)
			setPad2VelX(-4.0);
		else if(pad2X < -padsWidth /2)
			setPad2VelX(4.0);
		else
			setPad2VelX(0);
	} else {
		if(pX > pad2X)
			setPad2VelX(4.0);
		else if(pX < pad2X)
			setPad2VelX(-4.0);
		else
			setPad2VelX(0);
	}
}