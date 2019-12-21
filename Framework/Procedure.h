#pragma once
#include "Input.h"
#define FWPROC Procedure::GetInstance()

class Input;

class Procedure
{
	friend class Input;
public:
	Procedure();
	~Procedure();

	static Procedure* GetInstance();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);

private:
	void OnClose(WPARAM wParam, LPARAM lParam);		//CLOSE
	void OnMouseMove(WPARAM wParam, LPARAM lParam);	//KEYDOWN
};

