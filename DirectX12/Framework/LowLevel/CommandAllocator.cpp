#include "CommandAllocator.h"

CommandAllocator::CommandAllocator():m_CommandListType()
{
}

CommandAllocator::~CommandAllocator()
{
	Shutdown();
}

void CommandAllocator::Create(const ComPtr<ID3D12Device> &device, D3D12_COMMAND_LIST_TYPE type)
{
	m_Device = device;
	m_CommandListType = type;
}

void CommandAllocator::Shutdown()
{
	//ComPtr‚¾‚©‚çRelease‚¢‚ç‚È‚¢
	m_CommandAllocatorPool.clear();
}

ComPtr<ID3D12CommandAllocator> CommandAllocator::RequestAllocator(UINT64 completedFenceValue)
{
	ComPtr<ID3D12CommandAllocator> allocator = nullptr;

	if (!m_ReadyAllocators.empty())
	{
		auto& pair = m_ReadyAllocators.front();

		if (pair.first <= completedFenceValue)
		{
			allocator = pair.second;
			ThrowIfFailed(allocator.Get()->Reset());
			m_ReadyAllocators.pop();
		}
	}

	if (!allocator)
	{
		ThrowIfFailed(m_Device.Get()->CreateCommandAllocator(m_CommandListType, IID_PPV_ARGS(&allocator)));
		m_CommandAllocatorPool.push_back(allocator);
	}

	return allocator;
}

void CommandAllocator::DiscardAllocator(UINT64 fenceValue, const ComPtr<ID3D12CommandAllocator> &allocator)
{
	m_ReadyAllocators.push(std::make_pair(fenceValue, allocator));
}
