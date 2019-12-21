#pragma once
#include "GameObject.h"
class Player :
	public GameObject
{
public:
	float speed;
	float delay;
	int hp;
public:
	Player(const wchar_t* uri);
	~Player();

	bool Hit(int damage);

	void OnUpdate();

	void Move();
	void Shoot();
};

