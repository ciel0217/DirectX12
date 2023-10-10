#pragma once
#include <list>

class CRenderPipeline;
class CGameObject;

class CameraRender
{
private:
	CRenderPipeline* m_CurrentRender;

	const int OPACITY_RENDER_QUEUE = 3000;
	const int TRANSPARENT_RENDER_QUEUE = 7500;

public:
	CameraRender() { SetUpRender(); }
	virtual ~CameraRender() { UninitRender(); }

	void SetUpRender();
	void UninitRender();
	void Draw(std::list<CGameObject*> gameObjects[]);
};