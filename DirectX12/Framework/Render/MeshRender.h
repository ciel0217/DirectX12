#pragma once
#include "CRender.h"
#include "../Resources/Model.h"
struct WorldMatrix
{
	XMMATRIX world;
	XMMATRIX invWorld;
	XMMATRIX transposeInvWorld;
};


class MeshRender : public CRender
{
protected:
	std::shared_ptr<Model> m_Model;
	void SetUpRender()override;

public:
	MeshRender(CGameObject* self) : CRender(self) { SetUpRender(); }
	
	void Draw(CommandListSet *commandListSet, UINT materialIndex)override;
	std::vector<std::shared_ptr<Material>> GetMaterials()override { return m_Materials; }
};
