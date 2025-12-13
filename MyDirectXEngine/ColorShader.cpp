#include "ColorShader.h"
#include <cstring>
#include <d3dcompiler.h>

ColorShader::ColorShader(ID3D11Device* device, HWND hwnd, WCHAR*, WCHAR*)
{
	wchar_t vsFilename[128];
	wchar_t psFilename[128];

	VERIFY(wcscpy_s(vsFilename, 128, L"./color.vs"));
	VERIFY(wcscpy_s(psFilename, 128, L"./color.ps"));

	ID3D10Blob* errors;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	UINT numElements = 2;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[numElements];
	D3D11_BUFFER_DESC matrixDesc;


	VERIFY(D3DCompileFromFile(
		vsFilename,
		NULL,
		NULL,
		"ColorVertexShader",
		"vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		&vertexShaderBuffer,
		&errors
	));

	VERIFY(D3DCompileFromFile(
		psFilename,
		NULL,
		NULL,
		"ColorPixelShader",
		"ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		&pixelShaderBuffer,
		&errors
	));

	VERIFY(device->CreateVertexShader(
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(),
		NULL,
		&m_vertexShader
	));

	VERIFY(device->CreatePixelShader(
		pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(),
		NULL,
		&m_pixelShader
	));

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	VERIFY(device->CreateInputLayout(
		polygonLayout,
		numElements,
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(),
		&m_layout
	));

	vertexShaderBuffer->Release();
	pixelShaderBuffer->Release();

	matrixDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixDesc.ByteWidth = sizeof(matrixDesc);
	matrixDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixDesc.MiscFlags = 0;
	matrixDesc.StructureByteStride = 0;

	VERIFY(device->CreateBuffer(
		&matrixDesc,
		NULL,
		&m_matrix
	));


}

ColorShader::~ColorShader()
{
	m_matrix->Release();
	m_layout->Release();
	m_vertexShader->Release();
	m_pixelShader->Release();
}

bool ColorShader::Render(ID3D11DeviceContext* context, int indexCount)
{
	context->IASetInputLayout(m_layout);
	context->VSSetShader(m_vertexShader, NULL, 0);
	context->PSSetShader(m_pixelShader, NULL, 0);
	context->DrawIndexed(indexCount, 0, 0);
	return true;
}

bool ColorShader::SetParameter(ID3D11DeviceContext* context, XMMATRIX world, XMMATRIX view, XMMATRIX projection)
{
	world = XMMatrixTranspose(world);
	view = XMMatrixTranspose(view);
	projection = XMMatrixTranspose(projection);

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	VERIFY(context->Map(
		m_matrix,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedResource
	));

	Matrix* dataPtr = (Matrix*)mappedResource.pData;
	dataPtr->world = world;
	dataPtr->view = view;
	dataPtr->projection = projection;

	context->Unmap(m_matrix, 0);
	context->VSSetConstantBuffers(0, 1, &m_matrix);

	return true;
}
