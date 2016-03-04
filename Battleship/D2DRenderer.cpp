#include "stdafx.h"
#include "D2DRenderer.h"

D2DRenderer* D2DRenderer::pInstance = 0;

D2DRenderer*  D2DRenderer::GetInstance()
{
	if (!pInstance)
	{
		pInstance = new D2DRenderer();
	}
	return pInstance;
}

D2DRenderer::D2DRenderer()
{
}

D2DRenderer::~D2DRenderer()
{
	delete pInstance;
}

HRESULT D2DRenderer::CreateGraphics(HWND hWnd)
{
	MainhWnd = hWnd;
	// Initialize the Direct2D Factory.
	HRESULT hr;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the Direct2D Factory."),
			_T("Direct2D Error"), MB_OK | MB_ICONERROR);
		return hr;
	}

	// Get the dimensions of the client.
	RECT rc;
	GetClientRect(hWnd, &rc);

	// Initialize a Direct2D Size Structure.
	D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);


	// Create the Direct2D Render Target.
	hr = pD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hWnd, size), &pRT);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the Direct2D Render Target."),
			_T("Direct2D Error"), MB_OK | MB_ICONERROR);
		return hr;
	}

	// Set Dots Per Inch
	pRT->SetDpi(96.0f, 96.0f);

	// Create Brushes
	if (FAILED(pRT->CreateSolidColorBrush(D2DColor(Red), &redBrush)))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the Direct2D Solid Color Brush."),
			_T("Direct2D Error"), MB_OK | MB_ICONERROR);
		return hr;
	}
	if (FAILED(pRT->CreateSolidColorBrush(D2DColor(Blue), &blueBrush)))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the Direct2D Solid Color Brush."),
			_T("Direct2D Error"), MB_OK | MB_ICONERROR);
		return hr;
	}
	if (FAILED(pRT->CreateSolidColorBrush(D2DColor(CadetBlue), &cadetBlueBrush)))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the Direct2D Solid Color Brush."),
			_T("Direct2D Error"), MB_OK | MB_ICONERROR);
		return hr;
	}
	if (FAILED(pRT->CreateSolidColorBrush(D2DColor(Black), &blackBrush)))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the Direct2D Solid Color Brush."),
			_T("Direct2D Error"), MB_OK | MB_ICONERROR);
		return hr;
	}
	if (FAILED(pRT->CreateSolidColorBrush(D2DColor(Gray), &grayBrush)))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the Direct2D Solid Color Brush."),
			_T("Direct2D Error"), MB_OK | MB_ICONERROR);
		return hr;
	}
	if (FAILED(pRT->CreateSolidColorBrush(D2DColor(LightGray), &darkGrayBrush)))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the Direct2D Solid Color Brush."),
			_T("Direct2D Error"), MB_OK | MB_ICONERROR);
		return hr;
	}
	if (FAILED(pRT->CreateSolidColorBrush(D2DColor(White), &whiteBrush)))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the Direct2D Solid Color Brush."),
			_T("Direct2D Error"), MB_OK | MB_ICONERROR);
		return hr;
	}


	// Initialize the DirectWrite Factory.
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
		(IUnknown**)&pDWFactory);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the DirectWrite Factory."),
			_T("DirectWrite Error"), MB_OK | MB_ICONERROR);
		return hr;
	}

	// Create the DirectWrite Text Formats
	hr = pDWFactory->CreateTextFormat(_T("Veranda"), NULL,
		DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		42, _T(""), &pTitleText);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the DirectWrite Text Format."),
			_T("DirectWrite Error"), MB_OK | MB_ICONERROR);
		return hr;
	}

	pTitleText->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

	hr = pDWFactory->CreateTextFormat(_T("Veranda"), NULL,
		DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		22, _T(""), &pInfoText);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the DirectWrite Text Format."),
			_T("DirectWrite Error"), MB_OK | MB_ICONERROR);
		return hr;
	}

	pInfoText->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

	hr = pDWFactory->CreateTextFormat(_T("Veranda"), NULL,
		DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		16, _T(""), &pSmallText);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the DirectWrite Text Format."),
			_T("DirectWrite Error"), MB_OK | MB_ICONERROR);
		return hr;
	}

	pSmallText->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

	return S_OK;
}

void D2DRenderer::DestroyGraphics()
{
	// Release objects in opposite order that they are declared

	// Release the TextFormat
	SafeRelease(&pSmallText);
	SafeRelease(&pInfoText);
	SafeRelease(&pTitleText);

	// Release the DirectWrite Factory
	SafeRelease(&pDWFactory);

	// Release the Brushes
	SafeRelease(&whiteBrush);
	SafeRelease(&blackBrush);
	SafeRelease(&darkGrayBrush);
	SafeRelease(&grayBrush);
	SafeRelease(&redBrush);
	SafeRelease(&blueBrush);
	SafeRelease(&cadetBlueBrush);

	// Release the RenderTarget
	SafeRelease(&pRT);

	// Release the Direct2D Factory
	SafeRelease(&pD2DFactory);
}
