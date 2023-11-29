#pragma once
#include "../Resources/Material.h"
#include "../Resources/GpuBuffer.h"
#include "../LowLevel/BufferView.h"

class CGameObject;
struct CommandListSet;

class CRender
{
protected:
	std::vector<std::shared_ptr<Material>> m_Materials;
	CGameObject* m_Self;
	std::shared_ptr<ConstantBuffer> m_WorldCBuffer;
	std::shared_ptr<BufferView> m_WorldView;
	
	virtual void SetUpRender() = 0;
	
public:
	CRender() = delete;
	CRender(CGameObject* obj) : m_Self(obj) {}
	virtual ~CRender(){}

	virtual void Draw(CommandListSet *commandListSet, UINT materialIndex) = 0;

	virtual std::vector<std::shared_ptr<Material>> GetMaterials() { return m_Materials; }

	/*bool operator < (CRender* const rhs)const
	{
		if(m_Material->GetRenderQueue() == rhs->GetMaterial()->GetRenderQueue())
			return m_Material->GetMaterialName() < rhs->GetMaterial()->GetMaterialName();

		return m_Material->GetRenderQueue() < rhs->GetMaterial()->GetRenderQueue();
	}*/
};