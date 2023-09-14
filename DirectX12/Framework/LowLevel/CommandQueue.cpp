#include "CommandQueue.h"

CommandQueue::CommandQueue() :m_CommandListType()
{
}

CommandQueue::~CommandQueue()
{
	Shutdown();
}

void CommandQueue::Create(const ComPtr<ID3D12Device> &device, D3D12_COMMAND_LIST_TYPE type)
{
	m_CommandListType = type;

	D3D12_COMMAND_QUEUE_DESC qDesc = {};
	qDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	qDesc.Type = m_CommandListType;

	//�����m_CommandQueu�ɒl�͂��邩�킩��Ȃ�w
	ThrowIfFailed(device.Get()->CreateCommandQueue(&qDesc, IID_PPV_ARGS(m_CommandQueue.ReleaseAndGetAddressOf())));
	
	ThrowIfFailed(device.Get()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_Fence.ReleaseAndGetAddressOf())));
	m_FenceHandle = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (!m_FenceHandle)
	{
		ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
	}

	m_CommandAllocator.Create(device, m_CommandListType);

	return;
}

void CommandQueue::WaitForFence(UINT64 fenceValue)
{
	//�����̃t�F���X�l�����łɊ������Ă邩�ǂ���
	if (IsFenceCompleted(fenceValue))
		return;

	//���ĂȂ��ꍇ�͊�������܂ő҂�
	ThrowIfFailed(m_Fence.Get()->SetEventOnCompletion(fenceValue, m_FenceHandle));
	WaitForSingleObjectEx(m_FenceHandle, INFINITE, FALSE);
	m_LastFenceValue = fenceValue;
}

//���݂̃L���[����������܂ő�C
void CommandQueue::WaitForIdle()
{
	UINT64 fenceValue = IncrementFence();
	WaitForFence(fenceValue);
}

UINT64 CommandQueue::IncrementFence()
{
	ThrowIfFailed(m_CommandQueue.Get()->Signal(m_Fence.Get(), m_NextFenceValue));
	return m_NextFenceValue++;
}

UINT64 CommandQueue::ExecuteCommandList(ComPtr<ID3D12CommandList> commandList)
{
	//�`��֌W��graphicscommandlist�ɃL���X�g
	ThrowIfFailed(static_cast<ID3D12GraphicsCommandList*>(commandList.Get())->Close());
	m_CommandQueue.Get()->ExecuteCommandLists(1, commandList.GetAddressOf());
	
	return IncrementFence();
}

bool CommandQueue::IsFenceCompleted(UINT64 fenceValue)
{
	if (fenceValue > m_LastFenceValue)
		m_LastFenceValue = std::max(m_LastFenceValue, m_Fence->GetCompletedValue());

	return fenceValue <= m_LastFenceValue;
}

void CommandQueue::Shutdown()
{
	CloseHandle(m_FenceHandle);
	//ComPtr��Release���Ă΂Ȃ�
	//��d�J���ɂȂ�
}
