#pragma once

#include <string>

using namespace std;

class GameLogicListener
{
public:

	virtual void InGameMessage(string message) = 0;

	GameLogicListener(void);
	~GameLogicListener(void);
};

