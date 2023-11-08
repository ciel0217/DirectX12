#include "Texture2D.h"
#include <sstream>
#include <istream>
#include "DirectXTex/WICTextureLoader12.h"
#include <codecvt>


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

//コマンド実行まで行ってる
void Texture2D::CreateTexture(const ComPtr<ID3D12Device>& device, CommandContext* const context, ComPtr<ID3D12Resource>& uploadHeap, const std::string & name)
{
	auto commandListSet = context->RequestCommandListSet();
	
	std::unique_ptr<uint8_t[]> decodedData;
	D3D12_SUBRESOURCE_DATA subresource;
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;

	ThrowIfFailed(
		DirectX::LoadWICTextureFromFile(device.Get(), cv.from_bytes(name.c_str()).c_str(), m_TextureResource.ReleaseAndGetAddressOf(),
			decodedData, subresource)
	);

	D3D12_RESOURCE_DESC textureDesc = m_TextureResource->GetDesc();

	const UINT64 uploadBufferSize = GetRequiredIntermediateSize(m_TextureResource.Get(), 0, 1);

	CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);

	auto desc = CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);

	// Create the GPU upload buffer.
	ThrowIfFailed(
		device.Get()->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(uploadHeap.ReleaseAndGetAddressOf())));

	UpdateSubresources(commandListSet.m_CommandList.Get(), m_TextureResource.Get(), uploadHeap.Get(), 0, 0, 1, &subresource);

	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_TextureResource.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	commandListSet.m_CommandList.Get()->ResourceBarrier(1, &barrier);

	context->ExecuteCommandList(commandListSet.m_CommandList);
	context->DiscardCommandListSet(commandListSet);

	context->WaitForIdle();
}

void Texture2D::CreateTexture(const ComPtr<ID3D12Device>& device, CommandContext * const context, D3D12_RESOURCE_DESC resDesc)
{
	auto commandListSet = context->RequestCommandListSet();
	
	//D3D12_HEAP_PROPERTIES prop = {};
	//prop.Type = D3D12_HEAP_TYPE_DEFAULT;
	//prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	//prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	//prop.CreationNodeMask = 1;
	//prop.VisibleNodeMask = 1;
	
	//リソースの作成
	CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_DEFAULT);

	//D3D12_RESOURCE_DESC resDesc = {};
	//resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	//resDesc.Width = ;   //テクスチャ幅　　
	//resDesc.Height = windowHeight; //テクスチャ高さ　
	//resDesc.DepthOrArraySize = 1;
	//resDesc.MipLevels = 1;
	//resDesc.Format = DXGI_FORMAT_R16G16_FLOAT;
	//resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	//resDesc.SampleDesc.Count = 1;
	//resDesc.SampleDesc.Quality = 0;
	//resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET |
	//	D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	ThrowIfFailed(
		device.Get()->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, nullptr, IID_PPV_ARGS(m_TextureResource.ReleaseAndGetAddressOf())));

}

void Texture2D::ShutDown()
{
	//ComPtrだから要らない説
	//m_TextureResource->Release();
}
