#include "D3DClass.h"
#include <DirectXMath.h>
#include "globals.h"


D3DClass::D3DClass(HWND hwnd, int screenWidth, int screenHeight, int screenNear, int screenFar, bool vsync, bool fullscreen)
	: m_vsync(vsync)
{
	IDXGIFactory* factory = nullptr;
	if (FAILED(CreateDXGIFactory(
		__uuidof(IDXGIFactory),
		(void**)&factory))
	) {
		throw "Failed to create IDXGIFactory";
	}

	IDXGIAdapter* adapter = nullptr;
	if (FAILED(factory->EnumAdapters(0, &adapter))) {
		throw "Failed to get adapter";
	}

	IDXGIOutput* adapterOutput = nullptr;
	if (FAILED(adapter->EnumOutputs(0, &adapterOutput))) {
		throw "Failed to get adapter outputs";
	}

	UINT numModes = 0;
	if (FAILED(adapterOutput->GetDisplayModeList(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED,
		&numModes,
		NULL
	))) {
		throw "Failed to get display mode list";
	}

	std::unique_ptr<DXGI_MODE_DESC[]> displayList = std::make_unique<DXGI_MODE_DESC[]>(numModes);
	if (FAILED(adapterOutput->GetDisplayModeList(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED,
		&numModes,
		displayList.get()
	))) {
		throw "Failed to get display mode list";
	}

	UINT numerator = 0;
	UINT denominator = 0;
	for (UINT i = 0; i < numModes; i++) {
		DXGI_MODE_DESC display = displayList.get()[i];
		if (display.Width == screenWidth && display.Height == screenHeight) {
			numerator = display.RefreshRate.Numerator;
			denominator = display.RefreshRate.Denominator;
			break;
		}
	}

	DXGI_ADAPTER_DESC adapterDesc;
	if (FAILED(adapter->GetDesc(&adapterDesc))) {
		throw "Failed to get adapter description";
	}

	m_gpuMemory = adapterDesc.DedicatedVideoMemory/1024/1024;
	size_t stringLength = 0;
	VERIFY(wcstombs_s(&stringLength, m_gpuDescription, size_t(128), adapterDesc.Description, size_t(128)),
		"Failed to get name of GPU"
	);

	adapterOutput->Release();
	adapter->Release();
	factory->Release();

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = m_vsync ? numerator : 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = m_vsync ? denominator : 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = !fullscreen ? true : false;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.Flags = 0;

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	
	VERIFY(D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		&featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&m_swapChain,
		&m_device,
		NULL,
		&m_deviceContext
	), "Failed to create device and swap chain");

	ID3D11Texture2D* backBuffer = nullptr;
	VERIFY(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer),
		"Failed to get back buffer of swap chain"
	);

	VERIFY(m_device->CreateRenderTargetView(backBuffer, NULL, &m_renderTargetView),
		"Failed to create Render target view"
	);
	backBuffer->Release();

	D3D11_TEXTURE2D_DESC depthDesc;
	ZeroMemory(&depthDesc, sizeof(D3D11_TEXTURE2D_DESC));

	depthDesc.Width = screenWidth;
	depthDesc.Height = screenHeight;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;

	VERIFY(m_device->CreateTexture2D(&depthDesc, NULL, &m_depthStencilBuffer),
		"Failed to create 2D texture"
	);

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xff;
	depthStencilDesc.StencilWriteMask = 0xff;
	
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	VERIFY(m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState));
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	STRUCT(D3D11_DEPTH_STENCIL_VIEW_DESC, depthStencilViewDesc);
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	VERIFY(m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView));
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	STRUCT(D3D11_RASTERIZER_DESC, rasterizerDesc);
	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	VERIFY(m_device->CreateRasterizerState(&rasterizerDesc, &m_rasterState),
		"Failed to create rasterizer state"
	);
	m_deviceContext->RSSetState(m_rasterState);

	m_viewport.Width = static_cast<FLOAT>(screenWidth);
	m_viewport.Height = static_cast<FLOAT>(screenHeight);
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;
	m_deviceContext->RSSetViewports(1, &m_viewport);

	float fieldofView = DirectX::XM_PIDIV4;
	float screenAspect = static_cast<FLOAT>(screenWidth)/screenHeight;

	m_projMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldofView, screenAspect, screenNear, screenFar);
	m_worldMatrix = DirectX::XMMatrixIdentity();
	m_orthoMatrix = DirectX::XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenFar);
}

D3DClass::~D3DClass()
{
	m_rasterState->Release();
	m_depthStencilView->Release();
	m_depthStencilState->Release();
	m_depthStencilBuffer->Release();
	m_renderTargetView->Release();
	m_deviceContext->Release();
	m_device->Release();
	m_swapChain->Release();
}

void D3DClass::BeginScene(float x, float y, float z, float w)
{
	float color[4]{ 1.0, 0.7, 0.5, 1.0 };
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3DClass::EndScene()
{
	if (m_vsync) {
		m_swapChain->Present(1, 0);
	} else {
		m_swapChain->Present(0, 0);
	}
}

ID3D11Device* D3DClass::getDevice() const
{
	return m_device;
}

ID3D11DeviceContext* D3DClass::getDeviceContext() const
{
	return m_deviceContext;
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

int D3DClass::GetGPUMemory() const
{
	return m_gpuMemory;
}

std::string D3DClass::GetGPUName() const
{
	std::string result(m_gpuDescription, sizeof(m_gpuDescription));
	return result;
}

void D3DClass::setFullscreen(bool state)
{
	m_swapChain->SetFullscreenState(state, NULL);
}


