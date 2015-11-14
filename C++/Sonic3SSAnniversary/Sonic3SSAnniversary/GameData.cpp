#include "GameData.h"

#include "Logger.h"


const GLuint GameData::C_NOTHING = 0;
const GLuint GameData::C_REDSPHERE = 1;
const GLuint GameData::C_BLUESPHERE = 2;
const GLuint GameData::C_YELLOWSPHERE = 5;
const GLuint GameData::C_STARSPHERE = 3;
const GLuint GameData::C_RING = 4;

const GLbyte GameData::RM_CHECKERBOARD = 0;
const GLbyte GameData::RM_TEXTURE = 1;

const GLbyte GameData::AS_NO = 0; 
const GLbyte GameData::AS_YES = 1;

StageSection * GameData::stageSections;
GLuint GameData::numSections;

StageSection StageSection::Flip(bool horizontal) {
	StageSection result;

	result.MaxRings = this->MaxRings;

	if(horizontal) 
	{
		for(int z = 0; z < 16; z++)
			for(int x = 0; x < 16; x++) {
				result.SetValueAt(x, z, this->GetValueAt(15 - x, z)); 
				result.SetAvoidSearchAt(x, z, this->GetAvoidSearchAt(15 - x, z));
			}
	} else 
	{
		for(int z = 0; z < 16; z++)
			for(int x = 0; x < 16; x++) {
				result.SetValueAt(x, z, this->GetValueAt(x, 15 - z));
				result.SetAvoidSearchAt(x, z, this->GetAvoidSearchAt(x, 15 - z));
			}
	}

	return result;
}

GLbyte StageSection::GetAvoidSearchAt(GLint x, GLint z) {
	return this->AvoidSearch[z * 16 + x];
}

void StageSection::SetAvoidSearchAt(GLint x, GLint z, GLbyte value) {
	this->AvoidSearch[z * 16 + x] = value;
}

GLuint StageSection::GetValueAt(GLint x, GLint z) {
	return this->Data[z * 16 + x];
}

void StageSection::SetValueAt(GLint x, GLint z, GLuint value) {
	this->Data[z * 16 + x] = value;
}

StageSection::StageSection() {
	this->MaxRings = 0;
	for(int i = 0; i < 256; i++)
		this->Data[i] = 0;
}



GameData::GameData(GLuint width, GLuint depth)
{
	this->version = 200;
	this->width = width;
	this->depth = depth;
	this->elementsCount = width * depth;
	this->data = new GLuint[this->elementsCount];
	this->avoidSearch = new GLbyte[this->elementsCount];
	this->floorRenderingMode = GameData::RM_CHECKERBOARD;
	this->bumpMappingEnabled = GameData::AS_YES;
	this->startDirection = ivec2();
	this->startPoint = ivec2();
	this->emeraldColor = vec3();
	this->checkerColors[0] = this->checkerColors[1] = vec3();
	this->skyBackcolors[0] = this->skyBackcolors[1] = vec3();
	this->starsColors[0] = this->starsColors[1] = vec3();
	this->objectCount = new GLuint[6];
	this->maxRings = 0;
	this->collectedRings = 0;
	this->texture = string();
	this->bumpMap = string();

	for(int i = 0; i < this->elementsCount; i++) 
	{
		this->data[i] = GameData::C_NOTHING;
		this->avoidSearch[i] = GameData::AS_NO;
	}

	for(int i = 0; i < 6; i++) 
		this->objectCount[i] = 0;

	#ifdef GL_UTILS_LOG_ENABLED
		if(width % 2 != 0 || depth % 2 != 0)
			Logger::getDefaultLogger()->writeMessage(1, "GameData::GameData():", "Width and depth should be even numbers");
	#endif
}


GameData::~GameData(void)
{
	delete[] this->data;
	delete[] this->objectCount;
}

GLuint GameData::GetObjectCount(GLuint objectType) {
	return this->objectCount[objectType];
}

void GameData::SetAvoidSearchAt(GLint x, GLint z, GLbyte value) {
	this->avoidSearch[this->NormalizeZ(z) * this->width + this->NormalizeX(x)] = value;
}

GLbyte GameData::GetAvoidSearchAt(GLint x, GLint z) {
	return this->avoidSearch[this->NormalizeZ(z) * this->width + this->NormalizeX(x)];
}

GLbyte GameData::GetAvoidSearchAt(const ivec2& position) {
	return this->GetAvoidSearchAt(position.x, position.y);
}


