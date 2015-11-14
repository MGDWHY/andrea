#include "StageGenerator.h"

const int StageGenerator::MIN_STAGE = 1;
const int StageGenerator::MAX_STAGE = 134217728;

const vec3 StageGenerator::EMERALDS[8] = { vec3(0.0, 1.0, 0.0),
											vec3(1.0, 0.0, 0.0),
											vec3(1.0, 0.0, 1.0),
											vec3(0.0, 0.0, 1.0),
											vec3(0.7, 0.7, 0.7),
											vec3(1.0, 0.0, 0.0),
											vec3(0.5, 0.5, 1.0),
											vec3(1.0, 1.0, 0.0)
										};

const vec3 StageGenerator::PATTERNS[32] = {
	vec3(224, 128, 0), vec3(96, 32, 0),
	vec3(32, 96, 0), vec3(0, 224, 160),
	vec3(224, 96, 0), vec3(224, 224, 160),
	vec3(224, 224, 224), vec3(192, 0, 0),
	vec3(64, 32, 64), vec3(224, 128, 0),
	vec3(0, 64, 192), vec3(160, 224, 224),
	vec3(96, 32, 160), vec3(224, 192, 0),
	vec3(0, 0, 160), vec3(64, 160, 0),
	vec3(32, 224, 0), vec3(192, 224, 128),
	vec3(224, 96, 0), vec3(128, 224, 192),
	vec3(192, 0, 160), vec3(160, 192, 0),
	vec3(0, 0, 192), vec3(192, 160, 0),
	vec3(128, 0, 224), vec3(32, 224, 160),
	vec3(160, 224, 0), vec3(96, 0, 224),
	vec3(0, 192, 224), vec3(192, 32, 160),
	vec3(0, 0, 192), vec3(192, 192, 192)
};

const string StageGenerator::BUMPMAPS[1] = { "normal6.png" };

const ivec2 StageGenerator::START_POSITION = ivec2(28, 16);
const ivec2 StageGenerator::START_DIRECTION = GameLogic::DIRECTION_UP;

GameData * StageGenerator::GetStage(__int64 code) {
	int stageNumber = GetStageNumber(code);

	/* If the code is not valid, let's return NULL */
	if(stageNumber == -1)
		return NULL;

	/* Calculating sections */

	int topRight = (stageNumber - 1) % 128;
	int bottomRight = (1 + ((stageNumber - 1) % 127) * 3) % 127;
	int topLeft = (2 + ((stageNumber - 1) % 126) * 5) % 126;
	int bottomLeft = (3 + ((stageNumber - 1) % 125) * 7) % 125;

	GameData * result = GameData::FromSections(topLeft, topRight, bottomLeft, bottomRight);

	result->SetFloorRenderingMode(GameData::RM_CHECKERBOARD);
	result->SetBumpMappingEnabled(GameData::AS_YES);
	result->SetCheckerBoardColors(0, StageGenerator::PATTERNS[(topLeft % 16) * 2] * ( 1 / 255.0f));
	result->SetCheckerBoardColors(1, StageGenerator::PATTERNS[(topLeft % 16) * 2 + 1] * (1 / 255.0f));
	result->SetSkyBackColor(0, vec3(0, 23, 178) * ( 1 / 255.0f));
	result->SetSkyBackColor(1, vec3(84, 176, 255) * ( 1 / 255.0f));
	result->SetStarsColor(0, vec3(255, 255, 255) * ( 1 / 255.0f));
	result->SetStarsColor(1, vec3(255, 255, 128) * ( 1 / 255.0f));
	result->SetEmeraldColor(StageGenerator::EMERALDS[topRight % 8]);
	result->SetBumpMup(StageGenerator::BUMPMAPS[topRight % 1]);
	result->SetStartDirection(StageGenerator::START_DIRECTION);
	result->SetStartPoint(StageGenerator::START_POSITION);

	return result;
}

