#include "../Resources/FrameResources.h"
#include "DeferredRender.h"
#include "../Resources/Shader.h"
#include "../LowLevel/RootSignature.h"
#include "../LowLevel/PipelineStateObject.h"
#include "../LowLevel/Dx12GraphicsDevice.h"
#include "CameraRender.h"

/*
GBuffer - > Light計算 - > 半透明 - > 2D
までの一連を実装
*/

void DeferredRender::CalcTextureResource(std::string name, const ComPtr<ID3D12Device>& device, CommandContext * const commandContext, UINT index)
{
	
	m_TextureResourece[name] = std::make_shared<FrameResources>();
	m_TextureResourece[name]->Create(device, commandContext, m_DefaultMat->GetRenderSet()->pipelineStateObj->GetPsoDesc().m_RTVFormat[index]);
}

void DeferredRender::SetUpRender()
{
	std::string resoureceName[] = {"BaseColor", "Normal", "RoughMetaSpe"};
	m_ResoureceMax = sizeof(resoureceName) / sizeof(resoureceName[0]);

	m_DefaultMat = MaterialManager::GetInstance()->CreateMaterial("DefaultDeferred", "Framework/Shader/DefaultGBufferVS.cso",
		"Framework/Shader/DefaultGBufferPS.cso", MaterialManager::OPACITY_RENDER_QUEUE);

	m_ChangeFrameTexMat = MaterialManager::GetInstance()->CreateMaterial("ChageFrameTex", "Framework/Shader/Default2DVS.cso",
		"Framework/Shader/Default2DPS.cso", MaterialManager::D2_RENDER_QUEUE, e2DPipeline);

	{
		ComPtr<ID3D12Device> device = Dx12GraphicsDevice::GetInstance()->GetDevice();
		CommandContext* commandContext = Dx12GraphicsDevice::GetInstance()->GetGraphicContext();

		for (UINT i = 0; i < m_ResoureceMax; i++)
		{
			CalcTextureResource(resoureceName[i], device, commandContext, i);
			m_TextureResourece[resoureceName[i]]->GetTexture()->GetResource()->SetName(L"deferred");
		}
	}
	

}

void DeferredRender::UninitRender()
{

}

