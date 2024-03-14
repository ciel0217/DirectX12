#include "CameraRender.h"
#include "DeferredRender.h"
#include "LightRender.h"
#include "../Resources/CGameObject.h"
#include "CRender.h"
#include "../Resources/MaterialManager.h"
#include "../LowLevel/Dx12GraphicsDevice.h"
#include "../LowLevel/BufferView.h"
#include "../Resources/GpuBuffer.h"
#include "../Resources/CLight.h"
#include <memory>

void CameraRender::SetUpRender()
{
	m_CurrentGeometryRender.reset(new DeferredRender());
	m_CurrentGeometryRender->SetUpRender();

	m_CurrentLightRender.reset(new LightRender());
	m_CurrentLightRender->SetUpRender();

	m_VPCBuffer.reset(new ConstantBuffer());
	m_VPView.reset(new BufferView());

	Dx12GraphicsDevice* dxDevice = Dx12GraphicsDevice::GetInstance();
	m_VPCBuffer->CreateConstantBuffer(dxDevice->GetDevice(), sizeof(VP));
	DescriptorHeapManager::Instance().CreateConstantBufferView(m_VPCBuffer->GetResource().GetAddressOf(), m_VPView.get(), 1);

	m_ChangeFrameTexMat = MaterialManager::GetInstance()->CreateMaterial("ChageFrameTex", "Framework/Shader/Default2DVS.cso",
		"Framework/Shader/Default2DPS.cso", MaterialManager::D2_RENDER_QUEUE, e2DPipeline);

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

	XMMATRIX proj = XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 10000.0f);
	vp.Proj = XMMatrixTranspose(proj);
	vp.InverseProj = XMMatrixInverse(nullptr, vp.Proj);

	m_VPCBuffer->WriteData(&vp, sizeof(VP));
}

void CameraRender::Draw(std::list<std::shared_ptr<CGameObject >> gameObjects[])
{
	std::list<std::shared_ptr<RenderingSet>> opacityList;//不透明
	std::list<std::shared_ptr<RenderingSet>> transparentList;//半透明
	std::vector<std::shared_ptr<CLight>> lightList;

	//3Dと2DでLayer分けてるため
	for (int i = 0; i < 2; i++)
	{
		for (auto gameObject : gameObjects[i])
		{
			CRender * render = dynamic_cast<CRender*>(gameObject.get());

			if (!render)
			{
				
				std::shared_ptr<CLight> light = std::dynamic_pointer_cast<CLight>(gameObject);
				
				if (light)
				{
					
					lightList.push_back(light);

				}
				//if (typeid(*gameObject) == typeid(CLight))
				//{
				//	//型チェックしてるから自明なDownCast
				//	
				//}

				continue;
			}


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

	Dx12GraphicsDevice* dxDevice = Dx12GraphicsDevice::GetInstance();
	auto commandListSet = dxDevice->GetGraphicContext()->RequestCommandListSet();

	//ビューポート&シザー矩形設定
	commandListSet.m_CommandList.Get()->RSSetViewports(1, &m_ViewPort);
	commandListSet.m_CommandList.Get()->RSSetScissorRects(1, &m_ScissorRect);

	m_CurrentGeometryRender->Draw(opacityList, this, commandListSet);
	
	auto textureResources = m_CurrentGeometryRender->GetTextureResources();
	
	m_CurrentLightRender->Draw(textureResources, lightList, commandListSet);

	
	///一番最後にmainFrameに今までの描画を移す
	ID3D12DescriptorHeap*  const ppHeaps[] = { DescriptorHeapManager::Instance().GetD3dDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV).Get() };
	auto mainFrame = dxDevice->GetFrameResource(dxDevice->GetFrameIndex());
	auto lastTextureResource = m_CurrentLightRender->GetTextureResources();

	//使うDescriptorHeapの設定
	commandListSet.m_CommandList.Get()->SetDescriptorHeaps(1, ppHeaps);

	//ResourceBarrierの設定(描画前Ver)
	D3D12_RESOURCE_BARRIER beforeBarrier;
	beforeBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	beforeBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	beforeBarrier.Transition.pResource = mainFrame->GetTexture()->GetResource().Get();
	beforeBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	beforeBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	beforeBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		
	commandListSet.m_CommandList.Get()->ResourceBarrier(1, &beforeBarrier);

	//Clear
	const float clearColor[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	commandListSet.m_CommandList->ClearRenderTargetView(mainFrame->GetRTVBufferView()->m_CpuHandle, clearColor, 0, nullptr);

	//レンダーターゲットの設定
	commandListSet.m_CommandList.Get()->OMSetRenderTargets(1, &(mainFrame->GetRTVBufferView()->m_CpuHandle), FALSE, nullptr);

	commandListSet.m_CommandList.Get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	commandListSet.m_CommandList.Get()->SetPipelineState(m_ChangeFrameTexMat->GetRenderSet()->pipelineStateObj->GetPipelineState().Get());

	commandListSet.m_CommandList.Get()->SetGraphicsRootSignature(m_ChangeFrameTexMat->GetRenderSet()->rootSignature->GetRootSignature().Get());
	
	//一個しかないけど、こうしないと値が取れない。[""]でとってもいいけど、キーが決め打ちになるから嫌だ
	for(auto res : lastTextureResource)
		m_ChangeFrameTexMat->GetRenderSet()->rootSignature->SetGraphicsRootDescriptorTable(&commandListSet, "Texture", res.second->GetSRVBufferView());
	
	commandListSet.m_CommandList->DrawInstanced(4, 1, 0, 0);

	//ResourceBarrierの設定(描画前Ver)
	D3D12_RESOURCE_BARRIER afterBarrier;
	afterBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	afterBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	afterBarrier.Transition.pResource = mainFrame->GetTexture()->GetResource().Get();
	afterBarrier.Transition.StateBefore =D3D12_RESOURCE_STATE_RENDER_TARGET;
	afterBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	afterBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	commandListSet.m_CommandList.Get()->ResourceBarrier(1, &afterBarrier);

	dxDevice->GetGraphicContext()->ExecuteCommandList(commandListSet);
	dxDevice->GetGraphicContext()->DiscardCommandListSet(commandListSet);
}
