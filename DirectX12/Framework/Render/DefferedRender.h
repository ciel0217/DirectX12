#pragma once
#include "CRender.h"

class DefferedRender : public CRender
{
public:
	DefferedRender(){}
	~DefferedRender(){}
	void SetUpRender()override;
	void UninitRender()override;
	void Draw()override;
};