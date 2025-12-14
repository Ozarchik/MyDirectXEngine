#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

class Model
{
	struct VertexType {
		XMFLOAT3 position;
		XMFLOAT3 color;
	};

public:
	Model(ID3D11Device* device);
	~Model();

	void Render(ID3D11DeviceContext* context);
	int GetVertexCount();
	int GetIndexCount();

private:
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
	int m_vertexCount;
	int m_indexCount;
};

