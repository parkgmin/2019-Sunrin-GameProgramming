#include "pch.h"
#include "Sound.h"


Sound::Sound()
{
	engine = Audio::GetInstance();
}


Sound::~Sound()
{
}

Sound * Sound::GetInstance()
{
	static Sound a;
	return &a;
}

void Sound::loadFile(const wchar_t* fileName, SoundEvent* soundEvent)
{
	HRESULT hr = S_OK;

	// load file into wave
	WAVEFORMATEX* waveFormatEx;
	engine->LoadFile(fileName, soundEvent->audioData, &waveFormatEx, soundEvent->waveLength);

	soundEvent->waveFormat = waveFormatEx;

	// create source voice
	//hr = engine->xaudio2->CreateSourceVoice(&soundEvent->sourceVoice, &soundEvent->waveFormat);
	if (FAILED(hr = engine->xaudio2->CreateSourceVoice(&soundEvent->sourceVoice, waveFormatEx)))
	{
		wprintf(L"Error %#X creating source voice\n", hr);
		return;
	}

	ZeroMemory(&soundEvent->audioBuffer, sizeof(XAUDIO2_BUFFER));
	soundEvent->audioBuffer.AudioBytes = (UINT32)soundEvent->audioData.size();
	soundEvent->audioBuffer.pAudioData = (BYTE* const)&soundEvent->audioData[0];
	soundEvent->audioBuffer.pContext = nullptr;

}

void Sound::PlaySoundEvent(const SoundEvent* soundEvent)
{
	// handle errors
	HRESULT hr = S_OK;

	// submit the audio buffer to the source voice
	hr = soundEvent->sourceVoice->SubmitSourceBuffer(&soundEvent->audioBuffer);
	if (FAILED(hr))
		puts("소스 버퍼에 접근이 불가능합니다.");

	// start the source voice
	soundEvent->sourceVoice->Start();
}

void Sound::StopSoundEvent(const SoundEvent* soundEvent)
{
	soundEvent->sourceVoice->Stop();
	soundEvent->sourceVoice->FlushSourceBuffers();
}

void Sound::PauseSoundEvent(const SoundEvent * soundEvent)
{
	soundEvent->sourceVoice->Stop();
}
