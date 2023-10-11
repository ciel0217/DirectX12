#pragma once
#include "CRender.h"

struct WVP
{
	XMMATRIX w;
	XMMATRIX v;
	XMMATRIX p;
};

class MeshRender : public CRender
{
public:
	void SetUpRender()override;
	void Draw()override;
};
