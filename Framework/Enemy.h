#pragma once
#include "GameObject.h"
class Enemy : public GameObject
{
private:
	float timer;
public:
	float delay;
	const wchar_t* bulletUri;
	int hp;

	int direction;
	int speed;

public:
	Enemy(const wchar_t* uri); //그냥
	Enemy(
		const wchar_t* uri,
		const wchar_t* bulletUri); //총알 종류 여러개
	~Enemy();

	bool Hit(int damage);		//피격 시 일어날 일

	void OnUpdate();			//업데이트
	void Move();				//움직임 구현
	void Angle();
	virtual void Shoot(float angle, float speed,
		float angleRate, float speedRate);
								//총알 발사 구현
};

