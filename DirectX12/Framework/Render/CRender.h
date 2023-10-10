#pragma once
#include "../Resources/Material.h"

class CGameObject;

class CRender
{
protected:
	Material* m_Material;
	CGameObject* m_GameObject;

public:
	CRender() = delete;
	CRender(CGameObject* obj):m_GameObject(obj){}
	virtual ~CRender(){}


	virtual void Draw() = 0;

	Material* GetMaterial() { return m_Material; }

	bool operator < (CRender* const rhs)const
	{
		if(m_Material->GetRenderQueue() == rhs->GetMaterial()->GetRenderQueue())
			return m_Material->GetMaterialName() < rhs->GetMaterial()->GetMaterialName();

		return m_Material->GetRenderQueue() < rhs->GetMaterial()->GetRenderQueue();
	}
};