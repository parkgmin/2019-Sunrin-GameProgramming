#pragma once
#include "pch.h"
#include "Application.h"
#define FWINPUT Input::GetInstance()

class Application;
class Procedure;

class Input
{
	friend class Application;
	friend class Procedure;
public:
	Input();
	~Input();
	static Input* GetInstance();

private:
	int mouseX;
	int mouseY;

public:
	int GetMouseX();
	int GetMouseY();

private:
	bool lkey[256], rkey[256];
	void UpdateKeyState();

public:
	int GetMyKeyState(int vk);
	bool GetKeyDown(int vk);
	bool GetKeyPressed(int vk);
	bool GetKeyUp(int vk);
};

