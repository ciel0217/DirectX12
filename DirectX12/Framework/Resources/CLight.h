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
	bool isEnable;
	XMVECTOR lightDir;
	XMVECTOR lightColor;
	float lightPow;
	float attenuation;
	LightType lightType;
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