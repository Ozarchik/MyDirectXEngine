#include "camera.h"

Camera::Camera()
	: m_posX(0.0f)
	, m_posY(0.0f)
	, m_posZ(0.0f)
	, m_rotX(0.0f)
	, m_rotY(0.0f)
	, m_rotZ(0.0f)
{
}

Camera::~Camera()
{
}

XMMATRIX Camera::GetViewMatrix()
{
	return m_viewMatrix;
}

void Camera::Update()
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	up.x = 0;
	up.y = 1;
	up.z = 0;
	upVector = XMLoadFloat3(&up);
	
	position.x = m_posX;
	position.y = m_posY;
	position.z = m_posZ;
	positionVector = XMLoadFloat3(&position);
	
	lookAt.x = 0;
	lookAt.y = 0;
	lookAt.z = 1;
	lookAtVector = XMLoadFloat3(&lookAt);

	pitch = m_rotX * 0.0174532925f;
	yaw = m_rotY * 0.0174532925f;
	roll = m_rotZ * 0.0174532925f;

	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}

void Camera::SetPosition(float x, float y, float z)
{
	m_posX = x;
	m_posY = y;
	m_posZ = z;
}

void Camera::SetRotation(float x, float y, float z)
{
	m_rotX = x;
	m_rotY = y;
	m_rotZ = z;
}

XMFLOAT3 Camera::GetPosition() const
{
	return XMFLOAT3(m_posX, m_posY, m_posZ);
}

XMFLOAT3 Camera::GetRotation() const
{
	return XMFLOAT3(m_rotX, m_rotY, m_rotZ);
}
