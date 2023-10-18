#pragma once
#include "../LowLevel/DirectX12.h"

class Model;

class CGameObject
{
protected:
	XMFLOAT3 m_Position;
	XMFLOAT3 m_Scale;
	XMVECTOR m_Rotation;
	std::shared_ptr<Model> m_Model;

public:
	CGameObject(){}
	virtual ~CGameObject(){}

	virtual void Config() = 0;
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;

	std::shared_ptr<Model> GetModel() { return m_Model; }
};