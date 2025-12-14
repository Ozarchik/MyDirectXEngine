#pragma once

#include <DirectXMath.h>
using namespace DirectX;

class Camera
{
public:
	Camera();
	~Camera();

	XMMATRIX GetViewMatrix();
	void Update();

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	XMFLOAT3 GetPosition() const;
	XMFLOAT3 GetRotation() const;

private:
	float m_posX, m_posY, m_posZ;
	float m_rotX, m_rotY, m_rotZ;
	XMMATRIX m_viewMatrix;
};

