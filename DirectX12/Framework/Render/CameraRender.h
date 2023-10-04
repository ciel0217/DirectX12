#pragma once

class CRender;

class CameraRender
{
private:
	CRender* m_CurrentRender;

public:
	CameraRender() { SetUpRender(); }
	virtual ~CameraRender() { UninitRender(); }

	void SetUpRender();
	void UninitRender();
	void Draw();
};