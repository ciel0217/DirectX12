#include "LightRender.h"
#include "../LowLevel/BufferView.h"
#include "../Resources/GpuBuffer.h"
#include "../LowLevel/Dx12GraphicsDevice.h"
#include "../Resources/CLight.h"
#include "../LowLevel/DescriptorHeap.h"
#include "../Resources/MaterialManager.h"

void LightRender::SetUpRender()
{
	{
		m_LightStructuredBuffer.reset(new StructuredBuffer());
		m_LightView.reset(new BufferView());

		auto device = Dx12GraphicsDevice::GetInstance()->GetDevice();
		m_LightStructuredBuffer->CreateStructuredBuffer(device, MAX_LIGHT, sizeof(CLight));

		std::vector<D3D12_BUFFER_SRV> srvParam;
		srvParam.resize(1);
		srvParam[0].NumElements = MAX_LIGHT;
		srvParam[0].StructureByteStride = sizeof(CLight);

		DescriptorHeapManager::Instance().CreateStructuredBufferView(m_LightStructuredBuffer->GetResource().GetAddressOf(), m_LightView.get(), 1, srvParam);
	}

	{
		m_RenderMat = MaterialManager::GetInstance()->CreateMaterial("Lit", "Framework/Shader/LitVS.cso",
			"Framework/Shader/LitPS.cso", MaterialManager::LIGHT_RENDER_QUEUE, e2DPipeline);
	}

	{
		ComPtr<ID3D12Device> device = Dx12GraphicsDevice::GetInstance()->GetDevice();
		CommandContext* commandContext = Dx12GraphicsDevice::GetInstance()->GetGraphicContext();

		m_TextureResourece["Light"] = std::make_shared<FrameResources>();
		m_TextureResourece["Light"]->Create(device, commandContext, m_RenderMat->GetRenderSet()->pipelineStateObj->GetPsoDesc().m_RTVFormat[0]);
		
		m_TextureResourece["Light"]->GetTexture()->GetResource()->SetName(L"Light");
		
	}
}

void LightRender::UninitRender()
{
}

void LightRender::Draw(std::unordered_map<std::string, std::shared_ptr<FrameResources>>& textureResources, std::vector<std::shared_ptr<CLight>> lights, CommandListSet & commandListSet)
{
	m_LightStructuredBuffer->WriteData(lights.data(), sizeof(CLight) * lights.size(), 0);

	{
		Dx12GraphicsDevice* dxDevice = Dx12GraphicsDevice::GetInstance();

		ID3D12DescriptorHeap*  const ppHeaps[] = { DescriptorHeapManager::Instance().GetD3dDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV).Get() };

		//Žg‚¤DescriptorHeap‚ÌÝ’è
		commandListSet.m_CommandList.Get()->SetDescriptorHeaps(1, ppHeaps);


		/////////•`‰æ‘OResourceBarrior
		D3D12_RESOURCE_BARRIER beforeBarrier;

		beforeBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		beforeBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		beforeBarrier.Transition.pResource = m_TextureResourece["Light"]->GetTexture()->GetResource().Get();
		beforeBarrier.Transition.StateBefore =  D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
		beforeBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		beforeBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		commandListSet.m_CommandList->ResourceBarrier(1, &beforeBarrier);
		

		//clearColor
		const float clearColor[] = { 0.0f, 0.0f, 1.0f, 1.0f };

		//SetRenderTargets
		commandListSet.m_CommandList->OMSetRenderTargets(1, &(m_TextureResourece["Light"]->GetRTVBufferView()->m_CpuHandle), NULL, NULL);

		commandListSet.m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		std::shared_ptr<PipelineStateObject> pso = m_RenderMat->GetRenderSet()->pipelineStateObj;

		commandListSet.m_CommandList->SetPipelineState(pso->GetPipelineState().Get());
		commandListSet.m_CommandList->SetGraphicsRootSignature(m_RenderMat->GetRenderSet()->rootSignature->GetRootSignature().Get());
		
		{
			UINT index = 0;
			for (auto res : textureResources)
			{
				m_RenderMat->GetRenderSet()->rootSignature->SetGraphicsRootDescriptorTable(&commandListSet, res.first, res.second->GetSRVBufferView());
				//commandListSet.m_CommandList->SetGraphicsRootDescriptorTable(index++, res.second->GetSRVBufferView()->m_GpuHandle);
			}
		}

		m_RenderMat->GetRenderSet()->rootSignature->SetGraphicsRootDescriptorTable(&commandListSet, "Light", m_LightView);

		commandListSet.m_CommandList->DrawInstanced(4, 1, 0, 0);

		////////•`‰æŒãResourceBarrior
		D3D12_RESOURCE_BARRIER afterBarrier;

		afterBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		afterBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		afterBarrier.Transition.pResource = m_TextureResourece["Light"]->GetTexture()->GetResource().Get();
		afterBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		afterBarrier.Transition.StateAfter =  D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
		afterBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		commandListSet.m_CommandList->ResourceBarrier(1, &afterBarrier);

		
	}
}
