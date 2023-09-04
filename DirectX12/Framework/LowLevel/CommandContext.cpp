#include "CommandContext.h"

CommandContext::CommandContext() : m_CommandListType()
{
}

CommandContext::~CommandContext()
{
	Shutdown();
}

void CommandContext::Create(const ComPtr<ID3D12Device> &device, D3D12_COMMAND_LIST_TYPE type)
{
	m_CommandListType = type;

	//コマンド処理に必要なものを一括生成
	m_CommandAllocator.Create(device, m_CommandListType);
	m_CommandList.Create(device, m_CommandListType);
	m_CommandQueue.Create(device, m_CommandListType);
}

void CommandContext::Shutdown()
{
}

CommandListSet CommandContext::RequestCommandListSet(ID3D12PipelineState * state)
{

	UINT64 fenceValue = m_CommandQueue.FenceValue();
	//すでに使用済みのアロケータ、リストを再利用
	auto allocator = m_CommandAllocator.RequestAllocator(fenceValue);
	auto list = m_CommandList.RequestCommandList(fenceValue, allocator);
	
	ThrowIfFailed(allocator.Get()->Reset());
	ThrowIfFailed(list->Reset(allocator.Get(), state));

	return CommandListSet(list, allocator, fenceValue);
}

void CommandContext::DiscardCommandListSet(const CommandListSet & set)
{
	m_CommandAllocator.DiscardAllocator(set.m_FenceValue, set.m_Allocator);
	m_CommandList.DiscardCommandList(set.m_FenceValue, set.m_CommandList);
}

UINT64 CommandContext::ExecuteCommandList(const ComPtr<ID3D12GraphicsCommandList>& commandList)
{
	return m_CommandQueue.ExecuteCommandList(commandList);
}

void CommandContext::ExecuteCommandList(CommandListSet & set)
{
	set.m_FenceValue = m_CommandQueue.ExecuteCommandList(set.m_CommandList);
}

void CommandContext::WaitForIdle()
{
	m_CommandQueue.WaitForIdle();
}
