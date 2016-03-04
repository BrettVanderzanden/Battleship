#pragma once
class D2DRenderer
{
private:
	static D2DRenderer* pInstance;

public:
	D2DRenderer();
	~D2DRenderer();
	static D2DRenderer* GetInstance();

	HRESULT CreateGraphics(HWND hWnd);
	void DestroyGraphics();

#define D2DColor(clr) (D2D1::ColorF(D2D1::ColorF::clr))

	HWND MainhWnd;

	// The SafeRelease Pattern
	template <class T> void SafeRelease(T **ppT)
	{
		if (*ppT)
		{
			(*ppT)->Release();
			*ppT = NULL;
		}
	}

	// Convert COLORREF to D2D1_COLOR_F
	template <class T> D2D1_COLOR_F COLORREF_to_D2D1_COLOR_F(T cr)
	{
		return D2D1::ColorF((FLOAT)GetRValue(cr) / 255.0f,
			(FLOAT)GetGValue(cr) / 255.0f,
			(FLOAT)GetBValue(cr) / 255.0f,
			1.0);
	}

	// Convert D2D1_COLOR_F to COLORREF 
	template <class T> COLORREF D2D1_COLOR_F_to_COLORREF(T cf)
	{
		return RGB(cf.r * 255.0,
			cf.g * 255.0,
			cf.b * 255.0);
	}

	// DirectX Interfaces:
	ID2D1Factory* pD2DFactory = NULL;
	ID2D1HwndRenderTarget* pRT = NULL;

	// brushes
	ID2D1SolidColorBrush *redBrush = NULL;
	ID2D1SolidColorBrush *blueBrush = NULL;
	ID2D1SolidColorBrush *blackBrush = NULL;
	ID2D1SolidColorBrush *grayBrush = NULL;
	ID2D1SolidColorBrush *darkGrayBrush = NULL;
	ID2D1SolidColorBrush *whiteBrush = NULL;

	D2D1_COLOR_F bkgndColor = D2DColor(CornflowerBlue);

	IDWriteFactory* pDWFactory = NULL;
	IDWriteTextFormat* pTitleText = NULL;
	IDWriteTextFormat* pInfoText = NULL;
	IDWriteTextFormat* pSmallText = NULL;
};

