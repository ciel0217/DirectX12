#pragma once

class CRenderPipeline
{
protected:
	

public:
	CRenderPipeline(){}
	virtual ~CRenderPipeline(){}

	virtual void SetUpRender() = 0;
	virtual void UninitRender() = 0;
	virtual void Draw() = 0;
};