GLuint GameData::GetValueAt(const ivec2 &position) {
	return this->GetValueAt(position.x, position.y);
}

GLuint GameData::GetValueAt(GLint x, GLint z) {
	return this->data[this->NormalizeZ(z) * this->width + this->NormalizeX(x)];
}

void GameData::SetValueAt(GLint x, GLint z, GLuint value) {
	
	GLuint prevValue = this->GetValueAt(x, z);

	this->objectCount[prevValue]--;
	this->objectCount[value]++;

	this->data[this->NormalizeZ(z) * this->width + this->NormalizeX(x)] = value;
}

void GameData::SetValueAt(const ivec2 &position, GLuint value) {
	this->SetValueAt(position.x, position.y, value);
}

GLuint * GameData::GetData() {
	GLuint * dataCopy = new GLuint[this->elementsCount];

	for(unsigned int i = 0; i < this->elementsCount; i++)
		dataCopy[i] = this->data[i];
	
	return dataCopy;
}

void GameData::SetData(GLuint * newData) {

	for(int i = 0; i < this->elementsCount; i++)
		this->data[i] = newData[i];

	this->Update();

}

GLuint * GameData::GetRoundSubData(GLint minX, GLint maxX, GLint minZ, GLint maxZ) {
	GLuint * subData = new GLuint[(maxZ - minZ + 1) * (maxX - minX + 1)];
	
	for(GLint j = minZ; j <= maxZ; j++)
		for(GLint i = minX; i <= maxX; i++) {
			
			GLint ni = this->NormalizeX(i);
			GLint nj = this->NormalizeZ(j);

			subData[(j - minZ) * (maxX - minX + 1) + i - minX] = this->data[nj * this->width + ni];

		}

	return subData;
}


GLuint * GameData::GetRoundSubData(GLint x, GLint z, GLint radius) {

	GLuint * subData = new GLuint[(2 * radius + 1) * (2 * radius + 1)];
	GLuint ni, nj;

	for(GLint j = z - radius; j <= z + radius; j++)
		for(GLint i = x - radius; i <= x + radius; i++) 
		{

			ni = this->NormalizeX(i);
			nj = this->NormalizeZ(j);

			subData[(j + radius - z) * (2 * radius + 1) + i + radius - x] = this->data[nj * this->width + ni];

		}

		return subData;
}

void GameData::SetFloorRenderingMode(GLbyte mode) { this->floorRenderingMode = mode; }
GLbyte GameData::GetFloorRenderingMode()  { return this->floorRenderingMode; }

void GameData::SetBumpMappingEnabled(GLbyte enabled) { this->bumpMappingEnabled = enabled; }
GLbyte GameData::IsBumpMappingEnabled() { return this->bumpMappingEnabled; }


void GameData::SetTexture(string texture) { this->texture = texture; }
void GameData::SetBumpMup(string bumpMap) { this->bumpMap = bumpMap; }

string GameData::GetTexture() { return this->texture; }
string GameData::GetBumpMap() { return this->bumpMap; }

void GameData::SetMaxRings(GLuint maxRings) { this->maxRings = maxRings; }
GLuint GameData::GetMaxRings() { return this->maxRings; }

void GameData::SetCollectedRings(GLint collectedRings) { this->collectedRings = collectedRings; }
void GameData::IncrementCollectedRings() { this->collectedRings++; }
GLint GameData::GetCollectedRings() { return this->collectedRings; }
bool GameData::IsPerfect() { return this->collectedRings == this->maxRings; }
bool GameData::IsClear() { return this->objectCount[GameData::C_BLUESPHERE] == 0; }

void GameData::SetStartPoint(const ivec2 &startPoint) { this->startPoint = startPoint; }
ivec2 GameData::GetStartPoint() { return this->startPoint; }

void GameData::SetStartDirection(const ivec2 &startDirection) { this->startDirection = startDirection; }
ivec2 GameData::GetStartDirection() { return startDirection; }

void GameData::SetEmeraldColor(const vec3 &color) { this->emeraldColor = color; }
vec3 GameData::GetEmeraldColor() { return this->emeraldColor; }

void GameData::SetCheckerBoardColors(int index, vec3 color) { this->checkerColors[index] = color; }
vec3 GameData::GetCheckerBoardColors(int index) { return this->checkerColors[index]; }

void GameData::SetSkyBackColor(int index, vec3 color) { this->skyBackcolors[index] = color; }
vec3 GameData::GetSkyBackColor(int index) { return this->skyBackcolors[index]; }

