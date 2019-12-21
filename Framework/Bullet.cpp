#include "pch.h"
#include "Bullet.h"
#include "GameManager.h"


Bullet::Bullet(const wchar_t* uri)
	: GameObject(uri), angle(0.0f),
	speed(0.0f), speedRate(0.0f),
	angleRate(0.0f)
{
}


Bullet::~Bullet()
{
}


void Bullet::OnUpdate()
{
	Move();
}

		void Bullet::OnLateUpdate()
		{
			if (master == GAMEMGR->player)
			{
				//발사한 객체가 플레이어라면?
				list<Enemy*>::iterator i;
				for (i = GAMEMGR->enemyList.begin();
					i != GAMEMGR->enemyList.end();
					i++)
				{
					if ((*i)->col->Intersected(pos))
					{
						if ((*i)->Hit(1))
						{
							scene->Destroy(*i);
							i = GAMEMGR->enemyList.erase(i);
							if (i == GAMEMGR->enemyList.end())
								break;
						}
						scene->Destroy(this);								
					}
				}
			}
			if (master == GAMEMGR->enemy)
			{
				//발사한 객체가 플레이어라면?
				list<Player*>::iterator e;
				for (e = GAMEMGR->PlayerList.begin();
					e != GAMEMGR->PlayerList.end();
					e++)
				{
					if ((*e)->col->Intersected(pos))
					{
						if ((*e)->Hit(1))
						{
							scene->Destroy(*e);
							e = GAMEMGR->PlayerList.erase(e);
							if (e == GAMEMGR->PlayerList.end())
								break;
						}
						scene->Destroy(this);
					}
				}
			}
		}

		void Bullet::Move()
		{
			float rad = PI * angle * 2.0f;

	pos.x += (speed 
		* cosf(rad) 
		* FWAPP->DeltaTime());
	pos.y += (speed
		* sinf(rad)
		* FWAPP->DeltaTime());

	angle += angleRate;
	speed += speedRate;
	//printf("%f\n", rad);

	if (CheckOutOfScreen())
	{
		scene->Destroy(this);
	}

}

bool Bullet::CheckOutOfScreen()
{
	D2D1_SIZE_U size
		= sprite->bitmap->GetPixelSize();

	return pos.x < scale.x*size.width*(-1.0f) ||
		pos.x > FWGRAPHIC->GetWidth() + size.width*scale.x ||
		pos.y < scale.y*size.height*(-1.0f) ||
		pos.y > FWGRAPHIC->GetHeight() + size.height*scale.y;
}

void Bullet::OnDestroy()
{
	//printf("총알 삭제됨\n");
}