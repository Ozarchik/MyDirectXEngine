#include "Application.h"

Application::Application(HWND hwnd, int screenWidth, int screenHeight)
{
	m_directx = std::make_unique<D3DClass>(
		hwnd,
		screenWidth,
		screenHeight,
		SCREEN_NEAR,
		SCREEN_FAR,
		VSYNC,
		FULLSCREEN		
	);
}

void Application::Render()
{
	m_directx->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	m_directx->EndScene();
}
