#pragma once
#include <list>


class CRenderPipeline;
class CGameObject;

class CameraRender
{
private:
	std::shared_ptr<CRenderPipeline> m_CurrentRender;

	const int OPACITY_RENDER_QUEUE = 3000;
	const int TRANSPARENT_RENDER_QUEUE = 7500;

public:
	CameraRender() { SetUpRender(); }
	virtual ~CameraRender() { UninitRender(); }

	void SetUpRender();
	void UninitRender();
	void SetVPCBuffer(XMFLOAT3 Position, XMVECTOR Quaternion, XMFLOAT3 LookAtPoint);
	void Draw(std::list<CGameObject*> gameObjects[]);
};