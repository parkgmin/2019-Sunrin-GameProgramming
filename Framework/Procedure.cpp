#include "pch.h"
#include "Procedure.h"
#include "Application.h"
#include "Input.h"


Procedure::Procedure()
{
}

LRESULT Procedure::WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_MOUSEMOVE:
		FWPROC->OnMouseMove(wParam, lParam); break;
	case WM_DESTROY:
		FWPROC->OnClose(wParam, lParam); break;
	}
	return DefWindowProc(hWnd, Message, wParam, lParam);
}

Procedure::~Procedure()
{
}

Procedure * Procedure::GetInstance()
{
	static Procedure proc;
	return &proc;
}


void Procedure::OnClose(WPARAM wParam, LPARAM lParam)
{
	PostQuitMessage(0);
}
void Procedure::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	FWINPUT->mouseX = LOWORD(lParam);
	FWINPUT->mouseY = HIWORD(lParam);
}