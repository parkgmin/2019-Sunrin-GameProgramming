#pragma once
#include "GameObject.h"
#include "GameScene.h"
class Bullet :
	public GameObject
{
public:
	float speed;
	float angle;
	float speedRate;	//가속도
	float angleRate;	//각속도

	GameObject* master;	
	GameScene* game;
public:
	Bullet(const wchar_t* uri); 
	~Bullet();

	void OnUpdate();		
	void OnLateUpdate();
	void Move();
	bool CheckOutOfScreen();
	void OnDestroy();
};

