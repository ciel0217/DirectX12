#pragma once

#include "DirectX12Helper.h"
#include <queue>

class CommandList {
public:
	CommandList();
	~CommandList();

	void Create(const ComPtr<ID3D12Device> &device, D3D12_COMMAND_LIST_TYPE type);
	void Shutdown();

	//既に実行完了しているコマンドリストを要求
	ComPtr<ID3D12GraphicsCommandList> RequestCommandList(UINT64 completedFenceValue, const ComPtr<ID3D12CommandAllocator> &allocator);

	//実行完了したコマンドリストを返却
	void DiscardCommandList(UINT64 fenceValue, const ComPtr<ID3D12GraphicsCommandList>& list);

	//アロケータープールの要素数
	inline const size_t GetSize() { return m_CommandListPool.size(); }

private:
	D3D12_COMMAND_LIST_TYPE m_CommandListType;

	ComPtr<ID3D12Device> m_Device;
	std::vector<ComPtr<ID3D12GraphicsCommandList>> m_CommandListPool;
	std::queue<std::pair<UINT64, ComPtr<ID3D12GraphicsCommandList>>> m_ReadyCommandLists;
};