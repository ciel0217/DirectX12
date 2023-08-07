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

	//コマンド処理に必要なものを一括生成
	void Create(const ComPtr<ID3D12Device> &device, D3D12_COMMAND_LIST_TYPE type);
	void Shutdown();

	//コマンド発効に必要なコマンドリスト、アロケーター等のデータを取得
	//引数のパイプラインステートは、要求コマンドリストが引数のパイプラインステートのみしか使用しないなどドライバが最適化できる場合に指定する。
	//例えばBundleのみの描画を行う時。(https://docs.microsoft.com/en-us/windows/desktop/api/d3d12/nf-d3d12-id3d12graphicscommandlist-reset)
	CommandListSet RequestCommandListSet(ID3D12PipelineState* state = nullptr);

	//コマンドリストセットのコマンドリストとコマンドアロケーターを返却する
	void DiscardCommandListSet(const CommandListSet& set);

	//コマンドリストをコマンドキューに渡して実行する。戻り値は渡したコマンドリストのフェンス値
	UINT64 ExecuteCommandList(const ComPtr<ID3D12GraphicsCommandList> &commandList);

	//コマンドリストセットのフェンス値インクリメントも行う
	void ExecuteCommandList(CommandListSet& set);

	//すべてのコマンドキューが完了するまで待機
	void WaitForIdle();

	CommandQueue* GetCommandQueue() { return &m_CommandQueue; };
	ID3D12CommandQueue* GetDirectQueue() const { return m_CommandQueue.Getm_CommandQueue().Get(); }

protected:
	D3D12_COMMAND_LIST_TYPE m_CommandListType;

	CommandList m_CommandList;
	CommandAllocator m_CommandAllocator;
	CommandQueue m_CommandQueue;
};