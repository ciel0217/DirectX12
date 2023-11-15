#pragma once
#include "../LowLevel/DirectX12Helper.h"

class CameraRender;
class CGameObject;

class CRenderPipeline
{
protected:
	

public:
	CRenderPipeline(){}
	virtual ~CRenderPipeline(){}

	virtual void SetUpRender() = 0;
	virtual void UninitRender() = 0;
	virtual void Draw(std::list<CGameObject*> gameObjects[], CameraRender* cameraRender) = 0;
};