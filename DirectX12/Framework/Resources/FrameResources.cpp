#include "FrameResources.h"
#include "Texture2D.h"
#include "../LowLevel/DescriptorHeap.h"
#include "../LowLevel/Window.h"

void FrameResources::Create(const ComPtr<ID3D12Device>& device, const ComPtr<IDXGISwapChain3>& swapChain, UINT frameIndex)
{
	m_Texture2D = std::make_unique<Texture2D>();
	m_Texture2D->CreateFromBackBuffer(swapChain, frameIndex);
	m_Texture2D->GetResource()->SetName(L"Texture");

	m_RTVBufferView = std::make_shared<BufferView>();
	DescriptorHeapManager& heapManager = DescriptorHeapManager::Instance();
	heapManager.CreateRenderTargetView(m_Texture2D.get()->GetResource().GetAddressOf(), m_RTVBufferView.get(), 1);
}

void FrameResources::Create(const ComPtr<ID3D12Device>& device, CommandContext* const commandContext)
{	
	m_Texture2D = std::make_unique<Texture2D>();

	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resDesc.Width = SCREEN_WIDTH;   //テクスチャ幅　　
	resDesc.Height = SCREEN_HEIGHT; //テクスチャ高さ　
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resDesc.SampleDesc.Count = 1;
	resDesc.SampleDesc.Quality = 0;
	resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET |
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	m_Texture2D->CreateTexture(device, commandContext, resDesc);

	m_RTVBufferView = std::make_shared<BufferView>();
	m_SRVBufferView = std::make_shared<BufferView>();
	DescriptorHeapManager& heapManager = DescriptorHeapManager::Instance();
	heapManager.CreateRenderTargetView(m_Texture2D->GetResource().GetAddressOf(), m_RTVBufferView.get(), 1);
	heapManager.CreateTextureShaderResourceView(m_Texture2D->GetResource().GetAddressOf(), m_SRVBufferView.get(), 1);
}

void FrameResources::ShutDown()
{
	DescriptorHeapManager& heapManager = DescriptorHeapManager::Instance();
	heapManager.DiscardRenderTargetView(m_RTVBufferView.get());
	
	if (m_SRVBufferView)
		heapManager.DiscardShaderResourceView(m_SRVBufferView.get());
	//スマートポインタはrelease要らない説
	//m_Texture2D.release();
}
