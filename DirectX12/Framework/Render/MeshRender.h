#pragma once
#include "CRender.h"

struct WorldMatrix
{
	XMMATRIX w;
};


class Model;

class MeshRender : public CRender
{
protected:
	std::shared_ptr<Model> m_Model;
	void SetUpRender()override;

public:
	MeshRender(CGameObject* self) : CRender(self) { SetUpRender(); }
	
	void Draw()override;
};
