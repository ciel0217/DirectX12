#pragma once
#include "../LowLevel/DirectX12Helper.h"
#include "CGameObject.h"

enum LightType
{
	eDirLight = 0,
	ePointLight,
	eLightTypeMax
};

struct Light
{
	XMFLOAT3 lightDir;
	XMFLOAT3 lightColor;
	float lightPow;
	float attenuation;
	LightType lightType;
	bool isEnable;
};

class CLight : public CGameObject
{
protected:
	Light m_Light;

public:
	CLight(){}
	virtual ~CLight(){}

	Light& GetLight() { return m_Light; }
};