#pragma once
#include "DirectX12.h"

#define MAX_NUM_HEAP 1024

class DescriptorHeap
{
private:
	ComPtr<ID3D12DescriptorHeap> m_DescriptorHeap;
	const D3D12_DESCRIPTOR_HEAP_TYPE m_Type;

	D3D12_CPU_DESCRIPTOR_HANDLE m_CpuHandleStart;
	D3D12_GPU_DESCRIPTOR_HANDLE m_GpuHandleStart;
	UINT m_IncrementSize;

public:
	DescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type) : m_Type(type) {}
	~DescriptorHeap(){}

	bool Create(UINT numDescriptors);

	

};