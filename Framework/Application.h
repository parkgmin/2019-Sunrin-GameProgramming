#pragma once
#include "pch.h"
#include "Graphic.h"
#include "Input.h"
#define FWAPP Application::GetInstance()

class Graphic;
class Scene;
class Input;

class Application
{
	friend class Graphic;
	friend class Scene;
	friend class Input;
public:
	Application();
	~Application();

	static Application* GetInstance();
	void Run(const wchar_t* title, int width, int height, bool isFullscreen, Scene* firstScene);
	HWND GetHWND();

private:
	HRESULT Initialize(const wchar_t* title, int width, int height, bool isFullscreen, Scene* firstScene);
	HRESULT CreateFrame(const wchar_t* title, int width, int height, bool isFullscreen);
	void RunMessageLoop();
	void Uninitialize();
	void DeleteScenes();

private:
	HWND hwnd;
	Graphic* graphic;
	Scene* currentScene;
	Scene* nextScene;
	bool changeSceneCalled;
	float deltaTime;

private:
	void LateChangeScene();
	void UpdateTime();

public: 
	Scene* GetCurrentScene();
	float DeltaTime();
	void ChangeScene(Scene* nextScene);
	void Exit();

};

