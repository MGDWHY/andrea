#pragma once

#include <stdlib.h>
#include <fstream>
#include <string>
#include <gl/glew.h>
#include "vecmath.h"
#include "Global.h"

using namespace Vecmath;
using namespace std;

struct StageSection 
{
public:
	GLuint MaxRings;
	GLuint Data[16 * 16];
	GLbyte AvoidSearch[16 * 16];

	StageSection();

	GLbyte GetAvoidSearchAt(GLint x, GLint z);
	void SetAvoidSearchAt(GLint x, GLint z, GLbyte value);

	GLuint GetValueAt(GLint x, GLint z);
	void SetValueAt(GLint x, GLint z, GLuint value);
	
	StageSection Flip(bool horizonal);


};

class GameData
{
private:

	static GLuint numSections;

	GLuint version;

	GLuint width, depth;

	GLbyte floorRenderingMode, bumpMappingEnabled;

	string texture, bumpMap;

	ivec2 startPoint;
	ivec2 startDirection;

	vec3 emeraldColor;
	vec3 checkerColors[2];
	vec3 skyBackcolors[2];
	vec3 starsColors[2];

	GLuint maxRings;

	GLuint * data;

	GLbyte * avoidSearch;

	GLuint * objectCount;

	GLuint elementsCount;

	GLint collectedRings;

	static GLuint Normalize(GLint val, GLint max);

	static void WriteUInt(ofstream &os, GLuint value);
	static void WriteInt(ofstream &os, GLint value);
	static void WriteFloat(ofstream &os, GLfloat value);
	static void WriteString(ofstream &os, string value);

	static void ReadUInt(ifstream &is, GLuint &value);
	static void ReadInt(ifstream &is, GLint &value);
	static void ReadFloat(ifstream &is, GLfloat &value);
	static void ReadChars(ifstream &ts, char * value, int length);
	static void ReadString(ifstream &is, string &value, int length);

public:

	static StageSection * stageSections;

	static const GLuint C_NOTHING;
	static const GLuint C_REDSPHERE;
	static const GLuint C_BLUESPHERE;
	static const GLuint C_YELLOWSPHERE;
	static const GLuint C_STARSPHERE;
	static const GLuint C_RING;

	static const GLbyte RM_CHECKERBOARD;
	static const GLbyte RM_TEXTURE;

	static const GLbyte AS_NO; 
	static const GLbyte AS_YES;

	GameData(GLuint width, GLuint depth);
	~GameData(void);

	GLuint GetObjectCount(GLuint objectType);

	GLuint GetValueAt(const ivec2 &position);
	GLuint GetValueAt(GLint x, GLint z);
	void SetValueAt(GLint x, GLint z, GLuint value);
	void SetValueAt(const ivec2 &position, GLuint value);

	void SetAvoidSearchAt(GLint x, GLint z, GLbyte value);

	GLbyte GetAvoidSearchAt(GLint x, GLint z);
	GLbyte GetAvoidSearchAt(const ivec2& position);

	void SetMaxRings(GLuint maxRings);
	GLuint GetMaxRings();

	void SetCollectedRings(GLint collectedRings);
	void IncrementCollectedRings();
	GLint GetCollectedRings();
	bool IsPerfect();
	bool IsClear();

	void SetFloorRenderingMode(GLbyte mode);
	GLbyte GetFloorRenderingMode();

	void SetBumpMappingEnabled(GLbyte enabled);
	GLbyte IsBumpMappingEnabled();

	void SetTexture(string texture);
	void SetBumpMup(string bumpMap);

	void SetStartPoint(const ivec2 &startPoint);
	ivec2 GetStartPoint();

	void SetStartDirection(const ivec2 &startDirection);
	ivec2 GetStartDirection();

	void SetEmeraldColor(const vec3 &color);
	vec3 GetEmeraldColor();

	void SetCheckerBoardColors(int index, vec3 color);
	vec3 GetCheckerBoardColors(int index);
	
	void SetSkyBackColor(int index, vec3 color);
	vec3 GetSkyBackColor(int index);
	
	void SetStarsColor(int index, vec3 color);
	vec3 GetStarsColor(int index);

	string GetTexture();
	string GetBumpMap();

	void Update();

	GLuint * GetData();
	void SetData(GLuint * newData);

	GLuint * GetRoundSubData(GLint x, GLint z, GLint radius);
	GLuint * GetRoundSubData(GLint minX, GLint maxX, GLint minZ, GLint maxZ);

	GLuint GetWorldWidth();
	GLuint GetWorldDepth();

	ivec2 Normalize(const ivec2& position);
	GLuint NormalizeX(GLint x);
	GLuint NormalizeZ(GLint z);

	static void Initialize();
	
	static GameData * FromSections(int sectionTopLeft, int sectionTopRight, int sectionBottomLeft, int sectionBottomRight);
	
	static void SaveData(GameData * gameData, string fileName);
	static GameData * LoadData(string fileName);
};

