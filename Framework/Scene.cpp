#include "pch.h"
#include "Scene.h"
#include "Application.h"


Scene::Scene()
{
	app = Application::GetInstance();
	grp = Graphic::GetInstance();
}


Scene::~Scene()
{
	for (auto& i : bitmapList)
	{
		i->bitmap->Release();
		SAFE_DELETE(i);
	}
	bitmapList.clear();

	for (auto& i : gameObjectList)
	{
		SAFE_DELETE(i);
	}
	gameObjectList.clear();

	for (auto& i : uiList)
	{
		SAFE_DELETE(i);
	}
	uiList.clear();

	grp->DiscardDeviceResources();
}

void Scene::Update()
{
	for (auto& i : gameObjectList)
	{
		if (i->isActive)
			i->OnUpdate();
	}
	for (auto& i : uiList)
	{
		if (i->isActive)
			i->OnUpdate();
	}
	for (auto i = gameObjectList.begin(); i != gameObjectList.end(); ++i)
	{
		if ((*i)->_isDestroyed)
		{
			(*i)->OnDestroy();
			delete *i;
			i = gameObjectList.erase(i);
			if (i == gameObjectList.end())
			{
				break;
			}
		}
	}
	for (auto i = uiList.begin(); i != uiList.end(); ++i)
	{
		if ((*i)->_isDestroyed)
		{
			(*i)->OnDestroy();
			delete *i;
			i = uiList.erase(i);
			if (i == uiList.end())
			{
				break;
			}
		}
	}
}

void Scene::LateUpdate()
{
	for (auto& i : gameObjectList)
	{
		if (i->isActive)
			i->OnLateUpdate();
	}
	for (auto& i : uiList)
	{
		if (i->isActive)
			i->OnLateUpdate();
	}
}

void Scene::PushBackGameObject(GameObject * g)
{
	g->scene = this;
	gameObjectList.push_back(g);
}

void Scene::PushBackUI(GameObject * ui)
{
	ui->scene = this;
	uiList.push_back(ui);
}

void Scene::Destroy(GameObject * o)
{
	o->_isDestroyed = true;
}

void Scene::Initialize()
{
}



