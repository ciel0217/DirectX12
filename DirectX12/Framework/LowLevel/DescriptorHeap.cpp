#include "DescriptorHeap.h"
#include "DirectX12Helper.h"
#include "../MemoryAllocator/TLSFAllocator.h"
#include "BufferView.h"

bool DescriptorHeap::Create(const ComPtr<ID3D12Device> &device, UINT numDescriptors)
{
	D3D12_DESCRIPTOR_HEAP_FLAGS flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	if (m_DescriptorHeapType == D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = MAX_NUM_HEAP;//numDescriptorsでもいいんだけど、MAX_NUM_HEAPを超える数値を入力されたくない
	//heapDesc.NumDescriptors = numDescriptors;
	heapDesc.Type = m_DescriptorHeapType;
	heapDesc.Flags = flags;
	ThrowIfFailed(device.Get()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_DescriptorHeap.ReleaseAndGetAddressOf())));

	m_IncrimentSize = device.Get()->GetDescriptorHandleIncrementSize(m_DescriptorHeapType);
	m_CpuHandleStart = m_DescriptorHeap.Get()->GetCPUDescriptorHandleForHeapStart();
	m_GpuHandleStart = m_DescriptorHeap.Get()->GetGPUDescriptorHandleForHeapStart();

	m_Allocator = new TLSFAllocator(sizeof(BufferView));
	m_Allocator->Init(numDescriptors);

	return false;
}

void DescriptorHeap::Shutdown()
{
	delete m_Allocator;
}

void DescriptorHeap::AllocateBufferView(BufferView * bufferView, UINT descriptorNum)
{
	void * bufferViewPtr = m_Allocator->DivideMemory(descriptorNum);//データポインタを取得して
	Block* block = m_Allocator->GetBlockFromDataPtr(bufferViewPtr);//データポインタからブロックポインタを取得する

	bufferView->m_Location = m_Allocator->BlockLocalIndex(block);//blockのローカルインデックス
	bufferView->m_Size = block->m_Size;
	bufferView->m_GpuHandle = GetGpuHandle(bufferView->m_Location);
	bufferView->m_CpuHandle = GetCpuHandle(bufferView->m_Location);
}

void DescriptorHeap::DiscardBufferView(const BufferView & bufferView)
{
	//デスクリプターのインデックスからメモリアロケーターの管理するデータポインタを復元
	void* bufferViewPtr = m_Allocator->GetDataPtrFromBlockLocation(bufferView.m_Location);
	m_Allocator->ReleaseMemory(bufferViewPtr);
}

const D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::GetGpuHandle(unsigned int location)
{
	D3D12_GPU_DESCRIPTOR_HANDLE rtvHandle = m_GpuHandleStart;
	rtvHandle.ptr += m_IncrimentSize * static_cast<SIZE_T>(location);
	return rtvHandle;
}

const D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeap::GetCpuHandle(unsigned int location)
{
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_CpuHandleStart;
	rtvHandle.ptr += m_IncrimentSize * static_cast<SIZE_T>(location);
	return rtvHandle;
}

//////////////////////////////////////////////////////
//マネージャークラス
//////////////////////////////////////////////////////

DescriptorHeapManager* DescriptorHeapManager::m_Singleton;

DescriptorHeapManager::DescriptorHeapManager():
	m_RtvHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV),
	m_DsvHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV),
	m_CbvSrvHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
{
}

DescriptorHeapManager & DescriptorHeapManager::CreateInstance()
{
	if (!m_Singleton)m_Singleton = new DescriptorHeapManager();
	return *m_Singleton;
}

DescriptorHeapManager::~DescriptorHeapManager()
{
	Shutdown();
}

void DescriptorHeapManager::Create(const ComPtr<ID3D12Device>& device)
{
	m_RtvHeap.Create(device);
	m_DsvHeap.Create(device);
	m_CbvSrvHeap.Create(device);
}

