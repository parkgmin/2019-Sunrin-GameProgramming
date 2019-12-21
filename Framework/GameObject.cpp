#include "pch.h"
#include "GameObject.h"
#include "Application.h"


GameObject::GameObject()
	: pos({ 0.0f,0.0f }), rot({ 0,0.0f,0.0f }), scale({ 1.0f,1.0f }), alpha(1.0f), isActive(true), sprite(nullptr)
{
	Application* app = Application::GetInstance();
	col = new BoxCollider(this);
}

GameObject::GameObject(const wchar_t* uri)
	: pos({ 0.0f,0.0f }), rot({ 0,0.0f,0.0f }), scale({ 1.0f,1.0f }), alpha(1.0f), isActive(true), sprite(nullptr)
{
	Application* app = Application::GetInstance();
	col = new BoxCollider(this);
	if (FAILED(Graphic::LoadBitmapFromFile(FWAPP->GetCurrentScene(), uri, 0, 0, sprite)))
	{
		printf("게임오브젝트 이미지 로드 오류: %ws확인바람\n", uri);
	}
	SetColliderByScale();
}


GameObject::~GameObject()
{
	SAFE_DELETE(col);
}

void GameObject::OnUpdate()
{
}

void GameObject::OnLateUpdate()
{
}

void GameObject::OnDestroy()
{
}

void GameObject::SetPosition(float x, float y)
{
	pos.x = x;
	pos.y = y;
}

void GameObject::SetRotation(float angle, float pivotX, float pivotY)
{
	rot.angle = angle;
	rot.pivot.x = pivotX;
	rot.pivot.y = pivotY;
}

void GameObject::SetScale(float x, float y)
{
	scale.x = x;
	scale.y = y;
	SetColliderByScale();
}

void GameObject::SetColliderByScale()
{
	if (this->sprite != nullptr && this->sprite->bitmap != nullptr)
	{
		D2D_SIZE_U size = this->sprite->bitmap->GetPixelSize();
		this->col->left = size.width*0.5f * this->scale.x * (-1.0f);
		this->col->top = size.height*0.5f * this->scale.y * (-1.0f);
		this->col->right = size.width*0.5f * this->scale.x;
		this->col->bottom = size.height*0.5f * this->scale.y;
	}
	else
	{
		printf("콜라이더 크기 조정 실패, 객체의 이미지가 없습니다.\n");
	}
}


void GameObject::SwapAnimation(Animation* animation)
{
	if (animation) 
	{
		currentAnimation = animation;
		currentAnimation->currentSprite = currentAnimation->sprites.begin();
		this->sprite = *(currentAnimation->currentSprite);
	}
	else
	{
		printf("애니메이션이 없습니다.\n");
	}
}

void GameObject::UpdateAnimation()
{
	if(currentAnimation)
		currentAnimation->UpdateAnimation();
	else
		printf("애니메이션이 없습니다.\n");
}
