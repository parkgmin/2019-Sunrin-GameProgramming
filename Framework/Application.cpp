#include "pch.h"
#include "Application.h"
#include "Input.h"
#include "Procedure.h"
#include "Audio.h"


Application::Application() : hwnd(NULL), graphic(NULL), changeSceneCalled(false), currentScene(NULL)
{

}


Application::~Application()
{
}

Application * Application::GetInstance()
{
	static Application app;
	return &app;
}

void Application::Run(const wchar_t * title, int width, int height, bool isFullscreen, Scene * firstScene)
{
	if (SUCCEEDED(CoInitialize(NULL)))
	{
		{
			if (SUCCEEDED(Initialize(title, width, height, isFullscreen, firstScene)))
			{
				RunMessageLoop();
				Uninitialize();
			}
		}
		CoUninitialize();
	}
}

HRESULT Application::Initialize(const wchar_t* title, int width, int height, bool isFullscreen, Scene* firstScene)
{
	HRESULT hr;

	graphic = Graphic::GetInstance();
	graphic->width = width;
	graphic->height = height;
	graphic->isFullScreen = isFullscreen;
	hr = CreateFrame(title, width, height, isFullscreen);				//윈도우 클래스 등록 및 초기화
	if (FAILED(hr))
		return E_FAIL;

	hr = graphic->CreateDeviceIndependentResources();	//장치 의존적 자원 생성
	if (FAILED(hr))
		return E_FAIL;

	hr = graphic->CreateDeviceResources();	//장치 의존적 자원 생성
	if (FAILED(hr))
		return E_FAIL;

	Audio::GetInstance()->Initialize();

	ShowWindow(hwnd, SW_SHOWNORMAL);
	UpdateWindow(hwnd);

	currentScene = firstScene;
	currentScene->Initialize();

	return S_OK;
}
void Application::RunMessageLoop()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		UpdateTime();
		graphic->Render();
		FWAPP->currentScene->Update();
		if (changeSceneCalled)
		{
			LateChangeScene();
		}
		FWAPP->currentScene->LateUpdate();
		FWINPUT->UpdateKeyState();
	}
}

void Application::Uninitialize()
{
	DeleteScenes();
	graphic->DiscardDeviceIndependentResources();
	Audio::GetInstance()->Uninitialize();
}

void Application::DeleteScenes()
{
	SAFE_DELETE(currentScene);
	SAFE_DELETE(nextScene);
}

HWND Application::GetHWND()
{
	return hwnd;
}


HRESULT Application::CreateFrame(const wchar_t* title, int width, int height, bool isFullscreen)
{
	//윈도우 클래스 초기화
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof(WNDCLASSEX);			//cbSize값 초기화
	wcex.style = CS_HREDRAW | CS_VREDRAW;		//화면 크기 바뀔 때마다 다시 그릴 것 (종,횡)
	wcex.lpfnWndProc = Procedure::WndProc;	//윈도우 프로시저 함수 등록
	wcex.cbClsExtra = 0;						//예약 영역 사용 x
	wcex.cbWndExtra = 0;						//예약 영역 사용 X
	wcex.hInstance = GetModuleHandle(NULL);		//hInstance
	wcex.hbrBackground = NULL;					//배경 색 지정
	wcex.lpszMenuName = NULL;					//메뉴 지정 x
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);	//마우스 커서 지정
	wcex.lpszClassName = title;		//윈도우클래스이름

	//윈도우 클래스를 운영체제에 등록
	if (!RegisterClassEx(&wcex))
	{
		printf("failed");
		return 0;
	}

	//윈도우 생성 하기 전에 설정
	int wx, wy;
	RECT windowRect = { 0, };
	DWORD style, exstyle;
	if (isFullscreen)
	{
		wx = wy = 0L;
		windowRect.right = GetSystemMetrics(SM_CXSCREEN);
		windowRect.bottom = GetSystemMetrics(SM_CYSCREEN);
		style = WS_SYSMENU | WS_POPUP;
		exstyle = WS_EX_TOPMOST;
	}
	else
	{
		wx = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		wy = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

		windowRect.right = width;
		windowRect.bottom = height;
		style = WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_CAPTION;		//CW_USEDEFAULT
		exstyle = NULL;
	}

	//윈도우 조정
	AdjustWindowRectEx(&windowRect, style, FALSE, exstyle);

	//윈도우 생성
	hwnd = CreateWindowEx(exstyle,title, title, style,
		wx, wy, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
		NULL, NULL, wcex.hInstance, NULL);

	//GetWindowRect 써서 윈도우 크기 가져올 수 있음
	if (hwnd == NULL)
	{
		printf("CreateWindowEx Failed\n");
		return E_FAIL;
	}
	else
	{
		return S_OK;
	}
}

Scene * Application::GetCurrentScene()
{
	return currentScene;
}

float Application::DeltaTime()
{
	return deltaTime;
}

void Application::ChangeScene(Scene* nextScene)
{
	changeSceneCalled = true;
	this->nextScene = nextScene;
}

void Application::Exit()
{
	PostQuitMessage(0);
}

void Application::LateChangeScene()
{
	changeSceneCalled = false;

	//이전 씬 삭제
	SAFE_DELETE(currentScene);

	//다음 씬 시작
	currentScene = nextScene;
	graphic->CreateDeviceResources();
	currentScene->Initialize();
	nextScene = nullptr;
}

void Application::UpdateTime()
{
	static DWORD oldTime = timeGetTime();   //마지막 시간(temp변수)

	DWORD curTime = timeGetTime();      //현재 시간
	deltaTime = (curTime - oldTime)*0.001f;

	oldTime = curTime;
}
