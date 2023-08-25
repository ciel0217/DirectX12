#include "GpuBuffer.h"
#include "../LowLevel/CommandContext.h"

template<typename T>
void GpuReadOnlyBuffer::CreateWithData(const ComPtr<ID3D12Device>& device, const ComPtr<ID3D12GraphicsCommandList>& commandList, const std::vector<T>& data)
{
	D3D12_HEAP_PROPERTIES defaultHeapProp = { D3D12_HEAP_TYPE_DEFAULT };
	D3D12_HEAP_PROPERTIES uploadHeapProp = { D3D12_HEAP_TYPE_UPLOAD };

	D3D12_RESOURCE_DESC bufferDesc = {};
	bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	bufferDesc.Width = data.size() * sizeof(T);
	bufferDesc.Height = 1;
	bufferDesc.DepthOrArraySize = 1;
	bufferDesc.MipLevels = 1;
	bufferDesc.SampleDesc.Count = 1;
	bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	ComPtr<ID3D12Resource> uploadHeap;

	//コピーに使う一時的なCPU用のResource作成
	ThrowIfFailed(device.Get()->CreateCommittedResource
	(
		&uploadHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&bufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(uploadHeap.ReleaseAndGetAddressOf())
	));

	//GPU用のResource作成
	ThrowIfFailed(device.Get()->CreateCommittedResource
	(
		&defaultHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&bufferDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(m_Resource.ReleaseAndGetAddressOf())
	));

	//VRAMを直接Mapは出来ないから、一旦Map可能なbufferに書き込んで、コピーを行う
	T* mapPtr = nullptr;
	ThrowIfFailed(uploadHeap.Get()->Map(0, nullptr, reinterpret_cast<void**>(mapPtr)));//CPU からリードバック データを認識できるようにする。今回はnullptrのため認識できなくしてる(GPUReadOnly)
	memcpy(mapPtr, data.data(), bufferDesc.Width);

	commandList.Get()->CopyBufferRegion(m_Resource.Get(), 0, uploadHeap.Get(), 0, bufferDesc.Width);
}

void GpuReadOnlyBuffer::CreateNonData(const ComPtr<ID3D12Device>& device, const UINT size, const D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE)
{
	D3D12_HEAP_PROPERTIES defaultHeapProp = { D3D12_HEAP_TYPE_DEFAULT };

	D3D12_RESOURCE_DESC bufferDesc = {};
	bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	bufferDesc.Width = size;
	bufferDesc.Height = 1;
	bufferDesc.DepthOrArraySize = 1;
	bufferDesc.MipLevels = 1;
	bufferDesc.SampleDesc.Count = 1;
	bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	bufferDesc.Flags = flags;

	//GPU用のResource作成
	ThrowIfFailed(device.Get()->CreateCommittedResource
	(
		&defaultHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&bufferDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(m_Resource.ReleaseAndGetAddressOf())
	));
}

void GpuAndCpuBuffer::CreateNonData(const ComPtr<ID3D12Device>& device, const UINT size)
{
	D3D12_HEAP_PROPERTIES uploadHeapProperties = { D3D12_HEAP_TYPE_UPLOAD };

	D3D12_RESOURCE_DESC bufferDesc = {};
	bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	bufferDesc.Width = size;
	bufferDesc.Height = 1;
	bufferDesc.DepthOrArraySize = 1;
	bufferDesc.MipLevels = 1;
	bufferDesc.SampleDesc.Count = 1;
	bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	ThrowIfFailed(device->CreateCommittedResource(
		&uploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&bufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_Resource.ReleaseAndGetAddressOf())));

	ThrowIfFailed(m_Resource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&m_DataPtr)));
}

void GpuAndCpuBuffer::WriteData(const void * data, const UINT size, const UINT startOffset)
{
	memcpy(m_DataPtr, reinterpret_cast<const unsigned char*>(data) + startOffset, size);
}


void VertexBuffer::CreateVertexBuffer(const ComPtr<ID3D12Device>& device, CommandContext* const context, const std::vector<VERTEX_3D>& data)
{
	auto commandListSet = context->RequestCommandListSet();
	CreateWithData<VERTEX_3D>(device, commandListSet.m_CommandList, data);

	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = m_Resource.Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	commandListSet.m_CommandList.Get()->ResourceBarrier(1, &barrier);

	m_VertexBufferView.BufferLocation = GetGpuVirtualAddress();
	m_VertexBufferView.StrideInBytes = sizeof(VERTEX_3D);
	m_VertexBufferView.SizeInBytes = static_cast<UINT>(sizeof(VERTEX_3D) * data.size());
}

void IndexBuffer::CreateIndexBuffer(const ComPtr<ID3D12Device>& device, CommandContext* const context, const std::vector<UINT>& data)
{
	auto commandListSet = context->RequestCommandListSet();
	CreateWithData<UINT>(device, commandListSet.m_CommandList, data);

	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = m_Resource.Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	commandListSet.m_CommandList.Get()->ResourceBarrier(1, &barrier);

	m_IndexBufferView.BufferLocation = GetGpuVirtualAddress();
	m_IndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	m_IndexBufferView.SizeInBytes = static_cast<UINT>(sizeof(UINT) * data.size());

	m_IndexCount = static_cast<UINT>(data.size());
}

void ConstantBuffer::CreateConstantBuffer(const ComPtr<ID3D12Device>& device, const UINT size)
{
	m_Size = size;

	//256バイトにアラインメント
	const UINT constantBufferSize = (size + (D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT - 1)) & ~(D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT - 1);
	CreateNonData(device, size);
}