void DescriptorHeapManager::Shutdown()
{
}

void DescriptorHeapManager::CreateRenderTargetView(ID3D12Resource *const* textureBuffers, BufferView * dstView, unsigned int viewCount)
{
	m_RtvHeap.AllocateBufferView(dstView, viewCount);

	ID3D12Device* device = nullptr;
	textureBuffers[0]->GetDevice(__uuidof(*device), reinterpret_cast<void**>(&device));

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = dstView->m_CpuHandle;

	D3D12_RENDER_TARGET_VIEW_DESC rtvViewDesc = {};
	for (unsigned int i = 0; i < viewCount; i++) {
		auto desc = textureBuffers[i]->GetDesc();
		rtvViewDesc.Format = desc.Format;
		rtvViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		rtvViewDesc.Texture2D.MipSlice = 0;
		rtvViewDesc.Texture2D.PlaneSlice = 0;

		device->CreateRenderTargetView(textureBuffers[i], &rtvViewDesc, rtvHandle);
		rtvHandle.ptr += m_RtvHeap.GetIncrimentSize();
		
	}

	device->Release();
}

void DescriptorHeapManager::CreateConstantBufferView(ID3D12Resource ** constantBuffers, BufferView * dstView, unsigned int viewCount)
{
	m_CbvSrvHeap.AllocateBufferView(dstView, viewCount);

	ID3D12Device* device = nullptr;
	constantBuffers[0]->GetDevice(__uuidof(*device), reinterpret_cast<void**>(&device));

	D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle = dstView->m_CpuHandle;
	D3D12_CONSTANT_BUFFER_VIEW_DESC constantDesc = {};
	for (unsigned int i = 0; i < viewCount; i++) {
		auto desc = constantBuffers[i]->GetDesc();
		constantDesc.BufferLocation = constantBuffers[i]->GetGPUVirtualAddress();
		constantDesc.SizeInBytes = static_cast<UINT>(desc.Width);

		device->CreateConstantBufferView(&constantDesc, descriptorHandle);
		descriptorHandle.ptr += m_CbvSrvHeap.GetIncrimentSize();
	}

	device->Release();
}

void DescriptorHeapManager::CreateShaderResourceView(ID3D12Resource ** shaderResources, BufferView * dstView, unsigned int viewCount, const std::vector<D3D12_BUFFER_SRV>& buffers)
{
	m_CbvSrvHeap.AllocateBufferView(dstView, viewCount);

	ID3D12Device* device = nullptr;
	shaderResources[0]->GetDevice(__uuidof(*device), reinterpret_cast<void**>(&device));

	D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle = dstView->m_CpuHandle;
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	for (unsigned int i = 0; i < viewCount; i++) {
		srvDesc.Format = buffers[i].Flags == D3D12_BUFFER_SRV_FLAG_RAW ? DXGI_FORMAT_R32_TYPELESS : DXGI_FORMAT_UNKNOWN;
		srvDesc.Buffer = buffers[i];

		device->CreateShaderResourceView(shaderResources[i], &srvDesc, descriptorHandle);
		descriptorHandle.ptr += m_CbvSrvHeap.GetIncrimentSize();
	}

	device->Release();
}

void DescriptorHeapManager::CreateTextureShaderResourceView(ID3D12Resource ** textureResources, BufferView * dstView, unsigned int viewCount)
{
	m_CbvSrvHeap.AllocateBufferView(dstView, viewCount);

	ID3D12Device* device = nullptr;
	textureResources[0]->GetDevice(__uuidof(*device), reinterpret_cast<void**>(&device));

	D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle = dstView->m_CpuHandle;
	for (unsigned int i = 0; i < viewCount; i++) {
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		auto desc = textureResources[i]->GetDesc();
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = desc.Format;

		//テクスチャ配列が６枚あればキューブマップとして処理する...
		if (desc.DepthOrArraySize == 6) {
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
			srvDesc.TextureCube.MipLevels = desc.MipLevels;
		}
		else {
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = desc.MipLevels;
		}

		device->CreateShaderResourceView(textureResources[i], &srvDesc, descriptorHandle);
		descriptorHandle.ptr += m_CbvSrvHeap.GetIncrimentSize();
	}

	device->Release();
}

