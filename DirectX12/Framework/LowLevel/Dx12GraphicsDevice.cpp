#include "Dx12GraphicsDevice.h"


Dx12GraphicsDevice* Dx12GraphicsDevice::m_Instance;



BOOL Dx12GraphicsDevice::Init(HWND hWND)
{
	UINT dxgiFactoryFlags = 0;
#ifdef _DEBUG
	ComPtr<ID3D12Debug> debugController;
	
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) 
	{
		debugController->EnableDebugLayer();
		dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
	}
#endif

	ComPtr<IDXGIFactory4> factory4;
	ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory4)));

	ComPtr<IDXGIAdapter1> adapter1;
	for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != factory4->EnumAdapters1(adapterIndex, &adapter1); adapterIndex++) 
	{
		DXGI_ADAPTER_DESC1 desc;
		adapter1->GetDesc1(&desc);

		//仮想Direct3D12対応GPUは無視する
		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) 
			continue;
		

		//デバイス生成の確認が成功したら終了
		if (SUCCEEDED(D3D12CreateDevice(adapter1.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr))) 
			break;
		
	}

	//デバイス生成
	ThrowIfFailed(D3D12CreateDevice(adapter1.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device)));

	m_GraphicsCommandContext.Create(m_Device, D3D12_COMMAND_LIST_TYPE_DIRECT);

	//スワップチェーン生成
	{
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.BufferCount = FRAME_COUNT;
		swapChainDesc.Width = SCREEN_WIDTH;
		swapChainDesc.Height = SCREEN_HEIGHT;
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.SampleDesc.Count = 1;

		ComPtr<IDXGISwapChain1> swapChain;
		ID3D12CommandQueue* const queue = m_GraphicsCommandContext.GetDirectQueue();

		ThrowIfFailed(factory4->CreateSwapChainForHwnd(queue, hWND, &swapChainDesc, nullptr, nullptr, swapChain.ReleaseAndGetAddressOf()));

		//フルスクリーンサポートをオフ
		ThrowIfFailed(factory4->MakeWindowAssociation(hWND, DXGI_MWA_NO_ALT_ENTER));
		ThrowIfFailed(swapChain.As(&m_SwapChain));
	}

	DescriptorHeapManager& heapManager = DescriptorHeapManager::CreateInstance();
	heapManager.Create(m_Device);

	//ビューポート初期化
	m_ViewPort.TopLeftX = 0.0f;
	m_ViewPort.TopLeftY = 0.0f;
	m_ViewPort.Width = static_cast<float>(SCREEN_WIDTH);
	m_ViewPort.Height = static_cast<float>(SCREEN_HEIGHT);
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;

	m_ScissorRect = { 0, 0, static_cast<LONG>(SCREEN_WIDTH), static_cast<LONG>(SCREEN_HEIGHT) };

	{
		DepthInfo info(SCREEN_WIDTH, SCREEN_HEIGHT, DXGI_FORMAT_D32_FLOAT);
		m_DepthStencil.CreateDepth(m_Device, info);
		const ComPtr<ID3D12Resource>& res = m_DepthStencil.GetResource();
		
		DescriptorHeapManager::Intance().CreateDepthStencilView(res.GetAddressOf(), &m_DSV, 1);
	}

	for (int i = 0; i < FRAME_COUNT; i++)
		m_FrameResource[i].Create(m_Device, m_SwapChain, i);

	m_FrameIndex = m_SwapChain.Get()->GetCurrentBackBufferIndex();
	

	//Test用
	m_VShader.Create("Framework/Shader/TestVertex.cso");
	m_PShader.Create("Framework/Shader/TestPixel.cso");

	

	m_RootSignature.Create(m_Device, m_VShader, m_PShader);
	m_PSO.CreateGraphicPipeline(m_Device, &m_RootSignature, &m_VShader, &m_PShader);
	


	return 0;
}

void Dx12GraphicsDevice::Update()
{
}

void Dx12GraphicsDevice::Render()
{
	{
		auto commandListSet = m_GraphicsCommandContext.RequestCommandListSet();
		
		ComPtr<ID3D12DescriptorHeap>  const ppHeaps[] = { DescriptorHeapManager::Intance().GetD3dDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)};

		//使うDescriptorHeapの設定
		commandListSet.m_CommandList.Get()->SetDescriptorHeaps(1, ppHeaps->GetAddressOf());
		
		//ビューポート&シザー矩形設定
		commandListSet.m_CommandList.Get()->RSSetViewports(1, &m_ViewPort);
		commandListSet.m_CommandList.Get()->RSSetScissorRects(1, &m_ScissorRect);
		
		{
			//ResourceBarrierの設定(描画前Ver)
			D3D12_RESOURCE_BARRIER barrier;
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.pResource = m_FrameResource[m_FrameIndex].GetTexture()->GetResource().Get();
			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

			commandListSet.m_CommandList.Get()->ResourceBarrier(1, &barrier);
		}

		//レンダーターゲットの設定
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_FrameResource[m_FrameIndex].GetBufferView().m_CpuHandle;
		commandListSet.m_CommandList.Get()->OMSetRenderTargets(1, &rtvHandle, FALSE, &m_DSV.m_CpuHandle);

		//レンダーターゲットクリア
		const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
		commandListSet.m_CommandList.Get()->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

		//各オブジェクト描画

		//ResourceBarrierの設定(描画後)
		{
			D3D12_RESOURCE_BARRIER barrier;
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.pResource = m_FrameResource[m_FrameIndex].GetTexture()->GetResource().Get();
			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

			commandListSet.m_CommandList.Get()->ResourceBarrier(1, &barrier);
		}

		m_GraphicsCommandContext.ExecuteCommandList(commandListSet);
		m_GraphicsCommandContext.DiscardCommandListSet(commandListSet);
	}

	ThrowIfFailed(m_SwapChain->Present(1, 0));

	MoveToNextFrame();
}

void Dx12GraphicsDevice::MoveToNextFrame()
{
	auto commandQueue = m_GraphicsCommandContext.GetCommandQueue();
	const UINT64 currentFenceValue = m_FrameResource[m_FrameIndex].GetFenceValue();
	commandQueue->WaitForFence(currentFenceValue);

	m_FrameIndex = m_SwapChain->GetCurrentBackBufferIndex();

	const UINT64 fenceValue = commandQueue->IncrementFence();
	m_FrameResource[m_FrameIndex].SetFenceValue(fenceValue);

}

BOOL Dx12GraphicsDevice::Release()
{
	return 0;
}
