#include "pch.h"
#include "Enemy.h"
#include "Bullet.h"


Enemy::Enemy(const wchar_t* uri)
	: GameObject(uri), delay(0.3f),
	bulletUri(L"image/Bullet1.png"), hp(20), direction(1), speed(300.0f)
{
}

Enemy::Enemy(const wchar_t* uri,
		const wchar_t* bulletUri)
		: GameObject(uri), delay(0.3f),
		bulletUri(bulletUri), hp(5), direction(1), speed(300.0f)
{
}


Enemy::~Enemy()
{
}

void Enemy::OnUpdate()
{
	Move();
	timer += FWAPP->DeltaTime();	//시간 갱신
	if (timer >= delay)		//timer가 delay를 넘어서면?
	{
		timer = 0.0f;
		Shoot(0.1f, 20.0f, 0.00f, 3.0f);
		Shoot(0.2f, 30.0f, 0.00f, 3.0f);
		Shoot(0.3f, 170.0f, 0.00f, 3.0f);
		Shoot(0.4f, 200.0f, 0.00f, 3.0f);
		Shoot(0.5f, 120.0f, 0.00f, 3.0f);
		Shoot(0.6f, 50.0f, 0.00f, 3.0f);
		Shoot(0.8f, 10.0f, 0.00f, 3.0f);
		Shoot(0.9f, 100.0f, 0.00f, 3.0f);
		Shoot(0.10f, 100.0f, 0.00f, 3.0f);
		Shoot(0.11f, 150.0f, 0.00f, 3.0f);
		Shoot(0.12f, 150.0f, 0.00f, 3.0f);
	}
}

void Enemy::Move()
{
	pos.x += speed * FWAPP->DeltaTime()*direction;
	if (pos.x > 900.0f)
	{
		direction = -1;
		pos.x = 900.0f;
	}
	if (pos.x < 12.0f)
	{
		direction = 1;
		pos.x = 12.0f;
	}
}

void Enemy::Angle()
{

}

void Enemy::Shoot(float angle, float speed,
	float angleRate, float speedRate)
{
	
		//총을 쏠겁니다.
		timer = 0.0f;
		Bullet* b = new Bullet(bulletUri);
		b->pos.x = pos.x;
		b->pos.y = pos.y;
		b->speed = speed;
		b->angle = angle;
		b->speedRate = speedRate;
		b->angleRate = angleRate;
		b->master = this;
		scene->PushBackGameObject(b);
	
	
}

bool Enemy::Hit(int damage)	
{
	hp -= damage;
	if (hp <= 0)	
		return true;
	else
		return false;
}