#include "model.h"
#include "globals.h"

Model::Model(ID3D11Device* device)
{
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	
	m_vertexCount = 3;
	m_indexCount = 3;
	
	VertexType vertices[3];
	UINT indices[3];

	vertices[0].position = XMFLOAT3(-0.5f, -0.5f, 0.0f);
	vertices[0].color = XMFLOAT3(0.5f, 0.5f, 0.0f);

	vertices[1].position = XMFLOAT3(0.0f, 0.5f, 0.0f);
	vertices[1].color = XMFLOAT3(0.5f, 1.0f, 0.0f);

	vertices[2].position = XMFLOAT3(0.5f, -0.5f, 0.0f);
	vertices[2].color = XMFLOAT3(0.0f, 0.5f, 0.5f);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	STRUCT(D3D11_SUBRESOURCE_DATA, vertexData);
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.ByteWidth = sizeof(indices);
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	STRUCT(D3D11_SUBRESOURCE_DATA, indexData);
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	VERIFY(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer));
	VERIFY(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer));
}

Model::~Model()
{
	m_vertexBuffer->Release();
	m_indexBuffer->Release();
}

void Model::Render(ID3D11DeviceContext* context)
{
	UINT stride = sizeof(VertexType);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

int Model::GetVertexCount()
{
	return m_vertexCount;
}

int Model::GetIndexCount()
{
	return m_indexCount;
}
