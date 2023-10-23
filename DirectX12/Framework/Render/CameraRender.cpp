#include "CameraRender.h"
#include "DefferedRender.h"
#include "../Resources/CGameObject.h"
#include "CRender.h"
#include "../Resources/MaterialManager.h"
#include "../LowLevel/Dx12GraphicsDevice.h"
#include "../LowLevel/BufferView.h"
#include "../Resources/GpuBuffer.h"

void CameraRender::SetUpRender()
{
	m_CurrentRender.reset(new DefferedRender());
	m_CurrentRender->SetUpRender();

	m_VPCBuffer.reset(new ConstantBuffer());
	m_VPView.reset(new BufferView());

	Dx12GraphicsDevice* dxDevice = Dx12GraphicsDevice::GetInstance();
	m_VPCBuffer->CreateConstantBuffer(dxDevice->GetDevice(), sizeof(VP));
	DescriptorHeapManager::Instance().CreateConstantBufferView(m_VPCBuffer->GetResource().GetAddressOf(), m_VPView.get(), 1);
	
}

void CameraRender::UninitRender()
{
	//delete m_CurrentRender;
}

void CameraRender::SetVPCBuffer(XMFLOAT3 Position, XMVECTOR Quaternion, XMFLOAT3 LookAtPoint)
{
	
}

void CameraRender::Draw(std::list<CGameObject*> gameObjects[])
{
	std::list<CRender*> opacityList;//不透明
	std::list<CRender*> transparentList;//半透明

	
	//3Dと2DでLayer分けてるため
	for (int i = 0; i < 2; i++)
	{
		for (auto gameObject : gameObjects[i])
		{
			CRender* render = dynamic_cast<CRender*>(gameObject);
			if (!render)continue;

			//TODO::2D対応してないよ
			if (render->GetMaterial()->GetRenderQueue() < MaterialManager::OPACITY_RENDER_QUEUE)
				opacityList.push_back(render);
			else
				transparentList.push_back(render);
		}
	}

	opacityList.sort();
	transparentList.sort();

	///テストで書いてる
	{
		
		std::shared_ptr<PipelineStateObject> currentPSO = nullptr;
		Dx12GraphicsDevice* dxDevice = Dx12GraphicsDevice::GetInstance();

		auto commandListSet = dxDevice->GetGraphicContext()->RequestCommandListSet();

		ID3D12DescriptorHeap*  const ppHeaps[] = { DescriptorHeapManager::Instance().GetD3dDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV).Get() };

		//使うDescriptorHeapの設定
		commandListSet.m_CommandList.Get()->SetDescriptorHeaps(1, ppHeaps);

		UINT frameIndex = dxDevice->GetFrameIndex();
		FrameResources* frameResource = dxDevice->GetFrameResource(frameIndex);

		//ResourceBarrierの設定(描画前Ver)
		D3D12_RESOURCE_BARRIER barrier;
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = frameResource->GetTexture()->GetResource().Get();
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		commandListSet.m_CommandList.Get()->ResourceBarrier(1, &barrier);


		//レンダーターゲットの設定
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = frameResource->GetBufferView().m_CpuHandle;
		commandListSet.m_CommandList.Get()->OMSetRenderTargets(1, &rtvHandle, FALSE, &(dxDevice->GetDSV()->m_CpuHandle));

		//レンダーターゲットクリア
		const float clearColor[] = { 0.0f, 1.0f, 0.4f, 1.0f };
		commandListSet.m_CommandList.Get()->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		commandListSet.m_CommandList.Get()->ClearDepthStencilView(dxDevice->GetDSV()->m_CpuHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

		commandListSet.m_CommandList.Get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		for (auto gameObject : opacityList)
		{
			//各オブジェクト描画
			std::shared_ptr<PipelineStateObject> pso = gameObject->GetMaterial()->GetRenderSet()->pipelineStateObj;
			if (currentPSO != pso)
			{
				currentPSO = pso;

				commandListSet.m_CommandList.Get()->SetPipelineState(pso->GetPipelineState().Get());
				commandListSet.m_CommandList.Get()->SetGraphicsRootSignature(gameObject->GetMaterial()->GetRenderSet()->rootSignature->GetRootSignature().Get());

			}
			gameObject->Draw();
		}

		D3D12_RESOURCE_BARRIER barriera;
		barriera.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barriera.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barriera.Transition.pResource = frameResource->GetTexture()->GetResource().Get();
		barriera.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barriera.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		barriera.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		commandListSet.m_CommandList.Get()->ResourceBarrier(1, &barriera);


		dxDevice->GetGraphicContext()->ExecuteCommandList(commandListSet);
		dxDevice->GetGraphicContext()->DiscardCommandListSet(commandListSet);

	}
}
