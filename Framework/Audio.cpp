#include "pch.h"
#include "Audio.h"


Audio::Audio() : xaudio2(nullptr), masterVoice(nullptr)
{
}


Audio::~Audio()
{
}

void Audio::Initialize()
{
	if (FAILED(MFStartup(MF_VERSION)))
	{
		printf("WMF 초기화 실패\n");
	}

	if (FAILED(MFCreateAttributes(&sourceReaderConfiguration, 1)))
	{
		printf("WMF 소스리더 속성 생성 오류\n");
	}
	if (FAILED(sourceReaderConfiguration->SetUINT32(MF_LOW_LATENCY, true)))
	{
		printf("WMF 소스리더 속성 설정 오류\n");
	}

	if (FAILED(XAudio2Create(&xaudio2, 0, XAUDIO2_DEFAULT_PROCESSOR))) 
	{
		printf("오디오 초기화 실패, XAudio2Create\n");
	}
	if (FAILED(xaudio2->CreateMasteringVoice(&masterVoice)))
	{
		printf("오디오 초기화 실패, CreateMasteringVoice\n");
	}

}

void Audio::Uninitialize()
{
	MFShutdown();
	masterVoice->DestroyVoice();
	xaudio2->StopEngine();
}

void Audio::LoadFile(const wchar_t * path, std::vector<BYTE>& audioData, WAVEFORMATEX ** waveFormatEx, unsigned int & waveLength)
{
	//WMF를 활용하여 음원 디코딩
	HRESULT hr = S_OK;

	IMFSourceReader* sourceReader;
	hr = MFCreateSourceReaderFromURL(path, sourceReaderConfiguration, &sourceReader);	//1차 로드(7mb)
	if (FAILED(hr))
		printf("소스리더 생성 실패, 오디오 파일의 경로를 확인해주세요\n");

	//오디오 스트림만 사용
	DWORD streamIndex = (DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM;
	hr = sourceReader->SetStreamSelection((DWORD)MF_SOURCE_READER_ALL_STREAMS, false);
	if (FAILED(hr))
		printf("스트림 설정 실패\n");
	hr = sourceReader->SetStreamSelection(streamIndex, true);
	if (FAILED(hr))
		printf("스트림 설정 실패\n");

	//미디어타입 확인
	IMFMediaType* nativeMediaType;
	hr = sourceReader->GetNativeMediaType(streamIndex, 0, &nativeMediaType);
	if (FAILED(hr))
		printf("미디어 타입 식별 실패\n");

	//파일이 오디오 타입인지 확인
	GUID majorType{};
	hr = nativeMediaType->GetGUID(MF_MT_MAJOR_TYPE, &majorType);
	if (majorType != MFMediaType_Audio) {
		printf("오디오 파일이 아닙니다.\n");
		return;
	}

	// 압축형 오디오인지, 아닌지 확인
	GUID subType{};
	hr = nativeMediaType->GetGUID(MF_MT_MAJOR_TYPE, &subType);
	if (subType == MFAudioFormat_Float || subType == MFAudioFormat_PCM)
	{
		// the audio file is uncompressed
	}
	else
	{
		// the audio file is compressed; we have to decompress it first

		IMFMediaType* partialType = nullptr;
		hr = MFCreateMediaType(&partialType);
		if (FAILED(hr))
			printf("미디어 타입 생성 실패\n");

		hr = partialType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
		if (FAILED(hr))
			puts("미디어타입을 오디오로 변경하는데 실패하였습니다.");

		hr = partialType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
		if (FAILED(hr))
			puts("서브타입을 PCM으로 변경하는데 실패하였습니다.");

		hr = sourceReader->SetCurrentMediaType(streamIndex, NULL, partialType);
		if (FAILED(hr))
			puts("현재 미디어타입 설정 실패");
	}

	//이제 디코딩이 끝난 상태이니 로딩 진행
	//XAudio2 버퍼로 이동하기 위해 데이터를 압축 해제하고 로딩
	//FAILED 예외처리 해주기
	IMFMediaType* uncompressedAudioType = nullptr;
	hr = sourceReader->GetCurrentMediaType(streamIndex, &uncompressedAudioType);
	if (FAILED(hr))
	{
		printf("erroccured\n");
	}

	hr = MFCreateWaveFormatExFromMFMediaType(uncompressedAudioType, waveFormatEx, &waveLength);
	if (FAILED(hr))
	{
		printf("wave foramt 로드 실패\n");
	}


	//스트림 선택
	hr = sourceReader->SetStreamSelection(streamIndex, true);
	if (FAILED(hr))
	{
		printf("erroccured\n");
	}

	//데이터를 vector로 옮깁니다.
	IMFSample* sample = nullptr;
	IMFMediaBuffer* buffer = nullptr;
	BYTE* localAudioData = NULL;
	DWORD localAudioDataLength = 0;

	int count = 0;
	while (true)
	{
		DWORD flags = 0;
		hr = sourceReader->ReadSample(streamIndex, 0, nullptr, &flags, nullptr, &sample);
		if (FAILED(hr))
		{
			printf("erroccured\n");
		}

		//데이터가 여전히 사용 가능한지 체크
		if (flags&MF_SOURCE_READERF_CURRENTMEDIATYPECHANGED)
			break;

		//파일의 끝 체크
		if (flags & MF_SOURCE_READERF_ENDOFSTREAM)
			break;
		if (sample == nullptr)
			continue;

		//데이터를 버퍼로 전환
		hr = sample->ConvertToContiguousBuffer(&buffer);
		if (FAILED(hr))
		{
			printf("erroccured\n");
		}

		//버퍼를 잠그고, 데이터를 메모리로 복사
		hr = buffer->Lock(&localAudioData, nullptr, &localAudioDataLength);
		if (FAILED(hr))
		{
			printf("erroccured\n");
		}

		for (size_t i = 0; i < localAudioDataLength; i++)
			audioData.push_back(localAudioData[i]);

		//버퍼 잠금 해제
		hr = buffer->Unlock();
		if (FAILED(hr))
		{
			printf("erroccured\n");
		}
		localAudioData = nullptr;
		count++;
	}
	printf("%d", count);


}

Audio * Audio::GetInstance()
{
	static Audio a;
	return &a;
}
