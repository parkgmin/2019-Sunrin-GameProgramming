#pragma once
#include "GameObject.h"
#include "Player.h"
#include "Enemy.h"
#define GAMEMGR GameManager::GetInstance()
class GameManager :
	public GameObject
{

public:
	list<Enemy*> enemyList;
	Player* player;
	static GameManager* GetInstance();
	GameManager();
	~GameManager();
public:
	list<Player*> PlayerList;
	Enemy* enemy;
	//static GameManager* Getlnstance();
};
