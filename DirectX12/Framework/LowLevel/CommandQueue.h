#pragma once
#include "DirectX12Helper.h"
#include "../Utility/Utility.h"
#include "CommandAllocator.h"
//FenceÇ‡ê›íË

class CommandQueue
{

public:
	CommandQueue();
	virtual ~CommandQueue();

	void Create(const ComPtr<ID3D12Device> &device, D3D12_COMMAND_LIST_TYPE type);

	void WaitForFence(UINT64 fenceValue);
	void WaitForIdle();

	UINT64 IncrementFence();
	UINT64 ExecuteCommandList(ComPtr<ID3D12CommandList> commandList);

	bool IsFenceCompleted(UINT64 fenceValue);
	const UINT64 FenceValue() { return m_Fence->GetCompletedValue(); }
	void Shutdown();

private:
	CommandAllocator m_CommandAllocator;
	D3D12_COMMAND_LIST_TYPE m_CommandListType;
	GETSET(ComPtr<ID3D12CommandQueue>, m_CommandQueue);
	GETSET(UINT64, m_NextFenceValue);

	ComPtr<ID3D12Fence> m_Fence;
	HANDLE m_FenceHandle;
	UINT64 m_LastFenceValue;

};