#pragma once
#include "Scene.h"
#include "BoxCollider.h"
#include "Animation.h"

class Scene;
class BoxCollider;
class Animation;

struct Sprite;

typedef D2D_POINT_2F Point;
typedef D2D_RECT_F Rect;
typedef D2D_COLOR_F Color;
struct Rotation{
	float angle;	//Degree를 사용합니다. Radian이 아닙니다.
	Point pivot;	//회전 중심
};

class GameObject
{
public:
	GameObject();
	GameObject(const wchar_t* uri);
	virtual ~GameObject();

	virtual void OnUpdate();
	virtual void OnLateUpdate();
	virtual void OnDestroy();

	void SetPosition(float x, float y);
	void SetRotation(float angle, float pivotX, float pivotY);	//degree
	void SetScale(float x, float y);
	void SetColliderByScale();	//스케일 변화시 콜라이더 조정

public:
	Point pos;
	Rotation rot;
	Point scale;
	Point size;   //이미지 사이즈
	string name;
	BoxCollider* col;
	Sprite* sprite;
	float alpha;
	bool _isDestroyed;
	bool isActive;
	Scene* scene;

public:
	Animation* currentAnimation;
	void SwapAnimation(Animation* nextAnim);
	void UpdateAnimation();

};

