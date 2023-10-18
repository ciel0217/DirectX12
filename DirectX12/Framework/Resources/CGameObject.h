#pragma once
#include "../LowLevel/DirectX12.h"


class CGameObject
{
protected:
	XMFLOAT3 m_Position;
	XMFLOAT3 m_Scale;
	XMVECTOR m_Rotation;

public:
	CGameObject(){}
	virtual ~CGameObject(){}

	virtual void Config() = 0;
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;

};