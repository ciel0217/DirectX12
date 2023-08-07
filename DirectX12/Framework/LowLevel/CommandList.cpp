#include "CommandList.h"

CommandList::CommandList():m_CommandListType()
{
}

CommandList::~CommandList()
{
	Shutdown();
}

void CommandList::Create(const ComPtr<ID3D12Device> &device, D3D12_COMMAND_LIST_TYPE type)
{
	m_Device = device;
	m_CommandListType = type;
}

void CommandList::Shutdown()
{
	////ComPtrÇæÇ©ÇÁï Ç…óvÇÁÇ»Ç¢ê‡
	//for (size_t i = 0; i < m_CommandListPool.size(); i++) 
	//{
	//	m_CommandListPool[i]->Release();
	//}

	m_CommandListPool.clear();
}

ComPtr<ID3D12GraphicsCommandList> CommandList::RequestCommandList(UINT64 completedFenceValue, const ComPtr<ID3D12CommandAllocator> &allocator)
{
	ComPtr<ID3D12GraphicsCommandList> list = nullptr;

	if (!m_ReadyCommandLists.empty())
	{
		auto& pair = m_ReadyCommandLists.front();

		if (completedFenceValue >= pair.first)
		{
			list = pair.second;
			m_ReadyCommandLists.pop();
		}
	}

	if (!list)
	{
		ThrowIfFailed(m_Device.Get()->CreateCommandList(0, m_CommandListType, allocator.Get(), nullptr, IID_PPV_ARGS(&list)));
		ThrowIfFailed(list->Close());
		m_CommandListPool.push_back(list);
	}

	return list;
}

void CommandList::DiscardCommandList(UINT64 fenceValue, const ComPtr<ID3D12GraphicsCommandList>& list)
{
	m_ReadyCommandLists.push(std::make_pair(fenceValue, list));
}
