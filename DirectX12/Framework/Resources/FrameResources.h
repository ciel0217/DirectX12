#pragma once
#include "../LowLevel/DirectX12Helper.h"
#include "../LowLevel/BufferView.h"

class Texture2D;
class CommandContext;

class FrameResources
{
private:
	std::unique_ptr<Texture2D> m_Texture2D;
	
	std::shared_ptr<BufferView> m_RTVBufferView;
	std::shared_ptr<BufferView> m_SRVBufferView;
	UINT64 m_FenceValue;

public:
	FrameResources(){}
	~FrameResources() { ShutDown(); }

	void Create(const ComPtr<ID3D12Device> &device, const ComPtr<IDXGISwapChain3> &swapChain, UINT frameIndex);
	void Create(const ComPtr<ID3D12Device> &device, CommandContext* const commandContext);

	void ShutDown();

	const std::unique_ptr<Texture2D>& GetTexture()const { return m_Texture2D; }
	std::shared_ptr<BufferView> GetRTVBufferView() const{ return m_RTVBufferView; }
	std::shared_ptr<BufferView> GetSRVBufferView() const { return m_SRVBufferView; }

	const UINT64 GetFenceValue() { return m_FenceValue; }
	void SetFenceValue(UINT64 value) { m_FenceValue = value; }
};