void GameData::SetStarsColor(int index, vec3 color) { this->starsColors[index] = color; }
vec3 GameData::GetStarsColor(int index) { return this->starsColors[index]; }

void GameData::Update() {

	for(int i = 0; i < 5; i++)
		this->objectCount[i] = 0;

	for(GLint j = 0; j < this->depth; j++)
		for(GLint i = 0; i < this->width; i++) 
			this->objectCount[this->GetValueAt(i, j)]++;
}

GLuint GameData::GetWorldWidth() {
	return this->width;
}
GLuint GameData::GetWorldDepth() {
	return this->depth;
}

ivec2 GameData::Normalize(const ivec2& position) {
	return ivec2(this->NormalizeX(position.x), this->NormalizeZ(position.y));
}

GLuint GameData::NormalizeX(GLint x) {
	return GameData::Normalize(x, this->width);
}
GLuint GameData::NormalizeZ(GLint z) {
	return GameData::Normalize(z, this->depth);
}

GLuint GameData::Normalize(GLint val, GLint max) {
	if(val >= 0)
		return val % max;
	else
		return max + val % max;
}

void GameData::Initialize() {
	ifstream is;

	is.open(Global::DATA_DIR + "/sections.dat", ios_base::binary);

	#ifdef GL_UTILS_LOG_ENABLED
		if(!is.good())
			Logger::getDefaultLogger()->writeMessage(2, "GameData::Initialize()", "Can't open file");
	#endif
		
	
	GameData::ReadUInt(is, GameData::numSections);
	
	GameData::stageSections = new StageSection[GameData::numSections];

	for(int i = 0; i < GameData::numSections; i++)
		GameData::ReadChars(is, (char*) &(GameData::stageSections[i]), sizeof(StageSection));


	#ifdef GL_UTILS_LOG_ENABLED
		char buffer[100];
		sprintf(buffer, "Loaded %d stage sections", GameData::numSections);
		Logger::getDefaultLogger()->writeMessage(0, "GameData::Initialize()", string(buffer));
	#endif
}

GameData * GameData::FromSections(int sectionTopLeft, int sectionTopRight, int sectionBottomLeft, int sectionBottomRight) {

	GameData * result = new GameData(32, 32);

	StageSection sections[4];

	sections[0] = GameData::stageSections[sectionTopLeft];
	sections[1] = GameData::stageSections[sectionTopRight].Flip(true);
	sections[2] = GameData::stageSections[sectionBottomLeft].Flip(false);
	sections[3] = GameData::stageSections[sectionBottomRight].Flip(true).Flip(false);

	GLuint maxRings = 0;

	for(int z = 0; z <= 1; z++)
		for(int x = 0; x <= 1; x++) {

			maxRings += sections[z * 2 + x].MaxRings;

			for(int zs = 0; zs < 16; zs++)
				for(int xs = 0; xs < 16; xs++) {
					result->SetValueAt(x * 16 + xs, z * 16 + zs, sections[z * 2 + x].GetValueAt(xs, zs));
					result->SetAvoidSearchAt(x * 16 + xs, z * 16 + zs, sections[z * 2 + x].GetAvoidSearchAt(xs, zs));
				}

		}
	
	result->SetMaxRings(maxRings);

	result->Update();

	return result;
}

void GameData::SaveData(GameData * gameData, string fileName) {
	ofstream os;

	os.open(fileName, ios_base::binary);

	#ifdef GL_UTILS_LOG_ENABLED
		if(!os.good())
			Logger::getDefaultLogger()->writeMessage(2, "GameData::SaveData()", "Can't open file");
	#endif

	// version
	GameData::WriteUInt(os, 100);

	// width and depth
	GameData::WriteUInt(os, gameData->width);
	GameData::WriteUInt(os, gameData->depth);
	
	// texture and bump map
	GameData::WriteUInt(os, (GLuint)gameData->texture.length());
	GameData::WriteUInt(os, (GLuint)gameData->bumpMap.length());
	GameData::WriteString(os, gameData->texture);
	GameData::WriteString(os, gameData->bumpMap);

	// starting position
	GameData::WriteInt(os, gameData->GetStartPoint().x);
	GameData::WriteInt(os, gameData->GetStartPoint().y);

	// starting direction
	GameData::WriteInt(os, gameData->GetStartDirection().x);
	GameData::WriteInt(os, gameData->GetStartDirection().y);

	// emerald color
	GameData::WriteFloat(os, gameData->GetEmeraldColor().r);
	GameData::WriteFloat(os, gameData->GetEmeraldColor().g);
	GameData::WriteFloat(os, gameData->GetEmeraldColor().b);
	
	// max rings;
	GameData::WriteUInt(os, gameData->maxRings);	
	
	// data

	for(GLint j = 0; j < gameData->depth; j++)
		for(GLint i = 0; i < gameData->width; i++)
			GameData::WriteUInt(os, gameData->GetValueAt(i, j));

	BinaryIO::Write<GLbyte>(os, gameData->avoidSearch, gameData->GetWorldDepth() * gameData->GetWorldWidth());
			
	os.close();

	#ifdef GL_UTILS_LOG_ENABLED
		Logger::getDefaultLogger()->writeMessage(0, "GameData::SaveData()", "Data saved: " + string(fileName));
	#endif
}