int StageGenerator::GetStageNumber(__int64 code) {
	int stage = 0;
	__int64 c = code;
	int ca[39], cb[28];

	/* First bit of the code is always 1 */

	ca[38] = 1;

	/* Get the other bits from the code*/
	for(int i = 0; i < 38; i++) {
		ca[i] = c % 2;
		c /= 2;
	}

	/* getting the original ca bin inverting odd bits */
	for (int i = 1; i < 38; i = i + 2)
		ca[i] = 1 - ca[i];

	/* This bit is always 0 (it seems) */
	cb[27] = 0;

	/* Getting the level number (binary) from the code */
	for (int i = 0; i < 6 ; i++)
		cb[i] = ca[i + 26];
	
	for (int i = 6; i < 27; i++)
		cb[i] = ca[i - 6];

	/* Calculate the level number in decimal form */
	for(int i = 0; i < 28; i++)
		stage += cb[i] * (int)pow((double)2, i);

	/* The true level number is found by subtracting 19088742 from the result */
	stage -= 19088742;

	/* If the stage is <= 0, we add the max_stage (cyclic stages) */
	if(stage <= 0)
		stage += StageGenerator::MAX_STAGE;

	/* Checking that the given code is fully correct. If not, return -1 (invalid code) */
	if(GetStageCode(stage) != code)
		return -1;
	else
		return stage;
}

__int64 StageGenerator::GetStageCode(int number) {

	__int64 result = 0;

	#ifdef GL_UTILS_LOG_ENABLED
		if(number < StageGenerator::MIN_STAGE || number > StageGenerator::MAX_STAGE)
			Logger::getDefaultLogger()->writeMessage(1, "StageGenerator::GetStageCode()", "Invalid level code");
	#endif

	// cb -> stage number (binary form)
	// ca -> code (binary form)

	int ca[39], cb[28], tmp;
	
	/* Zeroing code */
	for(int i = 0; i < 39; i++)
		ca[i] = 0;

	/* The first bit of the code is always 1 */
	ca[38] = 1;

	/* Before calculating the binary form, stage number is increased by 19088742 */
	tmp = (number + 19088742);

	/* Calculating stage number in binary form */
	for(int i = 0; i < 28; i++) {
		cb[i] = tmp % 2;
		tmp /= 2;
	}

	/* Calculate code in binary form (note that some values (ca) are missing here) */

	for (int i = 0; i <= 5 ; i++)
		ca[i+26] = cb[i];
	for (int i = 6; i <= 26; i++)
		ca[i-6] = cb[i];

	/* Stage is decreased by 1 and then the binary form is recalculated */
	tmp = number - 1;

	for (int i = 0; i <= 27; i++)
	{
		cb[i] = tmp % 2;
		tmp /= 2;
	}

	/* The missing code values are filled with pseudo-random data generated from the stage number decreased by 1 */

	ca[37] = ( 1 + cb[6] + cb[23] ) % 2;
	ca[36] = ( 1 + cb[5] + cb[22] + cb[17] + cb[0] ) % 2;
	ca[35] = ( 1 + cb[4] + cb[21] + cb[16] ) % 2;
	ca[34] = ( 1 + cb[3] + cb[20] ) % 2;
	ca[33] = ( 1 + cb[2] + cb[19] ) % 2;
	ca[32] = ( 1 + cb[1] + cb[18] ) % 2;

	ca[25] = ( 0 + cb[11] + cb[16] ) % 2;
	ca[24] = ( 0 + cb[10] ) % 2;
	ca[23] = ( 0 + cb[9] + cb[26] ) % 2;
	ca[22] = ( 0 + cb[8] + cb[25] ) % 2;
	ca[21] = ( 0 + cb[7] + cb[24] ) % 2;

	/* Odd bits (1, 3, 5, ..., 37) of the code number are inverted */

	for (int i = 1; i < 38; i = i+2)
		ca[i] = 1 - ca[i];

	/* The code is calcutaled in decimal form */
	for (int i = 38; i >= 0; i--)
		result += ca[i] * (__int64)pow((double)2, i);

	return result;

}

StageGenerator::StageGenerator(void)
{
}


StageGenerator::~StageGenerator(void)
{
}



