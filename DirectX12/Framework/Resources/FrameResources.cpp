#include "FrameResources.h"
#include "Texture2D.h"
#include "../LowLevel/DescriptorHeap.h"

void FrameResources::Create(const ComPtr<ID3D12Device>& device, const ComPtr<IDXGISwapChain3>& swapChain, UINT frameIndex)
{
	m_Texture2D = std::make_unique<Texture2D>();
	m_Texture2D->CreateFromBackBuffer(swapChain, frameIndex);

	DescriptorHeapManager& heapManager = DescriptorHeapManager::Instance();
	heapManager.CreateRenderTargetView(m_Texture2D.get()->GetResource().GetAddressOf(), &m_BufferView, 1);
}

void FrameResources::ShutDown()
{
	DescriptorHeapManager& heapManager = DescriptorHeapManager::Instance();
	heapManager.DiscardRenderTargetView(m_BufferView);
	
	//スマートポインタはrelease要らない説
	//m_Texture2D.release();
}
