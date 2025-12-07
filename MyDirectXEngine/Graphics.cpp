#include "Graphics.h"

Graphics::Graphics(HWND hwnd, int screenWidth, int screenHeight)
	: m_hwnd(hwnd)
	, m_screenWidth(screenWidth)
	, m_screenHeight(screenHeight)
{
	m_directx = std::make_unique<D3DClass>(
		m_hwnd,
		m_screenWidth,
		m_screenHeight,
		m_screenNear,
		m_screenFar,
		m_vsync,
		m_fullscreen
	);
}

Graphics::~Graphics()
{
}

void Graphics::Render()
{
	m_directx->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	m_directx->EndScene();
}
