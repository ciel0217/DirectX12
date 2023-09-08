#pragma once
#include "DirectX12Helper.h"
#include <queue>

class CommandAllocator {
public:
	CommandAllocator();
	~CommandAllocator();

	void Create(const ComPtr<ID3D12Device> &device, D3D12_COMMAND_LIST_TYPE type);
	void Shutdown();

	//���Ɏ��s�������Ă���A���P�[�^�[��v��
	ComPtr<ID3D12CommandAllocator> RequestAllocator(UINT64 completedFenceValue);

	//���s���������A���P�[�^��ԋp
	void DiscardAllocator(UINT64 fenceValue, const ComPtr<ID3D12CommandAllocator> &allocator);

	//�A���P�[�^�[�v�[���̗v�f��
	inline size_t Size() { return m_CommandAllocatorPool.size(); }

private:
	D3D12_COMMAND_LIST_TYPE m_CommandListType;

	ComPtr<ID3D12Device> m_Device;
	std::vector<ComPtr<ID3D12CommandAllocator>> m_CommandAllocatorPool;
	std::queue<std::pair<UINT64, ComPtr<ID3D12CommandAllocator>>> m_ReadyAllocators;
};
