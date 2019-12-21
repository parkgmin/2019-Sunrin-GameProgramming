#pragma once
#include "GameObject.h"

typedef D2D_POINT_2F Point;

class GameObject;

class BoxCollider
{
public:
	GameObject* target;
	float left;
	float right;
	float top;
	float bottom;
	BoxCollider(GameObject* target);
	~BoxCollider();

	bool Intersected(const float x, const float y) const;
	bool Intersected(const Point &p) const;
	bool Intersected(const BoxCollider &r) const;
};