void DescriptorHeapManager::CreateDepthStencilView(ID3D12Resource *const* depthStencils, BufferView * dstView, unsigned int viewCount)
{
	m_DsvHeap.AllocateBufferView(dstView, viewCount);

	ID3D12Device* device = nullptr;
	depthStencils[0]->GetDevice(__uuidof(*device), reinterpret_cast<void**>(&device));

	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dstView->m_CpuHandle;

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	for (unsigned int i = 0; i < viewCount; ++i) {
		auto desc = depthStencils[i]->GetDesc();
		dsvDesc.Format = desc.Format;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

		device->CreateDepthStencilView(depthStencils[i], &dsvDesc, dsvHandle);
		dsvHandle.ptr += m_DsvHeap.GetIncrimentSize();
	}

	device->Release();
}

void DescriptorHeapManager::CreateUnorederdAcsessView(ID3D12Resource ** unorderdAccess, BufferView * dstView, unsigned int viewCount, const std::vector<D3D12_BUFFER_UAV>& buffers)
{
	m_CbvSrvHeap.AllocateBufferView(dstView, viewCount);

	ID3D12Device* device = nullptr;
	unorderdAccess[0]->GetDevice(__uuidof(*device), reinterpret_cast<void**>(&device));

	D3D12_CPU_DESCRIPTOR_HANDLE uavHandle = dstView->m_CpuHandle;

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	for (unsigned int i = 0; i < viewCount; ++i) {
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer = buffers[i];
		uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;

		device->CreateUnorderedAccessView(unorderdAccess[i], unorderdAccess[i], &uavDesc, uavHandle);
		uavHandle.ptr += m_CbvSrvHeap.GetIncrimentSize();
	}

	device->Release();
}

void DescriptorHeapManager::DiscardRenderTargetView(const BufferView & bufferView)
{
	m_RtvHeap.DiscardBufferView(bufferView);
}

void DescriptorHeapManager::DiscardConstantBufferView(const BufferView & bufferView)
{
	m_CbvSrvHeap.DiscardBufferView(bufferView);
}

void DescriptorHeapManager::DiscardShaderResourceView(const BufferView & bufferView)
{
	m_CbvSrvHeap.DiscardBufferView(bufferView);
}

void DescriptorHeapManager::DiscardDepthStencilView(const BufferView & bufferView)
{
	m_DsvHeap.DiscardBufferView(bufferView);
}

const ComPtr<ID3D12DescriptorHeap>& DescriptorHeapManager::GetD3dDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type)
{
	
	switch (type)
	{
	case D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV:
		return m_CbvSrvHeap.GetDescriptorHeap();
		break;
	case D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER:
		break;
	case D3D12_DESCRIPTOR_HEAP_TYPE_RTV:
		return m_RtvHeap.GetDescriptorHeap();
		break;
	case D3D12_DESCRIPTOR_HEAP_TYPE_DSV:
		return m_DsvHeap.GetDescriptorHeap();
		break;
	case D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES:
		break;
	default:
		break;
	}
	return nullptr;
}

DescriptorHeap * DescriptorHeapManager::GetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type)
{
	switch (type)
	{
	case D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV:
		return &m_CbvSrvHeap;
		break;
	case D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER:
		break;
	case D3D12_DESCRIPTOR_HEAP_TYPE_RTV:
		return &m_RtvHeap;
		break;
	case D3D12_DESCRIPTOR_HEAP_TYPE_DSV:
		return &m_DsvHeap;
		break;
	case D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES:
		break;
	default:
		break;
	}
	return nullptr;
}
