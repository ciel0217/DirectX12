#pragma once
#include "DirectX12Helper.h"
#include "CommandAllocator.h"
#include "CommandList.h"
#include "CommandQueue.h"

class CommandList;
class CommandAllocator;
class CommandQueue;

struct CommandListSet {
	CommandListSet(ComPtr<ID3D12GraphicsCommandList> &list, ComPtr<ID3D12CommandAllocator> &allocator, UINT64 fenceValue) :
		m_CommandList(list), m_Allocator(allocator), m_FenceValue(fenceValue) { }

	ComPtr<ID3D12GraphicsCommandList> m_CommandList;
	ComPtr<ID3D12CommandAllocator> m_Allocator;
	UINT64 m_FenceValue;
};

class CommandContext{
public:
	CommandContext();
	virtual ~CommandContext();

	//�R�}���h�����ɕK�v�Ȃ��̂��ꊇ����
	void Create(const ComPtr<ID3D12Device> &device, D3D12_COMMAND_LIST_TYPE type);
	void Shutdown();

	//�R�}���h�����ɕK�v�ȃR�}���h���X�g�A�A���P�[�^�[���̃f�[�^���擾
	//�����̃p�C�v���C���X�e�[�g�́A�v���R�}���h���X�g�������̃p�C�v���C���X�e�[�g�݂̂����g�p���Ȃ��Ȃǃh���C�o���œK���ł���ꍇ�Ɏw�肷��B
	//�Ⴆ��Bundle�݂̂̕`����s�����B(https://docs.microsoft.com/en-us/windows/desktop/api/d3d12/nf-d3d12-id3d12graphicscommandlist-reset)
	CommandListSet RequestCommandListSet(ID3D12PipelineState* state = nullptr);

	//�R�}���h���X�g�Z�b�g�̃R�}���h���X�g�ƃR�}���h�A���P�[�^�[��ԋp����
	void DiscardCommandListSet(const CommandListSet& set);

	//�R�}���h���X�g���R�}���h�L���[�ɓn���Ď��s����B�߂�l�͓n�����R�}���h���X�g�̃t�F���X�l
	UINT64 ExecuteCommandList(const ComPtr<ID3D12GraphicsCommandList> &commandList);

	//�R�}���h���X�g�Z�b�g�̃t�F���X�l�C���N�������g���s��
	void ExecuteCommandList(CommandListSet& set);

	//���ׂẴR�}���h�L���[����������܂őҋ@
	void WaitForIdle();

	CommandQueue* GetCommandQueue() { return &m_CommandQueue; };
	ID3D12CommandQueue* GetDirectQueue() const { return m_CommandQueue.Getm_CommandQueue().Get(); }

protected:
	D3D12_COMMAND_LIST_TYPE m_CommandListType;

	CommandList m_CommandList;
	CommandAllocator m_CommandAllocator;
	CommandQueue m_CommandQueue;
};