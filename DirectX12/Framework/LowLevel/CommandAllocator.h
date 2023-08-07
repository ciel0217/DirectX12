#pragma once
#include "DirectX12Helper.h"
#include <queue>

class CommandAllocator {
public:
	CommandAllocator();
	~CommandAllocator();

	void Create(const ComPtr<ID3D12Device> &device, D3D12_COMMAND_LIST_TYPE type);
	void Shutdown();

	//既に実行完了しているアロケーターを要求
	ComPtr<ID3D12CommandAllocator> RequestAllocator(UINT64 completedFenceValue);

	//実行完了したアロケータを返却
	void DiscardAllocator(UINT64 fenceValue, const ComPtr<ID3D12CommandAllocator> &allocator);

	//アロケータープールの要素数
	inline size_t Size() { return m_CommandAllocatorPool.size(); }

private:
	D3D12_COMMAND_LIST_TYPE m_CommandListType;

	ComPtr<ID3D12Device> m_Device;
	std::vector<ComPtr<ID3D12CommandAllocator>> m_CommandAllocatorPool;
	std::queue<std::pair<UINT64, ComPtr<ID3D12CommandAllocator>>> m_ReadyAllocators;
};
