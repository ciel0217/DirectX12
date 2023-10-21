#pragma once
#include "../Render/CameraRender.h"
#include "CGameObject.h"

class Camera : public CameraRender, public CGameObject
{
private:
	XMFLOAT3 m_LookAtPoint;
	float m_Fov;

public:
	void Config()override;
	void Uninit()override;
	void Init()override;
	void Update()override;

	void PreRender();
};