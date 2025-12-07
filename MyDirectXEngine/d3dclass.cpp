#include "D3DClass.h"

D3DClass::D3DClass(HWND hwnd, int screenWidth, int screenHeight, int screenNear, int screenFar, bool vsync, bool fullscreen)
{

}

D3DClass::~D3DClass()
{
}

void D3DClass::BeginScene(float x, float y, float z, float w)
{

}

void D3DClass::EndScene()
{

}

ID3D11Device* D3DClass::getDevice() const
{
	return nullptr;
}

ID3D11DeviceContext* D3DClass::getDeviceContext() const
{
	return nullptr;
}

const DirectX::XMMATRIX& D3DClass::getWorldMatrix() const
{
	return m_worldMatrix;
}

const DirectX::XMMATRIX& D3DClass::getProjMatrix() const
{
	return m_projMatrix;
}

const DirectX::XMMATRIX& D3DClass::getOrthoMatrix() const
{
	return m_orthoMatrix;
}