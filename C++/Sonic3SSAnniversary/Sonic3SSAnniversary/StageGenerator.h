#pragma once

#include "vecmath.h"
#include "Logger.h"
#include "GameLogic.h"
#include "GameData.h"
#include "Global.h"

using namespace Vecmath;

class StageGenerator
{
private:
	StageGenerator(void);
public:

	static const string BUMPMAPS[1];
	static const vec3 EMERALDS[8];
	static const vec3 PATTERNS[32];

	static const ivec2 START_POSITION;
	static const ivec2 START_DIRECTION;

	static const int MIN_STAGE;
	static const int MAX_STAGE;

	~StageGenerator(void);

	static GameData * GetStage(__int64 code);
	static __int64 GetStageCode(int number);
	static int GetStageNumber(__int64 code);
	
};

