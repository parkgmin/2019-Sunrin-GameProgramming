#include "pch.h"
#include <iostream>
#include "Application.h"
#include "GameScene.h"

int main()
{
	FWAPP->Run(
		L"연습", 1024, 768, false, new GameScene());

	return 0;
}
