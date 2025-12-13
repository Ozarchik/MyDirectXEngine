#pragma once

#include <DirectXMath.h>
#include <d3d11.h>
#include "globals.h"

using namespace DirectX;

class ColorShader
{
	struct Matrix {
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
	ColorShader(
		ID3D11Device* device,
		HWND hwnd,
		WCHAR*,
		WCHAR*
	);
	~ColorShader();

	bool Render(
		ID3D11DeviceContext* context,
		int indexCount
	);

	bool SetParameter(
		ID3D11DeviceContext* context,
		XMMATRIX world,
		XMMATRIX view,
		XMMATRIX projection
	);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrix;
};

