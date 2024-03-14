#pragma once
#include "CRenderPipeline.h"
#include "../LowLevel/Dx12GraphicsDevice.h"
#include "../Resources/MaterialManager.h"

class CommandContext;

class DeferredRender : public CGeometryRenderPipeline
{
private:
	std::shared_ptr<Material> m_DefaultMat;
	std::shared_ptr<Material> m_ChangeFrameTexMat;
	
	std::string m_ResoureceName[3] = { "BaseColor", "Normal", "RoughMetaSpe" };
	UINT m_ResoureceMax;
	D3D12_VIEWPORT m_ViewPort;
	D3D12_RECT m_ScissorRect;

	void CalcTextureResource(std::string name, const ComPtr<ID3D12Device> &device, CommandContext* const commandContext, UINT index);
public:
	DeferredRender(){}
	~DeferredRender(){}
	void SetUpRender()override;
	void UninitRender()override;
	void Draw(std::list<std::shared_ptr<RenderingSet>> gameObjects, CameraRender* cameraRender, CommandListSet& commandListSet)override;
};