GameData * GameData::LoadData(string fileName) {
	ifstream is;

	is.open(fileName, ios_base::binary);

	#ifdef GL_UTILS_LOG_ENABLED
		if(!is.good())
			Logger::getDefaultLogger()->writeMessage(2, "GameData::LoadData()", "Can't open file: " + fileName);
	#endif
	
	GLuint version;

	version = BinaryIO::Read<GLuint>(is);

	// width and depth
	GameData * gameData = new GameData(BinaryIO::Read<GLuint>(is), BinaryIO::Read<GLuint>(is));
	gameData->version = version;

	// floor properties

	gameData->floorRenderingMode = BinaryIO::Read<GLbyte>(is);
	gameData->bumpMappingEnabled = BinaryIO::Read<GLbyte>(is);

	// texture and bumpmap
	GLuint texStrSize, bumpStrSize;

	texStrSize = BinaryIO::Read<GLuint>(is);
	bumpStrSize = BinaryIO::Read<GLuint>(is);

	if(texStrSize > 0)
		GameData::ReadString(is, gameData->texture, texStrSize);

	if(bumpStrSize > 0)
		GameData::ReadString(is, gameData->bumpMap, bumpStrSize);

	// starting position
	gameData->SetStartPoint(BinaryIO::Read<ivec2>(is));

	// starting direction
	gameData->SetStartDirection(BinaryIO::Read<ivec2>(is));

	// emerald color
	gameData->SetEmeraldColor(BinaryIO::Read<vec3>(is));

	// checker colors
	BinaryIO::Read<vec3>(is, gameData->checkerColors, 2);

	// sky background colors
	BinaryIO::Read<vec3>(is, gameData->skyBackcolors, 2);

	// stars colors
	BinaryIO::Read<vec3>(is, gameData->starsColors, 2);

	// max rings
	gameData->SetMaxRings(BinaryIO::Read<GLuint>(is));

	//data
	BinaryIO::Read<GLuint>(is, gameData->data, gameData->GetWorldWidth() * gameData->GetWorldDepth());
	BinaryIO::Read<GLbyte>(is, gameData->avoidSearch, gameData->GetWorldDepth() * gameData->GetWorldWidth());

	is.close();

	#ifdef GL_UTILS_LOG_ENABLED
		Logger::getDefaultLogger()->writeMessage(0, "GameData::LoadData()", "Data loaded: " + string(fileName));
	#endif

	gameData->Update();

	
	return gameData;
}

void GameData::WriteUInt(ofstream &os, GLuint value) {
	const char * buffer = (char*)&(value);
	os.write(buffer, 4);
}
void GameData::WriteInt(ofstream &os, GLint value) {
	const char * buffer = (char*)&(value);
	os.write(buffer, 4);
}
void GameData::WriteFloat(ofstream &os, GLfloat value) {
	const char * buffer = (char*)&(value);
	os.write(buffer, 4);
}
void GameData::WriteString(ofstream &os, string value) {
	const char * buffer = value.c_str();
	os.write(buffer, value.length());
}

void GameData::ReadUInt(ifstream &is, GLuint &value) {
	is.read((char*)&value, 4);
}
void GameData::ReadInt(ifstream &is, GLint &value) {
	is.read((char*)&value, 4);
}
void GameData::ReadFloat(ifstream &is, GLfloat &value) {
	is.read((char*)&value, 4);
}

void GameData::ReadChars(ifstream &is, char * buffer, int length) {
	is.read(buffer, length);
}

void GameData::ReadString(ifstream &is, string &value, int length) {
	char * buffer = new char[length];
	is.read(buffer, length);
	value = string(buffer, length);
}

