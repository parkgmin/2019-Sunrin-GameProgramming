#pragma once
#include "Scene.h"
struct Sprite;
class GameObject;
class Animation
{
public:
	GameObject* target;
	list<Sprite*> sprites;
	list<Sprite*>::iterator currentSprite;
	float delay;
	float timer;
public:
	Animation(GameObject* target, float delay);
	~Animation();
	void PushBackSprite(const wchar_t* uri);
	void UpdateAnimation();
};

