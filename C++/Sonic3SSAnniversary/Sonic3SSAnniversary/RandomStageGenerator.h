#pragma once

#include "GameData.h"

class RandomStageGenerator
{

	RandomStageGenerator(void);

	static int * LevelToPmn(int level);

public:
	
	static const int LEVEL_MIN;
	static const int LEVEL_MAX;

	static GameData * Generate();


	~RandomStageGenerator(void);
};

