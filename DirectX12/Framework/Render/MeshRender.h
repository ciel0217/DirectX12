#pragma once
#include "CRender.h"
#include "../Resources/Model.h"
struct WorldMatrix
{
	XMMATRIX w;
};


class MeshRender : public CRender
{
protected:
	std::shared_ptr<Model> m_Model;
	void SetUpRender()override;

public:
	MeshRender(CGameObject* self) : CRender(self) { SetUpRender(); }
	
	void Draw(CommandListSet *commandListSet)override;
	std::shared_ptr<Material> GetMaterial()override { return m_Model->GetMaterials()[0]; }
};
