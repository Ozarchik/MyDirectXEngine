#include "Graphics.h"
#include "globals.h"

Graphics::Graphics(HWND hwnd, int screenWidth, int screenHeight)
	: m_hwnd(hwnd)
	, m_screenWidth(screenWidth)
	, m_screenHeight(screenHeight)
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

	m_shader = std::make_unique<ColorShader>(m_directx->getDevice(), hwnd, nullptr, nullptr);

	m_camera = std::make_unique<Camera>();
	m_camera->SetPosition(0.0f, 0.0f, -2.0f);
	m_camera->Update();

	m_model = std::make_unique<Model>(m_directx->getDevice());
}

Graphics::~Graphics()
{
}

void Graphics::Render()
{
	m_directx->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	m_camera->Update();

	XMMATRIX world = m_directx->getWorldMatrix();
	XMMATRIX view = m_camera->GetViewMatrix();
	XMMATRIX projection = m_directx->getProjMatrix();

	m_model->Render(m_directx->getDeviceContext());
	m_shader->SetParameter(m_directx->getDeviceContext(), world, view, projection);
	m_shader->Render(m_directx->getDeviceContext(), m_model->GetIndexCount());

	m_directx->EndScene();
}

void Graphics::setFullscreen(bool state)
{
	m_directx->setFullscreen(state);
}
