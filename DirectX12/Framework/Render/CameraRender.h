#pragma once
#include <list>
#include "../LowLevel/DirectX12.h"
#include "CRender.h"


class CGeometryRenderPipeline;
class CLightRenderPipeline;
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


class CameraRender
{
private:
	std::shared_ptr<CGeometryRenderPipeline> m_CurrentGeometryRender;
	std::shared_ptr<CLightRenderPipeline> m_CurrentLightRender;

	std::shared_ptr<ConstantBuffer> m_VPCBuffer;
	std::shared_ptr<BufferView> m_VPView;

	std::shared_ptr<Material> m_ChangeFrameTexMat;

	D3D12_VIEWPORT m_ViewPort;
	D3D12_RECT m_ScissorRect;

	void SetUpRender();
	void UninitRender();

public:
	CameraRender() { SetUpRender(); }
	virtual ~CameraRender() { UninitRender(); }

	std::shared_ptr<BufferView> GetVPBufferView() { return m_VPView; }
	void SetVPCBuffer(XMFLOAT3 Position, XMVECTOR Quaternion, XMFLOAT3 LookAtPoint);
	void Draw(std::list<std::shared_ptr<CGameObject>> gameObjects[]);
};