void DeferredRender::Draw(std::list<std::shared_ptr<RenderingSet>> gameObjects, CameraRender* cameraRender, CommandListSet& commandListSet)
{

	{
		std::shared_ptr<PipelineStateObject> currentPSO =nullptr;
		Dx12GraphicsDevice* dxDevice = Dx12GraphicsDevice::GetInstance();

		//auto commandListSet = dxDevice->GetGraphicContext()->RequestCommandListSet();

		////ビューポート&シザー矩形設定
		//commandListSet.m_CommandList.Get()->RSSetViewports(1, &m_ViewPort);
		//commandListSet.m_CommandList.Get()->RSSetScissorRects(1, &m_ScissorRect);

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

		/////////描画前ResourceBarrior
		std::vector<D3D12_RESOURCE_BARRIER> beforeBarriers;
		beforeBarriers.resize(m_ResoureceMax);

		{
			int count = 0;
		
			for (auto res : m_TextureResourece)
			{
				D3D12_RESOURCE_BARRIER barrier;
				barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
				barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
				barrier.Transition.pResource = res.second->GetTexture()->GetResource().Get();
				barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
				barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
				barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

				beforeBarriers[count++] = barrier;
			}

		/*	{
				D3D12_RESOURCE_BARRIER barrier;
				barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
				barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
				barrier.Transition.pResource = mainFrameResource->GetTexture()->GetResource().Get();
				barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
				barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
				barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

				beforeBarriers[count++] = barrier;
			}*/
		}
		commandListSet.m_CommandList.Get()->ResourceBarrier(beforeBarriers.size(), beforeBarriers.data());
		
		//clearColor
		const float clearColor[] = { 0.0f, 0.0f, 1.0f, 1.0f };

		//深度バッファclear
		commandListSet.m_CommandList.Get()->ClearDepthStencilView(dxDevice->GetDSV()->m_CpuHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
		
		//GBufferClear
		for (UINT i = 0; i < m_ResoureceMax; i++)
			commandListSet.m_CommandList.Get()->ClearRenderTargetView(rtvHandles[i], clearColor, 0, nullptr);
		
		////mainFrameResourceClear
	//	commandListSet.m_CommandList.Get()->ClearRenderTargetView(mainFrameResource->GetRTVBufferView()->m_CpuHandle, clearColor, 0, nullptr);

		//SetRenderTargets
		commandListSet.m_CommandList.Get()->OMSetRenderTargets(m_ResoureceMax, rtvHandles.data(), NULL, &(dxDevice->GetDSV()->m_CpuHandle));
		

		commandListSet.m_CommandList.Get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//DrawGBufer
		for (auto gameObject : gameObjects)
		{
			std::shared_ptr<PipelineStateObject> pso = gameObject->Mat->GetRenderSet()->pipelineStateObj;

			if (currentPSO != pso)
			{
				currentPSO = pso;

				commandListSet.m_CommandList.Get()->SetPipelineState(pso->GetPipelineState().Get());
				commandListSet.m_CommandList.Get()->SetGraphicsRootSignature(gameObject->Mat->GetRenderSet()->rootSignature->GetRootSignature().Get());
				//セットはオブジェクトの数関係なく一回だけ
				gameObject->Mat->GetRenderSet()->rootSignature->SetGraphicsRootDescriptorTable(&commandListSet, "VP", cameraRender->GetVPBufferView());

			}

			gameObject->Render->Draw(&commandListSet, gameObject->MatIndex);
		}

		//実行が終わるまで待つ
		dxDevice->GetGraphicContext()->WaitForIdle();

		std::vector<D3D12_RESOURCE_BARRIER> afterBarriers;
		afterBarriers.resize(m_ResoureceMax );

		{
			int count = 0;
			for (auto res : m_TextureResourece)
			{
				D3D12_RESOURCE_BARRIER barrier;
				barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
				barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
				barrier.Transition.pResource = res.second->GetTexture()->GetResource().Get();
				barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
				barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
				barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

				afterBarriers[count++] = barrier;
			}
			
		}
		commandListSet.m_CommandList.Get()->ResourceBarrier(afterBarriers.size(), afterBarriers.data());

		//dxDevice->GetGraphicContext()->ExecuteCommandList(commandListSet);
		//dxDevice->GetGraphicContext()->DiscardCommandListSet(commandListSet);

		////TODO::mainFrameにいったん戻す
		//commandListSet.m_CommandList->OMSetRenderTargets(1, &(mainFrameResource->GetRTVBufferView()->m_CpuHandle), NULL, NULL);

		//commandListSet.m_CommandList.Get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		//
		//std::shared_ptr<PipelineStateObject> pso = m_ChangeFrameTexMat->GetRenderSet()->pipelineStateObj;

		//commandListSet.m_CommandList->SetPipelineState(pso->GetPipelineState().Get());
		//commandListSet.m_CommandList->SetGraphicsRootSignature(m_ChangeFrameTexMat->GetRenderSet()->rootSignature->GetRootSignature().Get());
		//
		//UINT index = 0;
		//for (auto res : m_TextureResourece)
		//{
		//	commandListSet.m_CommandList->SetGraphicsRootDescriptorTable(index++, res.second->GetSRVBufferView()->m_GpuHandle);
		//}

		//commandListSet.m_CommandList->DrawInstanced(4, 1, 0, 0);
		//

		//D3D12_RESOURCE_BARRIER barrier;
		////描画後ResourceBarrier
		//{	
		//	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		//	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		//	barrier.Transition.pResource = mainFrameResource->GetTexture()->GetResource().Get();
		//	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		//	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		//	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		//}

		//commandListSet.m_CommandList.Get()->ResourceBarrier(1, &barrier);
		//
		/*dxDevice->GetGraphicContext()->ExecuteCommandList(commandListSet);
		dxDevice->GetGraphicContext()->DiscardCommandListSet(commandListSet);*/


	}
}
