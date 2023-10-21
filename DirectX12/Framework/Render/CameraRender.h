#pragma once
#include <list>
#include "../LowLevel/DirectX12.h"


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

class CameraRender
{
private:
	std::shared_ptr<CRenderPipeline> m_CurrentRender;

	std::shared_ptr<ConstantBuffer> m_VPCBuffer;
	std::shared_ptr<BufferView> m_VPView;

	void SetUpRender();
	void UninitRender();

public:
	CameraRender() { SetUpRender(); }
	virtual ~CameraRender() { UninitRender(); }

	
	void SetVPCBuffer(XMFLOAT3 Position, XMVECTOR Quaternion, XMFLOAT3 LookAtPoint);
	void Draw(std::list<CGameObject*> gameObjects[]);
};