#pragma once
#include "CRenderPipeline.h"

class DefferedRender : public CRenderPipeline
{
public:
	DefferedRender(){}
	~DefferedRender(){}
	void SetUpRender()override;
	void UninitRender()override;
	void Draw()override;
};