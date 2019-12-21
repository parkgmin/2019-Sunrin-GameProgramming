#include "pch.h"
#include "Player.h"
#include "Input.h"
#include "Bullet.h"


Player::Player(const wchar_t* uri) : GameObject(uri), speed(500.0f), delay(0.5f), hp(30)
{
}


Player::~Player()
{

}

void Player::OnUpdate()
{
	Move();
	Shoot();

	
}

void Player::Move()
{
	if (FWINPUT->GetKeyDown(VK_UP) ||
		FWINPUT->GetKeyPressed(VK_UP))
	{
		pos.y -= FWAPP->DeltaTime() * speed;
	}
	if (FWINPUT->GetKeyDown(VK_DOWN) ||
		FWINPUT->GetKeyPressed(VK_DOWN))
	{
		pos.y += FWAPP->DeltaTime() * speed;
	}
	if (FWINPUT->GetKeyDown(VK_RIGHT) ||
		FWINPUT->GetKeyPressed(VK_RIGHT))
	{
		pos.x += FWAPP->DeltaTime() * speed;
	}
	if (FWINPUT->GetKeyDown(VK_LEFT) ||
		FWINPUT->GetKeyPressed(VK_LEFT))
	{
		pos.x -= FWAPP->DeltaTime() * speed;
	}
}

void Player::Shoot()
{
	static float count = 0.0f;
	count += FWAPP->DeltaTime();	//시간 갱신
	if (count >= delay &&
		FWINPUT->GetMyKeyState(VK_SPACE)>0)		//count가 delay를 넘어서면?
	{
		//총을 쏠겁니다.
		count = 0.0f;
		Bullet* b = new Bullet(L"image/Bullet2.png");
		b->pos.x = pos.x;
		b->pos.y = pos.y;
		b->speed = 1500.0f;
		b->angle = 0.75f;
		b->master = this;
		scene->PushBackGameObject(b);
	}
}

bool Player::Hit(int damage)
{
	hp -= damage;
	if (hp <= 0)
		return true;
	else
		return false;
}