#include "Texture2D.h"

void Texture2D::CreateFromBackBuffer(const ComPtr<IDXGISwapChain3>& swapChain, UINT index)
{
	ThrowIfFailed(swapChain.Get()->GetBuffer(index, IID_PPV_ARGS(m_TextureResource.ReleaseAndGetAddressOf())));
}

void Texture2D::CreateDepth(const ComPtr<ID3D12Device>& device, const DepthInfo info)
{
	D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
	depthOptimizedClearValue.Format = info.m_Format;
	depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
	depthOptimizedClearValue.DepthStencil.Stencil = 0;

	D3D12_RESOURCE_DESC depthBufferDesc = {};
	depthBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthBufferDesc.Alignment = 0;
	depthBufferDesc.Width = info.m_Width;
	depthBufferDesc.Height = info.m_Height;
	depthBufferDesc.DepthOrArraySize = 1;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.Format = info.m_Format;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthBufferDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	//プロパティ設定用構造体の設定
	D3D12_HEAP_PROPERTIES prop = {};
	prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.CreationNodeMask = 1;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
	prop.VisibleNodeMask = 1;

	device.Get()->CreateCommittedResource(&prop,
		D3D12_HEAP_FLAG_NONE,
		&depthBufferDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthOptimizedClearValue,
		IID_PPV_ARGS(m_TextureResource.ReleaseAndGetAddressOf()));
}

void Texture2D::ShutDown()
{
	//ComPtrだから要らない説
	//m_TextureResource->Release();
}
