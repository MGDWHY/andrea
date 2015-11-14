#include "RandomStageGenerator.h"

const int RandomStageGenerator::LEVEL_MIN = 1;
const int RandomStageGenerator::LEVEL_MAX = 134217728;

int * RandomStageGenerator::LevelToPmn(int level) {
	int * pmn = new int[4];

	pmn[0] = (level - 1) % 128;
	pmn[1] = (1 + ((level - 1) % 127) * 3) % 127;
	pmn[2] = (2 + ((level - 1) % 126) * 5) % 126;
	pmn[3] = (3 + ((level - 1) % 125) * 7) % 125;

	return pmn;
}

RandomStageGenerator::RandomStageGenerator(void){}

RandomStageGenerator::~RandomStageGenerator(void){}
