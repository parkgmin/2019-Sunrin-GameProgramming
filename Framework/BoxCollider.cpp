#include "pch.h"
#include "BoxCollider.h"


BoxCollider::BoxCollider(GameObject* target) : top(0.0f), bottom(0.0f), left(0.0f), right(0.0f), target(target)
{
}


BoxCollider::~BoxCollider()
{
}

bool BoxCollider::Intersected(const float x, const float y) const
{
	return target->pos.x+left<x &&
		target->pos.x + right>x &&
		target->pos.y + top < y &&
		target->pos.y + bottom>y;
}

bool BoxCollider::Intersected(const Point & p) const
{
	
	return 
		target->pos.x + left<p.x &&
		target->pos.x + right>p.x &&
		target->pos.y + top < p.y &&
		target->pos.y + bottom>p.y;
}

bool BoxCollider::Intersected(const BoxCollider & r) const
{
	return
		target->pos.x + left < r.target->pos.x+r.right &&
		target->pos.x + right > r.target->pos.x+r.left &&
		target->pos.y + top < r.target->pos.y+r.bottom &&
		target->pos.y + bottom >r.target->pos.y+r.top;
}
