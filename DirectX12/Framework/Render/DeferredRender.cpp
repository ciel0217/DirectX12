#include "DeferredRender.h"
#include "../Resources/Shader.h"
#include "../LowLevel/RootSignature.h"
#include "../LowLevel/PipelineStateObject.h"
#include "../LowLevel/Dx12GraphicsDevice.h"


/*
GBuffer - > Light計算 - > 半透明 - > 2D
までの一連を実装
*/

void DeferredRender::CalcTextureResource(std::string name, const ComPtr<ID3D12Device>& device, CommandContext * const commandContext)
{
	m_TextureResourece[name] = std::make_shared<FrameResources>();
	m_TextureResourece[name]->Create(device, commandContext);
}

void DeferredRender::SetUpRender()
{
	std::string resoureceName[] = {"BaseColor", "Normal", "RoughMetaSpe"};
	m_ResoureceMax = sizeof(resoureceName) / sizeof(resoureceName[0]);

	{
		ComPtr<ID3D12Device> device = Dx12GraphicsDevice::GetInstance()->GetDevice();
		CommandContext* commandContext = Dx12GraphicsDevice::GetInstance()->GetGraphicContext();

		for (int i = 0; i < m_ResoureceMax; i++)
		{
			CalcTextureResource(resoureceName[i], device, commandContext);
		}
	}

	m_DefaultMat = MaterialManager::GetInstance()->CreateMaterial("DefaultDeferred", "Framework/Shader/DefaultGBufferVS.cso", "Framework/Shader/DefaultGBufferPS.cso", MaterialManager::OPACITY_RENDER_QUEUE);
	
	//ビューポート初期化&TODO:::これも色々できるといいかも
	m_ViewPort.TopLeftX = 0.0f;
	m_ViewPort.TopLeftY = 0.0f;
	m_ViewPort.Width = static_cast<float>(SCREEN_WIDTH);
	m_ViewPort.Height = static_cast<float>(SCREEN_HEIGHT);
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;

	m_ScissorRect = { 0, 0, static_cast<LONG>(SCREEN_WIDTH), static_cast<LONG>(SCREEN_HEIGHT) };
}

void DeferredRender::UninitRender()
{

}

void DeferredRender::Draw(std::list<CGameObject*> gameObjects[])
{
	std::list<std::shared_ptr<RenderingSet>> opacityList;//不透明
	std::list<std::shared_ptr<RenderingSet>> transparentList;//半透明


	//3Dと2DでLayer分けてるため
	for (int i = 0; i < 2; i++)
	{
		for (auto gameObject : gameObjects[i])
		{
			CRender * render = dynamic_cast<CRender*>(gameObject);

			if (!render)continue;

			std::vector<std::shared_ptr<Material>> materials = render->GetMaterials();

			for (UINT i = 0; i < materials.size(); i++)
			{

				std::shared_ptr<Material> material = materials[i];
				std::shared_ptr<RenderingSet> renderingSet;
				renderingSet.reset(new RenderingSet(render, material.get(), i));

				//TODO::2D対応してないよ
				if (material->GetRenderQueue() <= MaterialManager::OPACITY_RENDER_QUEUE)
					opacityList.push_back(renderingSet);
				else
					transparentList.push_back(renderingSet);
			}

		}
	}

	opacityList.sort();
	transparentList.sort();

	{
		std::shared_ptr<PipelineStateObject> currentPSO = m_DefaultMat->GetRenderSet()->pipelineStateObj;
		Dx12GraphicsDevice* dxDevice = Dx12GraphicsDevice::GetInstance();

		auto commandListSet = dxDevice->GetGraphicContext()->RequestCommandListSet();

		//ビューポート&シザー矩形設定
		commandListSet.m_CommandList.Get()->RSSetViewports(1, &m_ViewPort);
		commandListSet.m_CommandList.Get()->RSSetScissorRects(1, &m_ScissorRect);

		ID3D12DescriptorHeap*  const ppHeaps[] = { DescriptorHeapManager::Instance().GetD3dDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV).Get() };

		//使うDescriptorHeapの設定
		commandListSet.m_CommandList.Get()->SetDescriptorHeaps(1, ppHeaps);

		UINT frameIndex = dxDevice->GetFrameIndex();
		FrameResources* mainFrameResource = dxDevice->GetFrameResource(frameIndex);

		std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> rtvHandles;
		rtvHandles.reserve(m_ResoureceMax);

		for (auto res : m_TextureResourece)
		{
			rtvHandles.push_back(res.second->GetRTVBufferView()->m_CpuHandle);
		}
	
	}
}
