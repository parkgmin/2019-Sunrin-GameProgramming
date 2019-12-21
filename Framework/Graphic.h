#pragma once
#include "Scene.h"
#include "Application.h"
#define FWGRAPHIC Graphic::GetInstance()

class Scene;
class GameObject;
class Application;
struct Sprite;

class Graphic
{
	friend class Scene;
	friend class Application;
public:
	Graphic();
	~Graphic();

public:
	static Graphic* GetInstance();
	static HRESULT LoadBitmapFromFile(Scene* scene, PCWSTR uri, UINT destinationWidth, UINT destinationHeight, Sprite*& sprite);

public:
	HRESULT CreateDeviceIndependentResources();
	HRESULT CreateDeviceResources();
	void DiscardDeviceResources();
	void DiscardDeviceIndependentResources();

	int GetWidth();
	int GetHeight();

private:
	HRESULT Render();
	bool DrawGameObject(GameObject*& o);


private:
	Application* app;
	ID2D1Factory* factory;
	IWICImagingFactory* wicFactory;
	ID2D1HwndRenderTarget* renderTarget;
	int width, height;
	bool isFullScreen;

};

