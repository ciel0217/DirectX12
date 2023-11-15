#pragma once
#include "CRenderPipeline.h"
#include "../Resources/FrameResources.h"
#include <unordered_map>
#include "../LowLevel/Dx12GraphicsDevice.h"
#include "../Resources/MaterialManager.h"

class CommandContext;

class DeferredRender : public CRenderPipeline
{
private:
	std::unordered_map<std::string, std::shared_ptr<FrameResources>> m_TextureResourece;
	std::shared_ptr<Material> m_DefaultMat;
	UINT m_ResoureceMax;

	D3D12_VIEWPORT m_ViewPort;
	D3D12_RECT m_ScissorRect;

	void CalcTextureResource(std::string name, const ComPtr<ID3D12Device> &device, CommandContext* const commandContext);
public:
	DeferredRender(){}
	~DeferredRender(){}
	void SetUpRender()override;
	void UninitRender()override;
	void Draw(std::list<CGameObject*> gameObjects[], CameraRender* cameraRender)override;
};