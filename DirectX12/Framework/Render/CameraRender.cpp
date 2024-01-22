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

	//�r���[�|�[�g������&TODO:::������F�X�ł���Ƃ�������
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

//TODO:::LookAtPoint��Quaternion�Ōv�Z�ł������������
void CameraRender::SetVPCBuffer(XMFLOAT3 Position, XMVECTOR Quaternion, XMFLOAT3 LookAtPoint)
{
	VP vp;
	XMVECTOR eyePosition = XMVectorSet(Position.x, Position.y, Position.z, 0.0);
	//TODO::Quaternion����FocusPosition���v�Z����΂�������
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
	m_CurrentRender->Draw(gameObjects, this);
	//std::list<std::shared_ptr<RenderingSet>> opacityList;//�s����
	//std::list<std::shared_ptr<RenderingSet>> transparentList;//������

	//
	////3D��2D��Layer�����Ă邽��
	//for (int i = 0; i < 2; i++)
	//{
	//	for (auto gameObject : gameObjects[i])
	//	{
	//		CRender * render = dynamic_cast<CRender*>(gameObject.get());
	//	
	//		if (!render)continue;

	//		std::vector<std::shared_ptr<Material>> materials = render->GetMaterials();

	//		for (UINT i = 0; i < materials.size(); i++)
	//		{

	//			std::shared_ptr<Material> material = materials[i];
	//			std::shared_ptr<RenderingSet> renderingSet;
	//			renderingSet.reset(new RenderingSet(render, material.get(), i));

	//			//TODO::2D�Ή����ĂȂ���
	//			if (material->GetRenderQueue() <= MaterialManager::OPACITY_RENDER_QUEUE)
	//				opacityList.push_back(renderingSet);
	//			else
	//				transparentList.push_back(renderingSet);
	//		}
	//		
	//	}
	//}

	//opacityList.sort();
	//transparentList.sort();

	/////�e�X�g�ŏ����Ă�
	//{
	//	
	//	std::shared_ptr<PipelineStateObject> currentPSO = nullptr;
	//	Dx12GraphicsDevice* dxDevice = Dx12GraphicsDevice::GetInstance();

	//	auto commandListSet = dxDevice->GetGraphicContext()->RequestCommandListSet();

	//	//�r���[�|�[�g&�V�U�[��`�ݒ�
	//	commandListSet.m_CommandList.Get()->RSSetViewports(1, &m_ViewPort);
	//	commandListSet.m_CommandList.Get()->RSSetScissorRects(1, &m_ScissorRect);

	//	ID3D12DescriptorHeap*  const ppHeaps[] = { DescriptorHeapManager::Instance().GetD3dDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV).Get() };

	//	//�g��DescriptorHeap�̐ݒ�
	//	commandListSet.m_CommandList.Get()->SetDescriptorHeaps(1, ppHeaps);

	//	UINT frameIndex = dxDevice->GetFrameIndex();
	//	FrameResources* frameResource = dxDevice->GetFrameResource(frameIndex);

	//	//ResourceBarrier�̐ݒ�(�`��OVer)
	//	D3D12_RESOURCE_BARRIER barrier;
	//	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//	barrier.Transition.pResource = frameResource->GetTexture()->GetResource().Get();
	//	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	//	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	//	commandListSet.m_CommandList.Get()->ResourceBarrier(1, &barrier);


	//	//�����_�[�^�[�Q�b�g�̐ݒ�
	//	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = frameResource->GetRTVBufferView()->m_CpuHandle;
	//	commandListSet.m_CommandList.Get()->OMSetRenderTargets(1, &rtvHandle, FALSE, &(dxDevice->GetDSV()->m_CpuHandle));

	//	//�����_�[�^�[�Q�b�g�N���A
	//	const float clearColor[] = { 0.0f, 1.0f, 0.4f, 1.0f };
	//	commandListSet.m_CommandList.Get()->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	//	commandListSet.m_CommandList.Get()->ClearDepthStencilView(dxDevice->GetDSV()->m_CpuHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	//	commandListSet.m_CommandList.Get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//	for (auto gameObject : opacityList)
	//	{
	//		//�e�I�u�W�F�N�g�`��
	//		std::shared_ptr<PipelineStateObject> pso = gameObject->Mat->GetRenderSet()->pipelineStateObj;
	//		if (currentPSO != pso)
	//		{
	//			currentPSO = pso;

	//			commandListSet.m_CommandList.Get()->SetPipelineState(pso->GetPipelineState().Get());
	//			commandListSet.m_CommandList.Get()->SetGraphicsRootSignature(gameObject->Mat->GetRenderSet()->rootSignature->GetRootSignature().Get());
	//			//�Z�b�g�̓I�u�W�F�N�g�̐��֌W�Ȃ���񂾂�
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
