#include "Dx12GraphicsDevice.h"
#include "../Resources/MaterialManager.h"

Dx12GraphicsDevice* Dx12GraphicsDevice::m_Instance;

BOOL Dx12GraphicsDevice::Init(HWND hWND)
{

	UINT dxgiFactoryFlags = 0;
#ifdef _DEBUG

	SetDllDirectoryA("../Assimp/lib/x64-Debug");
	LoadLibraryExA("assimp-vc142-mtd.dll", NULL, NULL);
	ComPtr<ID3D12Debug> debugController;
	
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) 
	{
		debugController->EnableDebugLayer();
		dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
	}
#else
	SetDllDirectoryA("../Assimp/lib/x64-Release");
	LoadLibraryExA("assimp-vc142-mt.dll", NULL, NULL);
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
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	//デバイス生成
	ThrowIfFailed(D3D12CreateDevice(adapter1.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device)));

	m_GraphicsCommandContext = std::make_unique<CommandContext>();

	m_GraphicsCommandContext->Create(m_Device, D3D12_COMMAND_LIST_TYPE_DIRECT);

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
		ID3D12CommandQueue* const queue = m_GraphicsCommandContext->GetDirectQueue();

		ThrowIfFailed(factory4->CreateSwapChainForHwnd(queue, hWND, &swapChainDesc, nullptr, nullptr, swapChain.ReleaseAndGetAddressOf()));

		//フルスクリーンサポートをオフ
		ThrowIfFailed(factory4->MakeWindowAssociation(hWND, DXGI_MWA_NO_ALT_ENTER));
		ThrowIfFailed(swapChain.As(&m_SwapChain));
	
	}

	DescriptorHeapManager& heapManager = DescriptorHeapManager::CreateInstance();
	heapManager.Create(m_Device);

	{
		m_DepthStencil = std::make_unique<Texture2D>();
		DepthInfo info(SCREEN_WIDTH, SCREEN_HEIGHT, DXGI_FORMAT_D32_FLOAT);
		m_DepthStencil->CreateDepth(m_Device, info);
		const ComPtr<ID3D12Resource>& res = m_DepthStencil->GetResource();
		m_DSV = std::make_unique<BufferView>();
		DescriptorHeapManager::Instance().CreateDepthStencilView(res.GetAddressOf(), m_DSV.get(), 1);
		res->SetName(L"DSV");
	}

	for (int i = 0; i < FRAME_COUNT; i++)
	{
		m_FrameResource[i] = std::make_unique<FrameResources>();
		m_FrameResource[i]->Create(m_Device, m_SwapChain, i);
	}
	m_FrameIndex = m_SwapChain.Get()->GetCurrentBackBufferIndex();

	MaterialManager::Create();

	test = new TestScene();
	test->Config();
	test->Init();

	return 0;
}

void Dx12GraphicsDevice::Update()
{
	test->Update();
}

void Dx12GraphicsDevice::Render()
{

	{
		test->Draw();
	}

	ThrowIfFailed(m_SwapChain.Get()->Present(1, 0));

	MoveToNextFrame();
}

void Dx12GraphicsDevice::MoveToNextFrame()
{
	auto commandQueue = m_GraphicsCommandContext->GetCommandQueue();
	const UINT64 currentFenceValue = m_FrameResource[m_FrameIndex]->GetFenceValue();
	commandQueue->WaitForFence(currentFenceValue);

	m_FrameIndex = m_SwapChain->GetCurrentBackBufferIndex();

	const UINT64 fenceValue = commandQueue->IncrementFence();
	m_FrameResource[m_FrameIndex]->SetFenceValue(fenceValue);

}

BOOL Dx12GraphicsDevice::Release()
{
	test->Uninit();
	delete test;

	return 1;
}
