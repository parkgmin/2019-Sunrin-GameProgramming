#include "pch.h"
#include "GameManager.h"


GameManager * GameManager::GetInstance()
{
	static GameManager gm;
	return &gm;
}

GameManager::GameManager()
{
}


GameManager::~GameManager()
{
}

