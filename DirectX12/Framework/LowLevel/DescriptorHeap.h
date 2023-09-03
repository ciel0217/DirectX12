#pragma once
#include "DirectX12.h"

#define MAX_NUM_HEAP 1024

class TLSFAllocator;
struct BufferView;

class DescriptorHeap
{
private:
	ComPtr<ID3D12DescriptorHeap> m_DescriptorHeap;
	const D3D12_DESCRIPTOR_HEAP_TYPE m_DescriptorHeapType;

	D3D12_CPU_DESCRIPTOR_HANDLE m_CpuHandleStart;
	D3D12_GPU_DESCRIPTOR_HANDLE m_GpuHandleStart;
	UINT m_IncrimentSize;

	TLSFAllocator* m_Allocator;

public:
	DescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type) : m_DescriptorHeapType(type) {}
	~DescriptorHeap() { Shutdown(); }

	bool Create(const ComPtr<ID3D12Device> &device, UINT numDescriptors = MAX_NUM_HEAP);
	void Shutdown();

	void AllocateBufferView(BufferView* bufferView, UINT descriptorNum);
	void DiscardBufferView(const BufferView &bufferView);

	const D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle(unsigned int location);
	const D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(unsigned int location);

	//生ポインタとどっちがいいかわからない...
	const ComPtr<ID3D12DescriptorHeap>& GetDescriptorHeap() const{ return m_DescriptorHeap; }
	const UINT GetIncrimentSize(){ return m_IncrimentSize; }
};


class DescriptorHeapManager
{
private:
	DescriptorHeap m_RtvHeap;
	DescriptorHeap m_DsvHeap;
	DescriptorHeap m_CbvSrvHeap;

	static DescriptorHeapManager* m_Singleton;

	
public:
	static DescriptorHeapManager& CreateInstance();

	DescriptorHeapManager();
	~DescriptorHeapManager();

	static DescriptorHeapManager& Intance() { return *m_Singleton; }

	void Create(const ComPtr<ID3D12Device> &device);
	void Shutdown();

	void CreateRenderTargetView(ID3D12Resource*const* textureBuffers, BufferView* dstView, unsigned int viewCount);
	void CreateConstantBufferView(ID3D12Resource** constantBuffers, BufferView* dstView, unsigned int viewCount);
	void CreateShaderResourceView(ID3D12Resource** shaderResources, BufferView* dstView, unsigned int viewCount, const std::vector<D3D12_BUFFER_SRV>& buffers);
	void CreateTextureShaderResourceView(ID3D12Resource*const* textureResources, BufferView* dstView, unsigned int viewCount);
	void CreateDepthStencilView(ID3D12Resource*const* depthStencils, BufferView* dstView, unsigned int viewCount);
	void CreateUnorederdAcsessView(ID3D12Resource** unorederdAccess, BufferView* dstView, unsigned int viewCount, const std::vector<D3D12_BUFFER_UAV>& buffers);

	void DiscardRenderTargetView(const BufferView& bufferView);
	void DiscardConstantBufferView(const BufferView& bufferView);
	void DiscardShaderResourceView(const BufferView& bufferView);
	void DiscardDepthStencilView(const BufferView& bufferView);

	//Direct3Dのデスクリプタヒープをタイプから直接取得
	const ComPtr<ID3D12DescriptorHeap>& GetD3dDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type);
	DescriptorHeap* GetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type);
};