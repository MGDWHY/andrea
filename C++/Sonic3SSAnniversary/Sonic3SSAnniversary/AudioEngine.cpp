#include "AudioEngine.h"

using namespace std;
using namespace Global;

const int AudioEngine::SYNC_MUSIC = 0;
const int AudioEngine::SYNC_STAGE_CLEAR = 1;

const int AudioEngine::NUM_EFFECTS = 11;

const GLint AudioEngine::FX_RING = 0;
const GLint AudioEngine::FX_BLUESPHERE = 1;
const GLint AudioEngine::FX_GAMEOVER = 2;
const GLint AudioEngine::FX_JUMP = 3;
const GLint AudioEngine::FX_BOUNCE = 4;
const GLint AudioEngine::FX_EMERALD = 5;
const GLint AudioEngine::FX_SPLASH = 6;
const GLint AudioEngine::FX_TONG = 7;
const GLint AudioEngine::FX_PERFECT = 8;
const GLint AudioEngine::FX_OK = 9;
const GLint AudioEngine::FX_WRONG = 10;

AudioEngine::AudioEngine(void)
{
	BASS_Init(1, 44100, BASS_DEVICE_SPEAKERS, 0, NULL);
	BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, 10000);

	this->musicTempo = 0.0f;

	this->music = BASS_StreamCreateFile(FALSE, "sound/music2.wav", 0, 0, BASS_STREAM_DECODE);
	this->titleScreen = BASS_StreamCreateFile(FALSE, "sound/s&k.mp3", 0, 0, 0);
	this->stageClear = BASS_StreamCreateFile(FALSE, "sound/stageClear.wav", 0, 0, BASS_STREAM_DECODE);

	this->music = BASS_FX_TempoCreate(this->music, BASS_FX_FREESOURCE);
	this->stageClear = BASS_FX_TempoCreate(this->stageClear, BASS_FX_FREESOURCE);

	BASS_ChannelSetSync(this->music, BASS_SYNC_END, 0, AudioEngine::SyncProc, (void*) &(AudioEngine::SYNC_MUSIC));
	BASS_ChannelSetSync(this->stageClear, BASS_SYNC_END, 0, AudioEngine::SyncProc, (void*) &(AudioEngine::SYNC_STAGE_CLEAR));

	this->effects = new HSTREAM[AudioEngine::NUM_EFFECTS];
	this->effects[AudioEngine::FX_RING] = AudioEngine::LoadEffect("sound/ring.wav");
	this->effects[AudioEngine::FX_BLUESPHERE] = AudioEngine::LoadEffect("sound/bluesphere.wav");
	this->effects[AudioEngine::FX_GAMEOVER] = AudioEngine::LoadEffect("sound/redsphere.wav");
	this->effects[AudioEngine::FX_JUMP] = AudioEngine::LoadEffect("sound/jump.wav");
	this->effects[AudioEngine::FX_BOUNCE] = AudioEngine::LoadEffect("sound/bounce.wav");	
	this->effects[AudioEngine::FX_EMERALD] = AudioEngine::LoadEffect("sound/emerald.wav");
	this->effects[AudioEngine::FX_SPLASH] = AudioEngine::LoadEffect("sound/splash.wav");
	this->effects[AudioEngine::FX_TONG] = AudioEngine::LoadEffect("sound/tong.wav");
	this->effects[AudioEngine::FX_PERFECT] = AudioEngine::LoadEffect("sound/perfect.mp3");
	this->effects[AudioEngine::FX_OK] = AudioEngine::LoadEffect("sound/ok.wav");
	this->effects[AudioEngine::FX_WRONG] = AudioEngine::LoadEffect("sound/wrong.wav");

	#ifdef GL_UTILS_LOG_ENABLED
		int error = BASS_ErrorGetCode();

		if(error)
			Logger::getDefaultLogger()->writeMessage(2, "AudioEngine::AudioEngine()", "Can't initialize audio engine!");
		else
			Logger::getDefaultLogger()->writeMessage(0, "AudioEngine::AudioEngine()", "Sound initialized!");

	#endif
}

void AudioEngine::Configure(const GameConfig& gameConfig) {

	this->musicVolume = gameConfig.musicVolume;
	this->fxVolume = gameConfig.fxVolume;

	BASS_ChannelSetAttribute(this->music, BASS_ATTRIB_VOL, this->musicVolume / 100.0f);
	BASS_ChannelSetAttribute(this->titleScreen, BASS_ATTRIB_VOL, this->musicVolume / 100.0f);
	
	for(int i = 0; i < AudioEngine::NUM_EFFECTS; i++)
		BASS_ChannelSetAttribute(this->effects[i], BASS_ATTRIB_VOL, this->fxVolume / 100.0f);
}

void AudioEngine::PlayTitleScreen() { BASS_ChannelPlay(this->titleScreen, TRUE); }
void AudioEngine::StopTitleScreen() { BASS_ChannelPause(this->titleScreen); }

void AudioEngine::PlayStageClear() { BASS_ChannelPlay(this->stageClear, TRUE); }
void AudioEngine::StopStageClear() { BASS_ChannelPause(this->stageClear); }

void AudioEngine::PlayMusic() { BASS_ChannelPlay(this->music, FALSE); }
void AudioEngine::PauseMusic() { BASS_ChannelPause(this->music);  }
void AudioEngine::RestartMusic() { BASS_ChannelPlay(this->music, TRUE); }

void AudioEngine::SetMusicTempo(GLfloat tempo) {
	this->musicTempo = tempo;
	BASS_ChannelSetAttribute(this->music, BASS_ATTRIB_TEMPO, tempo);
}

GLfloat AudioEngine::GetMusicTempo() {
	return this->musicTempo;
}

void AudioEngine::PlayFX(GLint FXid) { BASS_ChannelPlay(this->effects[FXid], TRUE); }
void AudioEngine::StopFX(GLint FXid) { BASS_ChannelPause(this->effects[FXid]); }

void CALLBACK AudioEngine::SyncProc(HSYNC handle, DWORD channel, DWORD data, void *user) {
	int caller = *((int*)user);

	if(caller == AudioEngine::SYNC_MUSIC)
		BASS_ChannelSetPosition(channel, 1150424, BASS_POS_BYTE);
	else if(caller == AudioEngine::SYNC_STAGE_CLEAR)
		BASS_ChannelSetPosition(channel, 30000 * 4, BASS_POS_BYTE);
}

HSTREAM AudioEngine::LoadEffect(const char * fileName) {
	return BASS_StreamCreateFile(FALSE, fileName, 0, 0, 0);
}

void AudioEngine::SetMusicVolume(GLfloat value) {
	BASS_ChannelSetAttribute(this->music, BASS_ATTRIB_VOL, this->musicVolume / 100.0f * value);
}


AudioEngine::~AudioEngine(void) 
{ 
	BASS_StreamFree(this->music);
}
