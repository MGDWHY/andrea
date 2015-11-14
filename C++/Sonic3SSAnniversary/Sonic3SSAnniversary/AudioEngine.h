#pragma once

#include "Logger.h"
#include "bass/bass.h"
// #include "bass/bass_fx.h"
#include "gl\glew.h"
#include "Global.h"




class AudioEngine
{
private:

	static const int SYNC_MUSIC;
	static const int AudioEngine::SYNC_STAGE_CLEAR;

	static const int NUM_EFFECTS;

	HSTREAM music, titleScreen, stageClear;
	HSTREAM * effects;
	GLfloat musicTempo;

	int musicVolume, fxVolume;

	static HSTREAM LoadEffect(const char * fileName);

	static void CALLBACK SyncProc(HSYNC handle, DWORD channel, DWORD data, void *user);

public:

	static const GLint FX_RING;
	static const GLint FX_BLUESPHERE;
	static const GLint FX_GAMEOVER;
	static const GLint FX_JUMP;
	static const GLint FX_BOUNCE;
	static const GLint FX_EMERALD;
	static const GLint FX_SPLASH;
	static const GLint FX_TONG;
	static const GLint FX_PERFECT;
	static const GLint FX_OK;
	static const GLint FX_WRONG;


	AudioEngine(void);
	~AudioEngine(void);

	void Configure(const Global::GameConfig& gameConfig);

	void PlayFX(GLint FXid);
	void StopFX(GLint FXid);

	void PlayStageClear();
	void StopStageClear();

	void PlayTitleScreen();
	void StopTitleScreen();

	void PlayMusic();
	void PauseMusic();
	
	void RestartMusic();

	void SetMusicTempo(GLfloat tempo);
	GLfloat GetMusicTempo();

	void SetMusicVolume(GLfloat value);
};

