#pragma once
#include "Audio.h"
#include "SoundEvent.h"
#define FWSOUND Sound::GetInstance()

class Sound
{
private:
	// the main audio engine
	Audio* engine;				// the main audio engine: XAudio2 with Windows Media Component

	// handle message
public:
	// constructor and destructor
	Sound();
	~Sound();

	static Sound* GetInstance();

	// load files from disk
	void loadFile(const wchar_t* fileName, SoundEvent* soundEvent);

	// play sound
	void PlaySoundEvent(const SoundEvent* soundEvent);
	void StopSoundEvent(const SoundEvent* soundEvent);
	void PauseSoundEvent(const SoundEvent* soundEvent);
};

