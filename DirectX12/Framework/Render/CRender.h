#pragma once
#include "../Resources/Material.h"
#include "../Resources/GpuBuffer.h"
#include "../LowLevel/BufferView.h"

class CGameObject;
struct CommandListSet;

class CRender
{
protected:
	std::shared_ptr<Material> m_Material;
	std::shared_ptr<CGameObject> m_Self;
	std::shared_ptr<ConstantBuffer> m_WorldCBuffer;
	std::shared_ptr<BufferView> m_WorldView;
	
	virtual void SetUpRender() = 0;
	
public:
	CRender() = delete;
	CRender(CGameObject* obj) : m_Self(obj) {}
	virtual ~CRender(){}

	virtual void Draw(CommandListSet *commandListSet) = 0;

	virtual std::shared_ptr<Material> GetMaterial() { return m_Material; }

	bool operator < (CRender* const rhs)const
	{
		if(m_Material->GetRenderQueue() == rhs->GetMaterial()->GetRenderQueue())
			return m_Material->GetMaterialName() < rhs->GetMaterial()->GetMaterialName();

		return m_Material->GetRenderQueue() < rhs->GetMaterial()->GetRenderQueue();
	}
};