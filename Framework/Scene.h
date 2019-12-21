#pragma once
#include "pch.h"
#include "Graphic.h"
#include "GameObject.h"

using namespace std;

class Graphic;
class GameObject;
class Application;


struct Sprite
{
	const wchar_t* uri;
	ID2D1Bitmap* bitmap;
};

class Scene
{
	friend class Graphic;
	friend class Application;
private:
	Graphic* grp;
	Application* app;

public:
	list<Sprite*> bitmapList;	
	list<GameObject*> gameObjectList;
	list<GameObject*> uiList;

public:
	Scene();
	virtual ~Scene();

	void PushBackGameObject(GameObject* g);
	void PushBackUI(GameObject* ui);

	void Destroy(GameObject* o);

protected:
	virtual void Initialize();

private:
	void Update();
	void LateUpdate();
	
};

