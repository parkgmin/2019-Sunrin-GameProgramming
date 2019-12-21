#include "pch.h"
#include "Graphic.h"
#include "Application.h"


Graphic::Graphic() : factory(NULL), wicFactory(NULL), renderTarget(NULL)
{
	app = Application::GetInstance();
}


Graphic::~Graphic()
{
}

HRESULT Graphic::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;
	const D2D1_FACTORY_OPTIONS opts = { D2D1_DEBUG_LEVEL_INFORMATION };

	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory));

	if (SUCCEEDED(hr))
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);

	return hr;
}

HRESULT Graphic::CreateDeviceResources()
{
	HRESULT hr = S_OK;

	if (!renderTarget)
	{
		RECT rc;
		GetClientRect(app->hwnd, &rc);
		D2D1_SIZE_U size;
		if (!isFullScreen)
			size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
		else
			size = D2D1::SizeU(width, height);
		hr = factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(app->hwnd, size), &renderTarget);

	}
	return hr;
}

void Graphic::DiscardDeviceResources()
{
	SAFE_RELEASE(renderTarget);
}

void Graphic::DiscardDeviceIndependentResources()
{
	SAFE_RELEASE(factory);
	SAFE_RELEASE(wicFactory);
	SAFE_RELEASE(renderTarget);
}

HRESULT Graphic::Render()
{
	HRESULT hr = S_OK;
	hr = CreateDeviceResources();

	if (SUCCEEDED(hr))
	{
		//D2D1_SIZE_F renderTargetSize = ID2D1HwndRenderTarget_GetSize(app->renderTarget);
		D2D1_MATRIX_3X2_F identity = { 1.0f, 0.0f,0.0f,1.0f,0.0f,0.0f };
		D2D1_COLOR_F white = { 1.0f, 1.0f, 1.0f, 1.0f };

		renderTarget->BeginDraw();
		renderTarget->SetTransform(&identity);
		renderTarget->Clear(&white);

		//그리기
		for (auto& i : app->currentScene->gameObjectList) {
			DrawGameObject(i);
		}
		for (auto& i : app->currentScene->uiList) {
			DrawGameObject(i);
		}

		hr = renderTarget->EndDraw(NULL, NULL);
		//printf("%x rt:%p \n", hr,renderTarget);
	}
	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}
	return hr;
}

bool Graphic::DrawGameObject(GameObject*& o)
{
	if (o->sprite != nullptr&&o->sprite->bitmap != nullptr)
	{
		D2D1_SIZE_U size;
		size = o->sprite->bitmap->GetPixelSize();

		D2D1_RECT_F rect;
		rect.left = o->pos.x - size.width*0.5f;//* o->scale.x;
		rect.top = o->pos.y - size.height*0.5f;// *o->scale.y;
		rect.right = o->pos.x + size.width*0.5f;// *o->scale.x;
		rect.bottom = o->pos.y + size.height*0.5f;// *o->scale.y;
		renderTarget->SetTransform(D2D1::Matrix3x2F::Scale(o->scale.x, o->scale.y, o->pos)*D2D1::Matrix3x2F::Rotation(o->rot.angle, o->rot.pivot));
		renderTarget->DrawBitmap(o->sprite->bitmap, &rect, o->alpha, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
	}
	return true;
}

int Graphic::GetWidth()
{
	return width;
}

int Graphic::GetHeight()
{
	return height;
}


Graphic* Graphic::GetInstance()
{
	static Graphic g;
	return &g;
}

HRESULT Graphic::LoadBitmapFromFile(Scene* scene, PCWSTR uri, UINT destinationWidth, UINT destinationHeight, Sprite*& sprite)
{
	HRESULT hr = S_OK;

	Application* app = Application::GetInstance();
	Graphic* grp = Graphic::GetInstance();


	IWICBitmapDecoder* pDecoder = NULL;
	IWICBitmapFrameDecode* pSource = NULL;
	IWICStream* pStream = NULL;
	IWICFormatConverter* pConverter = NULL;
	IWICBitmapScaler* pScaler = NULL;

	if (sprite != nullptr)
		printf("에러 발생, sprite가 nullptr이 아닙니다.\n");

	for (auto& i : scene->bitmapList)
	{
		if (wcscmp(i->uri, uri) == 0)
		{
			/*printf("asdf");*/
			sprite = i;
			return S_OK;
		}
	}

	hr = grp->wicFactory->CreateDecoderFromFilename(uri, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);

	if (SUCCEEDED(hr))
	{
		// Create the initial frame.
		hr = pDecoder->GetFrame(0, &pSource);
	}

	if (SUCCEEDED(hr))
	{
		// Convert the image format to 32bppPBGRA
		// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
		hr = grp->wicFactory->CreateFormatConverter(&pConverter);
	}

	if (SUCCEEDED(hr))
	{
		// If a new width or height was specified, create an
		// IWICBitmapScaler and use it to resize the image.
		if (destinationWidth != 0 || destinationHeight != 0)
		{
			UINT originalWidth, originalHeight;
			hr = pSource->GetSize(&originalWidth, &originalHeight);
			if (SUCCEEDED(hr))
			{
				if (destinationWidth == 0)
				{
					FLOAT scalar = (FLOAT)destinationHeight / (FLOAT)originalHeight;
					destinationWidth = (UINT)(scalar * (FLOAT)(originalWidth));
				}
				else if (destinationHeight == 0)
				{
					FLOAT scalar = (FLOAT)(destinationWidth) / (FLOAT)(originalWidth);
					destinationHeight = (UINT)(scalar * (FLOAT)(originalHeight));
				}

				hr = grp->wicFactory->CreateBitmapScaler(&pScaler);
				if (SUCCEEDED(hr))
				{
					hr = pScaler->Initialize(pSource, destinationWidth, destinationHeight, WICBitmapInterpolationModeCubic);
				}

				if (SUCCEEDED(hr))
				{
					hr = pConverter->Initialize(
						pScaler,
						GUID_WICPixelFormat32bppPBGRA,
						WICBitmapDitherTypeNone,
						NULL,
						0.f,
						WICBitmapPaletteTypeMedianCut
					);
				}
			}
		}
		else // Don't scale the image.
		{
			hr = pConverter->Initialize(
				pSource,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeMedianCut
			);
		}
	}
	if (SUCCEEDED(hr))
	{
		sprite = new Sprite();
		// Create a Direct2D bitmap from the WIC bitmap.
		hr = grp->renderTarget->CreateBitmapFromWicBitmap(
			pConverter,
			NULL,
			&sprite->bitmap
		);

		sprite->uri = uri;

		scene->bitmapList.push_back(sprite);
	}

	SAFE_RELEASE(pDecoder);
	SAFE_RELEASE(pSource);
	SAFE_RELEASE(pStream);
	SAFE_RELEASE(pConverter);
	SAFE_RELEASE(pScaler);

	return hr;
}
