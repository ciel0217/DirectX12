#pragma once
#include <list>
#include "../LowLevel/DirectX12.h"
#include "CRender.h"
#include "../Resources/Material.h"

class CRenderPipeline;
class CGameObject;
class ConstantBuffer;
struct BufferView;

struct VP
{
	XMMATRIX View;
	XMMATRIX Proj;
	XMMATRIX InverseView;
	XMMATRIX InverseProj;
};

struct RenderingSet
{
	CRender* Render;
	Material* Mat;

	RenderingSet(CRender* render, Material* material)
	{
		Render = render;
		Mat = material;
	}

	bool operator < (RenderingSet* const rhs)const
	{
		if (Mat->GetRenderQueue() == rhs->Mat->GetRenderQueue())
			return Mat->GetMaterialName() < rhs->Mat->GetMaterialName();

		return Mat->GetRenderQueue() < rhs->Mat->GetRenderQueue();
	}
};

class CameraRender
{
private:
	std::shared_ptr<CRenderPipeline> m_CurrentRender;

	std::shared_ptr<ConstantBuffer> m_VPCBuffer;
	std::shared_ptr<BufferView> m_VPView;

	D3D12_VIEWPORT m_ViewPort;
	D3D12_RECT m_ScissorRect;

	void SetUpRender();
	void UninitRender();

public:
	CameraRender() { SetUpRender(); }
	virtual ~CameraRender() { UninitRender(); }

	
	void SetVPCBuffer(XMFLOAT3 Position, XMVECTOR Quaternion, XMFLOAT3 LookAtPoint);
	void Draw(std::list<CGameObject*> gameObjects[]);
};