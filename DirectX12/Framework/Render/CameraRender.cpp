#include "CameraRender.h"
#include "DeferredRender.h"
#include "../Resources/CGameObject.h"
#include "CRender.h"
#include "../Resources/MaterialManager.h"
#include "../LowLevel/Dx12GraphicsDevice.h"
#include "../LowLevel/BufferView.h"
#include "../Resources/GpuBuffer.h"

void CameraRender::SetUpRender()
{
	m_CurrentRender.reset(new DeferredRender());
	m_CurrentRender->SetUpRender();

	m_VPCBuffer.reset(new ConstantBuffer());
	m_VPView.reset(new BufferView());

	Dx12GraphicsDevice* dxDevice = Dx12GraphicsDevice::GetInstance();
	m_VPCBuffer->CreateConstantBuffer(dxDevice->GetDevice(), sizeof(VP));
	DescriptorHeapManager::Instance().CreateConstantBufferView(m_VPCBuffer->GetResource().GetAddressOf(), m_VPView.get(), 1);

	//ビューポート初期化&TODO:::これも色々できるといいかも
	m_ViewPort.TopLeftX = 0.0f;
	m_ViewPort.TopLeftY = 0.0f;
	m_ViewPort.Width = static_cast<float>(SCREEN_WIDTH);
	m_ViewPort.Height = static_cast<float>(SCREEN_HEIGHT );
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;

	m_ScissorRect = { 0, 0, static_cast<LONG>(SCREEN_WIDTH), static_cast<LONG>(SCREEN_HEIGHT) };
	
}

void CameraRender::UninitRender()
{
	//delete m_CurrentRender;
}

//TODO:::LookAtPointをQuaternionで計算できたら引数減る
void CameraRender::SetVPCBuffer(XMFLOAT3 Position, XMVECTOR Quaternion, XMFLOAT3 LookAtPoint)
{
	VP vp;
	XMVECTOR eyePosition = XMVectorSet(Position.x, Position.y, Position.z, 0.0);
	//TODO::QuaternionからFocusPositionを計算すればいいかも
	XMVECTOR forcusPosition = XMVectorSet(LookAtPoint.x, LookAtPoint.y, LookAtPoint.z, 0.0);
	XMVECTOR upDirection = XMVectorSet(0, 1, 0, 0);
	XMMATRIX view =  XMMatrixLookAtLH(eyePosition, forcusPosition, upDirection);
	vp.View = XMMatrixTranspose(view);
	vp.InverseView = XMMatrixInverse(nullptr, vp.View);

	XMMATRIX proj = XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1, 10000);
	vp.Proj = XMMatrixTranspose(proj);
	vp.InverseProj = XMMatrixInverse(nullptr, vp.Proj);

	m_VPCBuffer->WriteData(&vp, sizeof(VP));
}

void CameraRender::Draw(std::list<std::shared_ptr<CGameObject >> gameObjects[])
{
	Dx12GraphicsDevice* dxDevice = Dx12GraphicsDevice::GetInstance();
	auto commandListSet = dxDevice->GetGraphicContext()->RequestCommandListSet();

	//ビューポート&シザー矩形設定
	commandListSet.m_CommandList.Get()->RSSetViewports(1, &m_ViewPort);
	commandListSet.m_CommandList.Get()->RSSetScissorRects(1, &m_ScissorRect);

	m_CurrentRender->Draw(gameObjects, this, commandListSet);

	//	ID3D12DescriptorHeap*  const ppHeaps[] = { DescriptorHeapManager::Instance().GetD3dDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV).Get() };

	//	//使うDescriptorHeapの設定
	//	commandListSet.m_CommandList.Get()->SetDescriptorHeaps(1, ppHeaps);



	//	//ResourceBarrierの設定(描画前Ver)
	//	D3D12_RESOURCE_BARRIER barrier;
	//	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//	barrier.Transition.pResource = frameResource->GetTexture()->GetResource().Get();
	//	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	//	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	//	commandListSet.m_CommandList.Get()->ResourceBarrier(1, &barrier);


	//	//レンダーターゲットの設定
		
	//	commandListSet.m_CommandList.Get()->OMSetRenderTargets(1, &rtvHandle, FALSE, &(dxDevice->GetDSV()->m_CpuHandle));

		

	//	commandListSet.m_CommandList.Get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//	for (auto gameObject : opacityList)
	//	{
	//		//各オブジェクト描画
	//		std::shared_ptr<PipelineStateObject> pso = gameObject->Mat->GetRenderSet()->pipelineStateObj;
	//		if (currentPSO != pso)
	//		{
	//			currentPSO = pso;

	//			commandListSet.m_CommandList.Get()->SetPipelineState(pso->GetPipelineState().Get());
	//			commandListSet.m_CommandList.Get()->SetGraphicsRootSignature(gameObject->Mat->GetRenderSet()->rootSignature->GetRootSignature().Get());
	//			//セットはオブジェクトの数関係なく一回だけ
	//			gameObject->Mat->GetRenderSet()->rootSignature->SetGraphicsRootDescriptorTable(&commandListSet, "VP", m_VPView);

	//		}

	//		gameObject->Render->Draw(&commandListSet, gameObject->MatIndex);

	//		D3D12_RESOURCE_BARRIER barriera;
	//		barriera.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//		barriera.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//		barriera.Transition.pResource = frameResource->GetTexture()->GetResource().Get();
	//		barriera.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//		barriera.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	//		barriera.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	//		commandListSet.m_CommandList.Get()->ResourceBarrier(1, &barriera);

	//		dxDevice->GetGraphicContext()->ExecuteCommandList(commandListSet);
	//		dxDevice->GetGraphicContext()->DiscardCommandListSet(commandListSet);
	//	}
	//}
}
