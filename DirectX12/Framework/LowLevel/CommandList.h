#pragma once

#include "DirectX12Helper.h"
#include <queue>

class CommandList {
public:
	CommandList();
	~CommandList();

	void Create(const ComPtr<ID3D12Device> &device, D3D12_COMMAND_LIST_TYPE type);
	void Shutdown();

	//���Ɏ��s�������Ă���R�}���h���X�g��v��
	ComPtr<ID3D12GraphicsCommandList> RequestCommandList(UINT64 completedFenceValue, const ComPtr<ID3D12CommandAllocator> &allocator);

	//���s���������R�}���h���X�g��ԋp
	void DiscardCommandList(UINT64 fenceValue, const ComPtr<ID3D12GraphicsCommandList>& list);

	//�A���P�[�^�[�v�[���̗v�f��
	inline const size_t GetSize() { return m_CommandListPool.size(); }

private:
	D3D12_COMMAND_LIST_TYPE m_CommandListType;

	ComPtr<ID3D12Device> m_Device;
	std::vector<ComPtr<ID3D12GraphicsCommandList>> m_CommandListPool;
	std::queue<std::pair<UINT64, ComPtr<ID3D12GraphicsCommandList>>> m_ReadyCommandLists;
};