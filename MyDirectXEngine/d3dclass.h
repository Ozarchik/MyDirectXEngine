#pragma once

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <memory>
#include <string>

class D3DClass
{
public:
	D3DClass(
		HWND hwnd,
		int screenWidth,
		int screenHeight,
		int screenNear,
		int screenFar,
		bool vsync,
		bool fullscreen
	);
	~D3DClass();

	void BeginScene(float x, float y, float z, float w);
	void EndScene();

	ID3D11Device* getDevice() const;
	ID3D11DeviceContext* getDeviceContext() const;

	const DirectX::XMMATRIX& getWorldMatrix() const;
	const DirectX::XMMATRIX& getProjMatrix() const;
	const DirectX::XMMATRIX& getOrthoMatrix() const;

	int GetGPUMemory() const;
	std::string GetGPUName() const;


private:
	bool m_vsync;
	int m_gpuMemory;
	char m_gpuDescription[128];

	IDXGISwapChain* m_swapChain = nullptr;
	ID3D11Device* m_device = nullptr;
	ID3D11DeviceContext* m_deviceContext = nullptr;
	ID3D11RenderTargetView* m_renderTargetView = nullptr;
	ID3D11Texture2D* m_depthStencilBuffer = nullptr;
	ID3D11DepthStencilState* m_depthStencilState = nullptr;
	ID3D11DepthStencilView* m_depthStencilView = nullptr;
	ID3D11RasterizerState* m_rasterState = nullptr;
	D3D11_VIEWPORT m_viewport;

	DirectX::XMMATRIX m_worldMatrix;
	DirectX::XMMATRIX m_projMatrix;
	DirectX::XMMATRIX m_orthoMatrix;
};

