#pragma once
#include "../LowLevel/DirectX12Helper.h"
#include "../LowLevel/BufferView.h"

class Texture2D;

class FrameResources
{
private:
	std::unique_ptr<Texture2D> m_Texture2D;
	BufferView m_BufferView;

	UINT64 m_FenceValue;

public:
	FrameResources(){}
	~FrameResources() { ShutDown(); }

	void Create(const ComPtr<ID3D12Device> &device, const ComPtr<IDXGISwapChain3> &swapChain, UINT frameIndex);
	void ShutDown();

	const std::unique_ptr<Texture2D>& GetTexture()const { return m_Texture2D; }
	const BufferView& GetBufferView() const{ return m_BufferView; }

	const UINT64 GetFenceValue() { return m_FenceValue; }
	void SetFenceValue(UINT64 value) { m_FenceValue = value; }
};