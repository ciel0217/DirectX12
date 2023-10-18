#include "Camera.h"

void Camera::Config()
{
}

void Camera::Init()
{
	m_Position = XMFLOAT3(0.0, -5.0, 0.0);
	m_Rotation = XMQuaternionIdentity();
	m_LookAtPoint = XMFLOAT3(0.0, 0.0, 100.0);
}

void Camera::Uninit()
{
}

void Camera::Update()
{
	
}

void Camera::PreRender()
{
	SetVPCBuffer(m_Position, m_Rotation, m_LookAtPoint);
}
