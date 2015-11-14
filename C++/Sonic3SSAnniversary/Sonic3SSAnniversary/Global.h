#pragma once

#include <Windows.h>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include "lodepng.h"
#include "gl/glew.h"
#include "Logger.h"
#include "VertexArrayObject.h"
#include "Timer.h"
#include "binio.h"
#include "xbo.h"

using namespace std;
using namespace glutils;
using namespace ExtendedBinary3DObject;

namespace Global {

	/* ------------ DATA TYPES ----------------- */

	enum ProgramContext {
		CTX_DISCLAIMER, CTX_INTRO, CTX_MAIN_MENU, CTX_GAME, CTX_STAGE_CLEAR
	};

	enum Switch {
		SWITCH_OFF = 0,
		SWITCH_ON = 1
	};

	enum Quality {
		QUALITY_LOW = 512,
		QUALITY_MEDIUM = 1024,
		QUALITY_HIGH = 2048
	};
	
	enum GameMode {
		MODE_BLUESPHERES = 0,
		MODE_CUSTOMSTAGE = 1
	};

	// Display mode

	struct DisplayMode {
		int width;
		int height;
		int frequency;
		int bitsPerPixel;
		int operator== (const DisplayMode &other);
	};

	// Game configuration

	struct GameConfig {
		DisplayMode displayMode;
		int windowed;
		int quality;
		int anisotropicFilter;
		int stereo3D;
		int shadowsEnabled;
		int shadowQuality;
		int musicVolume;
		int fxVolume;
	};

	// Game stats

	struct ThreadStats {
		int frequency;
		int time;
		ThreadStats();
	};

	struct GameStats {
		ThreadStats fps;
		ThreadStats logic;
	};

	/* ------------ CONSTANTS --------------- */

	// OpenGL
	const int GL_MIN_MAJ_VERSION = 3;
	const int GL_MIN_MIN_VERSION = 3;

	// Game Directories
	const string TEXTURE_DIR = "textures";
	const string SHADERS_DIR = "shaders";
	const string DATA_DIR = "data";
	const string SOUND_DIR = "sound";
	const string MODELS_DIR = "models";

	// Files
	const string CONFIG_FILE = "config.cfg";
	const string STAGE_FILE = DATA_DIR + "/stages.dat";
	const string LOG_FILE = "default.log";

	// Display mode
	const int DM_MIN_FREQUENCY = 60;
	const int DM_BITS_PER_PIXEL = 32;
	
	// Default game config
	const int DEFAULT_WINDOWED = 1;
	const int DEFAULT_GRAPHICS_QUALITY = 2;
	const int DEFAULT_SHADOWS_ENABLED = SWITCH_ON;
	const int DEFAULT_STEREO3D = SWITCH_OFF;
	const int DEFAULT_SHADOW_QUALITY = QUALITY_HIGH;
	const int DEFAULT_MUSIC_VOLUME = 100;
	const int DEFAULT_FXVOLUME = 100;
	const int DEFAULT_ANISOTROPIC = 4;

	// PI Constant
	const float PI = 3.141592f;

	// Camera
	const GLfloat CAMERA_NEAR = 0.1f;
	const GLfloat CAMERA_FAR = 50.0f;
	const GLfloat CAMERA_FOVY = PI / 4.0f;
	const GLfloat CAMERA_Z = -2.5f;
	const GLfloat CAMERA_ROTATE_X = PI / 4.0f;
	const GLfloat CAMERA_STEREO_IOD = 0.02f;
	const GLfloat CAMERA_STEREO_SCREENZ = CAMERA_STEREO_IOD * 20.0f;

	/* -------------- FUNCTIONS ------------------ */

	// Random Numbers
	float Random(float x);
	float Random(float min, float max);

	// Display modes
	vector<DisplayMode> GetAvailableDisplayModes(int minFrequency, int bpp);

	// Game Stats
	void CollectGameStats(GameStats& gameStats, int executionTime, bool incFps, bool incLogic);

	// Game configuration load&store
	GameConfig LoadGameConfig();
	void StoreGameConfig(const GameConfig& gameConfig);

	// Stage files
	vector<string> LoadStageFiles();

	// Texture loading
	void PngLoad(const char* fileName);
	GLuint PngTexture(const char * fileName, GLint minFilter, GLint magFilter);

	// Model conversion from OBJ to XBO
	void ConvertModels();

	// XBO to VertexArray
	VertexArrayObject * CreateVertexArray(XBO * obj, int shadingGroup);

	// Get the executable directory
	void GetModuleDirectory(char * buffer);

	// Vector
	template<class T> bool Contains(vector<T>& v, T& element) {
		for(int i = 0; i < v.size(); i++) {
			if(v[i] == element)
				return true;
		}
		return false;
	}

	// GL Version Check
	void CheckGLVersion();

	// Logging
	void InitializeLog();

}